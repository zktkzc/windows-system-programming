
// 010DemoDlg.h: 头文件
//

#pragma once


// CMy010DemoDlg 对话框
class CMy010DemoDlg : public CDialogEx
{
	// 构造
public:
	CMy010DemoDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY010DEMO_DIALOG };
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
	CTreeCtrl m_Tree;
	CListCtrl m_List;
	HTREEITEM hRoot;
	char* szBuffer = nullptr;
	afx_msg void OnOpenFileDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void InitListTitle();
	void InitListData(WCHAR* szPath);
	CProgressCtrl m_Process;
	afx_msg void OnOpenFile();
	afx_msg void OnDeleteNode();
	afx_msg void OnRclickTreeLoadMenu(NMHDR* pNMHDR, LRESULT* pResult);
};
