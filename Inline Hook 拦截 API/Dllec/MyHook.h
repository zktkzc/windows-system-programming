#pragma once
#include <Windows.h>

class MyHook
{
public:
	MyHook();
	~MyHook();

	BOOL Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc);
	VOID UnHook();
	BOOL ReHook();

private:
	PROC m_FuncAddress;
	BYTE m_OldBytes[5];
	BYTE m_NewBytes[5];
};

