
// 010DemoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "010Demo.h"
#include "010DemoDlg.h"
#include "afxdialogex.h"

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


// CMy010DemoDlg 对话框



CMy010DemoDlg::CMy010DemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY010DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy010DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PROGRESS3, m_Process);
}

BEGIN_MESSAGE_MAP(CMy010DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32771, &CMy010DemoDlg::OnOpenFileDialog)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_32772, &CMy010DemoDlg::OnOpenFile)
	ON_COMMAND(ID_32773, &CMy010DemoDlg::OnDeleteNode)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CMy010DemoDlg::OnRclickTreeLoadMenu)
END_MESSAGE_MAP()


// CMy010DemoDlg 消息处理程序

BOOL CMy010DemoDlg::OnInitDialog()
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

	hRoot = m_Tree.InsertItem(L"最近打开的文件", NULL, NULL, TVI_ROOT, TVI_LAST);
	InitListTitle();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy010DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy010DemoDlg::OnPaint()
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
HCURSOR CMy010DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy010DemoDlg::OnOpenFileDialog()
{
	CFileDialog cFileDlg(TRUE, NULL, NULL, NULL, L"exe|*.exe|dll|*.dll|all|*.*||", this);
	cFileDlg.DoModal();
	CString csPath = cFileDlg.GetPathName();
	m_Tree.InsertItem(csPath.GetBuffer(0), NULL, NULL, hRoot, TVI_LAST);
	// 插入数据
	InitListData(csPath.GetBuffer(0));
}

// 拖放文件
void CMy010DemoDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TCHAR str[MAX_PATH];
	UINT nValue = DragQueryFile(hDropInfo, -1, NULL, 0);
	if (nValue <= 0)
	{
		return;
	}
	nValue = DragQueryFile(hDropInfo, nValue - 1, str, _countof(str));
	m_Tree.InsertItem(str, NULL, NULL, hRoot, TVI_LAST);
	// 插入数据
	InitListData(str);
	CDialogEx::OnDropFiles(hDropInfo);
}

// 初始化列表表头
void CMy010DemoDlg::InitListTitle()
{
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 网格线 整行选中
	m_List.InsertColumn(0, L"  ", 0, 80);
	m_List.InsertColumn(1, L"0", 0, 30);
	m_List.InsertColumn(2, L"1", 0, 30);
	m_List.InsertColumn(3, L"2", 0, 30);
	m_List.InsertColumn(4, L"3", 0, 30);
	m_List.InsertColumn(5, L"4", 0, 30);
	m_List.InsertColumn(6, L"5", 0, 30);
	m_List.InsertColumn(7, L"6", 0, 30);
	m_List.InsertColumn(8, L"7", 0, 30);
	m_List.InsertColumn(9, L"8", 0, 30);
	m_List.InsertColumn(10, L"9", 0, 30);
	m_List.InsertColumn(11, L"A", 0, 30);
	m_List.InsertColumn(12, L"B", 0, 30);
	m_List.InsertColumn(13, L"C", 0, 30);
	m_List.InsertColumn(14, L"D", 0, 30);
	m_List.InsertColumn(15, L"E", 0, 30);
	m_List.InsertColumn(16, L"F", 0, 30);

	m_List.InsertColumn(17, L"|", 0, 15);

	// ASCII
	m_List.InsertColumn(18, L"0", 0, 20);
	m_List.InsertColumn(19, L"1", 0, 20);
	m_List.InsertColumn(20, L"2", 0, 20);
	m_List.InsertColumn(21, L"3", 0, 20);
	m_List.InsertColumn(22, L"4", 0, 20);
	m_List.InsertColumn(23, L"5", 0, 20);
	m_List.InsertColumn(24, L"6", 0, 20);
	m_List.InsertColumn(25, L"7", 0, 20);
	m_List.InsertColumn(26, L"8", 0, 20);
	m_List.InsertColumn(27, L"9", 0, 20);
	m_List.InsertColumn(28, L"A", 0, 20);
	m_List.InsertColumn(29, L"B", 0, 20);
	m_List.InsertColumn(30, L"C", 0, 20);
	m_List.InsertColumn(31, L"D", 0, 20);
	m_List.InsertColumn(32, L"E", 0, 20);
	m_List.InsertColumn(33, L"F", 0, 20);
}

// 初始化列表数据
void CMy010DemoDlg::InitListData(WCHAR* szPath)
{
	if (szBuffer != nullptr)
	{
		delete szBuffer;
	}
	m_List.DeleteAllItems();
	// 打开文件
	HANDLE hFile = CreateFile(szPath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// 获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	szBuffer = new char[dwFileSize];
	// 清空内存
	memset(szBuffer, 0, dwFileSize);
	DWORD dwReadLength = 0;
	ReadFile(hFile, szBuffer, dwFileSize, &dwReadLength, NULL);
	// 总行数
	DWORD dwLines = (dwReadLength / 16 + 1);
	m_Process.SetRange32(0, dwLines);
	DWORD dwIndex = 0;
	DWORD dwLinesCount = 0;
	CString csValue;
	for (size_t i = 0; i < dwLines; i += 16)
	{
		csValue.Format(L"%08Xh", dwLinesCount);
		m_List.InsertItem(dwIndex, csValue, 100);
		for (size_t j = 0; j < 16; j++)
		{
			UCHAR cCode = szBuffer[i + j];
			csValue.Format(L"%02X", cCode);
			m_List.SetItemText(dwIndex, j + 1, csValue);
		}
		m_List.SetItemText(dwIndex, 17, L"|");
		for (size_t j = 0; j < 16; j++)
		{
			UCHAR cCode = szBuffer[i + j];
			if (cCode == 0)
			{
				csValue.Format(L".");
			}
			else
			{
				csValue.Format(L"%c", cCode);
			}
			m_List.SetItemText(dwIndex, j + 18, csValue);
		}
		dwIndex++;
		dwLinesCount += 0x10;
		m_Process.SetPos(i);
	}
	CloseHandle(hFile);
}


void CMy010DemoDlg::OnOpenFile()
{
	HTREEITEM hNode = m_Tree.GetSelectedItem();
	CString csTemp = m_Tree.GetItemText(hNode);
	InitListData(csTemp.GetBuffer(0));
}


void CMy010DemoDlg::OnDeleteNode()
{
	HTREEITEM hNode = m_Tree.GetSelectedItem();
	m_Tree.DeleteItem(hNode);
}


void CMy010DemoDlg::OnRclickTreeLoadMenu(NMHDR* pNMHDR, LRESULT* pResult)
{
	POINT pt = { 0 };
	GetCursorPos(&pt);
	HMENU hMenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MENU2));
	HMENU hSubMenu = GetSubMenu(hMenu, 0);
	TrackPopupMenu(hSubMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	*pResult = 0;
}
