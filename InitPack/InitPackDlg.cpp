
// InitPackDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "InitPack.h"
#include "InitPackDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInitPackDlg 对话框



CInitPackDlg::CInitPackDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INITPACK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInitPackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInitPackDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CInitPackDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CInitPackDlg 消息处理程序

BOOL CInitPackDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInitPackDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInitPackDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInitPackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CInitPackDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//std::string strExeFile = "D:\\testsoft\\InitPack\\Release\\HContorl.exe";
	wchar_t wszTmpFilePath[260] = { 0 };
	GetModuleFileName(NULL, wszTmpFilePath, MAX_PATH);
	PathRemoveFileSpec(wszTmpFilePath);
	std::wstring wstrTmpFilePath(wszTmpFilePath);
	std::wstring wstrExeFile = wstrTmpFilePath + L"\\HContorl.exe";
	PathRemoveFileSpec(wszTmpFilePath);
	PathAppend(wszTmpFilePath, L"\\Res\\HC.zip");
	std::wstring wstrImportFile(wszTmpFilePath);
	PathRemoveFileSpec(wszTmpFilePath);
	PathAppend(wszTmpFilePath, L"\\说明.txt");
	std::wstring wstrImportTxt(wszTmpFilePath);
	//std::wstring wstrImportDoc = 
	//std::string strImportFile = "D:\\testsoft\\InitPack\\Release\\HC.zip";
	ImportFileToSetupResourceW(wstrExeFile, wstrImportFile, 1000, L"EXEDATA");
	//std::string strImportDoc = "D:\\testsoft\\InitPack\\Release\\说明.txt";
	ImportFileToSetupResourceW(wstrExeFile, wstrImportTxt, 1001, L"EXEDATA");
	
}

BOOL CInitPackDlg::ImportFileToSetupResource(std::string strExeFile, std::string strImportFile, UINT uResourceID, const char* pResourceType)
{
	bool bSuccess = FALSE;
	if (!PathFileExistsA(strExeFile.c_str()))
	{
		return FALSE;
	}
	if (!PathFileExistsA(strImportFile.c_str()))
	{
		return FALSE;
	}
	FILE* pF = NULL;
	int error = fopen_s(&pF, strImportFile.c_str(), "rb");
	if (0 != error || pF == NULL)
	{
		return FALSE;
	}
	fseek(pF, 0, SEEK_END);
	DWORD dwFileSize = ftell(pF);
	fseek(pF, 0, SEEK_SET);

	char* pFileBuffer = new char[dwFileSize];
	fread(pFileBuffer, sizeof(char), dwFileSize, pF);
	fclose(pF);

	CString strFileSize;
	strFileSize.Format(L"%d", uResourceID);
	OutputDebugString(strFileSize);
	HANDLE hRes = BeginUpdateResourceA(strExeFile.c_str(), FALSE);
	if (UpdateResourceA(hRes, pResourceType, MAKEINTRESOURCEA(uResourceID), MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), pFileBuffer, dwFileSize))
	{
		SetLastError(0);
		int ret = EndUpdateResourceA(hRes, FALSE);
		int Count = 0;
		while (!ret)
		{
			DWORD dwError;
			dwError = GetLastError();
			CString strFileSize;
			strFileSize.Format(L"%d----------%d", dwFileSize, dwError);
			OutputDebugString(strFileSize);
			CString strError;
			strError.Format(_T("----------------Error%d"), Count);
			OutputDebugString(strError);
			Count++;
			Sleep(100);
			HANDLE hResNew = BeginUpdateResourceA(strExeFile.c_str(), FALSE);
			if (UpdateResourceA(hResNew, pResourceType, MAKEINTRESOURCEA(uResourceID), MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), pFileBuffer, dwFileSize))
			{
				SetLastError(0);
				ret = EndUpdateResourceA(hResNew, FALSE);
			}


			if (Count > 100)
			{
				OutputDebugString(L"Count > 100");
				break;
			}
		}
		bSuccess = TRUE;
	}
	else
	{
		EndUpdateResourceA(hRes, TRUE);
		CloseHandle(hRes);
	}
	delete[] pFileBuffer;
	return bSuccess;
}

BOOL CInitPackDlg::ImportFileToSetupResourceW(std::wstring strExeFile, std::wstring strImportFile, UINT uResourceID, const wchar_t* pResourceType)
{
	bool bSuccess = FALSE;
	if (!PathFileExistsW(strExeFile.c_str()))
	{
		return false;
	}
	if (!PathFileExistsW(strImportFile.c_str()))
	{
		return false;
	}
	FILE* pF = NULL;
	int error = _wfopen_s(&pF, strImportFile.c_str(), L"rb");
	if (0 != error || pF == NULL)
	{
		return false;
	}
	fseek(pF, 0, SEEK_END);
	DWORD dwFileSize = ftell(pF);
	fseek(pF, 0, SEEK_SET);

	char* pFileBuffer = new char[dwFileSize];
	fread(pFileBuffer, sizeof(char), dwFileSize, pF);
	fclose(pF);

	CString strFileSize;
	strFileSize.Format(L"%d", uResourceID);
	OutputDebugString(strFileSize);
	HANDLE hRes = BeginUpdateResourceW(strExeFile.c_str(), FALSE);
	if (UpdateResourceW(hRes, pResourceType, MAKEINTRESOURCEW(uResourceID), MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), pFileBuffer, dwFileSize))
	{
		SetLastError(0);
		int ret = EndUpdateResourceW(hRes, FALSE);
		int Count = 0;
		while (!ret)
		{
			Count++;
			Sleep(100);
			HANDLE hResNew = BeginUpdateResourceW(strExeFile.c_str(), FALSE);
			if (UpdateResourceW(hResNew, pResourceType, MAKEINTRESOURCEW(uResourceID), MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), pFileBuffer, dwFileSize))
			{
				SetLastError(0);
				ret = EndUpdateResourceW(hResNew, FALSE);
			}

			if (Count > 100)
			{
				OutputDebugString(L"Count > 100");
				break;
			}
		}
		bSuccess = TRUE;
	}
	else
	{
		EndUpdateResourceW(hRes, TRUE);
		CloseHandle(hRes);
	}
	delete[] pFileBuffer;
	return bSuccess;
}