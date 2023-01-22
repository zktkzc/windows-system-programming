// CMODULE.cpp: 实现文件
//

#include "pch.h"
#include "进程管理器.h"
#include "afxdialogex.h"
#include "CMODULE.h"
#include "进程管理器Dlg.h"


// CMODULE 对话框

IMPLEMENT_DYNAMIC(CMODULE, CDialogEx)

CMODULE::CMODULE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMODULE::~CMODULE()
{
}

void CMODULE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ModuleList);
}


BEGIN_MESSAGE_MAP(CMODULE, CDialogEx)
END_MESSAGE_MAP()


// CMODULE 消息处理程序


BOOL CMODULE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ModuleList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ModuleList.InsertColumn(0, L"模块名称", 0, 100);
	m_ModuleList.InsertColumn(1, L"模块基址", 0, 80);
	m_ModuleList.InsertColumn(2, L"模块大小", 0, 100);
	m_ModuleList.InsertColumn(3, L"模块路径", 0, 300);

	InitModuleList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMODULE::InitModuleList()
{
	WCHAR modBaseAddr[260]; // 模块基址
	WCHAR modBaseSize[260]; // 模块大小
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	ULONG uIndex = 0; // 索引
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwPid);
	bool ret = Module32First(hModuleSnap, &me32);
	if (ret)
	{
		do
		{
			wsprintf(modBaseAddr, L"%I64X", me32.modBaseAddr);
			wsprintf(modBaseSize, L"%d字节", me32.modBaseSize);
			m_ModuleList.InsertItem(uIndex, me32.szModule);
			m_ModuleList.SetItemText(uIndex, 1, modBaseAddr);
			m_ModuleList.SetItemText(uIndex, 2, modBaseSize);
			m_ModuleList.SetItemText(uIndex, 3, me32.szExePath);
			uIndex++;
		} while (Module32Next(hModuleSnap, &me32));
		CloseHandle(hModuleSnap);
	}
}
