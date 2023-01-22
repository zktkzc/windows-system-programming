
// WindowsManagementDlg.h: 头文件
//

#pragma once


// CWindowsManagementDlg 对话框
class CWindowsManagementDlg : public CDialogEx
{
	// 构造
public:
	CWindowsManagementDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSMANAGEMENT_DIALOG };
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
	CListCtrl m_List_Windows;
	DWORD dwIndex = 0;
	CImageList m_ImgList;
	DWORD dwIndexFlag = 0;
	void InitWindowsList();
	void InsertImage(CString lpImagePath);
	afx_msg void OnRclickListMenu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFlushWindow();
	afx_msg void OnEndTask();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow();
	afx_msg void OnHideWindow();
	afx_msg void OnMaximize();
	afx_msg void OnMinimize();
};
