#pragma once
#include "Multiplayer/Client.h"
class PongGame;
class StartMenu
{
public:
	static StartMenu* instance;
	Client& client;
	bool& isInGame;
	bool& isHost;
	StartMenu(Client&,bool& isInGame,bool& isHost);
	void Render();
};

