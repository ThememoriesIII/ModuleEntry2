
#include <iostream>
#include <vector>

extern "C"
{
#include <stdlib.h>
#include <Windows.h>
#include <TlHelp32.h>
}

struct dllList {
	std::wstring exname;
	uintptr_t modAddress;
	uintptr_t mod_L_address;
};
//get game process id
//�� process id �ͧ���
DWORD GetProcessID(wchar_t* exname);
//get module game
//���������ʨҡ�����ʹ�
MODULEENTRY32 GetModule(DWORD procid, wchar_t* modulename);
//���������ʨҡ�����ʹ� By ThememoriesIII
unsigned long getModule(DWORD pid, wchar_t* module_name, unsigned long* size);
//List all module name under game process
//��ʪ�������ŷ����������� process id game ������
std::vector<dllList> ListModule(DWORD proid, unsigned long* size);
wchar_t* GetWC(const char* c);