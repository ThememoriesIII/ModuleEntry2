#pragma once
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include "patternscan.h"

struct status {
	char name[32];
	int hp;
	int max_hp;
	int lv;
	int id;
	int code;
	uintptr_t adr;
	float dist;
};

void gotoxy(int x, int y);

class Battle
{
private:
	std::mutex mtx;
	DWORD ProcessID;
	HWND hWnd;
	HANDLE GameHandle;
	uintptr_t BASE_ADR;

	char name[32];
	int lv;
	int hp;
	int sp;
	int exp;
	int maxsp;
	int maxhp;
	int maxexp;
	float play_x;
	float play_y;

	int select_mob_flag;
	int mx;
	int my;
	
	char* Pattern;
	char* Mask;
	bool print_flag;

	std::vector<uintptr_t> adr_mob_list;
	std::vector<std::string> mob_name;
	std::vector<status> mob_list;
	std::vector<status> mob_finding;
	std::vector<status> sort_mob_list;

	void search_adr();
	void sort_distan_mob();
	void status_mob();
	void select_mob();
	void Attack_Monster();
	void player();
	void print_mob();
	void Monster();
	void Address();
	void FindMob();
public:
	Battle(HWND hwnd,DWORD processid, HANDLE gamehandle,uintptr_t adrstart,char *pattern,char *mask);
	~Battle();
	void Running(std::vector<std::string> name);
	void Status();
};

class test_t{
private:
	std::mutex mtx;
	std::vector<uintptr_t> adr_mob_list;
	DWORD Pid;
	HANDLE Ghandle;
	char* Mask;
	char* Pattern;


	void set_adr(std::vector<uintptr_t> list) {
		mtx.lock();
		adr_mob_list = list;
		mtx.unlock();
	}
	std::vector<uintptr_t> return_adr()
	{
		mtx.lock();
		adr_mob_list;
		mtx.unlock();
		return adr_mob_list;
	}
	void scan_adr() {
		set_adr(PatternListExModule(Ghandle, Pid, Pattern, Mask));
	}
public:
	test_t(DWORD processid, HANDLE gamehandle, uintptr_t adrstart, char* pattern, char* mask)
	{
		 Pid=processid;
		 Ghandle=gamehandle;
		 Mask=mask;
		 Pattern=pattern;
		while (true)
		{
			std::cout << "2" << std::endl;
			scan_adr();
		}
	}
	std::vector<uintptr_t> get_adr() {
		return adr_mob_list;
	};
};