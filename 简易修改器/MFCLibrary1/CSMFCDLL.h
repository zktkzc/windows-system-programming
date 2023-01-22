#pragma once
#include "afxdialogex.h"


// CCSMFCDLL 对话框

class CCSMFCDLL : public CDialogEx
{
	DECLARE_DYNAMIC(CCSMFCDLL)

public:
	CCSMFCDLL(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCSMFCDLL();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 子弹数量
	DWORD m_Value;
	afx_msg void OnBnClickedButton1();
};
