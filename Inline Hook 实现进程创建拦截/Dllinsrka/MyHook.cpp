#include "MyHook.h"

CMyHook::CMyHook()
{
	m_FuncAddress = NULL;
	memset(m_OldBytes, 0, 5);
	memset(m_NewBytes, 0, 5);
}

CMyHook::~CMyHook()
{
	UnHook();
	m_FuncAddress = NULL;
	memset(m_OldBytes, 0, 5);
	memset(m_NewBytes, 0, 5);
}

BOOL CMyHook::Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc)
{
	// 获取指定模块中指定函数的地址
	m_FuncAddress = (PROC)GetProcAddress(GetModuleHandle((LPCWSTR)pszModuleName), pszFuncName);
	if (m_FuncAddress == NULL)
	{
		return FALSE;
	}
	SIZE_T dwRet = 0;
	ReadProcessMemory(GetCurrentProcess(), m_FuncAddress, m_OldBytes, 5, &dwRet);
	// 构造5个字节的跳转指令 jmp address
	m_NewBytes[0] = '\xE9';
	// 构造地址
	*(DWORD*)(m_NewBytes + 1) = (DWORD)pfnHookFunc - (DWORD)m_FuncAddress - 5;
	BOOL retW = WriteProcessMemory(GetCurrentProcess(), m_FuncAddress, m_NewBytes, 5, &dwRet);
	if (retW == FALSE)
	{
		MessageBox(NULL, TEXT("Hook 失败！"), TEXT("提示"), MB_OK);
		return FALSE;
	}
	return TRUE;
}

VOID CMyHook::UnHook()
{
	if (m_FuncAddress != NULL)
	{
		SIZE_T dwRet = 0;
		WriteProcessMemory(GetCurrentProcess(), m_FuncAddress, m_OldBytes, 5, &dwRet);
	}
}

BOOL CMyHook::ReHook()
{
	if (m_FuncAddress != NULL)
	{
		SIZE_T dwRet = 0;
		WriteProcessMemory(GetCurrentProcess(), m_FuncAddress, m_NewBytes, 5, &dwRet);
		return TRUE;
	}
	return FALSE;
}
