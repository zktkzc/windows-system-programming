
// 文件浏览器Dlg.h: 头文件
//

#pragma once


// C文件浏览器Dlg 对话框
class C文件浏览器Dlg : public CDialogEx
{
	// 构造
public:
	C文件浏览器Dlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
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
	CListCtrl m_FileList;
	CString m_DirPath;
	DWORD dwIndexFlag = 0;
	afx_msg void OnBnClickedButton1();
	void FindDirFile(WCHAR* wcDirPath);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void On32771();
};
