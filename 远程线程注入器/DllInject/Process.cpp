// Process.cpp: 实现文件
//

#include "pch.h"
#include "DllInject.h"
#include "afxdialogex.h"
#include "Process.h"
#include <TlHelp32.h>
#include <process.h>
#include <Psapi.h>

#define WM_PROCESS_ID (WM_USER + 1)

// CProcess 对话框

IMPLEMENT_DYNAMIC(CProcess, CDialogEx)

CProcess::CProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CProcess::~CProcess()
{
}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ProcessList);
}


BEGIN_MESSAGE_MAP(CProcess, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CProcess::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CProcess::OnBnClickedButton1)
END_MESSAGE_MAP()


// CProcess 消息处理程序


BOOL CProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ProcessList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 网格线 整行选中
	m_ProcessList.InsertColumn(0, L"进程名", 0, 150);
	m_ProcessList.InsertColumn(1, L"进程ID", 0, 80);
	InitProcessList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProcess::InitProcessList()
{
	m_ProcessList.DeleteAllItems();
	HANDLE hTool = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 创建进程快照
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	WCHAR wcth32ProcessID[MAX_PATH];
	ULONG uIndex = 0;
	BOOL bSuccess = Process32First(hTool, &pe);
	if (bSuccess == TRUE)
	{
		do
		{
			wsprintf(wcth32ProcessID, L"%d", pe.th32ProcessID);
			m_ProcessList.InsertItem(uIndex, pe.szExeFile); // 进程名
			m_ProcessList.SetItemText(uIndex, 1, wcth32ProcessID); // 进程ID
			uIndex++;
		} while (Process32Next(hTool, &pe));
	}
}

// 刷新模块按钮
void CProcess::OnBnClickedButton2()
{
	InitProcessList();
}

// 选择进程按钮
void CProcess::OnBnClickedButton1()
{
	DWORD dwPos = (DWORD)m_ProcessList.GetFirstSelectedItemPosition(); // 获取选中行
	dwPos = dwPos - 1;
	CString ProcessID = m_ProcessList.GetItemText(dwPos, 1); // 获取选中行的进程ID
	int nProcessID = _ttoi(ProcessID);
	CWnd* pWnd = FindWindow(NULL, L"远程线程注入器"); // 获取窗口
	HWND hWnd = pWnd->GetSafeHwnd(); // 获取窗口句柄
	::PostMessage(hWnd, WM_PROCESS_ID, nProcessID, LPARAM(0)); // 发送消息
	OnOK(); // 退出窗口
}
