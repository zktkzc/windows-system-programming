#include <windows.h>
#include <iostream>

class CA
{
public:
	CA();
	~CA();
	virtual void print();

private:

};

CA::CA()
{
}

CA::~CA()
{
}

void CA::print()
{
	std::cout << "virtual print" << std::endl;
}

void print()
{
	std::cout << "hook print" << std::endl;
}

int main()
{
	CA obj;

	DWORD dwVfptr = *(int*)(&obj); // 获取虚表指针
	DWORD dwOldProtect = 0;
	VirtualProtect((void*)dwVfptr, 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	*(int*)dwVfptr = (int)print;
	obj.print();
	(&obj)->print();

	MessageBoxA(NULL, "test", "test", MB_OK);
	system("pause");
	MessageBoxA(NULL, "test", "test", MB_OK);
	system("pause");

	system("pause");
	return 0;
}