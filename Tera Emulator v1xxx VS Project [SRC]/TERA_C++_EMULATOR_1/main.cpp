#include <iostream>
#include <string>

#include "Server.hpp"
#include "EntityService.h"

int main() {
	SetConsoleTitle(L"Tera Emulator v1030");
	HANDLE _handler = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dc = COORD();
	dc.X = 1024;
	dc.Y = 800;
	SetConsoleScreenBufferSize(_handler, dc);
	SMALL_RECT sm;
	ZeroMemory(&sm, sizeof sm);
	sm.Bottom = 800;
	sm.Right = 1024;

	SetConsoleWindowInfo(_handler, true, &sm);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);





	EntityService::Initialize();
	Server *_server = new Server();
	bool run = true;
	if (!_server->Initialize())
	{
		std::cout << ">Initialization Error!\n\n";
		run = false;
	}



	std::string line;
	while (run)
	{
		std::cin >> line;
		if (line == "./start")
		{
			if (!_server->Run())
				std::cout << ">Server working!\n";
		}
		else  if (line == "./stop")
		{
			if (_server->Shutdown())
				std::cout << ">Server Shutdown!\n";
			else
				std::cout << ">Server is down!\n";
		}
		else if (line == "./exit")
		{
			_server->Shutdown();
			break;
		}
	}


	delete _server;
	_server = 0;

	system("pause");

	return 0;
}