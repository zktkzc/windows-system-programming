#pragma once
#include <windows.h>

class CMyHook
{
public:
	CMyHook();
	~CMyHook();

	// hook����
	BOOL Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc);

	// ж��hook����
	VOID UnHook();

	// ����hook����
	BOOL ReHook();

private:
	// ������ַ
	PROC m_FuncAddress;
	// �ɵ�5���ֽ�
	BYTE m_OldBytes[5];
	// �µ�5���ֽ�
	BYTE m_NewBytes[5];
};

