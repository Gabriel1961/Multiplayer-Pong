#pragma once
#include "IncludesAndDefines.h"
#ifdef __cplusplus__
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#include <string>
#include <iostream>
#include <thread>

/// <summary>
/// Contains logic for command line operations written in the console
/// Needs to be instiated once
/// </summary>
class CommandLine
{
private:
	std::future<void> thr;
	websocketpp::connection_hdl hdl;
	client* serv = NULL;
public:
	bool condition = true;
	CommandLine(client* s,const websocketpp::connection_hdl& hdl) : serv(s), hdl(hdl)
	{
		thr = std::async([this] { GetInput(); });
	}
	CommandLine()
	{
		thr = std::async([this] { GetInput(); });
	}
	void ClearScreen()
	{
		if (system("CLS")) system("clear");
	}

	void Exit()
	{
		exit(0);
	}

	void GetInput()
	{

		std::string s;
		while (condition)
		{
			std::getline(std::cin, s);
			int argIdx;
			if (s == "exit")
				Exit();
			else if (s == "clear" || s == "cls")
				ClearScreen();
			else if ((argIdx = s.find("print ")) != s.npos)
			{
				if (serv != NULL)
				{
					string message = s.substr(argIdx+5, s.length() - argIdx);
					serv->send(hdl, message, websocketpp::frame::opcode::text);
				}
				else
				{
					std::cout << "You cannot use this command.\n";
				}
			}
			else
				std::cout << "Unrecognized command.\n";
		}
	}

	~CommandLine()
	{
		condition = false;
		thr.wait();
	}
};