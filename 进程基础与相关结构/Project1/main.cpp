#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>

int main()
{
	// һ��������Ҫ�����������֣�1�������õ��ں˶��󣨽��̾���� 2�������������ݵĵ�ַ�ռ�

	STARTUPINFO StartupInfo;
	StartupInfo.cb = { sizeof(STARTUPINFO) };

	PROCESS_INFORMATION ProcessInformation;

	BOOL bRet = CreateProcess(
		"C:\\User\\rikvir\\Desktop\\createprocess.exe", // ��Ҫִ�еĿ�ִ�г����·��
		NULL, // Ҫ���ݸ���ִ��ģ��Ĳ���
		NULL, // ���̰�ȫ���ԣ������NULL�Ļ���ʹ��Ĭ�ϵİ�ȫ����
		NULL, // �̰߳�ȫ���ԣ������NULL�Ļ���ʹ��Ĭ�ϵİ�ȫ����
		FALSE, // ָ���˵�ǰ�����еĿɼ̳о���Ƿ���Ա��������½������̳�
		0, // ָ�����̵����ȼ��ʹ�����־
		NULL, // ָ���½���ʹ�õĻ�������
		NULL, // ָ���½���ʹ�õĵ�ǰĿ¼
		&StartupInfo, // ������Ϣ
		&ProcessInformation // ���������Ϣ
	);
	if (!bRet)
	{
		printf("CreateProcess Failed!\r\n");
	}
	else
	{
		printf("���̾����%d\r\n", ProcessInformation.hProcess);
		printf("�߳̾����%d\r\n", ProcessInformation.hThread);
		printf("����ID��%d\r\n", ProcessInformation.dwProcessId);
		printf("�߳�ID��%d\r\n", ProcessInformation.dwThreadId);
		CloseHandle(ProcessInformation.hProcess);
		CloseHandle(ProcessInformation.hThread);
	}

	// ������ǰ����
	// ExitProcess(0);

	// �򿪽���
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 672);
	if (hProcess != INVALID_HANDLE_VALUE)
	{
		// ������������
		TerminateProcess(hProcess, 0);
	}
	else
	{
		printf("OpenProcess Failed!");
	}

	// ��������
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet2 = Process32First(hSnap, &pe32);
	while (bRet2)
	{
		printf("��������%s������ID��%d", pe32.szExeFile, pe32.th32ProcessID);
		bRet2 = Process32Next(hSnap, &pe32);
	}

	system("pause");
	return 0;
}