#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>

int main()
{
	// 一个进程需要两个基本部分：1、管理用的内核对象（进程句柄） 2、包含代码数据的地址空间

	STARTUPINFO StartupInfo;
	StartupInfo.cb = { sizeof(STARTUPINFO) };

	PROCESS_INFORMATION ProcessInformation;

	BOOL bRet = CreateProcess(
		"C:\\User\\rikvir\\Desktop\\createprocess.exe", // 想要执行的可执行程序的路径
		NULL, // 要传递给可执行模块的参数
		NULL, // 进程安全属性，如果是NULL的话，使用默认的安全属性
		NULL, // 线程安全属性，如果是NULL的话，使用默认的安全属性
		FALSE, // 指定了当前进程中的可继承句柄是否可以被启动的新进程所继承
		0, // 指定进程的优先级和创建标志
		NULL, // 指定新进程使用的环境变量
		NULL, // 指定新进程使用的当前目录
		&StartupInfo, // 启动信息
		&ProcessInformation // 进程相关信息
	);
	if (!bRet)
	{
		printf("CreateProcess Failed!\r\n");
	}
	else
	{
		printf("进程句柄：%d\r\n", ProcessInformation.hProcess);
		printf("线程句柄：%d\r\n", ProcessInformation.hThread);
		printf("进程ID：%d\r\n", ProcessInformation.dwProcessId);
		printf("线程ID：%d\r\n", ProcessInformation.dwThreadId);
		CloseHandle(ProcessInformation.hProcess);
		CloseHandle(ProcessInformation.hThread);
	}

	// 结束当前进程
	// ExitProcess(0);

	// 打开进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 672);
	if (hProcess != INVALID_HANDLE_VALUE)
	{
		// 结束其他进程
		TerminateProcess(hProcess, 0);
	}
	else
	{
		printf("OpenProcess Failed!");
	}

	// 遍历进程
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet2 = Process32First(hSnap, &pe32);
	while (bRet2)
	{
		printf("进程名：%s，进程ID：%d", pe32.szExeFile, pe32.th32ProcessID);
		bRet2 = Process32Next(hSnap, &pe32);
	}

	system("pause");
	return 0;
}