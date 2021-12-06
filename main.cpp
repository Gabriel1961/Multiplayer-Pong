#include "RenderLoop.h"

#include "Physics/Collider2D.h"

int main(void)
{
	std::string uri;
	std::cin >> uri;
	if (uri == "")
		uri = "ws://79.114.95.69:9002";
	else
		uri = "ws://" + uri + ":9002";
	Client client(uri);
	auto f = std::async(&Client::ConnectToServer, &client);
	RenderLoop(client);
}