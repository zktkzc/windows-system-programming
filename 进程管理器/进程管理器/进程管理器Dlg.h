
// 进程管理器Dlg.h: 头文件
//

#pragma once
#include "afxcmn.h"
#include "psapi.h"
#include <process.h>
#include <TlHelp32.h>
#include "CMODULE.h"

extern DWORD dwPid;


// C进程管理器Dlg 对话框
class C进程管理器Dlg : public CDialogEx
{
	// 构造
public:
	C进程管理器Dlg(CWnd* pParent = nullptr);	// 标准构造函数

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
	CListCtrl m_List;
	// 初始化进程列表
	void InitProcessList();
	afx_msg void OnRclickList1Menu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void On15();
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32774();
};
