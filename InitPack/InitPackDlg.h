
// InitPackDlg.h: 头文件
//

#pragma once

#include <string>

// CInitPackDlg 对话框
class CInitPackDlg : public CDialogEx
{
// 构造
public:
	CInitPackDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INITPACK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	BOOL	ImportFileToSetupResource(std::string strExeFile, std::string strImportFile, UINT uResourceID, const char* pResourceType);
	BOOL	ImportFileToSetupResourceW(std::wstring strExeFile, std::wstring strImportFile, UINT uResourceID, const wchar_t* pResourceType);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
