
// ThreadTRKDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ThreadTRK.h"
#include "ThreadTRKDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CThreadTRKDlg 对话框



CThreadTRKDlg::CThreadTRKDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADTRK_DIALOG, pParent)
	, m_PID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadTRKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ThreadList);
	DDX_Text(pDX, IDC_EDIT1, m_PID);
}

BEGIN_MESSAGE_MAP(CThreadTRKDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CThreadTRKDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CThreadTRKDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CThreadTRKDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CThreadTRKDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CThreadTRKDlg 消息处理程序

BOOL CThreadTRKDlg::OnInitDialog()
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

	m_ThreadList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ThreadList.InsertColumn(0, L"线程ID", 0, 60);
	m_ThreadList.InsertColumn(1, L"所属进程ID", 0, 150);
	m_ThreadList.InsertColumn(2, L"线程优先级", 0, 150);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CThreadTRKDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThreadTRKDlg::OnPaint()
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
HCURSOR CThreadTRKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 遍历线程
void CThreadTRKDlg::OnBnClickedButton1()
{
	m_ThreadList.DeleteAllItems();
	CString csth32ThreadID; // 线程ID
	CString csth32OwnerProcessID; // 线程所属进程ID
	CString csthBasePri; // 线程优先级
	DWORD dwIndex = 0;
	UpdateData(TRUE);
	HANDLE hThreadSnap = NULL;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, m_PID); // 创建线程快照
	if (hThreadSnap == NULL)
	{
		AfxMessageBox(L"创建线程快照错误！");
		return;
	}
	THREADENTRY32 stcTe32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &stcTe32);
	do
	{
		if (m_PID == stcTe32.th32OwnerProcessID)
		{
			csth32ThreadID.Format(L"%d", stcTe32.th32ThreadID);
			csth32OwnerProcessID.Format(L"%d", stcTe32.th32OwnerProcessID);
			csthBasePri.Format(L"%d", stcTe32.tpBasePri);

			m_ThreadList.InsertItem(dwIndex, csth32ThreadID);
			m_ThreadList.SetItemText(dwIndex, 1, csth32OwnerProcessID);
			m_ThreadList.SetItemText(dwIndex, 2, csthBasePri);
			dwIndex++;
		}
	} while (Thread32Next(hThreadSnap, &stcTe32));
	UpdateData(FALSE);
}

// 暂停线程
void CThreadTRKDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);
	POSITION Pos = m_ThreadList.GetFirstSelectedItemPosition();
	int nSelect = -1;
	nSelect = m_ThreadList.GetNextSelectedItem(Pos);
	CString csThreadID = m_ThreadList.GetItemText(nSelect, 0);
	int nThreadID = _ttoi(csThreadID);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, nThreadID);
	if (hThread == NULL)
	{
		AfxMessageBox(L"获取线程句柄失败！");
		return;
	}
	SuspendThread(hThread);// 暂停线程
	UpdateData(FALSE);
}

// 恢复线程
void CThreadTRKDlg::OnBnClickedButton3()
{
	UpdateData(TRUE);
	POSITION Pos = m_ThreadList.GetFirstSelectedItemPosition();
	int nSelect = -1;
	nSelect = m_ThreadList.GetNextSelectedItem(Pos);
	CString csThreadID = m_ThreadList.GetItemText(nSelect, 0);
	int nThreadID = _ttoi(csThreadID);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, nThreadID);
	if (hThread == NULL)
	{
		AfxMessageBox(L"获取线程句柄失败！");
		return;
	}
	ResumeThread(hThread);// 恢复线程
	UpdateData(FALSE);
}

// 结束线程
void CThreadTRKDlg::OnBnClickedButton4()
{
	UpdateData(TRUE);
	POSITION Pos = m_ThreadList.GetFirstSelectedItemPosition();
	int nSelect = -1;
	nSelect = m_ThreadList.GetNextSelectedItem(Pos);
	CString csThreadID = m_ThreadList.GetItemText(nSelect, 0);
	int nThreadID = _ttoi(csThreadID);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, nThreadID);
	if (hThread == NULL)
	{
		AfxMessageBox(L"获取线程句柄失败！");
		return;
	}
	TerminateThread(hThread, 0); // 结束线程
	OnBnClickedButton1();
	UpdateData(FALSE);
}
