#include "Battle.h"

Battle::Battle(HWND hwnd, DWORD processid, HANDLE gamehandle, uintptr_t adrstart, char* pattern, char* mask) :ProcessID(processid), GameHandle(gamehandle)
{
	select_mob_flag = 0;
	hWnd = hwnd;
	Pattern = pattern;
	Mask = mask;
	//ReadProcessMemory(gamehandle, (LPCVOID)(adrstart), &BASE_ADR, 4, 0);
	BASE_ADR = adrstart;
	lv = 0;
	hp = 0;
	sp = 0;
	exp = 0;
	maxsp = 0;
	maxhp = 0;
	maxexp = 0;
	play_x = 0;
	play_y = 0;
	print_flag = false;
	std::cout << "DOMO BOT INITIAL VALUE" << std::endl;
}
Battle::~Battle()
{
	std::cout << "DOMO BOT IS END" << std::endl;
}
void Battle::search_adr()
{
	gotoxy(0, 30);
	std::cout << "Search start" << std::endl;
	mtx.lock();
	adr_mob_list = PatternListExModule2(GameHandle, ProcessID, Pattern, Mask);
	for (std::vector<uintptr_t>::iterator x = adr_mob_list.begin(); x != adr_mob_list.end(); x++)
	{
		std::cout << *x <<"\t"<< std::endl;
	}
	gotoxy(0, 30);
	std::cout << "Search finish" << std::endl;
	mtx.unlock();
}
void Battle::status_mob()
{
	mtx.lock();
	if (!adr_mob_list.empty())
	{
		std::vector<status> mob_buffer;
		for (std::vector<uintptr_t>::iterator i = adr_mob_list.begin(); i != adr_mob_list.end(); ++i)
		{
			status mob;
			float mobx = 0;
			float moby = 0;
			mob.adr = *i;
			/* here is plase you need to update address for get status character form main character in game */
			ReadProcessMemory(GameHandle, (LPCVOID)(*i), &mob.code, 4, 0);
			ReadProcessMemory(GameHandle, (LPCVOID)(*i + 0x04), &mob.id, 4, 0);
			ReadProcessMemory(GameHandle, (LPCVOID)(*i + 0x428), &mob.name, 32, 0);
			ReadProcessMemory(GameHandle, (LPCVOID)(*i + 0x454), &mob.hp, 4, 0);
			ReadProcessMemory(GameHandle, (LPCVOID)(*i + 0x458), &mob.max_hp, 4, 0);
			ReadProcessMemory(GameHandle, (LPCVOID)(*i + 0x64), &mobx, 4, 0);
			ReadProcessMemory(GameHandle, (LPCVOID)(*i + 0x6C), &moby, 4, 0);
			mob.dist = std::sqrt(std::pow(std::abs((mobx - play_x)), 2) + std::pow(std::abs((moby - play_y)), 2));
			mob_buffer.push_back(mob);
		}
		mob_list = mob_buffer;
	}
	//else
		//std::cout << "Address monster List is Emty" << std::endl;
	mtx.unlock();
	//}

}
void Battle::FindMob()
{
	if (!mob_list.empty())
	{
		std::vector<status> mobbuff;
		for (std::vector<status>::iterator i = mob_list.begin(); i != mob_list.end(); i++)
		{
			int nc = mob_name.size() - 1;
			while (nc >= 0)
			{
				if (std::strcmp((*i).name, mob_name[nc].c_str()) == 0)
					mobbuff.push_back(*i);
				nc--;
			}
		}
		mob_finding = mobbuff;
	}
}
void Battle::sort_distan_mob()
{
	//std::cout << "t2 run" << std::endl;
	//while (1)
	//{
	/* need to update this thread for mutex lock this way is to old*/
	mtx.lock();
	if(!mob_finding.empty())
	std::sort(mob_finding.begin(), mob_finding.end(), [](status a, status b) {
		return a.dist < b.dist;
		});
	mtx.unlock();
	//}
}


void Battle::player()
{
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x5A2800), &name, 32, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x56565C), &hp, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x565664), &sp, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x5A287C), &maxhp, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x5A2880), &maxsp, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x5A2900), &lv, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x51BD08), &play_x, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x51BD10), &play_y, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x5A28F8), &exp, 4, 0);
	ReadProcessMemory(GameHandle, (LPCVOID)(BASE_ADR + 0x5AC450), &maxexp, 4, 0);
	mtx.lock();
	/*need to find new way to print text to console cout to console is very slow and lock a thread to find mob and search*/
	if (print_flag == false)
	{
		gotoxy(0, 10);
		std::cout << "Player Name: " << name << " LV: " << lv << std::endl;
		std::cout << "HP: " << hp << " / " << maxhp << " SP: " << sp << " / " << maxsp << std::endl;
		std::cout << "pos x: " << play_x << " pos y: " << play_y << std::endl;
		std::cout << "ExP: " << exp << " / " << maxexp << std::endl;
		print_flag = true;
	}
	mtx.unlock();
}

void Battle::print_mob()
{
	mtx.lock();
	if (!mob_list.empty())
	{
		if (print_flag == true)
		{
			int couter = 1;
			int row = 20;
			gotoxy(0, 20);
			/*need to change iterator is esey to imprement but so slow to search you need to collect to heap and use pointer to collect data and search*/
			for (std::vector<status>::iterator i = mob_finding.begin(); i != mob_finding.end(); ++i)
			{
				if (couter == 5)
				{
					row++;
					gotoxy(0, row);
					couter = 1;
				}
				std::cout << i->dist << "\t";
				couter++;
			}
			print_flag = false;
		}
	}
	else
		std::cout << "monster List is Emty" << std::endl;
	mtx.unlock();
}

void Battle::Address()
{
	gotoxy(0, 25);
	std::cout << "thread addr start" << std::endl;
	std::thread SearchAddress;
	while (1)
	{
		SearchAddress = std::thread(&Battle::search_adr, this);
		SearchAddress.join();
	}
	SearchAddress.detach();
}

void Battle::Monster()
{
	std::cout << "thread mob start" << std::endl;
	std::thread t_mob[3];
	while (1)
	{
		// thsi thread need too imprement need thread pool to imprement
		// you need to try pub sub
		t_mob[0] = std::thread(&Battle::status_mob, this);
		t_mob[1] = std::thread(&Battle::FindMob, this);
		t_mob[2] = std::thread(&Battle::sort_distan_mob, this);
		t_mob[0].join();
		t_mob[1].join();
		t_mob[2].join();
	}
	t_mob[0].detach();
	t_mob[1].detach();
}

void Battle::Status()
{
	std::cout << "thread sat start" << std::endl;
	std::thread t_status[2];
	while (1)
	{
		// i need to try new idea to use pub sub to this thread extrac this thread to two process firt process for get data with processread() fuction and secon process need to subscrite to first process if first process is change first process can send signal to secon process to do a process work
		t_status[0] = std::thread(&Battle::player, this);
		t_status[0].join();
	}
	t_status[0].detach();
}

void Battle::select_mob()
{
	uintptr_t select = 0x1,
		box = 0x10101;
	uintptr_t base_code = 0x5341AC,
		base_adr = 0x5AF394;
	uintptr_t base_box;
	if (!mob_finding.empty())
	{
		// need yo update this address to process under comment line
		WriteProcessMemory(GameHandle, (LPVOID)(BASE_ADR + base_code), &mob_finding[0].id, 4, 0);
		WriteProcessMemory(GameHandle, (LPVOID)(BASE_ADR + base_adr), &mob_finding[0].adr, 4, 0);

		ReadProcessMemory(GameHandle, (LPVOID)(BASE_ADR + 0x534170), &base_box, 4, 0);
		WriteProcessMemory(GameHandle, (LPVOID)(base_box + 0xC5), &select, 4, 0);
		WriteProcessMemory(GameHandle, (LPVOID)(base_box + 0xA0), &box, 4, 0);
	}
}

void Battle::Attack_Monster()
{
	ReadProcessMemory(GameHandle, (LPVOID)(BASE_ADR + 0x5AF394), &select_mob_flag, 4, 0);
	if (select_mob_flag == 0)
	{
		select_mob();
	}
	else
	{
		while (1)
		{
			//need to update mouse positiom under commemnt
			PostMessageA(hWnd, WM_MOUSEMOVE, 1, MAKELPARAM(500, 130));
			PostMessageA(hWnd, WM_LBUTTONDBLCLK, 1, MAKELPARAM(500, 130));
			Sleep(1000);
		}

	}
}

void Battle::Running(std::vector<std::string> name)
{
	//need to update sequen to thread
	search_adr();
	status_mob();
	FindMob();
	sort_distan_mob();
	print_mob();
	/*std::thread t_run = std::thread(&Battle::Address, this);
	std::thread t_run2 = std::thread(&Battle::Status, this);
	//test_t search_a(ProcessID, GameHandle,BASE_ADR, Pattern, Mask);
	mob_name = name;
	while (1)
	{
		status_mob();
		FindMob();
		sort_distan_mob();
		print_mob();
		//Attack_Monster();
	}*/
}



void gotoxy(int x, int y)
{
	// need to change this fnunction to mnew console
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
