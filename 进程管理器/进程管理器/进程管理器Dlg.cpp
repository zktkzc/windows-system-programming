
// 进程管理器Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "进程管理器.h"
#include "进程管理器Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD dwPid;

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


// C进程管理器Dlg 对话框



C进程管理器Dlg::C进程管理器Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C进程管理器Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(C进程管理器Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &C进程管理器Dlg::OnRclickList1Menu)
	ON_COMMAND(ID_32773, &C进程管理器Dlg::On15)
	ON_COMMAND(ID_32771, &C进程管理器Dlg::On32771)
	ON_COMMAND(ID_32772, &C进程管理器Dlg::On32772)
	ON_COMMAND(ID_32774, &C进程管理器Dlg::On32774)
END_MESSAGE_MAP()


// C进程管理器Dlg 消息处理程序

BOOL C进程管理器Dlg::OnInitDialog()
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
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 设置整行选中

	m_List.InsertColumn(0, L"进程名", 0, 150);
	m_List.InsertColumn(1, L"父进程", 0, 50);
	m_List.InsertColumn(2, L"进程ID", 0, 50);
	m_List.InsertColumn(3, L"线程数", 0, 50);
	m_List.InsertColumn(4, L"优先级", 0, 50);
	m_List.InsertColumn(5, L"进程路径", 0, 300);

	InitProcessList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C进程管理器Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C进程管理器Dlg::OnPaint()
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
HCURSOR C进程管理器Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 初始化进程列表
void C进程管理器Dlg::InitProcessList()
{
	// 创建进程快照
	HANDLE hToolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	PROCESS_MEMORY_COUNTERS pmc; // 进程内存信息
	WCHAR wcthProcessID[260]; // 进程ID
	WCHAR wcthParentProcessID[260]; // 父进程ID
	WCHAR wccntThreads[260]; // 线程数
	WCHAR wcpcPriClassBase[260]; // 优先级
	WCHAR wcProcessPath[260]; // 进程路径
	WIN32_FIND_DATA wfd;
	ULONG uIndex = 0; // 行索引
	bool ret = Process32First(hToolHelp, &pe);
	if (ret)
	{
		do
		{
			// 打开进程，获取进程句柄
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe.th32ProcessID);
			GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
			GetModuleFileNameEx(hProcess, NULL, wcProcessPath, 260);
			if (hProcess == NULL)
			{
				wsprintf(wcProcessPath, L"System Module");
			}
			wsprintf(wcthParentProcessID, L"%d", pe.th32ParentProcessID);
			wsprintf(wcthProcessID, L"%d", pe.th32ProcessID);
			wsprintf(wccntThreads, L"%d", pe.cntThreads);
			wsprintf(wcpcPriClassBase, L"%d", pe.pcPriClassBase);
			m_List.InsertItem(uIndex, pe.szExeFile);
			m_List.SetItemText(uIndex, 1, wcthParentProcessID);
			m_List.SetItemText(uIndex, 2, wcthProcessID);
			m_List.SetItemText(uIndex, 3, wccntThreads);
			m_List.SetItemText(uIndex, 4, wcpcPriClassBase);
			m_List.SetItemText(uIndex, 5, wcProcessPath);
			uIndex++;
		} while (Process32Next(hToolHelp, &pe));
	}
}


void C进程管理器Dlg::OnRclickList1Menu(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT pt = {};
	GetCursorPos(&pt); // 获取鼠标位置
	HMENU hMenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MENU1)); // 加载右键菜单
	hMenu = GetSubMenu(hMenu, 0);
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	*pResult = 0;
}

// 刷新列表
void C进程管理器Dlg::On15()
{
	m_List.DeleteAllItems();
	InitProcessList();
}

// 创建进程
void C进程管理器Dlg::On32771()
{
	CFileDialog fileDlg(true, L"exe", L"*.exe", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"exe files (*.exe)||", NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		CString csProcessPath = fileDlg.GetPathName();
		STARTUPINFO si = {};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi;
		CreateProcess(csProcessPath, NULL, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	}
}

// 结束进程
void C进程管理器Dlg::On32772()
{
	// 获取选中行
	DWORD dwPos = (DWORD)m_List.GetFirstSelectedItemPosition();
	dwPos = dwPos - 1;
	// 获取进程ID
	CString cProcessID = m_List.GetItemText(dwPos, 2);
	int nProcessID = _ttoi(cProcessID);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nProcessID);
	// 结束进程
	//TerminateProcess(hProcess, nProcessID);
	char buffer[260];
	sprintf(buffer, "taskkill /pid %d -t -f", nProcessID);
	system(buffer);
}

// 查看模块
void C进程管理器Dlg::On32774()
{
	// 获取选中行
	DWORD dwPos = (DWORD)m_List.GetFirstSelectedItemPosition();
	dwPos = dwPos - 1;
	// 获取进程ID
	CString cProcessID = m_List.GetItemText(dwPos, 2);
	int nProcessID = _ttoi(cProcessID);
	dwPid = nProcessID;

	CMODULE m_Module;
	m_Module.DoModal(); // 显示窗口
}
