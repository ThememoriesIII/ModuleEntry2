#pragma once
#include "processReadtool.h"
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

//scan pattern in address
//‡∑’¬∫·æ∑‡∑‘≈

void* PatternScan(char* base, size_t size, char* pattern, char* mask);
// List Pattern module in game
//À“·æ∑‡∑‘≈„π‚¡¥Ÿ≈∑—ÈßÀ¡¥¢Õß‡°¡ 
std::vector<uintptr_t> ListPatternEx(HANDLE hprocess, uintptr_t begin, uintptr_t end, char* pattern, char* mask);
// initilial begin and endaddress for a List module name under game process 
std::vector<uintptr_t> PatternListExModule(HANDLE hprocess, DWORD processid, char* pattern, char* mask);

void ListPatternEx2(HANDLE hprocess, uintptr_t begin, uintptr_t end, char* pattern, char* mask, std::vector<uintptr_t> &AddressList);
std::vector<uintptr_t> PatternListExModule2(HANDLE hprocess, DWORD processid, char* pattern, char* mask);