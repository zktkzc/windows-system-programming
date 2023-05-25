// CCreateFile.cpp: 实现文件
//

#include "pch.h"
#include "文件浏览器.h"
#include "afxdialogex.h"
#include "CCreateFile.h"


// CCreateFile 对话框

IMPLEMENT_DYNAMIC(CCreateFile, CDialogEx)

CCreateFile::CCreateFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_FileName(_T(""))
{

}

CCreateFile::~CCreateFile()
{
}

void CCreateFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_FileName);
}


BEGIN_MESSAGE_MAP(CCreateFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCreateFile::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCreateFile 消息处理程序


void CCreateFile::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_FileName == L"")
	{
		MessageBox(L"请输入文件名！", L"提示", MB_OK);
		return;
	}
	else
	{
		CString csTempPath;
		csTempPath = g_DirFilePath + L"\\" + m_FileName;
		HANDLE hFile = CreateFileW(csTempPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(L"创建失败！", L"错误", MB_OK);
			OnOK();
			return;
		}
		else
		{
			MessageBox(L"创建成功！", L"提示", MB_OK);
			OnOK();
			return;
		}
	}
	UpdateData(FALSE);
}
