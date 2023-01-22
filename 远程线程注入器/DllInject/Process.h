#pragma once
#include "afxdialogex.h"


// CProcess 对话框

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcess();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ProcessList;
	virtual BOOL OnInitDialog();
	void InitProcessList();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
