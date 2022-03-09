#pragma once
#ifndef _LOG_INFO_H
#define  _LOG_INFO_H
#if _MSC_VER
#if _MSC_VER <= 1800 //MS VC++ 12.0 _MSC_VER = 1800 (VisualStudio 2013)
#else
#include "../CrossPlatform/CrossPlatformDef.h"
#endif
#else
#include "../CrossPlatform/CrossPlatformDef.h"
#endif
#define CP_PLATFORM_WINDOWS
#ifdef CP_PLATFORM_WINDOWS
#include <atlbase.h>
#include <shlobj.h>
#include <yvals.h>
#include <stdio.h>
#include <DbgHelp.h>
#pragma comment( lib, "Dbghelp.lib" )
#endif

/********************************************
#1.支持日志级别
#2.在程序运行时通过配置LogConfig.ini文件动态改变日志输出
#3.支持多线程;
#4.不独占日志文件;
#5.自动清理历史日志,默认间隔7天,可通过LogConfig.ini文件配置;
#6.自动清理过大的单个日志文件,默认大于10M清理,可通过LogConfig.ini文件配置;
#7.LogConfig.ini配置文件放置DSOC目录下;
#8.LogConfig.ini配置格式:
[LogConfig]
LogLevel=10
HistroyInternalDay=7
SingleLogFileMaxSize=10
**********************************************/

//使用文件流统一写入日志内容的模式开关
//#define FSTREAM_MODE

#define MAX_PATH_RE 1024
#define IF_VALUE_HANDLE(h) if(h != NULL && h != INVALID_HANDLE_VALUE)
#define IF_UNVALUE_HANDLE(h) if(h == NULL || h == INVALID_HANDLE_VALUE)

#ifndef WSTD_DEL
#define WSTD_DEL(x) { if ((x)) { delete (x); (x)=NULL; } }
#endif

#ifndef WSTD_DELM
#define WSTD_DELM(x) { if ((x)) { delete[] (x); (x)=NULL; } }
#endif

#ifndef WSTD_CLOSE_HANDLE
#define WSTD_CLOSE_HANDLE(x) {if((x)){	CloseHandle((x)); (x)=NULL;}}
#endif

#ifndef GOOD_HANDLE
#define GOOD_HANDLE(a) (((a)!=INVALID_HANDLE_VALUE) && ((a)!=NULL))
#endif

#ifndef SHOWINFO_BUFSIZE
#define SHOWINFO_BUFSIZE 4096
#endif

#ifndef __FILENAMEW__
#define __FILENAMEW__ wcsrchr(__FILEW__,L'\\') ? wcsrchr(__FILEW__,L'\\') + 1 : __FILEW__
#endif

#ifndef __FILENAME__
#define __FILENAME__ strrchr(__FILE__,'\\') ? strrchr(__FILE__,'\\') + 1 : __FILE__
#endif

#ifdef CP_PLATFORM_WINDOWS
void ShowInfoToDbgViewExA(LPCSTR lpFuncName, char *szFormat, ...);

void ShowInfoToDbgViewExW(LPCWSTR lpFuncName, WCHAR *wszFormat, ...);

LPSTR UnicodeToAnsi(LPCWSTR lpUnicode);
LPWSTR AnsiToUnicode(LPCSTR lpAnsi);

LPSTR UnicodeToUtf8(LPCWSTR lpUnicode);
LPWSTR Utf8ToUnicode(LPCSTR lpUtf8);
#endif

#ifndef __FUNCTIONT__
#ifdef UNICODE
#define __FUNCTIONT__ __FUNCTIONW__
#else
#define __FUNCTIONT__ __FUNCTION__
#endif // !UNICODE
#endif // !__FUNCTIONT__

#ifdef CP_PLATFORM_WINDOWS
LPSTR AnsiToUtf8(LPCSTR lpAnsi);
LPSTR Utf8ToAnsi(LPCSTR lpUtf8);

static BOOL IsWow64();
BOOL IsWow64Static();

BOOL RemoveDirectoryRecurve(LPCWSTR lpDirectoryPath);
BOOL CreateDirectoryRecurve(LPCWSTR lpDirectoryPath);
#endif

#ifdef _WIN32
ULONGLONG SYSTEMTIMEToTimeStamp(const SYSTEMTIME st);
ULONGLONG FILETIMEToTimeStamp(const FILETIME ft);
#endif

#ifdef CP_PLATFORM_WINDOWS
// 低于或等于 当前lvl 的日志会被输出
#define DBG_LVL_ALWAYS        0               ///< ALWAYS
#define DBG_LVL_ERROR         10              ///< ERROR
#define DBG_LVL_INFO          20              ///< INFO
#define DBG_LVL_DEBUG         30              ///< DEBUG
#define DBG_LVL_HIDE          40              ///< HIDE

void InitMiniDumpInfo();

void InitLogInfo(LPCTSTR lpDirName = NULL, LPCTSTR lpFileName = NULL);
void UnInitLogInfo();
void StartClearHistroyLogThread();

void WriteLogA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpMessage);
void WriteLogA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpMessage, DWORD dwErrorCode = 0);

void WriteLogW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpMessage);
void WriteLogW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpMessage, DWORD dwErrorCode = 0);

void WriteLogExA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpFormat, ...);
void WriteLogExA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpFormat, ...);

void WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpFormat, ...);
void WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpFormat, ...);

void WriteLogW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpMessage);
void WriteLogExW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpFormat, ...);

void WriteLogA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpMessage);
void WriteLogExA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpFormat, ...);

void WriteLogW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpMessage);
void WriteLogExW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpFormat, ...);

void WriteLogA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpMessage);
void WriteLogExA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpFormat, ...);

void WriteLogW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCWSTR lpMessage);
void WriteLogExW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCWSTR lpFormat, ...);

void WriteLogA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCSTR lpMessage);
void WriteLogExA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, DWORD dwErrorCode, LPCSTR lpFormat, ...);

//重构下路径，当路径中存在一个'%'时，打印会奔溃
char* ReWriteLogPathA(char* dest, const char* src);
wchar_t* ReWriteLogPathW(wchar_t* dest, const wchar_t* src);


#ifdef UNICODE
#define WriteLog WriteLogW
#else
#define WriteLog WriteLogA
#endif // !UNICODE

#ifdef UNICODE
#define WriteLogEx WriteLogExW
#else
#define WriteLogEx WriteLogExA
#endif // !UNICODE


/*-------------------------------------------------*/
class  CLogFile{
public:
	CLogFile(LPCWSTR lpDirName = NULL, LPCWSTR lpFileName = NULL);
	~CLogFile();

	void WriteLogA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpMessage);
	void WriteLogA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpMessage);
	
	void WriteLogW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpMessage);
	void WriteLogW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpMessage);

	void WriteLogExA(DWORD dwLogLevel, LPCSTR lpFuncName, LPCSTR lpFormat, ...);
	void WriteLogExA(DWORD dwLogLevel, LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, LPCSTR lpFormat, ...);

	void WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFuncName, LPCWSTR lpFormat, ...);
	void WriteLogExW(DWORD dwLogLevel, LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, LPCWSTR lpFormat, ...);

	void WriteLogW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpMessage);
	void WriteLogExW(LPCWSTR lpFuncName, DWORD dwLogLevel, LPCWSTR lpFormat, ...);

	void WriteLogA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpMessage);
	void WriteLogExA(LPCSTR lpFuncName, DWORD dwLogLevel, LPCSTR lpFormat, ...);

	void WriteLogW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpMessage);
	void WriteLogExW(LPCWSTR lpFileName, LPCWSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCWSTR lpFormat, ...);

	void WriteLogA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpMessage);
	void WriteLogExA(LPCSTR lpFileName, LPCSTR lpFuncName, DWORD dwCodeLine, DWORD dwLogLevel, LPCSTR lpFormat, ...);

	void ClearHistroyLog();
	void StartClearHistroyLogThread();
private:
	void InitLogFilePath(BOOL bCreate = TRUE);
	BOOL InitLogFile();
	void InitLogConfig();
	BOOL WriteData(LPCSTR lpBuffer);
	BOOL IsCurrentLogLevelLog(DWORD dwLogLevel);

#ifdef _WIN32
	static unsigned WINAPI ClearHistroyLogThread(void* pVoid);
	BOOL IsHistoryLogDelete(const FILETIME ft);
#endif

private:
	#ifdef _WIN32
	CRITICAL_SECTION m_csFileLog;
	HANDLE m_hLogFile;
	#endif
	WCHAR m_szLogFilePath[MAX_PATH];
	WCHAR m_szLogDirName[MAX_PATH];
	WCHAR m_szLogFileName[MAX_PATH];
	DWORD m_dwProcessId;
	DWORD m_dwLogLevel;
	DWORD m_dwSingleLogFileMaxSize;
	DWORD m_dwHistroyInternalDay;
	BOOL m_bClear;
};

#endif
#endif