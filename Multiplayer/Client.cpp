#include "Client.h"
#include <CommandLine.h>
#include "Menu/StartMenu.h"
#include "Pong/PongGame.h"
Client* Client::instance = 0;

void OnMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    string payload = msg->get_payload();
    if (msg->get_opcode() == websocketpp::frame::opcode::BINARY)
    {
        std::unique_lock lk(Client::instance->serverResponse.mtx);

        Message* mesg = Deserialize(payload);
        if (mesg->type == MessageType::roomId)
        {
            Client::instance->serverResponse.msg = mesg;
            Client::instance->serverResponse.hasReceivedResponse = true;
            lk.unlock();
            Client::instance->serverResponse.cv.notify_all();
            return;
        }
        else if (mesg->type == MessageType::dataInt)
        {
            Client::instance->serverResponse.msg = mesg;
            Client::instance->serverResponse.hasReceivedResponse = true;
            lk.unlock();
            Client::instance->serverResponse.cv.notify_all();
            return;
        }
        else if (mesg->type == MessageType::startGame)
        {
            StartMenu::instance->isInGame = 1;
        }
        else if (mesg->type == MessageType::data3Vec2)
        {
            Client::instance->serverResponse.msg = mesg;
            Client::instance->serverResponse.hasReceivedResponse = true;
            lk.unlock();
            return;
            //Client::instance->serverResponse.cv.notify_all();
        }
        else if (mesg->type == MessageType::score)
        {
            auto mggg = (ScoreMessage*)mesg;
            PongGame::instance->p1.score = mggg->p1;
            PongGame::instance->p2.score = mggg->p2;
        }
        delete mesg;
    }
}

void OnConnect(client* c, websocketpp::connection_hdl hdl)
{

}

Client::Client(const std::string& ipAdress) : ipAdress(ipAdress) {
    instance = this;
}

void Client::ConnectToServer()
{
    client c;
    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.clear_access_channels(websocketpp::log::alevel::all);
        c.set_access_channels(websocketpp::log::alevel::connect);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        c.set_message_handler(bind(&OnMessage, &c, ::_1, ::_2));
        c.set_open_handler(bind(&OnConnect, &c, ::_1));
        
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(ipAdress, ec);
        hdl = con;
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << endl;
            return;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);
        CommandLine cmd(&c, con);
        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        c.run();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << endl;
    }
    std::cout << "Disconnected from server\n";
}