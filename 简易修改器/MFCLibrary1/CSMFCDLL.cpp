// CSMFCDLL.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "afxdialogex.h"
#include "CSMFCDLL.h"


// CCSMFCDLL 对话框

IMPLEMENT_DYNAMIC(CCSMFCDLL, CDialogEx)

CCSMFCDLL::CCSMFCDLL(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_Value(0)
{

}

CCSMFCDLL::~CCSMFCDLL()
{
}

void CCSMFCDLL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Value);
}


BEGIN_MESSAGE_MAP(CCSMFCDLL, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCSMFCDLL::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCSMFCDLL 消息处理程序


void CCSMFCDLL::OnBnClickedButton1()
{
	UpdateData(TRUE);
	HMODULE hServerDll = GetModuleHandle(L"server.dll"); // 获取模块基址
	DWORD dwTempAddress;
	SIZE_T dwRetNum;
	ReadProcessMemory(GetCurrentProcess(), (LPCVOID)((DWORD)hServerDll + 0x4F518C), &dwTempAddress, 4, &dwRetNum);
	BOOL bFlag = WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwTempAddress + 0x4B0), &m_Value, 4, &dwRetNum);
	if (bFlag)
	{
		AfxMessageBox(L"修改完成！");
	}
	else
	{
		AfxMessageBox(L"修改失败！");
	}
	UpdateData(FALSE);
}
