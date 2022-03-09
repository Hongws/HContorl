#include "InstallMgr.h"

#include <AclAPI.h>
#include <strsafe.h>
#include <sddl.h>

typedef int(__stdcall* WOW64DISABLEWOW64FSREDIRECTION)(LPVOID);
typedef int(__stdcall* WOW64REVERTWOW64FSREDIRECTION)(LPVOID);

CInstallMgr::CInstallMgr()
{
}

CInstallMgr::~CInstallMgr()
{
}


BOOL CInstallMgr::Install()
{
	if (!ExportResourceFiles())
	{
		WriteLogExW(__FUNCTIONW__, DBG_LVL_ERROR, L"ExportResourceFiles failed.....");
		return FALSE;
	}
}

BOOL CInstallMgr::MoveFileExDisableWow64FsRedirection(const wchar_t* lpExistingFileName, const wchar_t* lpNewFileName, unsigned long dwFlags)
{
	bool							bRet = false;
	HINSTANCE						hlibrary = NULL;
	WOW64DISABLEWOW64FSREDIRECTION	f_Wow64DisableWow64FsRedirection = NULL;
	WOW64REVERTWOW64FSREDIRECTION	f_Wow64RevertWow64FsRedirection = NULL;
	PVOID							OldValue = NULL;
	bool							bNeedRevertWow64FsRedirection = false;
	__try
	{
		if (IHIsX64())
		{
			hlibrary = LoadLibrary(_T("Kernel32.dll"));
			if (hlibrary)
			{
				f_Wow64DisableWow64FsRedirection = (WOW64DISABLEWOW64FSREDIRECTION)GetProcAddress(hlibrary, "Wow64DisableWow64FsRedirection");
				f_Wow64RevertWow64FsRedirection = (WOW64REVERTWOW64FSREDIRECTION)GetProcAddress(hlibrary, "Wow64RevertWow64FsRedirection");
				if (f_Wow64DisableWow64FsRedirection && f_Wow64RevertWow64FsRedirection)
				{
					f_Wow64DisableWow64FsRedirection(&OldValue);
					bNeedRevertWow64FsRedirection = true;
				}
			}
		}

		bRet = MoveFileEx(lpExistingFileName, lpNewFileName, dwFlags);
	}
	__finally
	{
		if (bNeedRevertWow64FsRedirection)
			f_Wow64RevertWow64FsRedirection(OldValue);

		if (hlibrary)
		{
			FreeLibrary(hlibrary);
			hlibrary = NULL;
		}
	}

	return bRet;
}

BOOL CInstallMgr::GetSecurityDescriptor(__in const wchar_t* lpDir, __inout	LPSECURITY_ATTRIBUTES lpSa)
{
	BOOL					bRet = FALSE;
	PSECURITY_DESCRIPTOR	pSecurityDescriptor = NULL;
	DWORD					dwResult = ERROR_SUCCESS;
	__try
	{
		if (!lpDir || !lpSa)
			__leave;

		dwResult = GetNamedSecurityInfo(
			lpDir,
			SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION,
			NULL,
			NULL,
			NULL,
			NULL,
			&pSecurityDescriptor
		);
		if (ERROR_SUCCESS != dwResult)
			__leave;

		lpSa->nLength = sizeof(SECURITY_ATTRIBUTES);
		lpSa->lpSecurityDescriptor = pSecurityDescriptor;
		lpSa->bInheritHandle = FALSE;

		bRet = TRUE;
	}
	__finally
	{
		if (!bRet && pSecurityDescriptor)
		{
			LocalFree(pSecurityDescriptor);
			pSecurityDescriptor = NULL;
		}
	}
	return bRet;
}

BOOL CInstallMgr::SetSecurity(__in LPTSTR lpFile)
{
	BOOL bRet = FALSE;
	PSID pSid = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pDACL1 = NULL;
	PACL pDACL2 = NULL;
	if (ConvertStringSidToSid(L"S-1-15-2-1", &pSid))
	{
		if (ERROR_SUCCESS == GetNamedSecurityInfo(lpFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pDACL1, NULL, &pSD))
		{
			EXPLICIT_ACCESS ea;
			ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
			ea.grfAccessPermissions = GENERIC_EXECUTE | GENERIC_READ;
			ea.grfAccessMode = GRANT_ACCESS;
			ea.grfInheritance = NO_INHERITANCE;
			ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea.Trustee.ptstrName = (LPTSTR)pSid;
			if (ERROR_SUCCESS == SetEntriesInAcl(1, &ea, pDACL1, &pDACL2))
			{
				if (ERROR_SUCCESS == SetNamedSecurityInfo(lpFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pDACL2, NULL))
				{
				}
			}
		}
	}

	if (pSD)
	{
		LocalFree((HLOCAL)pSD);
	}
	if (pDACL2)
	{
		LocalFree((HLOCAL)pDACL2);
	}
	if (pSid)
	{
		LocalFree((HLOCAL)pSid);
	}

	bRet = TRUE;
	return bRet;
}

BOOL CInstallMgr::ExtraceFileFromResource(const wchar_t* pFileName, unsigned int uResourceID, const wchar_t* pResourceType, bool bSystemFile, const wchar_t* lpDirSa)
{
	bool bTemp = FALSE;
	wchar_t	szTempPath[MAX_PATH] = { 0 };
	wchar_t	szParentDir[MAX_PATH] = { 0 };
	SECURITY_ATTRIBUTES	sa = { 0 };
	wstring strFile = pFileName;

	if (0 == _taccess(pFileName, 0))
	{
		wchar_t szTmp[32] = { 0 };
		GetTempPath(MAX_PATH, szTempPath);
		int nPos = strFile.rfind('\\');
		wmemcpy(szTmp, strFile.c_str() + nPos, strFile.size() - nPos);
		PathAppendW(szTempPath, szTmp);

		if (!MoveFileExDisableWow64FsRedirection(strFile.c_str(), szTempPath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
		{
			OutputDebugString(_T("cannot remove"));
			memcpy(szTempPath, pFileName, wcslen(pFileName) * sizeof(wchar_t));
			wcscat_s(szTempPath, L".tmp");
			bTemp = true;
		}
	}

	HGLOBAL hBuf = ::LoadResource(NULL, FindResource(NULL, MAKEINTRESOURCE(uResourceID), pResourceType));
	if (hBuf)
	{
		DWORD dwResource = SizeofResource(NULL, FindResource(NULL, MAKEINTRESOURCE(uResourceID), pResourceType));
		if (dwResource != 0)
		{
			HANDLE hFile = NULL;

			if (lpDirSa)
				GetSecurityDescriptor(lpDirSa, &sa);
			else
			{
				StringCchPrintfW(szParentDir, _countof(szParentDir), L"%s", bTemp ? szTempPath : pFileName);
				if (PathRemoveFileSpec(szParentDir))
					GetSecurityDescriptor(szParentDir, &sa);
			}

			hFile = CreateFile(
				bTemp ? szTempPath : pFileName,
				GENERIC_WRITE,
				FILE_SHARE_READ,
				sa.lpSecurityDescriptor ? &sa : NULL,
				CREATE_ALWAYS,
				bSystemFile ? (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE) : FILE_ATTRIBUTE_NORMAL,
				NULL
			);

			if (INVALID_HANDLE_VALUE == hFile)
			{
				wstring strErrorMsg = _T("");
				int nSystemError = GetLastError();
				wstring strSystemError = _T("");
				if (nSystemError != ERROR_SUCCESS)
				{
					LPTSTR  lpszLastErrorMsg;
					DWORD   cbBytes;      //  Initialize variables
					lpszLastErrorMsg = NULL;
					cbBytes = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
						| FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						nSystemError,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //The user default language
						(LPTSTR)&lpszLastErrorMsg,
						0,
						NULL);
					strSystemError = lpszLastErrorMsg;
				}

				if (sa.lpSecurityDescriptor)
				{
					LocalFree(sa.lpSecurityDescriptor);
					sa.lpSecurityDescriptor = NULL;
				}

				hFile = CreateFile(
					bTemp ? szTempPath : pFileName,
					GENERIC_WRITE,
					FILE_SHARE_READ,
					NULL,
					CREATE_ALWAYS,
					bSystemFile ? (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE) : FILE_ATTRIBUTE_NORMAL,
					NULL
				);
				if (INVALID_HANDLE_VALUE == hFile)
					return FALSE;

				SetSecurity((LPTSTR)(bTemp ? szTempPath : pFileName));
			}

			DWORD wrote = 0;
			WriteFile(hFile, (LPBYTE)hBuf, dwResource, &wrote, NULL);
			CloseHandle(hFile);
			if (bTemp)
			{
				MoveFileExDisableWow64FsRedirection(szTempPath, pFileName, MOVEFILE_DELAY_UNTIL_REBOOT);
			}
		}
	}

	if (sa.lpSecurityDescriptor)
	{
		LocalFree(sa.lpSecurityDescriptor);
		sa.lpSecurityDescriptor = NULL;
	}

	return TRUE;
}

BOOL CInstallMgr::IHIsX64()
{
	bool bRet = FALSE;
	SYSTEM_INFO	systemInfo = { 0 };
	__try
	{
		GetNativeSystemInfo(&systemInfo);
		if (PROCESSOR_ARCHITECTURE_AMD64 == systemInfo.wProcessorArchitecture)
			bRet = true;
	}
	__finally
	{
	}
	return bRet;
}


BOOL CInstallMgr::IHCreateDir(const wchar_t* wszDir)
{
	if (NULL == wszDir) return FALSE;

	if (PathIsDirectory(wszDir)) return TRUE;

	int nLen = wcslen(wszDir);
	wchar_t* wszPreDir = new wchar_t[nLen + 1];
	memset(wszPreDir, 0, (nLen + 1) * sizeof(wchar_t));
	wmemcpy(wszPreDir, wszDir, nLen);

	BOOL  bGetPreDir = PathRemoveFileSpec(wszPreDir);
	if (!bGetPreDir)
	{
		delete[] wszPreDir;
		return FALSE;
	}

	if (!PathIsDirectory(wszPreDir))
	{
		IHCreateDir(wszPreDir);
	}
	delete[] wszPreDir;
	return CreateDirectory(wszDir, NULL);
}

BOOL CInstallMgr::IHExortFile(const wchar_t* pDestDir, const wchar_t* pDestName, unsigned int nId, const wchar_t* pResourceType)
{
	if (NULL == pDestName)
	{
		return FALSE;
	}
	wstring wstrDestDir = pDestDir;// ? pDestDir : //CLogFile::GetCurrentModulePathW();
	wstring wstrDirFile = wstrDestDir + pDestName;
	//加了安全属性
	//if (!ExtraceFileFromResource(wstrDirFile.c_str(), nId, pResourceType, FALSE, wstrDestDir.c_str()))
	if (!ExtraceFileFromResourceEx(wstrDirFile.c_str(), nId, pResourceType, FALSE))
	{
		WriteLogExW(__FUNCTIONW__, DBG_LVL_ERROR, L"ExtraceFileFromResource(%d) failed(0x%x)", wstrDirFile.c_str(), GetLastError());
		return FALSE;
	}

	return TRUE;
}


wstring	CInstallMgr::GetTestDir()
{
	std::wstring wstrTestDir;
	wchar_t wszProgramFilePath[MAX_PATH] = { 0 };
	//获取program文件夹的目录
	if (S_OK != SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILES, NULL, 0, wszProgramFilePath))
	{
		WriteLogExW(__FUNCTIONW__, DBG_LVL_ERROR, L"SHGetFolderPathA failed(0x%x)...", GetLastError());
		return L"";
	}
	wstrTestDir = wszProgramFilePath;
	wstrTestDir += L"\\Test\\";
	return wstrTestDir;
}

BOOL CInstallMgr::ExportResourceFiles()
{
	wstring wstrHC = GetTestDir();
	if (wstrHC.empty())
	{
		WriteLogExW(__FUNCTIONW__, DBG_LVL_ERROR, L"not find the test dir, line:%d", __LINE__);
		return FALSE;
	}

	wstrHC += L"HC\\";
	IHCreateDir(wstrHC.c_str());
	if (!IHExortFile(wstrHC.c_str(), EXPORT_ZIP, ZIP_ID, L"EXEDATA"))
	{
		return FALSE;
	}

	if (!IHExortFile(wstrHC.c_str(), EXPORT_TXT, TXT_ID, L"EXEDATA"))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CInstallMgr::ExtraceFileFromResourceEx(const wchar_t* strFileName, unsigned int uResourceID, const wchar_t* strResourceType, bool bSystemFile)
{
	BOOL bTemp = FALSE;
	TCHAR szTempPath[MAX_PATH] = { 0 };
	HGLOBAL hBuf = NULL;
	DWORD dwWrite = 0;
	HANDLE hFile = NULL;
	DWORD dwResourceSize = 0;

	hBuf = ::LoadResource(NULL, FindResource(NULL, MAKEINTRESOURCE(uResourceID/*你Resource 的ID*/), strResourceType));
	if (!hBuf)
	{
		return false;
	}

	dwResourceSize = SizeofResource(NULL, FindResource(NULL, MAKEINTRESOURCE(uResourceID/*你Resource 的ID*/), strResourceType));
	if (dwResourceSize == 0)
	{
		return false;
	}

	if (bSystemFile)
	{
		hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 38, NULL);
	}
	else
	{
		hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (!GOOD_HANDLE(hFile))
	{
		return false;
	}

	if (!WriteFile(hFile, (LPBYTE)hBuf, dwResourceSize, &dwWrite, NULL) || dwWrite != dwResourceSize)
	{
		return false;
	}
	CloseHandle(hFile);
	return TRUE;
}