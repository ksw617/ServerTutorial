#include "pch.h"
#include "ClientSession.h"
#include <SendBufferManager.h>
#include "SessionManager.h"

#include "ClientPacketHandler.h"

void ClientSession::OnConnected()
{
	SessionManager::Get().Add(static_pointer_cast<ClientSession>(shared_from_this()));

}

int ClientSession::OnRecvPacket(BYTE* buffer, int len)
{
	shared_ptr<PacketSession> session = GetPacketSession();
	ClientPacketHandler::HandlePacket(session, buffer, len);

	return len;
}

void ClientSession::OnSend(int len)
{
}

void ClientSession::OnDisconnected()
{
	SessionManager::Get().Remove(static_pointer_cast<ClientSession>(shared_from_this()));
}
