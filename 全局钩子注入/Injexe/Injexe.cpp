#include "pch.h"
#include <iostream>
#include <Windows.h>

HHOOK g_hHook = NULL;

LRESULT GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}

int main()
{
	HMODULE hDll = NULL;
	hDll = ::LoadLibrary(TEXT("InjDll.dll"));
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, hDll, 0);
	if (g_hHook == NULL)
	{
		printf("Hook Failed!\n");
		system("pause");
		return -1;
	}
	else
	{
		printf("Hook Success!\n");
		system("pause");
	}
	::UnhookWindowsHookEx(g_hHook); // 卸载hook
	printf("Unhook!\n");
	system("pause");
	return 0;
}
