#include "pch.h"
#include "ClientPacketHandler.h"

void ClientPacketHandler::Init()
{
	PacketHandler::Init();

	//C_LOGIN == 0 == ID
	packetHandlers[C_LOGIN] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int len)
		{
			return HandlePacket<Protocol::C_LOGIN>(Handle_C_LOGIN, session, buffer, len);
		};
}


bool Handle_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int len)
{
	printf("Invalid Handle\n");
	return false;
}


bool Handle_C_LOGIN(shared_ptr<PacketSession>& session, Protocol::C_LOGIN& packet)
{
	printf("Client Login\n");
	
	Protocol::S_LOGIN loginPacket;
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPacket);

	session->Send(sendBuffer);

	return true;
}

