#pragma once
#include "IocpObj.h"

class ServerService;
class AcceptEvent;

class Listener : public IocpObj
{
private:
	shared_ptr<ServerService> serverService = nullptr;
	SOCKET socket = INVALID_SOCKET;
	vector<AcceptEvent*> acceptEvents; // Ãß°¡
public:
	Listener() = default;
	virtual ~Listener();
public:
	HANDLE GetHandle() override { return (HANDLE)socket; };
	void ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred) override;

public:
	bool StartAccept(shared_ptr<ServerService> service);
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);
	void CloseSocket();

};

