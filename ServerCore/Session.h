#pragma once
#include "IocpObj.h"
class Session : public IocpObj
{
private:
	SOCKET socket = INVALID_SOCKET;
	SOCKADDR_IN sockAddr = {};
public:
	BYTE recvBuffer[1024] = {};
public:
	Session();
	virtual ~Session();
public:
	SOCKET GetSocket() const { return socket; }
	HANDLE GetHandle() override { return (HANDLE)socket; };
public:
	void SetSockAddr(SOCKADDR_IN address) { sockAddr = address; }
public:
	void ProcessConnect();
	//��� �ҷ������ϱ� ���漱�� ����
	void ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred) override;
};

