#include "Message.h"

#include "Message.h"

Message* Deserialize(const std::string& str)
{
    MessageType type;
    memcpy(&type, &str[0], sizeof(int));
    switch (type)
    {
    case MessageType::joinRoom:
        return (_Deserialize<JoinRoomMessage>(str));
    case MessageType::deleteRoom:
        return (_Deserialize<DeleteRoomMessage>(str));
    case MessageType::roomId:
        return (_Deserialize<RoomIdMessage>(str));
    case MessageType::dataInt:
        return (_Deserialize<DataIntMessage>(str));
    case MessageType::data3Vec2:
        return (_Deserialize<Data3Vec2Message>(str));
    case MessageType::score:
        return (_Deserialize<ScoreMessage>(str));
    default:
        return _Deserialize<Message>(str);
        break;
    }
}