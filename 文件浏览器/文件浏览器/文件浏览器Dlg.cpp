
// 文件浏览器Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "文件浏览器.h"
#include "文件浏览器Dlg.h"
#include "afxdialogex.h"
#include "CCreateFile.h"
#include "CWriteFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

WCHAR wcPath[MAX_PATH] = { 0 };
CString g_DirFilePath;

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


// C文件浏览器Dlg 对话框



C文件浏览器Dlg::C文件浏览器Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
	, m_DirPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C文件浏览器Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FileList);
	DDX_Text(pDX, IDC_EDIT1, m_DirPath);
}

BEGIN_MESSAGE_MAP(C文件浏览器Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &C文件浏览器Dlg::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &C文件浏览器Dlg::OnRclickList1)
	ON_COMMAND(ID_32771, &C文件浏览器Dlg::On32771)
	ON_COMMAND(ID_32772, &C文件浏览器Dlg::On32772)
	ON_COMMAND(ID_32773, &C文件浏览器Dlg::On32773)
	ON_COMMAND(ID_32774, &C文件浏览器Dlg::On32774)
	ON_COMMAND(ID_32775, &C文件浏览器Dlg::On32775)
	ON_COMMAND(ID_32776, &C文件浏览器Dlg::On32776)
END_MESSAGE_MAP()


// C文件浏览器Dlg 消息处理程序

BOOL C文件浏览器Dlg::OnInitDialog()
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
	m_FileList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_FileList.InsertColumn(0, L"名称", 0, 100);
	m_FileList.InsertColumn(1, L"类型", 0, 50);
	m_FileList.InsertColumn(2, L"路径", 0, 400);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C文件浏览器Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C文件浏览器Dlg::OnPaint()
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
HCURSOR C文件浏览器Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C文件浏览器Dlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	memset(wcPath, 0, MAX_PATH);
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.lpszTitle = L"选择目录";
	bi.ulFlags = 0;
	bi.lParam = 0;
	bi.iImage = 0;
	bi.lpfn = NULL;
	bi.pszDisplayName = wcPath;

	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(lp, wcPath);
	if (!wcscmp(wcPath, L""))
	{
		MessageBox(L"选择失败", L"错误", MB_OK);
		return;
	}
	else
	{
		MessageBox(L"选择成功", L"提示", MB_OK);
		m_DirPath.Format(L"%s", wcPath);
		m_FileList.DeleteAllItems();
		FindDirFile(wcPath);
	}
	UpdateData(FALSE);
}

void C文件浏览器Dlg::FindDirFile(WCHAR* wcDirPath)
{
	WCHAR wcRootPath[1024];
	WCHAR wcSubPath[1024];
	WIN32_FIND_DATA wfdFileData = { 0 };
	DWORD dwIndex = 0;
	wsprintf(wcRootPath, L"%s*.*", wcDirPath);
	HANDLE hFile = FindFirstFile(wcRootPath, &wfdFileData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			memset(wcSubPath, 0, 1024);
			if ('.' == wfdFileData.cFileName[0])
			{
				continue;
			}
			wsprintf(wcSubPath, L"%s%s", wcDirPath, wfdFileData.cFileName);
			if (wfdFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 目录类型
				m_FileList.InsertItem(dwIndex, wfdFileData.cFileName, dwIndex);
				m_FileList.SetItemText(dwIndex, 1, L"目录");
				m_FileList.SetItemText(dwIndex, 2, wcSubPath);
			}
			else
			{
				// 文件类型
				m_FileList.InsertItem(dwIndex, wfdFileData.cFileName, dwIndex);
				m_FileList.SetItemText(dwIndex, 1, L"文件");
				m_FileList.SetItemText(dwIndex, 2, wcSubPath);
			}
			dwIndex++;
		} while (FindNextFile(hFile, &wfdFileData));
	}
}


void C文件浏览器Dlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	dwIndexFlag = pNMItemActivate->iItem; // 获取点击到的是哪一行
	POINT pt = {};
	GetCursorPos(&pt);
	HMENU hMenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MENU1)); // 加载菜单
	hMenu = GetSubMenu(hMenu, 0); // 加载子菜单
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL); // 弹出菜单
	*pResult = 0;
}


void C文件浏览器Dlg::On32771()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	CString csDirPath = m_FileList.GetItemText(dwIndexFlag, 2); // 获取文件夹路径
	CString csDirType = m_FileList.GetItemText(dwIndexFlag, 1); // 获取文件类型
	if (csDirType == L"文件")
	{
		MessageBox(L"该路径不是目录", L"错误", MB_OK);
		return;
	}
	csDirPath += L"\\";
	WCHAR wcDirPath[1024];
	memset(wcDirPath, 0, 1024);
	CStringW strW = CT2CW(csDirPath);
	wcscpy(wcDirPath, strW);
	m_FileList.DeleteAllItems();
	FindDirFile(wcDirPath);
	UpdateData(FALSE);
}

// 刷新列表
void C文件浏览器Dlg::On32772()
{
	// TODO: 在此添加命令处理程序代码
	m_FileList.DeleteAllItems();
	FindDirFile(wcPath);
}

// 删除文件
void C文件浏览器Dlg::On32773()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	CString csDirType = m_FileList.GetItemText(dwIndexFlag, 1);
	CString csFilePath = m_FileList.GetItemText(dwIndexFlag, 2);
	if (csDirType == L"文件")
	{
		BOOL bRet = DeleteFile(csFilePath);
		if (bRet)
		{
			m_FileList.DeleteAllItems();
			FindDirFile(wcPath);
		}
		else
		{
			MessageBox(L"删除失败！", L"错误", MB_OK);
		}
		return;
	}
	else
	{
		MessageBox(L"无法删除目录！", L"提示", MB_OK);
		return;
	}
	UpdateData(FALSE);
}

// 文本模式打开文件
void C文件浏览器Dlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	CString csDirType = m_FileList.GetItemText(dwIndexFlag, 1);
	if (csDirType == L"文件")
	{
		CString csFilePath = m_FileList.GetItemText(dwIndexFlag, 2);
		CString csTemp = L"notepad.exe ";
		csFilePath = csTemp + csFilePath;
		USES_CONVERSION;
		char* pPath = T2A(csFilePath.GetBuffer(0));
		WinExec(pPath, SW_SHOWNORMAL);
	}
	else
	{
		MessageBox(L"无法打开目录！", L"提示", MB_OK);
		return;
	}
	UpdateData(FALSE);
}

// 创建文件
void C文件浏览器Dlg::On32775()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	g_DirFilePath = m_DirPath;
	CCreateFile m_CDlg;
	m_CDlg.DoModal();
	FindDirFile(wcPath);
	UpdateData(FALSE);
}

// 写入文件
void C文件浏览器Dlg::On32776()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	g_DirFilePath = m_FileList.GetItemText(dwIndexFlag, 2);
	CWriteFile m_WDlg;
	m_WDlg.DoModal();
	FindDirFile(wcPath);
	UpdateData(FALSE);
}
