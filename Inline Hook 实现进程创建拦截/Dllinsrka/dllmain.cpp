// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

CMyHook m_myHookObject;

BOOL
WINAPI
MyCreateProcessW(
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
)
{
	if (MessageBox(NULL, TEXT("是否拦截?"), TEXT("提示"), MB_YESNO) == IDYES)
	{
		MessageBox(NULL, TEXT("启动的进程已经被拦截"), TEXT("提示"), MB_OK);
		return TRUE;
	}
	else
	{
		m_myHookObject.UnHook();
		CreateProcessW(
			lpApplicationName,
			lpCommandLine,
			lpProcessAttributes,
			lpThreadAttributes,
			bInheritHandles,
			dwCreationFlags,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			lpProcessInformation
		);
		m_myHookObject.ReHook();
		return FALSE;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		m_myHookObject.Hook((LPSTR)"kernel32.dll", (LPSTR)"CreateProcessW", (PROC)MyCreateProcessW);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		m_myHookObject.UnHook();
		break;
	}
	return TRUE;
}

