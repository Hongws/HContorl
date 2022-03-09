// HContorl.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "InstallMgr.h"


// 隐藏控制台
#pragma comment(linker,"/subsystem:windows /entry:wmainCRTStartup")

int _tmain(int argc, _TCHAR* argv[])
{
	MessageBox(nullptr, L"--1--", nullptr, 0);
	InitLogInfo();
	InitMiniDumpInfo();
	WriteLogExW(__FUNCTIONW__, DBG_LVL_INFO, L"HControl:exist the installing.....");
	//唯一性
	HANDLE hMutex = CreateMutexW(NULL, TRUE, L"Global\\HContol_{2C288998-4683-4f83-B1C0-FF699985DB3F}");
	if (hMutex && ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hMutex);
		WriteLogExW(__FUNCTIONW__, DBG_LVL_ERROR, L"HControl:exist the installing.....");
		return 0;
	}

	//命令行 本身exe占有1
	for (int i = 0; i < argc; i++)
	{			
		OutputDebugString(argv[i]);
		OutputDebugString(L"\n");
	}

	CInstallMgr mgr;
	mgr.Install();

	MessageBox(nullptr, L"--2--", nullptr, 0);
	if (hMutex)
		CloseHandle(hMutex);

	return 0;
}
