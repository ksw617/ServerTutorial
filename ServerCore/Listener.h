#pragma once
#include "IocpObj.h"

class ServerService;
class AcceptEvent;

class Listener : public IocpObj
{
private:
	//스마트 포인터로 변환
	shared_ptr<ServerService> serverService = nullptr;
	SOCKET socket = INVALID_SOCKET;
public:
	Listener() = default;
	virtual ~Listener();
public:
	HANDLE GetHandle() override { return (HANDLE)socket; };
	void ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred) override;

public:
	//스마트 포인터로 변환
	bool StartAccept(shared_ptr<ServerService> service);
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);
	void CloseSocket();

};

