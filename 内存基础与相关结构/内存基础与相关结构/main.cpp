#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <Psapi.h>

int main()
{
	// ��ȡϵͳ��Ϣ
	SYSTEM_INFO lpSystemInfo = { 0 };
	GetSystemInfo(&lpSystemInfo);

	// ��ȡ�ڴ�״̬��Ϣ
	MEMORYSTATUSEX lpBuffer = { 0 };
	lpBuffer.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&lpBuffer);

	// ��ȡ�����ڴ���Ϣ
	PROCESS_MEMORY_COUNTERS ppsmemCounters = { 0 };
	GetProcessMemoryInfo(GetCurrentProcess(), &ppsmemCounters, sizeof(PROCESS_MEMORY_COUNTERS));

	// ���뱾�����ڴ�
	LPVOID lpBuffer1 = VirtualAlloc(NULL, 0x1024, MEM_COMMIT, PAGE_READWRITE);
	if (lpBuffer1 == NULL)
	{
		printf("Alloc Failed ErrorCode: %d", GetLastError());
		system("pause");
	}
	RtlZeroMemory(lpBuffer1, 0x1024); // ����ڴ�
	DWORD dwOldProtect = 0;
	VirtualProtect(lpBuffer1, 0x1024, PAGE_EXECUTE_READ, &dwOldProtect); // ���ñ�������
	MEMORY_BASIC_INFORMATION lpBuffer2 = { 0 };
	VirtualQuery(lpBuffer1, &lpBuffer2, sizeof(MEMORY_BASIC_INFORMATION)); // ��ѯ�ڴ�
	memcpy(lpBuffer1, L"123456", 14);
	VirtualFree(lpBuffer1, 0, MEM_RELEASE); // �ͷ��ڴ�

	// ����̶�д
	WCHAR szBufferWrite[] = L"tkzc00";
	WCHAR szBufferRead[MAX_PATH] = { 0 };
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 155360);
	LPVOID lpBuffer3 = VirtualAllocEx(hProcess, NULL, 0x100, MEM_DECOMMIT | MEM_RESERVE, PAGE_READWRITE);
	SIZE_T dwWriteSize = 0;
	WriteProcessMemory(hProcess, lpBuffer3, szBufferWrite, (wcslen(szBufferWrite) + 1) * 2, &dwWriteSize);
	SIZE_T dwReadSize = 0;
	ReadProcessMemory(hProcess, lpBuffer3, szBufferRead, (wcslen(szBufferRead) + 1) * 2, &dwReadSize);
	VirtualFreeEx(hProcess, lpBuffer3, 0, MEM_RELEASE);
	CloseHandle(hProcess);

	// �ѵĻ���ʹ��
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0); // ������
	LPVOID lpBuffer4 = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 1024); // �����ڴ�
	memcpy(lpBuffer4, L"abcd", 10); // �����ڴ�
	HeapFree(hHeap, HEAP_NO_SERIALIZE, lpBuffer4); // �ͷ��ڴ�
	HeapDestroy(hHeap); // ���ٶ�

	system("pause");
	return 0;
}