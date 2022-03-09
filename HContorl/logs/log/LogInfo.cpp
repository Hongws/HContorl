
#include "LogInfo.h"
#include "time.h"

#include <fstream>
#include <sstream>
#ifndef LOGINFO_NOC11
#include <mutex>
std::mutex g_mutexLogStringStream;
#endif

std::ostringstream g_LogStringStream;

#define LOG_LEVEL_ALWALYS_W L"always"
#define LOG_LEVEL_ERROR_W L"error "
#define LOG_LEVEL_INFO_W L"info  "
#define LOG_LEVEL_DEBUG_W L"debug "

#define LOG_LEVEL_ALWALYS_A "always"
#define LOG_LEVEL_ERROR_A "error "
#define LOG_LEVEL_INFO_A "info  "
#define LOG_LEVEL_DEBUG_A "debug "

#ifdef CP_PLATFORM_WINDOWS
char* GetLogLevelDescA(DWORD nLogLevel)
{
	char* lpLogLevelDescA = NULL;
	switch (nLogLevel)
	{
	case DBG_LVL_ALWAYS:
		lpLogLevelDescA = LOG_LEVEL_ALWALYS_A;
		break;
	case DBG_LVL_ERROR:
		lpLogLevelDescA = LOG_LEVEL_ERROR_A;
		break;
	case DBG_LVL_INFO:
		lpLogLevelDescA = LOG_LEVEL_INFO_A;
		break;
	case DBG_LVL_DEBUG:
		lpLogLevelDescA = LOG_LEVEL_DEBUG_A;
		break;
	default:
		break;
	}
	return lpLogLevelDescA;
}

WCHAR* GetLogLevelDescW(DWORD nLogLevel)
{
	WCHAR *lpLogLevelDescW = NULL;
	switch (nLogLevel)
	{
	case DBG_LVL_ALWAYS:
		lpLogLevelDescW = LOG_LEVEL_ALWALYS_W;
		break;
	case DBG_LVL_ERROR:
		lpLogLevelDescW = LOG_LEVEL_ERROR_W;
		break;
	case DBG_LVL_INFO:
		lpLogLevelDescW = LOG_LEVEL_INFO_W;
		break;
	case DBG_LVL_DEBUG:
		lpLogLevelDescW = LOG_LEVEL_DEBUG_W;
		break;
	default:
		break;
	}
	return lpLogLevelDescW;
}
int GetVariableLengthA(LPCSTR szFormat, ...)
{
	int nLen = 0;
	va_list args;
	va_start(args, szFormat);

	nLen =  _vscprintf(szFormat, args) + 1;

	va_end(args);

	return nLen;
}
int GetVariableLengthW(LPCWSTR wszFormat, ...)
{
	int nLen = 0;
	va_list args;
	va_start(args, wszFormat);

	nLen =  _vscwprintf(wszFormat, args) + 1;

	va_end(args);

	return nLen;
}

void ShowInfoToDbgViewExA(LPCSTR lpFuncName, char *szFormat, ...)
{
	int nMsgLen = 0, nInfoLen = 0;
	char *lpMessage = NULL;
	char *lpInfo = NULL;
	va_list args;
	va_start(args, szFormat);

	nMsgLen = _vscprintf(szFormat, args) + 1;
	lpMessage = new char[nMsgLen];
	memset(lpMessage, 0, nMsgLen);

	_vsnprintf_s(lpMessage, nMsgLen, nMsgLen, szFormat, args);	

	nInfoLen = nMsgLen + strlen(lpFuncName) + 16;
	lpInfo = new char[nInfoLen];
	memset(lpInfo, 0, nInfoLen);

	sprintf_s(lpInfo, nInfoLen, "[%s] [%s]\r\n", lpFuncName, lpMessage);

	OutputDebugStringA(lpInfo);

	va_end(args);

	WSTD_DELM(lpMessage);
	WSTD_DELM(lpInfo);
	return;
}

void ShowInfoToDbgViewExW(LPCWSTR lpFuncName, WCHAR *wszFormat, ...)
{
	int nMsgLen = 0, nInfoLen = 0;
	wchar_t *lpMessage = NULL;
	wchar_t *lpInfo = NULL;
	va_list args;
	va_start(args, wszFormat);

	char szMessage[SHOWINFO_BUFSIZE] = { 0 };
	nMsgLen = _vscwprintf(wszFormat, args) + 1;
	lpMessage = new wchar_t[nMsgLen];
	memset(lpMessage, 0, nMsgLen);

	_vsnwprintf_s(lpMessage, nMsgLen, nMsgLen, wszFormat, args);

	nInfoLen = nMsgLen + wcslen(lpFuncName) + 16;
	lpInfo = new wchar_t[nInfoLen];
	memset(lpInfo, 0, nInfoLen);

	swprintf_s(lpInfo, nInfoLen, L"[%s] [%s]\r\n", lpFuncName, lpMessage);

	OutputDebugStringW(lpInfo);

	va_end(args);

	WSTD_DELM(lpMessage);
	WSTD_DELM(lpInfo);
	return;
}

LPSTR UnicodeToAnsi(LPCWSTR lpUnicode)
{
	LPSTR lpResult = NULL;

	size_t nLen = wcslen(lpUnicode);
	DWORD dwNum = WideCharToMultiByte(CP_ACP, 0, lpUnicode, (DWORD)nLen, NULL, 0, NULL, NULL);

	lpResult = new CHAR[dwNum + 1];
	if (NULL != lpResult)
	{
		lpResult[0] = 0;
		WideCharToMultiByte(CP_ACP, 0, lpUnicode, (DWORD)nLen, lpResult, dwNum, NULL, NULL);
		lpResult[dwNum] = 0;
	}

	return lpResult;
}

LPWSTR AnsiToUnicode(LPCSTR lpAnsi)
{
	LPWSTR lpResult = NULL;

	size_t nLen = strlen(lpAnsi);
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, lpAnsi, (DWORD)nLen, NULL, 0);

	lpResult = new WCHAR[dwNum + 1];
	if (NULL != lpResult)
	{
		lpResult[0] = 0;
		MultiByteToWideChar(CP_ACP, 0, lpAnsi, (DWORD)nLen, lpResult, dwNum);
		lpResult[dwNum] = 0;
	}

	return lpResult;
}

LPSTR UnicodeToUtf8(LPCWSTR lpUnicode)
{
	LPSTR lpResult = NULL;

	size_t nLen = wcslen(lpUnicode);
	DWORD dwNum = WideCharToMultiByte(CP_UTF8, 0, lpUnicode, (DWORD)nLen, NULL, 0, NULL, NULL);

	lpResult = new CHAR[dwNum + 1];
	if (NULL != lpResult)
	{
		lpResult[0] = 0;
		WideCharToMultiByte(CP_UTF8, 0, lpUnicode, (DWORD)nLen, lpResult, dwNum, NULL, NULL);
		lpResult[dwNum] = 0;
	}

	return lpResult;
}
LPWSTR Utf8ToUnicode(LPCSTR lpUtf8)
{
	LPWSTR lpResult = NULL;

	size_t nLen = strlen(lpUtf8);
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, lpUtf8, (DWORD)nLen, NULL, 0);

	lpResult = new WCHAR[dwNum + 1];
	if (NULL != lpResult)
	{
		lpResult[0] = 0;
		MultiByteToWideChar(CP_UTF8, 0, lpUtf8, (DWORD)nLen, lpResult, dwNum);
		lpResult[dwNum] = 0;
	}

	return lpResult;
}

LPSTR AnsiToUtf8(LPCSTR lpAnsi)
{
	LPWSTR lpUnicode = NULL;
	LPSTR lpResult = NULL;
	lpUnicode = AnsiToUnicode(lpAnsi);
	if(lpUnicode)
	{
		lpResult = UnicodeToAnsi(lpUnicode);		
	}
	WSTD_DELM(lpUnicode);
	return lpResult;
}
LPSTR Utf8ToAnsi(LPCSTR lpUtf8)
{
	LPWSTR lpUnicode = NULL;
	LPSTR lpResult = NULL;
	lpUnicode = Utf8ToUnicode(lpUtf8);
	if(lpUnicode)
	{
		lpResult = UnicodeToUtf8(lpUnicode);
	}
	return lpResult;
}

BOOL IsWow64()
{
	typedef BOOL(WINAPI *LPFN_GETNATIVESYSTEMINFO)(LPSYSTEM_INFO);
	BOOL bIsWow64 = FALSE;

	SYSTEM_INFO systemInfo = { 0 };
	LPFN_GETNATIVESYSTEMINFO fnGetNativeSystemInfo = NULL;
	HMODULE hHandle = LoadLibrary(L"kernel32.dll");
	IF_UNVALUE_HANDLE(hHandle)
		return bIsWow64;

	fnGetNativeSystemInfo = (LPFN_GETNATIVESYSTEMINFO)GetProcAddress(hHandle, "GetNativeSystemInfo");
	if (NULL == fnGetNativeSystemInfo)
	{
		fnGetNativeSystemInfo = (LPFN_GETNATIVESYSTEMINFO)GetProcAddress(hHandle, "GetNativeSystemInfo");
	}

	if (NULL != fnGetNativeSystemInfo)
	{
		fnGetNativeSystemInfo(&systemInfo);
	}

	if(PROCESSOR_ARCHITECTURE_AMD64 == systemInfo.wProcessorArchitecture ||
		PROCESSOR_ARCHITECTURE_IA64 == systemInfo.wProcessorArchitecture)
	{
		bIsWow64 = TRUE;
	}
	if(GOOD_HANDLE(hHandle))
	{
		FreeLibrary(hHandle);
	}
	return bIsWow64;
}

BOOL IsWow64Static()
{
	return IsWow64();
}

BOOL RemoveDirectoryRecurve(LPCWSTR lpDirectoryPath)
{
	WriteLogExW(DBG_LVL_DEBUG, __FILENAMEW__, __FUNCTIONW__, __LINE__, L"RemoveDirectoryRecurve %s", lpDirectoryPath);
	if (_tcsstr(lpDirectoryPath,L"DGLogFile") == NULL)
	{
		WriteLogW(DBG_LVL_DEBUG, __FILENAMEW__, __FUNCTIONW__, __LINE__, L"lpDirectoryPath is not find DGLogFile Return");
		return FALSE;
	}

	BOOL bRet = FALSE;
	WIN32_FIND_DATAW FindData = { 0 };
	HANDLE hHandle = NULL;
	wchar_t szFullFilePath[MAX_PATH] = { 0 };
	const wchar_t *lpPos = NULL;

	if(!lpDirectoryPath || wcslen(lpDirectoryPath) <= 0)
		return FALSE;

	if(!PathFileExists(lpDirectoryPath))
		return TRUE;

	wcsncpy_s(szFullFilePath, lpDirectoryPath, wcslen(lpDirectoryPath));
	lpPos = wcsrchr(lpDirectoryPath, '\\');
	if( wcslen(lpDirectoryPath) == lpPos - lpDirectoryPath + 1 )
		wcscat_s(szFullFilePath, L"*.*");
	else
		wcscat_s(szFullFilePath, L"\\*.*");

	hHandle = FindFirstFile(szFullFilePath, &FindData);
	do
	{
		wchar_t szFilePath[MAX_PATH] = { 0 };
		if ( 0 == wcscmp(FindData.cFileName, L".") || 0 == wcscmp(FindData.cFileName, L".."))
			continue;

		wsprintf(szFilePath, L"%s\\%s", lpDirectoryPath, FindData.cFileName);
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			bRet = RemoveDirectoryRecurve(szFilePath);
		}
		else
		{
			WriteLogExW(DBG_LVL_DEBUG, __FILENAMEW__, __FUNCTIONW__, __LINE__, L"DeleteFileW %s", szFilePath);
			bRet = ::DeleteFileW(szFilePath);
		}
		if(!bRet) break;

	}while(::FindNextFile(hHandle, &FindData));

	::FindClose(hHandle);

	WriteLogExW(DBG_LVL_DEBUG, __FILENAMEW__, __FUNCTIONW__, __LINE__, L"RemoveDirectory %s", lpDirectoryPath);
	bRet = ::RemoveDirectory(lpDirectoryPath);
	return bRet;
}

BOOL CreateDirectoryRecurve(LPCTSTR lpDirectoryPath)
{
	if(!lpDirectoryPath || wcslen(lpDirectoryPath) <= 0)
		return FALSE;

	if (::PathFileExists(lpDirectoryPath)) return TRUE;

	wchar_t   szPreDir[MAX_PATH] = { 0 };
	wcsncpy_s(szPreDir, lpDirectoryPath, wcslen(lpDirectoryPath));
 
	if (!::PathRemoveFileSpec(szPreDir)) return FALSE;

	if (!::PathFileExists(szPreDir))
	{
		CreateDirectoryRecurve(szPreDir);
	}

	return ::CreateDirectory(lpDirectoryPath, NULL);
}

ULONGLONG SYSTEMTIMEToTimeStamp(const SYSTEMTIME st)
{
	struct tm gm1 = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	return mktime(&gm1);
}

ULONGLONG FILETIMEToTimeStamp(const FILETIME ft)
{
	SYSTEMTIME st = { 0 };
	FileTimeToSystemTime(&ft, &st);

	return SYSTEMTIMEToTimeStamp(st);
}


static DWORD g_dwLogLevel = DBG_LVL_ERROR;//日志等级
static CRITICAL_SECTION g_csFileLog;
static HANDLE g_hLogFile = INVALID_HANDLE_VALUE;
static WCHAR g_szLogFilePath[MAX_PATH] = {0};
static WCHAR g_szLogDirName[MAX_PATH] = { 0 };
static WCHAR g_szLogFileName[MAX_PATH] = { 0 };
static WCHAR g_szPdbFilePath[MAX_PATH] = { 0 };
static WCHAR g_bLogInit = FALSE;
static DWORD g_dwSingleLogFileMaxSize = 100 * 1024 * 1024;
static DWORD g_dwHistroyInternalDay = 7;
static BOOL	g_bClear = FALSE;

inline BOOL IsCurrentLogLevelLog(DWORD dwLogLevel)
{
	return dwLogLevel <= g_dwLogLevel;//等级小于等于配置输出
}

static void InitLogFilePath(BOOL bCreate)
{
	memset(g_szLogFilePath, 0, MAX_PATH * 2);

	TCHAR szPath[MAX_PATH] = { 0 };
	SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath);

	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	WCHAR szProcessName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szProcessName, MAX_PATH);
	PathStripPathW(szProcessName);
	PathRemoveExtensionW(szProcessName);

	TCHAR szLogDir[MAX_PATH] = { 0 };
	if(wcslen(g_szLogDirName))
		_stprintf_s(szLogDir, MAX_PATH, _T("%s\\HLogFile\\%s\\%04d-%02d-%02d"), szPath, g_szLogDirName, st.wYear, st.wMonth, st.wDay);
	else
		_stprintf_s(szLogDir, MAX_PATH, _T("%s\\HLogFile\\%s\\%04d-%02d-%02d"), szPath, szProcessName, st.wYear, st.wMonth, st.wDay);

	if (!PathFileExists(szLogDir))
	{
		if(bCreate)
		{
			if (!CreateDirectoryRecurve(szLogDir))
			{
				RtlZeroMemory(szLogDir, MAX_PATH * 2);
				_tcscpy_s(szLogDir, MAX_PATH, szPath);
			}
		}
	}
	if (wcslen(g_szLogFileName))
	{
		_stprintf_s(g_szLogFilePath, MAX_PATH, _T("%s\\%s.log"), szLogDir, g_szLogFileName);
		_stprintf_s(g_szPdbFilePath, MAX_PATH, _T("%s\\%s.dmp"), szLogDir, g_szLogFileName);
	}
	else
	{
		_stprintf_s(g_szLogFilePath, MAX_PATH, _T("%s\\%s.log"), szLogDir, szProcessName);
		_stprintf_s(g_szPdbFilePath, MAX_PATH, _T("%s\\%s.dmp"), szLogDir, szProcessName);
	}
	return;
}

LONG __stdcall UnhandledExceptionFilterCall(__in struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	HANDLE hDumpFile = CreateFile(g_szPdbFilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION MinExcInfo;
		MinExcInfo.ClientPointers = TRUE;
		MinExcInfo.ExceptionPointers = ExceptionInfo;
		MinExcInfo.ThreadId = GetCurrentThreadId();
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
			MiniDumpNormal, &MinExcInfo, NULL, NULL);

		CloseHandle(hDumpFile);
	}

#ifdef FSTREAM_MODE
	std::ofstream LogFileStream;
	LogFileStream.open(g_szLogFilePath, std::ios::app | std::ios::out);
	if (LogFileStream.good())
	{
		LogFileStream.seekp(0, std::ios_base::end);
#ifndef LOGINFO_NOC11
		std::lock_guard<std::mutex> guard(g_mutexLogStringStream);
#endif			
		std::string strTemp = g_LogStringStream.str();
		LogFileStream << strTemp.c_str();
		g_LogStringStream.str("");
		LogFileStream.rdbuf()->close();
		LogFileStream.close();
	}
#endif // FSTREAM_MODE


	return EXCEPTION_EXECUTE_HANDLER;
}

void DisableSetUnhandledExceptionFilter()
{
	HMODULE hModule = LoadLibrary(_T("kernel32.dll"));
	IF_UNVALUE_HANDLE(hModule)
		return;
	void* addr = (void*)GetProcAddress(hModule, "SetUnhandledExceptionFilter");
	if (addr != NULL)
	{
		unsigned char code[16];
		int size = 0;
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD dwOldFlag = 0;
		DWORD dwTempFlag = 0;
		VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}

void InitMiniDumpInfo()
{
	if (wcslen(g_szPdbFilePath) == 0)
	{
		InitLogFilePath(TRUE);
	}
	SetUnhandledExceptionFilter(UnhandledExceptionFilterCall);
	DisableSetUnhandledExceptionFilter();
}
static BOOL InitLogFile()
{
	InitLogFilePath(TRUE);
	_ASSERT(wcslen(g_szLogFilePath));

	g_hLogFile = CreateFile(g_szLogFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!GOOD_HANDLE(g_hLogFile))
	{
		ShowInfoToDbgViewExW(__FUNCTIONW__, L"CreateFile:%s failed, error:%d.", g_szLogFilePath, GetLastError());
		return FALSE;
	}	
	
	LARGE_INTEGER fileSize = { 0 };
	GetFileSizeEx(g_hLogFile, &fileSize);
	if (fileSize.QuadPart >= g_dwSingleLogFileMaxSize)//10MB
	{
		WSTD_CLOSE_HANDLE(g_hLogFile);
		DeleteFile(g_szLogFilePath);
		g_hLogFile = CreateFile(g_szLogFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!GOOD_HANDLE(g_hLogFile))
		{
			ShowInfoToDbgViewExW(__FUNCTIONW__, L"CreateFile:%s failed, error:%d.", g_szLogFilePath, GetLastError());
			return FALSE;
		}
	}
	return TRUE;
}

static void InitLogConfig()
{
	TCHAR szLogConfigPath[MAX_PATH] = { 0 };

	if(IsWow64())
		SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86, NULL, SHGFP_TYPE_DEFAULT, szLogConfigPath);
	else
		SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_DEFAULT, szLogConfigPath);

	PathAppend(szLogConfigPath, L"DSOC");
	PathAppend(szLogConfigPath, L"LogConfig.ini");

	//ShowInfoToDbgViewExW(__FUNCTIONW__, L"LogConfigPath:%s.", szLogConfigPath);

	if(PathFileExists(szLogConfigPath))
	{
		g_dwLogLevel = GetPrivateProfileInt(_T("LogConfig"),_T("LogLevel"), DBG_LVL_INFO, szLogConfigPath);
		g_dwHistroyInternalDay = GetPrivateProfileInt(_T("LogConfig"),_T("HistroyInternalDay"), 7, szLogConfigPath);
		g_dwSingleLogFileMaxSize = GetPrivateProfileInt(_T("LogConfig"),_T("SingleLogFileMaxSize"), 10, szLogConfigPath);
		g_dwSingleLogFileMaxSize = g_dwSingleLogFileMaxSize * 1024 * 1024;
	}

	LPWSTR lpLogLevelW = NULL;
	switch (g_dwLogLevel)
	{
	case DBG_LVL_ALWAYS:
		lpLogLevelW = _T("always");
		break;	
	case DBG_LVL_ERROR:
		lpLogLevelW = _T("error");
		break;	
	case DBG_LVL_INFO:
		lpLogLevelW = _T("info");
		break;
	case DBG_LVL_DEBUG:
		lpLogLevelW = _T("debug");
		break;
	default:
		break;
	}

	//ShowInfoToDbgViewExW(__FUNCTIONW__, L"LogLevel = %s", lpLogLevelW);
	return;
}

static BOOL WriteData(LPCSTR lpBuffer)
{
	BOOL bRet = TRUE;
	do 
	{
		LARGE_INTEGER li = { 0 };
		li.QuadPart = 0;
		if(!SetFilePointerEx(g_hLogFile, li, NULL, FILE_END))
		{
			bRet = FALSE;
			break;
		}

		DWORD dwWrite = 0;
		DWORD dwLen = 0;
		dwLen = strlen(lpBuffer);

		if (!WriteFile(g_hLogFile, lpBuffer, dwLen, &dwWrite, NULL) || dwLen != dwWrite)
		{
			bRet = FALSE;
			break;
		}		
	} while (FALSE);
	return bRet;
}

unsigned WINAPI FileStreamFlushThread(LPVOID lpIn)
{
	InitLogFilePath(TRUE);
	InitLogConfig();

	while(true)
	{
		std::ofstream LogFileStream;
		LogFileStream.open(g_szLogFilePath, std::ios::app | std::ios::out);
		if (LogFileStream.good())
		{
			LogFileStream.seekp(0, std::ios_base::end);
#ifndef LOGINFO_NOC11
			std::lock_guard<std::mutex> guard(g_mutexLogStringStream);
#endif			
			std::string strTemp = g_LogStringStream.str();
			LogFileStream << strTemp.c_str();
			g_LogStringStream.str("");
			LogFileStream.rdbuf()->close();
			LogFileStream.close();			
		}
		
		Sleep(1000);
	}
	return 0;
}

void StartFileStreamFlushThread(LPVOID lpIn)
{
	unsigned ThreadId;
	HANDLE hHandle;
	hHandle = (HANDLE)_beginthreadex(NULL, 0, FileStreamFlushThread, lpIn, 0, &ThreadId);
	WSTD_CLOSE_HANDLE(hHandle);
}

BOOL InitFileStreamMode()
{
	StartFileStreamFlushThread(NULL);
	return TRUE;
}

static BOOL WriteData_FileStream(LPCSTR lpBuffer)
{
#ifndef LOGINFO_NOC11
	std::lock_guard<std::mutex> guard(g_mutexLogStringStream);
#endif		
	if (g_LogStringStream.good())
	{
	//	LogFileStream << lpBuffer;
		g_LogStringStream << lpBuffer;
	}
	else
	{
		OutputDebugStringW(L"WriteData_FileStream g_LogStringStream is bad");
	}
	return TRUE;
}

BOOL  IsHistoryLogDelete(const FILETIME ft)
{
	UINT nDay = 0;
	time_t tCurTime = 0;
	time_t tFileTime = 0;

	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);

	tCurTime = SYSTEMTIMEToTimeStamp(st);

	tFileTime = FILETIMEToTimeStamp(ft);
#pragma warning(push)
#pragma warning(disable:4244)
	nDay = (tCurTime - tFileTime) / ( 60 * 60 * 24);
#pragma warning(pop)

	return nDay > g_dwHistroyInternalDay ? TRUE : FALSE;
}
static unsigned WINAPI GlobalClearHistroyLogThread(void* pVoid)
{
	InitLogFilePath(FALSE);
	_ASSERT(wcslen(g_szLogFilePath));

	WCHAR szLogDirectory[MAX_PATH] = { 0 };

	wcsncpy_s(szLogDirectory, g_szLogFilePath, wcslen(g_szLogFilePath));
	PathRemoveFileSpec(szLogDirectory);
	PathRemoveFileSpec(szLogDirectory);

	if (_tcslen(szLogDirectory) == 0 || !PathFileExists(szLogDirectory))
	{
		return 0;
	}

	WIN32_FIND_DATAW FindData = { 0 };
	HANDLE hHandle = NULL;
	wchar_t szFullFilePath[MAX_PATH] = { 0 };

	wcsncpy_s(szFullFilePath, szLogDirectory, wcslen(szLogDirectory));
	PathAppend(szFullFilePath, L"*.*");
	WriteLogW(DBG_LVL_DEBUG, __FILENAMEW__, __FUNCTIONW__, __LINE__, szFullFilePath);

	while(!g_bClear)
	{
		InitLogConfig();
		hHandle = FindFirstFile(szFullFilePath, &FindData);
		if(GOOD_HANDLE(hHandle))
		{
			do {
				wchar_t szFilePath[MAX_PATH] = { 0 };
				if (0 == wcscmp(FindData.cFileName, L".") || 0 == wcscmp(FindData.cFileName, L".."))
					continue;

				wsprintf(szFilePath, L"%s\\%s", szLogDirectory, FindData.cFileName);
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
					IsHistoryLogDelete(FindData.ftCreationTime))
				{
					RemoveDirectoryRecurve(szFilePath);
				}
			} while (::FindNextFile(hHandle, &FindData));
		}

		::FindClose(hHandle);
		::RemoveDirectory(szLogDirectory);

		for(int i = 0; i < 24 * 60; i++)
		{
			Sleep(60 * 1000);
		}
	}
	return 0;
}
void InitLogInfo(LPCTSTR lpDirName, LPCTSTR lpFileName)
{
	if(!g_bLogInit)
	{
		if (lpDirName && wcslen(lpDirName))
		{
			wcsncpy_s(g_szLogDirName, lpDirName, wcslen(lpDirName));
		}

		if (lpFileName && wcslen(lpFileName))
		{
			wcsncpy_s(g_szLogFileName, lpFileName, wcslen(lpFileName));
		}

		InitializeCriticalSection(&g_csFileLog);
		
		g_bLogInit = TRUE;

#ifdef FSTREAM_MODE
		InitFileStreamMode();
#endif // FSTREAM_MODE

	}
	return;
}

void UnInitLogInfo()
{
	g_bClear = TRUE;

	DeleteCriticalSection(&g_csFileLog);

	WSTD_CLOSE_HANDLE(g_hLogFile);
	return;
}

void StartClearHistroyLogThread()
{
	if (!g_bLogInit)
		return;
	_beginthreadex(NULL, NULL, GlobalClearHistroyLogThread, NULL, NULL, NULL);
}

void WriteLogA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpMessage)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nInfoLen = 0;
		char *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";		

		SYSTEMTIME st;
		GetLocalTime(&st);
		
		nInfoLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMessage);

		lpLogData = new char[nInfoLen];
		memset(lpLogData, 0, nInfoLen);

		sprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMessage);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpLogData);
#else
		WriteData(lpLogData);
#endif // FSTREAM_MODE	

		WSTD_CLOSE_HANDLE(g_hLogFile);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&g_csFileLog);
	return;
}
void WriteLogA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpMessage, DWORD dwErrorCode)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nInfoLen = 0;
		char *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [ErrorCode:%d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		SYSTEMTIME st;
		GetLocalTime(&st);

		nInfoLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			dwErrorCode, lpFileName, lpFuncName, dwCodeLine, lpMessage);

		lpLogData = new char[nInfoLen];
		memset(lpLogData, 0, nInfoLen);

		sprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			dwErrorCode, lpFileName, lpFuncName, dwCodeLine, lpMessage);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpLogData);
#else
		WriteData(lpLogData);
#endif // FSTREAM_MODE		

		WSTD_CLOSE_HANDLE(g_hLogFile);	
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&g_csFileLog);
	return;
}

void WriteLogW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpMessage)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nInfoLen = 0;
		wchar_t *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";

		SYSTEMTIME st;
		GetLocalTime(&st);

		nInfoLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(), 
			lpFuncName, lpMessage);

		lpLogData = new wchar_t[nInfoLen];
		memset(lpLogData, 0, nInfoLen * 2);

		swprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMessage);

		lpAnsi = UnicodeToAnsi(lpLogData);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpAnsi);
#else
		WriteData(lpAnsi);
#endif // FSTREAM_MODE		

		WSTD_CLOSE_HANDLE(g_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpLogData);	
	}
	LeaveCriticalSection(&g_csFileLog);
	return;
}

void WriteLogW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpMessage, DWORD dwErrorCode)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nInfoLen = 0;
		wchar_t *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [ErrorCode:%d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		SYSTEMTIME st;
		GetLocalTime(&st);

		nInfoLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			dwErrorCode,lpFileName, lpFuncName, dwCodeLine, lpMessage);

		lpLogData = new wchar_t[nInfoLen];
		memset(lpLogData, 0, nInfoLen * 2);

		swprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			dwErrorCode,lpFileName, lpFuncName, dwCodeLine, lpMessage);

		lpAnsi = UnicodeToAnsi(lpLogData);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpAnsi);
#else
		WriteData(lpAnsi);
#endif // FSTREAM_MODE		

		WSTD_CLOSE_HANDLE(g_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&g_csFileLog);
}
void WriteLogExA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpFormat, ...)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nMsgLen, nLogDataLen = 0;
		char *lpMsgData = NULL, *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen =   _vscprintf(lpFormat, argList) + 1;
		lpMsgData = new char[nMsgLen];
		memset(lpMsgData, 0, nMsgLen);

		_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);

		lpLogData = new char[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen);

		sprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpLogData);
#else
		WriteData(lpLogData);
#endif // FSTREAM_MODE		

		va_end(argList);

		WSTD_CLOSE_HANDLE(g_hLogFile);		
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&g_csFileLog);
	return;
}

void WriteLogExA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpFormat,...)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nMsgLen, nLogDataLen = 0;
		char *lpMsgData = NULL, *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen = _vscprintf(lpFormat, argList) + 1;
		lpMsgData = new char[nMsgLen];
		memset(lpMsgData, 0, nMsgLen);

		_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);
		lpLogData = new char[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen);

		sprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpLogData);
#else
		WriteData(lpLogData);
#endif // FSTREAM_MODE	

		va_end(argList);

		WSTD_CLOSE_HANDLE(g_hLogFile);		
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&g_csFileLog);
	return;
}


void WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpFormat,...)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nMsgLen, nLogDataLen = 0;
		wchar_t *lpMsgData = NULL, *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen =   _vscwprintf(lpFormat, argList) + 1;
		lpMsgData = new wchar_t[nMsgLen];
		memset(lpMsgData, 0, nMsgLen * 2);

		_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);

		lpLogData = new wchar_t[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen * 2);

		swprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);

		lpAnsi = UnicodeToAnsi(lpLogData);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpAnsi);
#else
		WriteData(lpAnsi);
#endif // FSTREAM_MODE	

		va_end(argList);

		WSTD_CLOSE_HANDLE(g_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&g_csFileLog);
	return;
}

void WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpFormat,...)
{
	if(!g_bLogInit) return;

	InitLogConfig();

	EnterCriticalSection(&g_csFileLog);
#ifdef FSTREAM_MODE
	if (IsCurrentLogLevelLog(dwLogLevel))
#else
	if (IsCurrentLogLevelLog(dwLogLevel) &&
		InitLogFile())
#endif // FSTREAM_MODE
	{
		int nMsgLen, nLogDataLen = 0;
		wchar_t *lpMsgData = NULL, *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen =   _vscwprintf(lpFormat, argList) + 1;
		lpMsgData = new wchar_t[nMsgLen];
		memset(lpMsgData, 0, nMsgLen * 2);

		_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);

		lpLogData = new wchar_t[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen * 2);

		swprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);

		lpAnsi = UnicodeToAnsi(lpLogData);

#ifdef FSTREAM_MODE
		WriteData_FileStream(lpAnsi);
#else
		WriteData(lpAnsi);
#endif // FSTREAM_MODE	

		va_end(argList);

		WSTD_CLOSE_HANDLE(g_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&g_csFileLog);
	return;
}

void WriteLogW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpMessage)
{
	WriteLogW(dwLogLevel, lpFuncName, lpMessage);
}
void WriteLogExW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpFormat, ...)
{
	int nMsgLen;
	wchar_t *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscwprintf(lpFormat, argList) + 1;
	lpMsgData = new wchar_t[nMsgLen];
	memset(lpMsgData, 0, nMsgLen * 2);
	_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogW(dwLogLevel, lpFuncName, lpMsgData);
	WSTD_DELM(lpMsgData);
}

void WriteLogA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpMessage)
{
	WriteLogA(dwLogLevel, lpFuncName, lpMessage);
}
void WriteLogExA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpFormat, ...)
{
	int nMsgLen;
	char *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscprintf(lpFormat, argList) + 1;
	lpMsgData = new char[nMsgLen];
	memset(lpMsgData, 0, nMsgLen);
	_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogA(dwLogLevel, lpFuncName, lpMsgData);
	WSTD_DELM(lpMsgData);
}

void WriteLogW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpMessage)
{
	WriteLogW(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMessage);
}
void WriteLogExW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpFormat, ...)
{
	int nMsgLen;
	wchar_t *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscwprintf(lpFormat, argList) + 1;
	lpMsgData = new wchar_t[nMsgLen];
	memset(lpMsgData, 0, nMsgLen * 2);
	_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogW(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMsgData);
	WSTD_DELM(lpMsgData);
}

void WriteLogA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpMessage)
{
	WriteLogA(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMessage);
}
void WriteLogExA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpFormat, ...)
{
	int nMsgLen;
	char *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscprintf(lpFormat, argList) + 1;
	lpMsgData = new char[nMsgLen];
	memset(lpMsgData, 0, nMsgLen);
	_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogA(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMsgData);
	WSTD_DELM(lpMsgData);
}


void WriteLogW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCWSTR lpMessage)
{
	WriteLogW(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMessage, dwErrorCode);
}
void WriteLogExW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCWSTR lpFormat, ...)
{
	int nMsgLen;
	wchar_t *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscwprintf(lpFormat, argList) + 1;
	lpMsgData = new wchar_t[nMsgLen];
	memset(lpMsgData, 0, nMsgLen * 2);
	_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogW(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMsgData, dwErrorCode);
	WSTD_DELM(lpMsgData);
}

void WriteLogA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCSTR lpMessage)
{
	WriteLogA(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMessage, dwErrorCode);
}
void WriteLogExA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCSTR lpFormat, ...)
{
	int nMsgLen;
	char *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscprintf(lpFormat, argList) + 1;
	lpMsgData = new char[nMsgLen];
	memset(lpMsgData, 0, nMsgLen);
	_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogA(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMsgData, dwErrorCode);
	WSTD_DELM(lpMsgData);
}

char* ReWriteLogPathA(char* dest, const char* src)
{
	char* strDest = dest;
	//assert((dest != NULL) && (src != NULL));
	while ((*dest++ = *src++) != '\0')
	{
		if (*src == '%')
		{
			*dest++ = '%';
		}
	}
	return strDest;
}

wchar_t* ReWriteLogPathW(wchar_t* dest, const wchar_t* src)
{
	wchar_t* strDest = dest;
	//assert((dest != NULL) && (src != NULL));
	while ((*dest++ = *src++) != '\0')
	{
		if (*src == L'%')
		{
			*dest++ = L'%';
		}
	}
	return strDest;
}

CLogFile::CLogFile(LPCWSTR lpDirName,LPCWSTR lpFileName)
{
	m_hLogFile = NULL;
	m_dwLogLevel = DBG_LVL_ERROR;
	memset(m_szLogDirName, 0, MAX_PATH * 2);
	memset(m_szLogFileName, 0, MAX_PATH * 2);

	InitializeCriticalSection(&m_csFileLog);

	m_dwProcessId = GetCurrentProcessId();

	if (lpDirName && wcslen(lpDirName))
	{
		wcsncpy_s(m_szLogDirName, lpDirName, wcslen(lpDirName));
	}

	if (lpFileName && wcslen(lpFileName))
	{
		wcsncpy_s(m_szLogFileName, lpFileName, wcslen(lpFileName));
	}

	m_dwHistroyInternalDay = 7;//gh:超过7天日志自动清理
	m_dwSingleLogFileMaxSize = 10 * 1024 * 1024;//gh:单个日志文件超过10M,自动清理

	m_bClear = FALSE;
	
	return;
}
CLogFile::~CLogFile()
{
	DeleteCriticalSection(&m_csFileLog);
	WSTD_CLOSE_HANDLE(m_hLogFile);
	m_bClear = TRUE;
	return;
}

void CLogFile::WriteLogA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpMessage)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if (this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nInfoLen = 0;
		char *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";

		SYSTEMTIME st;
		GetLocalTime(&st);

		nInfoLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMessage);

		lpLogData = new char[nInfoLen];
		memset(lpLogData, 0, nInfoLen);

		sprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMessage);

		WriteData(lpLogData);

		WSTD_CLOSE_HANDLE(m_hLogFile);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}
void CLogFile::WriteLogA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpMessage)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if (this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nInfoLen = 0;
		char *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		SYSTEMTIME st;
		GetLocalTime(&st);

		nInfoLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMessage);

		lpLogData = new char[nInfoLen];
		memset(lpLogData, 0, nInfoLen);

		sprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMessage);

		WriteData(lpLogData);

		WSTD_CLOSE_HANDLE(m_hLogFile);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}

void CLogFile::WriteLogW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpMessage)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if (this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nInfoLen = 0;
		wchar_t *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";

		SYSTEMTIME st;
		GetLocalTime(&st);

		nInfoLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMessage);

		lpLogData = new wchar_t[nInfoLen];
		memset(lpLogData, 0, nInfoLen * 2);

		swprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMessage);

		lpAnsi = UnicodeToAnsi(lpLogData);

		WriteData(lpAnsi);

		WSTD_CLOSE_HANDLE(m_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}
void CLogFile::WriteLogW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpMessage)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if (this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nInfoLen = 0;
		wchar_t *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		SYSTEMTIME st;
		GetLocalTime(&st);

		nInfoLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMessage);

		lpLogData = new wchar_t[nInfoLen];
		memset(lpLogData, 0, nInfoLen * 2);

		swprintf_s(lpLogData, nInfoLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMessage);

		lpAnsi = UnicodeToAnsi(lpLogData);

		WriteData(lpAnsi);

		WSTD_CLOSE_HANDLE(m_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}

void CLogFile::WriteLogExA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpFormat,...)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if(this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nMsgLen, nLogDataLen = 0;
		char *lpMsgData = NULL, *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen =   _vscprintf(lpFormat, argList) + 1;
		lpMsgData = new char[nMsgLen];
		memset(lpMsgData, 0, nMsgLen);

		_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);

		lpLogData = new char[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen);

		sprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);


		WriteData(lpLogData);	

		va_end(argList);

		WSTD_CLOSE_HANDLE(m_hLogFile);		
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}


void CLogFile::WriteLogExA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpFormat,...)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if (this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nMsgLen, nLogDataLen = 0;
		char *lpMsgData = NULL, *lpLogData = NULL;
		char *lpLogFormat = "[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen = _vscprintf(lpFormat, argList) + 1;
		lpMsgData = new char[nMsgLen];
		memset(lpMsgData, 0, nMsgLen);

		_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthA(lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);
		lpLogData = new char[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen);

		sprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescA(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);

		WriteData(lpLogData);	

		va_end(argList);

		WSTD_CLOSE_HANDLE(m_hLogFile);	
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}

void CLogFile::WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpFormat,...)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if (this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nMsgLen, nLogDataLen = 0;
		wchar_t *lpMsgData = NULL, *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FUNC:%s] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen =   _vscwprintf(lpFormat, argList) + 1;
		lpMsgData = new wchar_t[nMsgLen];
		memset(lpMsgData, 0, nMsgLen * 2);

		_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);

		lpLogData = new wchar_t[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen * 2);

		swprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFuncName, lpMsgData);

		lpAnsi = UnicodeToAnsi(lpLogData);

		WriteData(lpAnsi);	

		va_end(argList);

		WSTD_CLOSE_HANDLE(m_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}


void CLogFile::WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpFormat,...)
{
	this->InitLogConfig();

	EnterCriticalSection(&m_csFileLog);
	if (this->IsCurrentLogLevelLog(dwLogLevel) &&
		this->InitLogFile())
	{
		int nMsgLen, nLogDataLen = 0;
		wchar_t *lpMsgData = NULL, *lpLogData = NULL;
		char *lpAnsi = NULL;
		wchar_t *lpLogFormat = L"[LEVEL:%s] [TIME:%02u:%02u:%02u] [PID:%4d] [TID:%4d] [FILE:%s] [FUNC:%s] [LINE:%d] [%s]\r\n";

		va_list argList;
		va_start(argList, lpFormat);

		nMsgLen =   _vscwprintf(lpFormat, argList) + 1;
		lpMsgData = new wchar_t[nMsgLen];
		memset(lpMsgData, 0, nMsgLen * 2);

		_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);

		SYSTEMTIME st;
		GetLocalTime(&st);

		nLogDataLen = GetVariableLengthW(lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);

		lpLogData = new wchar_t[nLogDataLen];
		memset(lpLogData, 0, nLogDataLen * 2);

		swprintf_s(lpLogData, nLogDataLen, lpLogFormat, GetLogLevelDescW(dwLogLevel), st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), GetCurrentThreadId(),
			lpFileName, lpFuncName, dwCodeLine, lpMsgData);

		lpAnsi = UnicodeToAnsi(lpLogData);

		WriteData(lpAnsi);

		va_end(argList);

		WSTD_CLOSE_HANDLE(m_hLogFile);		
		WSTD_DELM(lpAnsi);
		WSTD_DELM(lpMsgData);
		WSTD_DELM(lpLogData);
	}
	LeaveCriticalSection(&m_csFileLog);
	return;
}

void CLogFile::WriteLogW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpMessage)
{
	WriteLogW(dwLogLevel, lpFuncName, lpMessage);
}
void CLogFile::WriteLogExW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpFormat, ...)
{
	int nMsgLen;
	wchar_t *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscwprintf(lpFormat, argList) + 1;
	lpMsgData = new wchar_t[nMsgLen];
	memset(lpMsgData, 0, nMsgLen * 2);
	_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogW(dwLogLevel, lpFuncName, lpMsgData);
	WSTD_DELM(lpMsgData);
}

void CLogFile::WriteLogA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpMessage)
{
	WriteLogA(dwLogLevel, lpFuncName, lpMessage);
}
void CLogFile::WriteLogExA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpFormat, ...)
{
	int nMsgLen;
	char *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscprintf(lpFormat, argList) + 1;
	lpMsgData = new char[nMsgLen];
	memset(lpMsgData, 0, nMsgLen);
	_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogA(dwLogLevel, lpFuncName, lpMsgData);
	WSTD_DELM(lpMsgData);
}

void CLogFile::WriteLogW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpMessage)
{
	WriteLogW(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMessage);
}
void CLogFile::WriteLogExW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpFormat, ...)
{
	int nMsgLen;
	wchar_t *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscwprintf(lpFormat, argList) + 1;
	lpMsgData = new wchar_t[nMsgLen];
	memset(lpMsgData, 0, nMsgLen * 2);
	_vsnwprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogW(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMsgData);
	WSTD_DELM(lpMsgData);
}

void CLogFile::WriteLogA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpMessage)
{
	WriteLogA(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMessage);
}
void CLogFile::WriteLogExA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpFormat, ...)
{
	int nMsgLen;
	char *lpMsgData = NULL;
	va_list argList;
	va_start(argList, lpFormat);
	nMsgLen = _vscprintf(lpFormat, argList) + 1;
	lpMsgData = new char[nMsgLen];
	memset(lpMsgData, 0, nMsgLen);
	_vsnprintf_s(lpMsgData, nMsgLen, nMsgLen, lpFormat, argList);
	WriteLogA(dwLogLevel, lpFileName, lpFuncName, dwCodeLine, lpMsgData);
	WSTD_DELM(lpMsgData);
}

void CLogFile::StartClearHistroyLogThread()
{
	_beginthreadex(NULL, NULL, ClearHistroyLogThread, this, NULL, NULL);
}

void CLogFile::InitLogFilePath(BOOL bCreate)
{
	memset(m_szLogFilePath, 0, MAX_PATH * 2);
	WCHAR szPath[MAX_PATH] = { 0 };
	SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath);

	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	WCHAR szProcessName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szProcessName, MAX_PATH);
	PathStripPathW(szProcessName);
	PathRemoveExtensionW(szProcessName);

	WCHAR szLogDir[MAX_PATH] = { 0 };
	swprintf_s(szLogDir, MAX_PATH - 1, _T("%s\\DGLogFile\\%s\\%04d-%02d-%02d"), szPath, szProcessName, st.wYear, st.wMonth, st.wDay);
	if (!PathFileExists(szLogDir))
	{
		if(bCreate)
		{
			if (!CreateDirectoryRecurve(szLogDir))
			{
				RtlZeroMemory(szLogDir, MAX_PATH * sizeof(TCHAR));
				_tcscpy_s(szLogDir, MAX_PATH, szPath);
			}
		}
	}
	swprintf_s(m_szLogFilePath, MAX_PATH - 1, _T("%s\\%s.log"), szLogDir, szProcessName);
	return;
}

BOOL CLogFile::InitLogFile()
{
	this->InitLogFilePath(TRUE);
	_ASSERT(wcslen(m_szLogFilePath));

	m_hLogFile = CreateFile(m_szLogFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!GOOD_HANDLE(m_hLogFile))
	{
		ShowInfoToDbgViewExW(__FUNCTIONW__, L"CreateFile:%s failed, error:%d.", m_szLogFilePath, GetLastError());
		return FALSE;
	}	

	LARGE_INTEGER fileSize = { 0 };
	GetFileSizeEx(m_hLogFile, &fileSize);
	if (fileSize.QuadPart >= m_dwSingleLogFileMaxSize)//10MB
	{
		WSTD_CLOSE_HANDLE(m_hLogFile);
		DeleteFile(m_szLogFilePath);
		m_hLogFile = CreateFile(m_szLogFilePath, GENERIC_WRITE,  FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!GOOD_HANDLE(m_hLogFile))
		{
			ShowInfoToDbgViewExW(__FUNCTIONW__, L"CreateFile:%s failed, error:%d.", m_szLogFilePath, GetLastError());
			return FALSE;
		}
	}
	return TRUE;
}
void CLogFile::InitLogConfig()
{
	TCHAR szLogConfigPath[MAX_PATH] = { 0 };	
	if(IsWow64())
		SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86, NULL, SHGFP_TYPE_DEFAULT, szLogConfigPath);
	else
		SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_DEFAULT, szLogConfigPath);

	PathAppend(szLogConfigPath, L"DSOC");
	PathAppend(szLogConfigPath, L"LogConfig.ini");

	//ShowInfoToDbgViewExW(__FUNCTIONW__, L"LogConfigPath:%s.", szLogConfigPath);

	if(PathFileExists(szLogConfigPath))
	{
		m_dwLogLevel = GetPrivateProfileInt(_T("LogConfig"),_T("LogLevel"), DBG_LVL_ERROR, szLogConfigPath);
		m_dwHistroyInternalDay = GetPrivateProfileInt(_T("LogConfig"),_T("HistroyInternalDay"), 7, szLogConfigPath);
		m_dwSingleLogFileMaxSize = GetPrivateProfileInt(_T("LogConfig"),_T("SingleLogFileMaxSize"), 10, szLogConfigPath);
		m_dwSingleLogFileMaxSize = m_dwSingleLogFileMaxSize * 1024 * 1024;
	}

	LPWSTR lpLogLevelW = NULL;
	switch (m_dwLogLevel)
	{
	case DBG_LVL_ALWAYS:
		lpLogLevelW = _T("always");
		break;	
	case DBG_LVL_ERROR:
		lpLogLevelW = _T("error");
		break;	
	case DBG_LVL_INFO:
		lpLogLevelW = _T("info");
		break;
	case DBG_LVL_DEBUG:
		lpLogLevelW = _T("debug");
		break;
	default:
		break;
	}

	//this->WriteLogExW(DBG_LVL_INFO, __FUNCTIONW__, L"LogLevel = %s", lpLogLevelW);
	return;
}
BOOL CLogFile::WriteData(LPCSTR lpBuffer)
{
	BOOL bRet = TRUE;
	do 
	{
		LARGE_INTEGER li = { 0 };
		li.QuadPart = 0;
		if(!::SetFilePointerEx(m_hLogFile, li, NULL, FILE_END))
		{
			bRet = FALSE;
			break;
		}

		DWORD dwWrite = 0;
		DWORD dwLen = 0;
		dwLen = strlen(lpBuffer);

		if (!::WriteFile(m_hLogFile, lpBuffer, dwLen, &dwWrite, NULL) || dwLen != dwWrite)
		{
			bRet = FALSE;
			break;
		}		
	} while (FALSE);
	return bRet;
}
BOOL CLogFile::IsCurrentLogLevelLog(DWORD dwLogLevel)
{
	return dwLogLevel <= m_dwLogLevel;//等级小于等于配置输出
}

unsigned WINAPI CLogFile::ClearHistroyLogThread(void* pVoid)
{
	CLogFile *pLogFile = (CLogFile*)pVoid;
	pLogFile->ClearHistroyLog();
	return 0;
}
void CLogFile::ClearHistroyLog()
{
	WCHAR szLogDirectory[MAX_PATH] = { 0 };

	this->InitLogFilePath(FALSE);
	_ASSERT(wcslen(m_szLogFilePath));	

	wcsncpy_s(szLogDirectory, m_szLogFilePath, wcslen(m_szLogFilePath));
	PathRemoveFileSpec(szLogDirectory);
	PathRemoveFileSpec(szLogDirectory);

	WIN32_FIND_DATAW FindData = { 0 };
	HANDLE hHandle = NULL;
	wchar_t szFullFilePath[MAX_PATH] = { 0 };

	wcsncpy_s(szFullFilePath, szLogDirectory, wcslen(szLogDirectory));
	PathAppend(szFullFilePath, L"*.*");

	while(!m_bClear)
	{
		this->InitLogConfig();
		hHandle = FindFirstFile(szFullFilePath, &FindData);
		if(GOOD_HANDLE(hHandle))
		{
			do {
				wchar_t szFilePath[MAX_PATH] = { 0 };
				if (0 == wcscmp(FindData.cFileName, L".") || 0 == wcscmp(FindData.cFileName, L".."))
					continue;

				wsprintf(szFilePath, L"%s\\%s", szLogDirectory, FindData.cFileName);
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
					this->IsHistoryLogDelete(FindData.ftCreationTime))
				{
					RemoveDirectoryRecurve(szFilePath);
				}
			} while (::FindNextFile(hHandle, &FindData));
		}

		if (GOOD_HANDLE(hHandle))
			::FindClose(hHandle);
		::RemoveDirectory(szLogDirectory);

		for(int i = 0; i < 24 * 60; i++)
		{
			Sleep(60 * 1000);
		}
	}
	return;
}
BOOL  CLogFile::IsHistoryLogDelete(const FILETIME ft)
{
	UINT nDay = 0;
	time_t tCurTime = 0;
	time_t tFileTime = 0;

	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);

	tCurTime = SYSTEMTIMEToTimeStamp(st);

	tFileTime = FILETIMEToTimeStamp(ft);
#pragma warning(push)
#pragma warning(disable:4244)
	nDay = (tCurTime - tFileTime) / ( 60 * 60 * 24);
#pragma warning(pop)

	return nDay > m_dwHistroyInternalDay ? TRUE : FALSE;
}

#endif