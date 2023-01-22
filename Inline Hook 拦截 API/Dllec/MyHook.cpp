#include "MyHook.h"

MyHook::MyHook()
{
	m_FuncAddress = NULL;
	memset(m_OldBytes, 0, 5);
	memset(m_NewBytes, 0, 5);
}

MyHook::~MyHook()
{
	UnHook();
	m_FuncAddress = NULL;
	memset(m_OldBytes, 0, 5);
	memset(m_NewBytes, 0, 5);
}

BOOL MyHook::Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc)
{
	// 获取指定模块中指定函数的地址
	m_FuncAddress = GetProcAddress(GetModuleHandle(pszModuleName), pszFuncName);
	if (m_FuncAddress == NULL)
	{
		return FALSE;
	}
	// 保存头5个字节
	size_t stRet = 0;
	ReadProcessMemory(GetCurrentProcess(), m_FuncAddress, m_OldBytes, 5, &stRet);
	// 构造jmp指令 jmp Address
	m_NewBytes[0] = '\xE9';
	*(DWORD*)(m_NewBytes + 1) = (DWORD)pfnHookFunc - (DWORD)m_FuncAddress - 5;
	// 写5个字节
	WriteProcessMemory(GetCurrentProcess(), m_FuncAddress, m_NewBytes, 5, &stRet);
	return TRUE;
}

VOID MyHook::UnHook()
{
	if (m_FuncAddress != NULL)
	{
		size_t stRet = 0;
		WriteProcessMemory(GetCurrentProcess(), m_FuncAddress, m_OldBytes, 5, &stRet);
	}
}

BOOL MyHook::ReHook()
{
	if (m_FuncAddress != NULL)
	{
		size_t stRet = 0;
		WriteProcessMemory(GetCurrentProcess(), m_FuncAddress, m_NewBytes, 5, &stRet);
		return TRUE;
	}
	return FALSE;
}
