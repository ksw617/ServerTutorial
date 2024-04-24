#include "pch.h"
#include "ServerSession.h"

void ServerSession::OnConnected()
{
}

int ServerSession::OnRecvPacket(BYTE* buffer, int len)
{
	this_thread::sleep_for(1s);

	//printf("%s\n", &buffer[4]);

	return len;

}

void ServerSession::OnSend(int len)
{
}

void ServerSession::OnDisconnected()
{
	printf("OnDisconnected\n");

}
