
// WindowsManagementDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WindowsManagement.h"
#include "WindowsManagementDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <Psapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef BOOL(_stdcall* funIsHungAppWindow)(_In_ HWND hWnd);

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


// CWindowsManagementDlg 对话框



CWindowsManagementDlg::CWindowsManagementDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDOWSMANAGEMENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowsManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List_Windows);
}

BEGIN_MESSAGE_MAP(CWindowsManagementDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CWindowsManagementDlg::OnRclickListMenu)
	ON_COMMAND(ID_32776, &CWindowsManagementDlg::OnFlushWindow)
	ON_COMMAND(ID_32775, &CWindowsManagementDlg::OnEndTask)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CWindowsManagementDlg::OnClickList1)
	ON_COMMAND(ID_32771, &CWindowsManagementDlg::OnShowWindow)
	ON_COMMAND(ID_32772, &CWindowsManagementDlg::OnHideWindow)
	ON_COMMAND(ID_32773, &CWindowsManagementDlg::OnMaximize)
	ON_COMMAND(ID_32774, &CWindowsManagementDlg::OnMinimize)
END_MESSAGE_MAP()


// CWindowsManagementDlg 消息处理程序

BOOL CWindowsManagementDlg::OnInitDialog()
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

	m_List_Windows.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_List_Windows.InsertColumn(0, L"窗口名称", 0, 300);
	m_List_Windows.InsertColumn(1, L"运行状态", 0, 60);
	m_List_Windows.InsertColumn(2, L"进程ID", 0, 50);
	m_List_Windows.InsertColumn(3, L"窗口句柄", 0, 60);
	m_List_Windows.InsertColumn(4, L"窗口路径", 0, 500);

	m_ImgList.Create(18, 18, ILC_MASK | ILC_COLOR32, 0, 1);
	m_List_Windows.SetImageList(&m_ImgList, LVSIL_SMALL); // 设置图标 小图标

	InitWindowsList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWindowsManagementDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWindowsManagementDlg::OnPaint()
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
HCURSOR CWindowsManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWindowsManagementDlg::InitWindowsList()
{
	HINSTANCE hInstance = LoadLibrary(L"user32.dll");
	dwIndex = 0;
	CString csWindowTitle;
	funIsHungAppWindow IsHungAppWindow = (funIsHungAppWindow)GetProcAddress(hInstance, "IsHungAppWindow");
	m_List_Windows.DeleteAllItems();

	// 图标

	CWnd* pWnd = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
	while (pWnd)
	{
		if (pWnd->IsWindowVisible() && !pWnd->GetOwner()) // 窗口可见且为顶层窗口
		{
			pWnd->GetWindowText(csWindowTitle);
			csWindowTitle.TrimLeft();
			csWindowTitle.TrimRight();
			if (!csWindowTitle.IsEmpty())
			{
				TCHAR tcProcessID[MAX_PATH];
				TCHAR tcWindowHandle[MAX_PATH];
				TCHAR tcWindowPath[MAX_PATH];
				DWORD dwProcessID = 0;
				GetWindowThreadProcessId(pWnd->m_hWnd, &dwProcessID);
				wsprintf(tcProcessID, L"%d", dwProcessID);
				m_List_Windows.InsertItem(dwIndex, csWindowTitle, dwIndex);
				if (IsHungAppWindow(pWnd->m_hWnd)) // 确定系统是否认为指定的应用程序未响应
				{
					m_List_Windows.SetItemText(dwIndex, 1, L"未响应");
				}
				else
				{
					m_List_Windows.SetItemText(dwIndex, 1, L"正在运行");
				}
				m_List_Windows.SetItemText(dwIndex, 2, tcProcessID);
				wsprintf(tcWindowHandle, L"%X", pWnd->m_hWnd);
				m_List_Windows.SetItemText(dwIndex, 3, tcWindowHandle);
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
				GetModuleFileNameEx(hProcess, NULL, tcWindowPath, MAX_PATH);
				m_List_Windows.SetItemText(dwIndex, 4, tcWindowPath);
				InsertImage(tcWindowPath);
				dwIndex++;
			}
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CWindowsManagementDlg::InsertImage(CString lpImagePath)
{
	HICON hIcon = NULL;
	hIcon = ::ExtractIcon(NULL, lpImagePath, 0);
	if (hIcon == NULL)
	{
		m_ImgList.Add(LoadIcon(NULL, IDI_APPLICATION));
	}
	else
	{
		m_ImgList.Add(hIcon);
	}
}

void CWindowsManagementDlg::OnRclickListMenu(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	dwIndexFlag = pNMItemActivate->iItem; // 获取当前选中行
	POINT pt = { 0 };
	GetCursorPos(&pt);
	HMENU hMenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MENU1)); // 加载菜单
	hMenu = GetSubMenu(hMenu, 0); // 获取子菜单
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL); // 弹出右键菜单
	*pResult = 0;
}

// 刷新窗口
void CWindowsManagementDlg::OnFlushWindow()
{
	InitWindowsList();
}

// 结束任务
void CWindowsManagementDlg::OnEndTask()
{
	CString csProcessID = m_List_Windows.GetItemText(dwIndexFlag, 2);
	DWORD dwProcessID = _wtoi(csProcessID);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL)
	{
		MessageBox(L"任务结束失败！", L"提示", MB_OK);
	}
	else
	{
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		MessageBox(L"任务结束成功！", L"提示", MB_OK);
		InitWindowsList();
	}
}


void CWindowsManagementDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	dwIndexFlag = pNMItemActivate->iItem; // 获取当前选中行
	*pResult = 0;
}

// 显示窗口
void CWindowsManagementDlg::OnShowWindow()
{
	CString csWindowHandle = m_List_Windows.GetItemText(dwIndexFlag, 3);
	DWORD dwWindowHandle = _tcstoi64(csWindowHandle.GetBuffer(), NULL, 16);
	ShowWindowAsync((HWND)dwWindowHandle, SW_RESTORE);
}

// 隐藏窗口
void CWindowsManagementDlg::OnHideWindow()
{
	CString csWindowHandle = m_List_Windows.GetItemText(dwIndexFlag, 3);
	DWORD dwWindowHandle = _tcstoi64(csWindowHandle.GetBuffer(), NULL, 16);
	ShowWindowAsync((HWND)dwWindowHandle, SW_HIDE);
}

// 最大化
void CWindowsManagementDlg::OnMaximize()
{
	CString csWindowHandle = m_List_Windows.GetItemText(dwIndexFlag, 3);
	DWORD dwWindowHandle = _tcstoi64(csWindowHandle.GetBuffer(), NULL, 16);
	ShowWindowAsync((HWND)dwWindowHandle, SW_MAXIMIZE);
}

// 最小化
void CWindowsManagementDlg::OnMinimize()
{
	CString csWindowHandle = m_List_Windows.GetItemText(dwIndexFlag, 3);
	DWORD dwWindowHandle = _tcstoi64(csWindowHandle.GetBuffer(), NULL, 16);
	ShowWindowAsync((HWND)dwWindowHandle, SW_MINIMIZE);
}
