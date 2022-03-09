#pragma once

#include <Windows.h>
#include "logs/log/LogInfo.h"

#include <tchar.h>
using namespace std;


#define EXPORT_ZIP		L"HC.zip"
#define EXPORT_TXT		L"说明.txt"
#define ZIP_ID			1000
#define TXT_ID			1001

class CInstallMgr
{
public:
	CInstallMgr();
	~CInstallMgr();

public:
	BOOL	Install();

private:
	BOOL	MoveFileExDisableWow64FsRedirection(const wchar_t* lpExistingFileName, const wchar_t* lpNewFileName, unsigned long dwFlags);
	BOOL	GetSecurityDescriptor(__in const wchar_t* lpDir, __inout	LPSECURITY_ATTRIBUTES lpSa);
	BOOL	SetSecurity(__in LPTSTR lpFile);
	BOOL	ExtraceFileFromResource(const wchar_t* pFileName, unsigned int uResourceID, const wchar_t* pResourceType, bool bSystemFile, const wchar_t* lpDirSa);

	BOOL	ExtraceFileFromResourceEx(const wchar_t* strFileName, unsigned int uResourceID, const wchar_t* strResourceType, bool bSystemFile);

	BOOL	IHIsX64();
	//循环创建文件夹 C盘需要管理员权限，程序需要以管理员启动
	BOOL	IHCreateDir(const wchar_t* wszDir);
	BOOL	IHExortFile(const wchar_t* pDestDir, const wchar_t* pDestName, unsigned int nId, const wchar_t* pResourceType);



	wstring	GetTestDir();
	BOOL	ExportResourceFiles();	
};

