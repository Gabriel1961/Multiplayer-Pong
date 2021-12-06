#include "PongGame.h"
using namespace glm;
PongGame* PongGame::instance = 0;
Texture* ballTex;
PongGame::PongGame(bool& isInGame,bool isHost) : isInGame(isInGame),isHost(isHost)// Start of game
{
	instance = this;
	ballTex = new Texture("Resources/trump.png");
	
	ball = Ball(Circle(30, 1, 0));
	p1.paddle.SetDefaultTransform(true);
	p2.paddle.SetDefaultTransform(false);
	if (isHost)
	{
		p1.paddle.isAffectedByInput = true;
	}
	else
	{
		p2.paddle.isAffectedByInput = true;
	}
}

void CollisionChecking(Ball& b, Paddle& p)
{
	if (b.collider.Collide(&p.collider))
	{

		if (b.collider.wasHitBefore == false)
		{
			b.speed += b.speed * 0.1f;
			b.speed.x = -b.speed.x;
		}
		b.collider.wasHitBefore = true;
	}
	else b.collider.wasHitBefore = false;
}

void PongGame::Render()
{
	if (isHost)
	{
		auto& resp = Client::instance->serverResponse;
		std::unique_lock<std::mutex>lk(resp.mtx);
		if (resp.hasReceivedResponse && resp.msg)
		{
			if (resp.msg->type == MessageType::data3Vec2)
			{
				auto v1 = ((Data3Vec2Message*)resp.msg)->v1;
				p2.paddle.Pos() = {v1.x, v1.y};
				p2.paddle.speed = 0;
				resp.CleanUpMsg();
			}
		}
		lk.unlock();
		if (ball.IsOnLeftEdge())
		{

			p2.score++;
			ball.SetDefaultTransform(true);

			ScoreMessage scoreMsg(p1.score, p2.score);
			Client::instance->SendMessageToServer(scoreMsg);
		}
		else if (ball.IsOnRightEdge())
		{

			p1.score++;
			ball.SetDefaultTransform(false);

			ScoreMessage scoreMsg(p1.score, p2.score);
			Client::instance->SendMessageToServer(scoreMsg);
		}
		CollisionChecking(ball,p2.paddle);
		CollisionChecking(ball,p1.paddle);
	}
	else
	{
		auto& resp = Client::instance->serverResponse;
		std::unique_lock<std::mutex>lk(resp.mtx);
		if (resp.hasReceivedResponse && resp.msg)
		{
			if (resp.msg->type == MessageType::data3Vec2)
			{
				auto msg = ((Data3Vec2Message*)resp.msg);
				ball.Pos() = { msg->v1.x,msg->v1.y };
				ball.speed = { msg->v2.x,msg->v2.y };
				p1.paddle.Pos() = { msg->v3.x,msg->v3.y };
				p1.paddle.speed = 0;
				resp.CleanUpMsg();
			}
		}
	}

	ballTex->Bind(0);
	ball.Render();
	p1.paddle.Render();
	p2.paddle.Render();

	if (isHost)
	{

		Data3Vec2Message ballPosMsg;
		ballPosMsg.v1 = { ball.Pos().x,ball.Pos().y };
		ballPosMsg.v2 = { ball.speed.x,ball.speed.y };
		ballPosMsg.v3 = { p1.paddle.Pos().x ,p1.paddle.Pos().y };
		Client::instance->SendMessageToServer(ballPosMsg);
	}
	else
	{
		Data3Vec2Message pongData;
		pongData.v1 = { p2.paddle.Pos().x,p2.paddle.Pos().y };
		Client::instance->SendMessageToServer(pongData);
	}
}

PongGame::~PongGame()
{
	instance = 0;
}
void PongGame::UIRender()
{
	ImGui::Begin("Score", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
	vec2 ws = {ImGui::GetWindowSize().x, ImGui::GetWindowSize().y};
	ws /= 2;
	ImVec4 blu = { 0,0,1,1 };
	ImVec4 red = { 1,0,0,1 };
	ImGui::TextColored(blu, std::to_string(p1.score).c_str());
	ImGui::SameLine();
	ImGui::TextColored({1,1,1,1}, " = ");
	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
	ImGui::TextColored(red, std::to_string(p2.score).c_str());

	ImGui::SetWindowPos({ Window_Width / 2 - ws.x, 10 });
	ImGui::End();
}
