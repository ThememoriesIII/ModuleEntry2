#include <iostream>
#include <Windows.h>
#include <vector>
#include <chrono>
#include "patternscan.h"
#include "Battle.h"

std::vector<std::string> Inn_Basement = {"Caskmaster","Greedy Rat","Pupu President Corpse","Toxic Trunk","Weird Caskmaster" };

int main()
{

	char pattern[] = "\x34\xE5\x88\x00";
	char mask[] = "xxx?";

	std::vector<std::string> mob_name = {"Greedy Rat","Pupu President Corpse"};
	std::vector<std::string> mob_name2 = { "Greedy Rat" };
	HWND hWnd = FindWindowEx(0, 0, const_cast<wchar_t*>(L"DOMO - Dream Of Mirror Online"), 0);
	std::cout << hWnd << std::endl;
	DWORD processid = GetProcessID(const_cast<wchar_t*>(L"DOMO.exe"));
	MODULEENTRY32 GameModule = GetModule(processid, const_cast<wchar_t*>(L"DOMO.exe"));
	uintptr_t adrstart = (uintptr_t)GameModule.modBaseAddr;
	HANDLE handleprocess = OpenProcess(PROCESS_ALL_ACCESS, false, processid);
	Battle Runner(hWnd,processid, handleprocess, adrstart, pattern, mask);

	Runner.Running(mob_name2);
	//Runner.Status();
	return 0;
}