
// FindMemoryDlg.h: 头文件
//

#pragma once


// CFindMemoryDlg 对话框
class CFindMemoryDlg : public CDialogEx
{
	// 构造
public:
	CFindMemoryDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDMEMORY_DIALOG };
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
	CListCtrl m_List;
	// 进程ID
	DWORD m_Pid;
	// 搜索值
	DWORD m_ScanValue;
	// 内存地址
	DWORD m_Address;
	// 存储数值
	DWORD m_Value;
	BOOL CompareApage(DWORD dwAddr, DWORD dwValue);
	BOOL FindFirst(DWORD dwValue);
	BOOL FindNext(DWORD dwValue);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
