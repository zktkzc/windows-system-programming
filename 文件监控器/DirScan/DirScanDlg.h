
// DirScanDlg.h: 头文件
//

#pragma once


// CDirScanDlg 对话框
class CDirScanDlg : public CDialogEx
{
	// 构造
public:
	CDirScanDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRSCAN_DIALOG };
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
	CEdit m_Dir;
	afx_msg void OnBnClickedButton3();
	CButton m_FileName;
	CButton m_DirName;
	CButton m_FileAtt;
	CButton m_FileSize;
	CButton m_FileWriteTime;
	CButton m_FileSecAtt;
	CEdit m_EditLog;
	CString m_Log;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void InitThreadFun();
	afx_msg void OnBnClickedButton2();
};
