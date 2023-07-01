// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

typedef int
(WINAPI
	* fnMessageBoxA)(
		_In_opt_ HWND hWnd,
		_In_opt_ LPCSTR lpText,
		_In_opt_ LPCSTR lpCaption,
		_In_ UINT uType);

int
WINAPI
MyMessageBoxA(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType)
{
	fnMessageBoxA NewFunc = (fnMessageBoxA)((DWORD)MessageBoxA + 2);
	return NewFunc(hWnd, "HotFix Hook!", "Hook", uType);
}

BOOL Hook(const char* szModuleName, const char* szFuncName, PROC pfnFunc)
{
	BYTE ShortJmp[2] = { 0xeb,0xf9 };
	BYTE NewCode[5] = { 0xe9,0, };
	FARPROC pOldFuncAddr = (FARPROC)GetProcAddress(GetModuleHandleA(szModuleName), szFuncName);
	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)((DWORD)pOldFuncAddr - 5), 7, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	DWORD dwAddr = (DWORD)pfnFunc - (DWORD)pOldFuncAddr; // 获取要跳转的地址
	*(DWORD*)(NewCode + 1) = dwAddr;
	memcpy((LPVOID)((DWORD)pOldFuncAddr - 5), NewCode, 5);
	memcpy(pOldFuncAddr, ShortJmp, 2);
	VirtualProtect((LPVOID)((DWORD)pOldFuncAddr - 5), 7, dwOldProtect, &dwOldProtect);
	return TRUE;
}

BOOL UnHook(const char* szModuleName, const char* szFuncName)
{
	BYTE ShortJmp[2] = { 0x8b,0xff };
	BYTE NewCode[5] = { 0x90,0x90,0x90,0x90,0x90 };
	FARPROC pOldFuncAddr = (FARPROC)GetProcAddress(GetModuleHandleA(szModuleName), szFuncName);
	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)((DWORD)pOldFuncAddr - 5), 7, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy((LPVOID)((DWORD)pOldFuncAddr - 5), NewCode, 5);
	memcpy(pOldFuncAddr, ShortJmp, 2);
	VirtualProtect((LPVOID)((DWORD)pOldFuncAddr - 5), 7, dwOldProtect, &dwOldProtect);
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Hook("User32.dll", "MessageBoxA", (PROC)MyMessageBoxA);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		UnHook("User32.dll", "MessageBoxA");
		break;
	}
	return TRUE;
}

