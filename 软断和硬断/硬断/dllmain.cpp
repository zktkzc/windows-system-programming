// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>
#include <TlHelp32.h>

size_t MessageBoxAddr = (size_t)GetProcAddress(GetModuleHandleA("User32.dll"), "MessageBoxA");

LONG Handler(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	if ((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == MessageBoxAddr)
	{
		const char* szStr = "tkzc00";
		*(DWORD*)(ExceptionInfo->ContextRecord->Esp + 0x8) = (DWORD)szStr;
		ExceptionInfo->ContextRecord->Eip += 2;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else
	{
		ExceptionInfo->ContextRecord->Dr0 = MessageBoxAddr;
		ExceptionInfo->ContextRecord->Dr7 = 0x405;
		return EXCEPTION_CONTINUE_SEARCH;
	}

	return 0;
}

VOID SetThreadHook(HANDLE hThread)
{
	CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_ALL; // 获取线程上下文里的所有寄存器
	GetThreadContext(hThread, &ctx);
	ctx.Dr0 = MessageBoxAddr;
	ctx.Dr7 = 0x1;
	SetThreadContext(hThread, &ctx);
}

VOID SetHook()
{
	HANDLE hThreadShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, GetCurrentProcessId());
	THREADENTRY32* te32 = new THREADENTRY32;
	te32->dwSize = sizeof(THREADENTRY32);
	HANDLE hThread = NULL;
	while (Thread32First(hThreadShot, te32))
	{
		if (GetCurrentProcessId() == te32->th32OwnerProcessID)
		{
			hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32->th32ThreadID);
			SetThreadHook(hThread);
			CloseHandle(hThread);
		}
	}
	CloseHandle(hThread);
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
		SetHook();
	case DLL_THREAD_ATTACH:
		SetThreadHook(GetCurrentThread());
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

