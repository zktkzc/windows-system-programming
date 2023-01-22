#include <Windows.h>
#include "MyHook.h"

MyHook m_MyHookObj;

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
	if (MessageBox(NULL, "�Ƿ����أ�", "��ʾ", MB_YESNO) == IDYES)
	{
		MessageBox(NULL, "�����ĳ�������", "��ʾ", MB_OK);
	}
	else
	{
		m_MyHookObj.UnHook();
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
		m_MyHookObj.ReHook();
	}
	return TRUE;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // dll����
	{
		m_MyHookObj.Hook((LPSTR)"kernel32.dll", (LPSTR)"CreateProcessW", (PROC)MyCreateProcessW);
		break;
	}
	case DLL_PROCESS_DETACH: // dllж��
	{
		m_MyHookObj.UnHook();
		break;
	}
	default:
		break;
	}
}