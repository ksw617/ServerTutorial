#pragma once
#include <PacketHandler.h>
#include "Protocol.pb.h"

enum : uint16
{
	C_LOGIN = 0,
	S_LOGIN = 1,
};

bool Handle_C_LOGIN(shared_ptr<PacketSession>& session, Protocol::C_LOGIN& packet);

class ClientPacketHandler : public PacketHandler
{
public:
	static void Init();
public:
	//Send
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_LOGIN& packet) { return PacketHandler::MakeSendBuffer(packet, S_LOGIN); }
};

