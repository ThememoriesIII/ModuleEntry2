#pragma once
#include "patternscan.h"

void* PatternScan(char* base, size_t size, char* pattern, char* mask)
{
	size_t patternlength = strlen(pattern);
	for (unsigned int i = 0; i < (size - patternlength); i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternlength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (void*)(base + i);
		}
	}
	return nullptr;
}
std::vector<uintptr_t> ListPatternEx(HANDLE hprocess, uintptr_t begin, uintptr_t end, char* pattern, char* mask)
{
	uintptr_t currentChuck = begin;
	SIZE_T byteRead;
	std::vector<uintptr_t> list_address;
	while (currentChuck < end)
	{
		char buffer[1024];
		DWORD oldproc;
		if (VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), PAGE_EXECUTE_READWRITE, &oldproc))
		{
			ReadProcessMemory(hprocess, (void*)currentChuck, buffer, sizeof(buffer), &byteRead);
			VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), oldproc, &oldproc);
			//std::cout << std::uppercase << std::hex << begin << " " << std::uppercase << std::hex << end << " check chuck " << std::uppercase << std::hex << currentChuck << "byte read" << std::uppercase << std::hex << byteRead << std::endl;
			if (byteRead == 0)return {};
			void* internalAddress = PatternScan((char*)& buffer, byteRead, pattern, mask);
			if (internalAddress != nullptr)
			{
				//std::cout << "get" << std::endl;
				uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)& buffer;
				list_address.push_back((uintptr_t)currentChuck + (uintptr_t)offsetFromBuffer);
				currentChuck += byteRead;
			}
			else
			{
				currentChuck = currentChuck + byteRead;
			}
		}
		else
		{
			currentChuck = currentChuck + 0x400;
		}
	}
	return list_address;
}
std::vector<uintptr_t> PatternListExModule(HANDLE hprocess, DWORD processid, char* pattern, char* mask)
{
	std::vector<dllList>ALL_module = ListModule(processid, 0);
	std::vector<uintptr_t> AddressList;
	AddressList = ListPatternEx(hprocess, ALL_module[0].modAddress, 0x2FFFFFFF, pattern, mask);
	return AddressList;
}

std::vector<uintptr_t> PatternListExModule2(HANDLE hprocess, DWORD processid, char* pattern, char* mask)
{
	std::vector<dllList>ALL_module = ListModule(processid, 0);
	std::vector<uintptr_t> AddressList0;
	std::thread searh_ingame[4];
	searh_ingame[0] = std::thread(ListPatternEx2, hprocess, ALL_module[0].modAddress, 0xC000000, std::ref(pattern), std::ref(mask), std::ref(AddressList0));
	searh_ingame[1] = std::thread(ListPatternEx2, hprocess, 0xC000400, 0x18000000, std::ref(pattern), std::ref(mask), std::ref(AddressList0));
	searh_ingame[2] = std::thread(ListPatternEx2, hprocess, 0x18000400, 0x24000000, std::ref(pattern), std::ref(mask), std::ref(AddressList0));
	searh_ingame[3] = std::thread(ListPatternEx2, hprocess, 0x24000400, 0x30000000, std::ref(pattern), std::ref(mask), std::ref(AddressList0));


	searh_ingame[0].join();
	searh_ingame[1].join();
	searh_ingame[2].join();
	searh_ingame[3].join();

	if (!AddressList0.empty())
		for (std::vector<uintptr_t>::iterator x = AddressList0.begin(); x != AddressList0.end(); x++)
		{
			std::cout << std::uppercase << std::hex << *x << "\t" << std::endl;
		}
	else
		std::cout << "Emty" << std::endl;
	return AddressList0;
}

void ListPatternEx2(HANDLE hprocess, uintptr_t begin, uintptr_t end, char* pattern, char* mask, std::vector<uintptr_t>& AddressList)
{
	uintptr_t currentChuck = begin;
	SIZE_T byteRead;
	char buffer[1024];
	DWORD oldproc;
	while (currentChuck < end)
	{
		if (VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), PAGE_EXECUTE_READWRITE, &oldproc))
		{
			ReadProcessMemory(hprocess, (void*)currentChuck, buffer, sizeof(buffer), &byteRead);
			VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), oldproc, &oldproc);
			if (byteRead == 0)
				break;
			void* internalAddress = PatternScan((char*)& buffer, byteRead, pattern, mask);
			if (internalAddress != nullptr)
			{
				//std::cout << "get" << std::endl;
				uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)& buffer;
				AddressList.push_back((uintptr_t)currentChuck + (uintptr_t)offsetFromBuffer);
				currentChuck = currentChuck + byteRead;
			}
			else
			{
				currentChuck = currentChuck + byteRead;
			}
		}
		else
		{
			currentChuck = currentChuck + 0x400;
		}
	}
}