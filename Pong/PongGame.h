#pragma once
#include <Common.h>
#include <OpenGLWrapper/Quad.h>
#include "Physics/Collider2D.h"
#include <Circle.h>
#include <Input/Input.h>
#include <functional>
#include "Paddle.h"
#include "Ball.h"
#include "Multiplayer/Client.h"

struct Player
{
	Paddle paddle;
	int score;
};
class PongGame
{

	bool& isInGame;
	bool isHost;

	glm::vec2 ballPos;
	glm::vec2 paddlePos;
	float ballRadius;
	glm::vec2 paddleSize;
	Ball ball;
public:
	static PongGame* instance;
	Player p1;
	Player p2;


	PongGame(bool& isInGame,bool isHost);
	void Render();
	void UIRender();
	~PongGame();
};

