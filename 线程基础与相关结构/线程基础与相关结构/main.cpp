#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>

DWORD ThreadCallBackA(LPVOID lpThreadParameter)
{
	for (size_t i = 0; i < 1000000; i++)
	{
		printf("ThreadCallBackA %d\r\n", i);
	}
	return 0;
}

DWORD ThreadCallBackB(LPVOID lpThreadParameter)
{
	for (size_t i = 0; i < 1000000; i++)
	{
		printf("ThreadCallBackB %d\r\n", i);
	}
	return 0;
}

int main()
{
	// 创建线程
	DWORD dwAThreadId = 0;
	HANDLE hThreadA = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadCallBackA, NULL, NULL, &dwAThreadId);
	DWORD dwBThreadId = 0;
	HANDLE hThreadB = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadCallBackB, NULL, NULL, &dwBThreadId);

	WaitForSingleObject(hThreadA, INFINITE);
	WaitForSingleObject(hThreadB, INFINITE);

	for (size_t i = 0; i < 1000000; i++)
	{
		printf("Main %d\r\n", i);
	}
	return 0;

	// 遍历线程
	DWORD dwProcessId = 17264;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessId);
	THREADENTRY32 te32;
	te32.dwSize = sizeof(THREADENTRY32);
	BOOL bRet = Thread32First(hSnap, &te32);
	while (bRet)
	{
		if (te32.th32OwnerProcessID == dwProcessId)
		{
			printf("线程ID：%d，进程ID：%d，线程优先级：%d", te32.th32ThreadID, te32.th32OwnerProcessID, te32.tpBasePri);
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
			// TerminateThread(hThread, -1);
			SuspendThread(hThread);
			ResumeThread(hThread);
		}
		bRet = Thread32Next(hSnap, &te32);
	}

	printf("End!\r\n");
	system("pause");
	return 0;
}