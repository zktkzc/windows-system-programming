// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <windows.h>

typedef int
(WINAPI
	* fnMessageBoxA)(
		_In_opt_ HWND hWnd,
		_In_opt_ LPCSTR lpText,
		_In_opt_ LPCSTR lpCaption,
		_In_ UINT uType);

fnMessageBoxA OldMessageBoxA = NULL;

int WINAPI MyMessageBoxA(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType)
{
	return OldMessageBoxA(hWnd, "Hook!", "Hook", uType);
}

VOID IATHook()
{
	// 保存函数原始地址
	OldMessageBoxA = (fnMessageBoxA)GetProcAddress(GetModuleHandleA("User32.dll"), "MessageBoxA");
	// 分析PE文件
	// DOS头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);// 获取当前主模块的模块句柄(HMODULE)，实际上就是模块的加载基址
	// 获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + (DWORD)pDosHeader->e_lfanew);
	// 扩展头
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)&pNtHeader->OptionalHeader;
	// 获取数据目录表中的导入表的地址
	DWORD dwImportTableOffset = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	PIMAGE_IMPORT_DESCRIPTOR pImportTable = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDosHeader + dwImportTableOffset);

	DWORD* pFirstThunk;
	while (pImportTable->Characteristics && pImportTable->FirstThunk != NULL)
	{
		pFirstThunk = (DWORD*)(pImportTable->FirstThunk + (DWORD)pDosHeader);
		while (*(DWORD*)pFirstThunk != NULL)
		{
			if (*(DWORD*)pFirstThunk == (DWORD)OldMessageBoxA)
			{
				// 如果相等，说明找到要hook的函数了
				DWORD dwOldProtece = NULL;
				VirtualProtect(pFirstThunk, 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtece);
				DWORD dwFuncAddr = (DWORD)MyMessageBoxA;
				memcpy(pFirstThunk, (DWORD*)&dwFuncAddr, 4);
				VirtualProtect(pFirstThunk, 0x1000, dwOldProtece, &dwOldProtece);
			}
			pFirstThunk++;
		}
		pImportTable++; // 指向下一个导入表结构
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
		IATHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

