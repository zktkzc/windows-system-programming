#pragma once
#include "afxdialogex.h"
#include "文件浏览器Dlg.h"

// CCreateFile 对话框

class CCreateFile : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateFile)

public:
	CCreateFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCreateFile();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_FileName;
	afx_msg void OnBnClickedButton1();
};
