#include "pch.h"
#include "ServerSession.h"

#include "Protocol.pb.h"

void ServerSession::OnConnected()
{
}

int ServerSession::OnRecvPacket(BYTE* buffer, int len)
{
	
	Protocol::TEST packet;
	packet.ParseFromArray(buffer + sizeof(PacketHeader), len = sizeof(PacketHeader));

	printf("ID : %d, HP : %d\n", packet.id(), packet.hp());

	return len;

}

void ServerSession::OnSend(int len)
{
}

void ServerSession::OnDisconnected()
{
	printf("OnDisconnected\n");

}
