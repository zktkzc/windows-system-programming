
// FindMemoryDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FindMemory.h"
#include "FindMemoryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD g_Pid = 0; // 进程ID
DWORD g_dwValueAddr[10000] = { 0 }; // 记录搜索到的内存地址的数组
DWORD g_dwListCount = 0; // 搜索后的内存地址总数
HANDLE g_hProcess = NULL; // 进程句柄

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


// CFindMemoryDlg 对话框



CFindMemoryDlg::CFindMemoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDMEMORY_DIALOG, pParent)
	, m_Pid(0)
	, m_ScanValue(0)
	, m_Address(0)
	, m_Value(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFindMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_Pid);
	DDX_Text(pDX, IDC_EDIT2, m_ScanValue);
	DDX_Text(pDX, IDC_EDIT3, m_Address);
	DDX_Text(pDX, IDC_EDIT4, m_Value);
}

BEGIN_MESSAGE_MAP(CFindMemoryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFindMemoryDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFindMemoryDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFindMemoryDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CFindMemoryDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CFindMemoryDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CFindMemoryDlg 消息处理程序

BOOL CFindMemoryDlg::OnInitDialog()
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

	m_List.InsertColumn(0, L"Address", 0, 100);
	m_List.InsertColumn(1, L"Value", 0, 100);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFindMemoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFindMemoryDlg::OnPaint()
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
HCURSOR CFindMemoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CFindMemoryDlg::CompareApage(DWORD dwAddr, DWORD dwValue)
{
	BYTE arrByte[4096] = { 0 };
	DWORD* pNumber;
	if (!ReadProcessMemory(g_hProcess, (LPCVOID)dwAddr, arrByte, 4096, NULL))
	{
		return FALSE;
	}
	else
	{
		for (int i = 0; i < 4096 - 3; i++)
		{
			pNumber = (DWORD*)&arrByte[i];
			if (*pNumber == dwValue)
			{
				if (g_dwListCount >= 10000)
				{
					return FALSE;
				}
				g_dwValueAddr[g_dwListCount++] = dwAddr + i;
			}
		}
	}
}

BOOL CFindMemoryDlg::FindFirst(DWORD dwValue)
{
	const DWORD dwTwoGB = (1024 * 1024 * 1024) * 2;
	int nFlag = 4 * 1024;
	int nBase = 640 * 1024;
	if (g_hProcess == nullptr)
	{
		return FALSE;
	}
	else
	{
		for (int i = 0; i < dwTwoGB; i += nFlag)
		{
			CompareApage(i, dwValue);
		}
	}
	return TRUE;
}

// 再次搜索
BOOL CFindMemoryDlg::FindNext(DWORD dwValue)
{
	int nCount = g_dwListCount;
	g_dwListCount = 0;
	int nFindValue = 0;
	BOOL nFlag = FALSE;
	for (int i = 0; i < nCount; i++)
	{
		if (ReadProcessMemory(g_hProcess, (LPCVOID)g_dwValueAddr[i], &nFindValue, sizeof(nFindValue), NULL))
		{
			if (nFindValue == dwValue)
			{
				g_dwValueAddr[g_dwListCount++] = g_dwValueAddr[i];
				nFlag = TRUE;
			}
		}
	}
	return nFlag;
}


// 设置进程ID按钮
void CFindMemoryDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	g_Pid = m_Pid;
	UpdateData(FALSE);
	AfxMessageBox(L"设置完成！");
}

// 首次搜索按钮
void CFindMemoryDlg::OnBnClickedButton2()
{
	m_List.DeleteAllItems();
	UpdateData(TRUE);
	WCHAR wcAddress[260];
	WCHAR wcValue[260];
	g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_Pid);
	FindFirst(m_ScanValue);
	for (int i = 0; i < g_dwListCount; i++)
	{
		memset(wcAddress, 0, sizeof(WCHAR) * 260);
		memset(wcValue, 0, sizeof(WCHAR) * 260);
		wsprintf(wcAddress, L"%d", g_dwValueAddr[i]);
		wsprintf(wcValue, L"%d", m_ScanValue);
		m_List.InsertItem(i, wcAddress);
		m_List.SetItemText(i, 1, wcValue);
	}
	UpdateData(FALSE);
	AfxMessageBox(L"搜索完成！");
}

// 再次搜索按钮
void CFindMemoryDlg::OnBnClickedButton3()
{
	m_List.DeleteAllItems();
	UpdateData(TRUE);
	WCHAR wcAddress[260];
	WCHAR wcValue[260];
	FindNext(m_ScanValue);
	for (int i = 0; i < g_dwListCount; i++)
	{
		memset(wcAddress, 0, sizeof(WCHAR) * 260);
		memset(wcValue, 0, sizeof(WCHAR) * 260);
		wsprintf(wcAddress, L"%d", g_dwValueAddr[i]);
		wsprintf(wcValue, L"%d", m_ScanValue);
		m_List.InsertItem(i, wcAddress);
		m_List.SetItemText(i, 1, wcValue);
	}
	UpdateData(FALSE);
	AfxMessageBox(L"搜索完成！");
}

// 读取内存按钮
void CFindMemoryDlg::OnBnClickedButton4()
{
	UpdateData(TRUE);
	ReadProcessMemory(g_hProcess, (LPCVOID)m_Address, &m_Value, sizeof(m_Value), NULL);
	UpdateData(FALSE);
	AfxMessageBox(L"读取完成！");
}

// 写入内存按钮
void CFindMemoryDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	WriteProcessMemory(g_hProcess, (LPVOID)m_Address, &m_Value, sizeof(m_Value), NULL);
	UpdateData(FALSE);
	AfxMessageBox(L"写入完成！");
}
