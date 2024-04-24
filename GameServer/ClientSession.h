#pragma once
#include <PacketSession.h>

class ClientSession : public PacketSession
{
public:
	~ClientSession() {}
public:
	virtual void OnConnected() override;
	virtual int OnRecvPacket(BYTE* buffer, int len) override;
	virtual void OnSend(int len) override;
	virtual void OnDisconnected() override;

};
