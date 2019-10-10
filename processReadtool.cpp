#include "processReadtool.h"

wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t outSize;
	mbstowcs_s(&outSize, wc, cSize, c, cSize - 1);
	return wc;
}
DWORD GetProcessID(wchar_t* exname)
{
	PROCESSENTRY32 processentry = { 0 };
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hsnapshot)return 0;
	processentry.dwSize = sizeof(processentry);
	if (!Process32First(hsnapshot, &processentry))return 0;
	do {
		if (!wcscmp(processentry.szExeFile, exname))
		{
			CloseHandle(hsnapshot);
			//std::wcout << "FOUND " << exname << std::endl;
			return processentry.th32ProcessID;
		}
	} while (Process32Next(hsnapshot, &processentry));
	//std::wcout << "NOT FOUND " << exname << std::endl;
	CloseHandle(hsnapshot);
	return 0;
}
MODULEENTRY32 GetModule(DWORD procid, wchar_t* modulename)
{
	MODULEENTRY32 modEntry = { 0 };
	HANDLE hsanpshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procid);
	if (hsanpshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		do
		{
			if (!wcscmp(modEntry.szModule, modulename))
			{
				break;
			}
		} while (Module32Next(hsanpshot, &modEntry));
		CloseHandle(hsanpshot);
	}
	return modEntry;
}
std::vector<dllList> ListModule(DWORD proid, unsigned long* size)
{
	std::vector<dllList> mhod;
	HANDLE sanpshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proid);
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	while (Module32Next(sanpshot, &me32))
	{
		//std::wcout << "Module Name = " << me32.szModule << std::endl;
		dllList dump = { const_cast<wchar_t*>(me32.szModule),(uintptr_t)me32.modBaseAddr,(uintptr_t)(me32.modBaseAddr + me32.modBaseSize) };
		mhod.push_back(dump);
	}
	return mhod;
}