#include "StartMenu.h"
#include "ScreenSize.h"
#include <Common.h>
#include <string>
#include <cstring>
#include "./Input/Input.h"
#include <utility>
#include <Multiplayer/Message.h>
using namespace ImGui;
StartMenu* StartMenu::instance = 0;
ImVec2 toim(const glm::vec2& v)
{
	return { v.x,v.y };
}
std::string menuWindowName = "Pong";
bool isInJoinMenu = false;
bool isInHostMenu = false;
bool hasSentHostRequest = false;
char roomCode[11];
int numOfPlayerInRoom = 1;
int roomCodeInt = 0;
int joinErrorCode = JoinStatus_NONE;
void OnPasteCallback(int key, int action)
{
	if (key == GLFW_KEY_V && Input::Keyboard::GetKey(GLFW_KEY_LEFT_CONTROL) && isInJoinMenu)
	{
		ImGui::SetWindowFocus(nullptr);
		const char* c = glfwGetClipboardString(NULL);
		if (c)
		{
			int len = std::min((size_t)10, strlen(c));
			for (int i = 0; i <= len; i++)
				roomCode[i] = c[i];
			roomCode[len] = 0;
		}
		ImGui::SetWindowFocus("Join Game");
	}
}
void InitInput()
{
	using namespace ImGui;
	Input::Keyboard::KeyDown += OnPasteCallback;
}
StartMenu::StartMenu(Client& client, bool& isInGame,bool& isHost) : client(client),isInGame(isInGame),isHost(isHost)
{
	instance = this;
	InitInput();
}
void StartMenu::Render()
{
	using namespace glm;
	using namespace ImGui;
	if (!isInGame)
	{
		ImGui::Begin(menuWindowName.c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		vec2 menuSize = { Window_Width / 2.5f,Window_Width / 3.0f };
		ImGui::SetWindowSize(toim(menuSize));
		vec2 menuPos = { Window_Width / 3.0f, Window_Height / 4.0f };
		SetWindowPos(toim(menuPos));
		vec2 butSize = { menuSize.x / 3.0f,30 };
		if (!isInJoinMenu && !isInHostMenu)
		{
			menuWindowName = "Pong";
			ImGui::SetCursorPos(toim(menuSize / 3.0f));
			if (ImGui::Button("Host", toim(butSize)))
			{
				isInHostMenu = true;
				isInJoinMenu = false;
			}
			ImGui::SetCursorPos(toim(menuSize / 3.0f + vec2{ 0,40 }));
			if (ImGui::Button("Join", toim(butSize)))
			{
				isInJoinMenu = true;
				isInHostMenu = false;
			}
		}
		else if (isInHostMenu)
		{
			menuWindowName = "Host Game Players:" + std::to_string(numOfPlayerInRoom) + "/2";

			SetCursorPosX(GetCursorPos().x + 10);
			SetCursorPosY(GetCursorPos().y + 10);
			if (ImGui::Button("Back"))
			{
				isInHostMenu = false;
				hasSentHostRequest = false;
				goto end;
			}
			if (hasSentHostRequest == false)
			{
				hasSentHostRequest = true;
				auto msg = new Message(MessageType::hostRoom);
				auto reply = client.SendMessage_And_WaitForReply<Message>(msg);
				if (reply)
				{
					if (reply->type == MessageType::roomId) /// the Response is valid
					{
						roomCodeInt = ((RoomIdMessage*)reply)->roomId;
						glfwSetClipboardString(NULL, std::to_string(roomCodeInt).c_str());
					}
					else /// A response could not received
						roomCodeInt = -1;
				}
				else /// A response could not received
					roomCodeInt = -1;
				delete msg;
				delete reply;
			}
			if (roomCodeInt == -1) // if there was an error getting the roomCode
			{
				ImGui::TextColored({ 1,0,0,1 }, "An invalid response from the server \nwas received or the connection to\n the server could no be established.");
			}
			else // A valid roomCode
			{
				ImGui::Text("RoomCode:");
				SameLine();
				auto code = std::to_string(roomCodeInt);
				ImGui::InputText("", &code[0], code.size(), ImGuiInputTextFlags_ReadOnly);
			}
		}
		else if (isInJoinMenu)
		{
			menuWindowName = "Join Game";
			SetCursorPosX(GetCursorPos().x + 10);
			SetCursorPosY(GetCursorPos().y + 10);
			if (ImGui::Button("Back"))	
				isInJoinMenu = false;
			SetCursorPosX(GetCursorPos().x + 10);
			SetCursorPosY(GetCursorPos().y + 10);
			ImGui::Text("Room Code:");
			ImGui::SameLine();
			SetCursorPosY(GetCursorPos().y - 3);
			ImGui::InputText("", roomCode, 10, ImGuiInputTextFlags_AutoSelectAll);
			SetCursorPosX(GetCursorPos().x + menuSize.x / 3.0f);
			SetCursorPosY(GetCursorPos().y + menuSize.y * 0.1f);
			if (Button("Join", toim(butSize)))
			{
				auto msg = std::make_unique<JoinRoomMessage>(atoi(roomCode));
				auto reply = client.SendMessage_And_WaitForReply(msg.get());
				assert(reply->type == MessageType::dataInt);
				joinErrorCode = ((DataIntMessage*)reply)->val;
				delete reply;
			}
			SetCursorPosX(GetCursorPos().x + menuSize.x / 3.5f);
			SetCursorPosY(GetCursorPos().y + menuSize.y * 0.03f);
			if (!(joinErrorCode == JoinStatus_Succes || joinErrorCode == JoinStatus_NONE))
				TextColored({1,0,0,1}, "Invalid room code.");
		}
		isHost = isInHostMenu;
	end: {}
		ImGui::End();
	}
}
