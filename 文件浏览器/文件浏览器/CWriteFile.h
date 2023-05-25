#pragma once
#include "afxdialogex.h"
#include "文件浏览器Dlg.h"

// CWriteFile 对话框

class CWriteFile : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteFile)

public:
	CWriteFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWriteFile();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_WriteData;
	afx_msg void OnBnClickedButton1();
};
