// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

PROC FuncAddress;
BYTE OldBytes[5];
BYTE NewBytes[5];

BOOL Hook(const char* pszModuleName, const char* pszFuncName, PROC pfnHookFunc)
{
	FuncAddress = GetProcAddress(GetModuleHandleA(pszModuleName), pszFuncName);
	if (FuncAddress == NULL)
	{
		return FALSE;
	}
	DWORD dwRet = 0;
	BOOL bFlag = ReadProcessMemory(GetCurrentProcess(), FuncAddress, OldBytes, 5, &dwRet);
	if (!bFlag)
	{
		return FALSE;
	}
	NewBytes[0] = '\xE9';
	*(DWORD*)(NewBytes + 1) = (DWORD)pfnHookFunc - (DWORD)FuncAddress - 5;
	WriteProcessMemory(GetCurrentProcess(), FuncAddress, NewBytes, 5, &dwRet);
	if (!bFlag)
	{
		return FALSE;
	}
	return TRUE;
}

VOID UnHook()
{
	if (FuncAddress != NULL)
	{
		DWORD dwRet = 0;
		WriteProcessMemory(GetCurrentProcess(), FuncAddress, OldBytes, 5, &dwRet);
	}
}

VOID ReHook()
{
	if (FuncAddress != NULL)
	{
		DWORD dwRet = 0;
		WriteProcessMemory(GetCurrentProcess(), FuncAddress, NewBytes, 5, &dwRet);
	}
}

int WINAPI MyMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
	UnHook();
	MessageBoxA(hWnd, "msg", "msg", MB_OK);
	ReHook();
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		memset(OldBytes, 0, 5);
		memset(NewBytes, 0, 5);
		Hook("User32.dll", "MessageBoxA", (PROC)MyMessageBoxA);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		UnHook();
		break;
	}
	return TRUE;
}

