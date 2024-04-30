#include "pch.h"
#include "ServerPacketHandler.h"

void ServerPacketHandler::Init()
{	
	PacketHandler::Init();

	//S_LOGIN == 1 == ID
	packetHandlers[S_LOGIN] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int len)
		{
			return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len);
		};
}


bool Handle_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int len)
{
	printf("Invalid Handle\n");
	return false;
}


bool Handle_S_LOGIN(shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet)
{
	printf("Connected\n");
	//todo
	return true;
}