#pragma once
#include <PacketHandler.h>
#include "Protocol.pb.h"

enum : uint16
{
	C_LOGIN = 0,
	S_LOGIN = 1,
};

bool Handle_S_LOGIN(shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet);

class ServerPacketHandler : public PacketHandler
{
public:
	static void Init();

public:
	//Send
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_LOGIN& packet) { return PacketHandler::MakeSendBuffer(packet, C_LOGIN); }
};

