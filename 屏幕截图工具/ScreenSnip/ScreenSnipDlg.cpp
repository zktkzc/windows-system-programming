
// ScreenSnipDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ScreenSnip.h"
#include "ScreenSnipDlg.h"
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


// CScreenSnipDlg 对话框



CScreenSnipDlg::CScreenSnipDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCREENSNIP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenSnipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenSnipDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CScreenSnipDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CScreenSnipDlg 消息处理程序

BOOL CScreenSnipDlg::OnInitDialog()
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

void CScreenSnipDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScreenSnipDlg::OnPaint()
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
HCURSOR CScreenSnipDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScreenSnipDlg::OnBnClickedButton1()
{
	CWnd* pDesktop = GetDesktopWindow(); // 获取桌面窗口
	CDC* pDeskDC = pDesktop->GetDC();
	CRect re;
	pDesktop->GetClientRect(&re); // 获取屏幕大小

	// 创建位图操作类
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDeskDC, re.Width(), re.Height());

	CDC cMemoryDC;
	cMemoryDC.CreateCompatibleDC(pDeskDC); // 创建画板
	CBitmap* pold = cMemoryDC.SelectObject(&bmp); // 创建画笔
	// 开始绘制 
	cMemoryDC.BitBlt(0, 0, re.Width(), re.Height(), pDeskDC, 0, 0, SRCCOPY); // SRCCOPY 将源矩形直接复制到目标矩形
	CPoint po;
	GetCursorPos(&po); // 获取鼠标位置
	HICON icon = (HICON)GetCursor();
	cMemoryDC.DrawIcon(po.x - 10, po.y - 10, icon);
	cMemoryDC.SelectObject(pold); // 选择画笔

	BITMAP bit; // 创建位图
	bmp.GetBitmap(&bit);

	DWORD dwBmSize = bit.bmWidth * bit.bmHeight;
	LPSTR lpData = (LPSTR)GlobalAlloc(GPTR, dwBmSize);

	BITMAPINFOHEADER pBitInfo; // BITMAPINFOHEADER 结构包含与设备无关的位图 (DIB) 的维度和颜色格式的相关信息
	pBitInfo.biBitCount = 24; // 指定每个像素的位数
	pBitInfo.biClrImportant = 0; // 指定在显示位图时被视为重要的颜色索引数。 如果此值为零，则所有颜色都很重要
	pBitInfo.biCompression = BI_RGB; // 压缩的视频和 YUV 格式  BI_RGB —— 未压缩的 RGB
	pBitInfo.biHeight = bit.bmHeight; // 指定位图的高度（以像素为单位）
	pBitInfo.biPlanes = 1; // 指定目标设备的平面数。 此值必须设置为 1
	pBitInfo.biSize = sizeof(BITMAPINFOHEADER); // 指定结构所需的字节数
	pBitInfo.biSizeImage = dwBmSize; // 指定图像的大小（以字节为单位） 对于未压缩的 RGB 位图，可将其设置为 0
	pBitInfo.biWidth = bit.bmWidth; // 指定位图的宽度（以像素为单位）
	pBitInfo.biXPelsPerMeter = 0; // 指定位图的目标设备的水平分辨率（以像素为单位）
	pBitInfo.biYPelsPerMeter = 0; // 指定位图的目标设备的垂直分辨率（以像素为单位）

	// GetDIBits 函数检索指定兼容位图的位，并使用指定格式将其作为 DIB 复制到缓冲区中
	GetDIBits(pDeskDC->m_hDC, bmp, 0, pBitInfo.biHeight, lpData, (BITMAPINFO*)&pBitInfo, DIB_RGB_COLORS);

	BITMAPFILEHEADER bmFileHeader; // BITMAPFILEHEADER 结构包含有关包含 DIB 的文件的类型、大小和布局的信息
	bmFileHeader.bfReserved1 = bmFileHeader.bfReserved2 = 0; // 保留;必须为零
	bmFileHeader.bfType = ((WORD)('M' << 8) | 'B'); // 文件类型;必须是 BM
	bmFileHeader.bfSize = dwBmSize + 54; // 位图文件的大小（以字节为单位
	bmFileHeader.bfOffBits = 54; // 从位图文件头结构的开头到位图位的偏移量（以字节为单位）

	CFile file;
	CString strFileName = AfxGetApp()->m_pszHelpFilePath; // 本软件的路径
	strFileName += L"ScreenSnap\\";

	CreateDirectory(strFileName, NULL); // 创建目录

	CTime m_Time = GetTickCount64();
	CString csTime = m_Time.Format("%Y-%m-%d %H-%M-%S");
	strFileName += csTime;
	strFileName += L".bmp";

	BOOL bIsOpen = file.Open(strFileName, CFile::modeCreate | CFile::modeWrite); // 创建并打开文件
	if (bIsOpen)
	{
		file.Write(&bmFileHeader, sizeof(BITMAPFILEHEADER));
		file.Write(&pBitInfo, sizeof(BITMAPINFOHEADER));
		file.Write(lpData, dwBmSize);
		file.Close();
	}
	else
	{
		AfxMessageBox(L"保存文件失败！");
		return;
	}
	GlobalFree(lpData);
	AfxMessageBox(L"保存文件成功！");
}