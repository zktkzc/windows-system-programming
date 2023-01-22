#pragma once
#include <windows.h>

class CMyHook
{
public:
	CMyHook();
	~CMyHook();

	// hook函数
	BOOL Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc);

	// 卸载hook函数
	VOID UnHook();

	// 重新hook函数
	BOOL ReHook();

private:
	// 函数地址
	PROC m_FuncAddress;
	// 旧的5个字节
	BYTE m_OldBytes[5];
	// 新的5个字节
	BYTE m_NewBytes[5];
};

