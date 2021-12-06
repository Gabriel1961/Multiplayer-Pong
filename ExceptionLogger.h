#pragma once
void LogException(websocketpp::exception const& e)
{
	std::cout << "Echo failed because: "<< "(" << e.what() << ")" << endl; 
}
#define excheck(x) try{x;} catch (websocketpp::exception const& e) {LogException(e);}