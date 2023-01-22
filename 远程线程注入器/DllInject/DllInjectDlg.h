
// DllInjectDlg.h: 头文件
//

#pragma once


// CDllInjectDlg 对话框
class CDllInjectDlg : public CDialogEx
{
	// 构造
public:
	CDllInjectDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLLINJECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	CEdit m_DllPath;
	afx_msg void OnBnClickedButton2();
	DWORD m_Pid;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void InitModuleList();
	CListCtrl m_ModuleList;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	WCHAR* wcModuleName;
	void Inject(DWORD dwPid, TCHAR* szMudulePath);
	void UnInject(DWORD dwPid, TCHAR* szDllName);
	afx_msg void OnBnClickedButton3();
	CString m_csDllPath;
	afx_msg void OnBnClickedButton6();
};
