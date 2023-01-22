
// DirScanDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DirScan.h"
#include "DirScanDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_FILENAME (WM_USER + 1)
#define WM_DIRNAME (WM_USER + 2)
#define WM_FILEATT (WM_USER + 3)
#define WM_FILESIZE (WM_USER + 4)
#define WM_FILEWRITETIME (WM_USER + 5)
#define WM_FILESECATT (WM_USER + 6)

WCHAR szPath[MAX_PATH] = { 0 };

static VOID DispaFileName()
{
	HANDLE hEvent = FindFirstChangeNotification(szPath, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
	HWND hWnd = FindWindow(NULL, L"文件监控器");
	while (true)
	{
		DWORD dwObject = WaitForSingleObject(hEvent, INFINITE);
		if (!dwObject)
		{
			PostMessage(hWnd, WM_FILENAME, WPARAM(0), LPARAM(0));
			FindNextChangeNotification(hEvent);
		}
	}
}

static VOID DispaDirName()
{
	HANDLE hEvent = FindFirstChangeNotification(szPath, TRUE, FILE_NOTIFY_CHANGE_DIR_NAME);
	HWND hWnd = FindWindow(NULL, L"文件监控器");
	while (true)
	{
		DWORD dwObject = WaitForSingleObject(hEvent, INFINITE);
		if (!dwObject)
		{
			PostMessage(hWnd, WM_DIRNAME, WPARAM(0), LPARAM(0));
			FindNextChangeNotification(hEvent);
		}
	}
}

static VOID DispaFileAtt()
{
	HANDLE hEvent = FindFirstChangeNotification(szPath, TRUE, FILE_NOTIFY_CHANGE_ATTRIBUTES);
	HWND hWnd = FindWindow(NULL, L"文件监控器");
	while (true)
	{
		DWORD dwObject = WaitForSingleObject(hEvent, INFINITE);
		if (!dwObject)
		{
			PostMessage(hWnd, WM_FILEATT, WPARAM(0), LPARAM(0));
			FindNextChangeNotification(hEvent);
		}
	}
}

static VOID DispaFileSize()
{
	HANDLE hEvent = FindFirstChangeNotification(szPath, TRUE, FILE_NOTIFY_CHANGE_SIZE);
	HWND hWnd = FindWindow(NULL, L"文件监控器");
	while (true)
	{
		DWORD dwObject = WaitForSingleObject(hEvent, INFINITE);
		if (!dwObject)
		{
			PostMessage(hWnd, WM_FILESIZE, WPARAM(0), LPARAM(0));
			FindNextChangeNotification(hEvent);
		}
	}
}

static VOID DispaFileWriteTime()
{
	HANDLE hEvent = FindFirstChangeNotification(szPath, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE);
	HWND hWnd = FindWindow(NULL, L"文件监控器");
	while (true)
	{
		DWORD dwObject = WaitForSingleObject(hEvent, INFINITE);
		if (!dwObject)
		{
			PostMessage(hWnd, WM_FILEWRITETIME, WPARAM(0), LPARAM(0));
			FindNextChangeNotification(hEvent);
		}
	}
}

static VOID DispaFileSecAtt()
{
	HANDLE hEvent = FindFirstChangeNotification(szPath, TRUE, FILE_NOTIFY_CHANGE_SECURITY);
	HWND hWnd = FindWindow(NULL, L"文件监控器");
	while (true)
	{
		DWORD dwObject = WaitForSingleObject(hEvent, INFINITE);
		if (!dwObject)
		{
			PostMessage(hWnd, WM_FILESECATT, WPARAM(0), LPARAM(0));
			FindNextChangeNotification(hEvent);
		}
	}
}

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


// CDirScanDlg 对话框



CDirScanDlg::CDirScanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRSCAN_DIALOG, pParent)
	, m_Log(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Dir);
	DDX_Control(pDX, IDC_CHECK1, m_FileName);
	DDX_Control(pDX, IDC_CHECK2, m_DirName);
	DDX_Control(pDX, IDC_CHECK3, m_FileAtt);
	DDX_Control(pDX, IDC_CHECK4, m_FileSize);
	DDX_Control(pDX, IDC_CHECK5, m_FileWriteTime);
	DDX_Control(pDX, IDC_CHECK6, m_FileSecAtt);
	DDX_Control(pDX, IDC_EDIT2, m_EditLog);
	DDX_Text(pDX, IDC_EDIT2, m_Log);
}

BEGIN_MESSAGE_MAP(CDirScanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDirScanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDirScanDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDirScanDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDirScanDlg 消息处理程序

BOOL CDirScanDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDirScanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDirScanDlg::OnPaint()
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
HCURSOR CDirScanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 选择按钮
void CDirScanDlg::OnBnClickedButton1()
{
	memset(szPath, 0, MAX_PATH);
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = L"选择目录";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(lp, szPath);
	if (!wcscmp(szPath, L""))
	{
		AfxMessageBox(L"选择目录失败！");
	}
	else
	{
		AfxMessageBox(L"选择目录成功！");
		m_Dir.SetWindowText(szPath);
	}
}

// 退出程序按钮
void CDirScanDlg::OnBnClickedButton3()
{
	OnOK();
}


BOOL CDirScanDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_FILENAME:
	{
		UpdateData(TRUE);
		if (m_Log.IsEmpty())
		{
			m_Log = L"文件名称被改变了";
		}
		else
		{
			m_Log = m_Log + L"\r\n" + L"文件名称被改变了";
		}
		UpdateData(FALSE);
		m_EditLog.LineScroll(m_EditLog.GetLineCount()); // 获取总行数，设置刷新到最后一行
		break;
	}
	case WM_DIRNAME:
	{
		UpdateData(TRUE);
		if (m_Log.IsEmpty())
		{
			m_Log = L"目录名称被改变了";
		}
		else
		{
			m_Log = m_Log + L"\r\n" + L"目录名称被改变了";
		}
		UpdateData(FALSE);
		m_EditLog.LineScroll(m_EditLog.GetLineCount()); // 获取总行数，设置刷新到最后一行
		break;
	}
	case WM_FILEATT:
	{
		UpdateData(TRUE);
		if (m_Log.IsEmpty())
		{
			m_Log = L"文件属性被改变了";
		}
		else
		{
			m_Log = m_Log + L"\r\n" + L"文件属性被改变了";
		}
		UpdateData(FALSE);
		m_EditLog.LineScroll(m_EditLog.GetLineCount()); // 获取总行数，设置刷新到最后一行
		break;
	}
	case WM_FILESIZE:
	{
		UpdateData(TRUE);
		if (m_Log.IsEmpty())
		{
			m_Log = L"文件大小被改变了";
		}
		else
		{
			m_Log = m_Log + L"\r\n" + L"文件大小被改变了";
		}
		UpdateData(FALSE);
		m_EditLog.LineScroll(m_EditLog.GetLineCount()); // 获取总行数，设置刷新到最后一行
		break;
	}
	case WM_FILEWRITETIME:
	{
		UpdateData(TRUE);
		if (m_Log.IsEmpty())
		{
			m_Log = L"文件写入时间被改变了";
		}
		else
		{
			m_Log = m_Log + L"\r\n" + L"文件写入时间被改变了";
		}
		UpdateData(FALSE);
		m_EditLog.LineScroll(m_EditLog.GetLineCount()); // 获取总行数，设置刷新到最后一行
		break;
	}
	case WM_FILESECATT:
	{
		UpdateData(TRUE);
		if (m_Log.IsEmpty())
		{
			m_Log = L"文件安全属性被改变了";
		}
		else
		{
			m_Log = m_Log + L"\r\n" + L"文件安全属性被改变了";
		}
		UpdateData(FALSE);
		m_EditLog.LineScroll(m_EditLog.GetLineCount()); // 获取总行数，设置刷新到最后一行
		break;
	}
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// 初始化线程
void CDirScanDlg::InitThreadFun()
{
	if (m_FileName.GetCheck() == true)
	{
		AfxBeginThread((AFX_THREADPROC)DispaFileName, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	if (m_DirName.GetCheck() == true)
	{
		AfxBeginThread((AFX_THREADPROC)DispaDirName, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	if (m_FileSize.GetCheck() == true)
	{
		AfxBeginThread((AFX_THREADPROC)DispaFileSize, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	if (m_FileAtt.GetCheck() == true)
	{
		AfxBeginThread((AFX_THREADPROC)DispaFileAtt, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	if (m_FileWriteTime.GetCheck() == true)
	{
		AfxBeginThread((AFX_THREADPROC)DispaFileWriteTime, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	if (m_FileSecAtt.GetCheck() == true)
	{
		AfxBeginThread((AFX_THREADPROC)DispaFileSecAtt, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

// 开启监控按钮
void CDirScanDlg::OnBnClickedButton2()
{
	InitThreadFun();
	AfxMessageBox(L"开始监控", MB_OK);
}
