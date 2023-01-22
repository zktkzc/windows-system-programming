#include <stdio.h>
#include <windows.h>

DWORD WINAPI LOOP1(LPVOID lp)
{
	while (true)
	{
		printf("1\n");
	}
	return 0;
}

DWORD WINAPI LOOP2(LPVOID lp)
{
	while (true)
	{
		printf("2\n");
	}
	return 0;
}

int main()
{
	CreateThread(NULL, NULL, LOOP1, NULL, NULL, NULL);
	CreateThread(NULL, NULL, LOOP2, NULL, NULL, NULL);
	system("pause");
	return 0;
}