#include "RenderLoop.h"

std::string uri = "ws://79.114.95.69:9002";
#include "Physics/Collider2D.h"

int main(void)
{
	Client client(uri);
	auto f = std::async(&Client::ConnectToServer, &client);
	RenderLoop(client);
}