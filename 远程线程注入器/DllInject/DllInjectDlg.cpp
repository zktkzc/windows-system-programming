
// DllInjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DllInject.h"
#include "DllInjectDlg.h"
#include "afxdialogex.h"
#include "Process.h"
#include <TlHelp32.h>
#include <process.h>
#include <Psapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_PROCESS_ID (WM_USER + 1)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDllInjectDlg 对话框



CDllInjectDlg::CDllInjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLLINJECT_DIALOG, pParent)
	, m_Pid(0)
	, m_csDllPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDllInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_DllPath);
	DDX_Text(pDX, IDC_EDIT2, m_Pid);
	DDX_Control(pDX, IDC_LIST1, m_ModuleList);
	DDX_Text(pDX, IDC_EDIT1, m_csDllPath);
}

BEGIN_MESSAGE_MAP(CDllInjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDllInjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDllInjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDllInjectDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDllInjectDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CDllInjectDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CDllInjectDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDllInjectDlg 消息处理程序

BOOL CDllInjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_ModuleList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ModuleList.InsertColumn(0, L"模块名称", 0, 100);
	m_ModuleList.InsertColumn(1, L"模块基址", 0, 100);
	m_ModuleList.InsertColumn(2, L"模块大小", 0, 100);
	m_ModuleList.InsertColumn(3, L"模块路径", 0, 100);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDllInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDllInjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDllInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 选择模块按钮
void CDllInjectDlg::OnBnClickedButton1()
{
	CFileDialog fileDlg(true, L"dll", L"*.dll", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"dll files(*.dll)||", NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		m_DllPath.SetWindowTextW(fileDlg.GetPathName());
	}
}

// 选择进程按钮
void CDllInjectDlg::OnBnClickedButton2()
{
	CProcess m_ProcessDlg;
	m_ProcessDlg.DoModal();
	UpdateData(TRUE);
	InitModuleList();
	UpdateData(FALSE);
}


BOOL CDllInjectDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_PROCESS_ID:
	{
		UpdateData(TRUE);
		m_Pid = pMsg->wParam;
		UpdateData(FALSE);
		break;
	}
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDllInjectDlg::InitModuleList()
{
	UpdateData(TRUE);
	m_ModuleList.DeleteAllItems();
	WCHAR modBaseAddr[MAX_PATH];
	WCHAR modSize[MAX_PATH];
	ULONG uIndex = 0;
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	HANDLE hTool = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_Pid); // 创建模块快照
	if (hTool != NULL)
	{
		BOOL bRet = Module32First(hTool, &me32);
		if (bRet == TRUE)
		{
			do
			{
				wsprintf(modBaseAddr, L"%X", me32.modBaseAddr);
				wsprintf(modSize, L"%d 字节", me32.modBaseSize);
				m_ModuleList.InsertItem(uIndex, me32.szModule);
				m_ModuleList.SetItemText(uIndex, 1, modBaseAddr);
				m_ModuleList.SetItemText(uIndex, 2, modSize);
				m_ModuleList.SetItemText(uIndex, 3, me32.szExePath);
				uIndex++;
			} while (Module32Next(hTool, &me32));
		}
		CloseHandle(hTool);
	}
	else
	{
		AfxMessageBox(L"初始化模块列表失败！");
	}
	UpdateData(FALSE);
}

// 刷新模块按钮
void CDllInjectDlg::OnBnClickedButton4()
{
	UpdateData(TRUE);
	if (m_Pid)
	{
		InitModuleList();
	}
	UpdateData(FALSE);
}

// 选择模块按钮
void CDllInjectDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	DWORD dwPos = (DWORD)m_ModuleList.GetFirstSelectedItemPosition();
	dwPos = dwPos - 1;
	CString ModuleName = m_ModuleList.GetItemText(dwPos, 1);
	wcModuleName = ModuleName.AllocSysString();
	AfxMessageBox(L"选择模块成功！");
	UpdateData(FALSE);
}

// 注入
void CDllInjectDlg::Inject(DWORD dwPid, TCHAR* szMudulePath)
{
	size_t dwWriteSize = 0;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid); // 获取进程句柄
	LPVOID pAddress = VirtualAllocEx(hProcess, NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAddress == NULL)
	{
		AfxMessageBox(L"注入失败！");
		return;
	}
	WriteProcessMemory(hProcess, pAddress, szMudulePath, ((wcslen(szMudulePath) + 1) * 2), &dwWriteSize);
	// 创建远程线程
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, pAddress, NULL, NULL);
	if (hRemoteThread == NULL)
	{
		AfxMessageBox(L"注入失败！");
		return;
	}
	WaitForSingleObject(hRemoteThread, INFINITE);
	VirtualFreeEx(hProcess, pAddress, ((wcslen(szMudulePath) + 1) * 2), MEM_RELEASE);
	CloseHandle(hProcess);
	CloseHandle(hRemoteThread);
	AfxMessageBox(L"完成注入！");
}

void CDllInjectDlg::UnInject(DWORD dwPid, TCHAR* szDllName)
{
	ULONG uIndex = 0;
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bFlag = FALSE;
	HANDLE hTool = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE32, m_Pid); // 创建进程和模块快照
	if (hTool != NULL)
	{
		BOOL bRet = Module32First(hTool, &me32);
		if (bRet == TRUE)
		{
			do
			{
				bFlag = (_tcsicmp(me32.szModule, szDllName) == 0 || _tcsicmp(me32.szExePath, szDllName));
				if (bFlag)
				{
					break;
				}
				uIndex++;
			} while (Module32Next(hTool, &me32));
		}
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		if (hProcess == NULL)
		{
			AfxMessageBox(L"卸载模块失败！");
			return;
		}
		LPTHREAD_START_ROUTINE lpThreadFun = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "FreeLibrary");
		if (lpThreadFun == NULL)
		{
			AfxMessageBox(L"卸载模块失败！");
			return;
		}
		// 远程卸载模块
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, me32.modBaseAddr, 0, NULL);
		if (hThread == NULL)
		{
			AfxMessageBox(L"卸载模块失败！");
			return;
		}
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		CloseHandle(hProcess);
		CloseHandle(hTool);
		AfxMessageBox(L"卸载模块成功！");
		return;
	}
}

// 注入模块按钮
void CDllInjectDlg::OnBnClickedButton3()
{
	UpdateData(TRUE);
	WCHAR* wcModulePath = m_csDllPath.AllocSysString();
	Inject(m_Pid, wcModulePath);
	UpdateData(FALSE);
}

// 卸载模块按钮
void CDllInjectDlg::OnBnClickedButton6()
{
	UpdateData(TRUE);
	UnInject(m_Pid, wcModuleName);
	UpdateData(FALSE);
}
