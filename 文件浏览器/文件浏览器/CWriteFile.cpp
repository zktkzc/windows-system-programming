// CWriteFile.cpp: 实现文件
//

#include "pch.h"
#include "文件浏览器.h"
#include "afxdialogex.h"
#include "CWriteFile.h"


// CWriteFile 对话框

IMPLEMENT_DYNAMIC(CWriteFile, CDialogEx)

CWriteFile::CWriteFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_WriteData(_T(""))
{

}

CWriteFile::~CWriteFile()
{
}

void CWriteFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_WriteData);
}


BEGIN_MESSAGE_MAP(CWriteFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWriteFile::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWriteFile 消息处理程序


void CWriteFile::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_WriteData == L"")
	{
		MessageBox(L"写入内容为空！", L"提示", MB_OK);
		return;
	}
	HANDLE hFile = CreateFileW(g_DirFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"打开文件失败！", L"错误", MB_OK);
		return;
	}
	BOOL bRet = WriteFile(hFile, m_WriteData, m_WriteData.GetLength(), NULL, NULL);
	if (bRet)
	{
		MessageBox(L"写入成功！", L"提示", MB_OK);
		OnOK();
	}
	else
	{
		MessageBox(L"写入失败！", L"提示", MB_OK);
		return;
	}
	UpdateData(FALSE);
}
