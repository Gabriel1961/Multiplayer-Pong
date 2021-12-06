#pragma once
#include <IncludesAndDefines.h>
#include "./Multiplayer/Message.h"
#include <chrono>
#include <mutex>
struct ServerResponse
{
	Message* msg = 0;
	bool hasReceivedResponse = false;
	std::mutex mtx;
	std::condition_variable cv;
	void CleanUpMsg()
	{
		delete msg;
		hasReceivedResponse = false;
		msg = 0;
	}
};
class Client
{
public:
	static Client* instance;
	client c;
	websocketpp::connection_hdl hdl;
	std::string ipAdress;
	ServerResponse serverResponse;
	Client(const std::string& ipAdress);
	void ConnectToServer();
	template<typename T>
	void SendMessageToServer(T msg)
	{
		try
		{
			string s = Serialize(msg);
			c.send(hdl, s, websocketpp::frame::opcode::binary);
		}
		catch (std::exception ex)
		{
			std::cout << ex.what();
		}
	}
	template<typename T>
	Message* SendMessage_And_WaitForReply(T* msg)
	{
		serverResponse.msg =0;
		serverResponse.hasReceivedResponse = false;
		std::unique_lock lk(serverResponse.mtx);
		try
		{
			string s = Serialize(*msg);
			c.send(hdl, s, websocketpp::frame::opcode::binary);
		}
		catch (std::exception ex)
		{
			std::cout << ex.what();
		}
		using namespace std::chrono;
		serverResponse.cv.wait_until(
			lk,
			steady_clock::now() + 5s,
			[&] {return serverResponse.hasReceivedResponse; }
		);
		serverResponse.hasReceivedResponse = false;
		return serverResponse.msg;
	}
}; 