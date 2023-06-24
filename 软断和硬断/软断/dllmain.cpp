// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>

size_t MessageBoxAddr = (size_t)GetProcAddress(GetModuleHandleA("User32.dll"), "MessageBoxA");

ULONG_PTR ulpExceptionAddress = 0;
UCHAR ucOldCode = 0;

LONG Handler(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if ((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == ulpExceptionAddress)
		{
			const char* szStr = "tkzc00";
			*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 0x8) = (DWORD)szStr;
			ExceptionInfo->ContextRecord->Eip += 2;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	return 0;
}

VOID SetHook(ULONG_PTR ulpAddress)
{
	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)ulpAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	ulpExceptionAddress = ulpAddress;
	ucOldCode = *(UCHAR*)ulpAddress;
	*(UCHAR*)ulpAddress = 0xCC;
	VirtualProtect((LPVOID)ulpAddress, 1, dwOldProtect, &dwOldProtect);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		AddVectoredExceptionHandler(1, (PVECTORED_EXCEPTION_HANDLER)Handler);
		SetHook(MessageBoxAddr);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

