#include "pch.h"
#include "ClientSession.h"
#include <SendBufferManager.h>
#include "SessionManager.h"


void ClientSession::OnConnected()
{
	SessionManager::Get().Add(static_pointer_cast<ClientSession>(shared_from_this()));

}

int ClientSession::OnRecvPacket(BYTE* buffer, int len)
{
	return len;
}

void ClientSession::OnSend(int len)
{
}

void ClientSession::OnDisconnected()
{
	SessionManager::Get().Remove(static_pointer_cast<ClientSession>(shared_from_this()));
}
