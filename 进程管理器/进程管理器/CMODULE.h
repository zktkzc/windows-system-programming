#pragma once
#include "afxdialogex.h"


// CMODULE 对话框

class CMODULE : public CDialogEx
{
	DECLARE_DYNAMIC(CMODULE)

public:
	CMODULE(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMODULE();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ModuleList;
	virtual BOOL OnInitDialog();
	void InitModuleList();
};
