#include "pch.h"
#include "ServerSession.h"

#include "Protocol.pb.h"

void ServerSession::OnConnected()
{
}

int ServerSession::OnRecvPacket(BYTE* buffer, int len)
{
	
	Protocol::Login packet;
	packet.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader));

	if (packet.has_player())
	{
		const Protocol::Player& player = packet.player();
		printf("ID : %d, Name : %s\n", player.id(), player.name().c_str());

	}



	return len;

}

void ServerSession::OnSend(int len)
{
}

void ServerSession::OnDisconnected()
{
	printf("OnDisconnected\n");

}
