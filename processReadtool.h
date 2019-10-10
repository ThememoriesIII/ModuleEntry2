
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
//หา process id ของเกมส
DWORD GetProcessID(wchar_t* exname);
//get module game
//หาโมดูลเกมสจากโปรเซสไอดี
MODULEENTRY32 GetModule(DWORD procid, wchar_t* modulename);
//หาโมดูลเกมสจากโปรเซสไอดี By ThememoriesIII
unsigned long getModule(DWORD pid, wchar_t* module_name, unsigned long* size);
//List all module name under game process
//ลิสชื่อโมดูลที่อยู่ภายใต้ process id game ทั้งหมด
std::vector<dllList> ListModule(DWORD proid, unsigned long* size);
wchar_t* GetWC(const char* c);