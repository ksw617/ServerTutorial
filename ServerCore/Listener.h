#pragma once
#include "IocpObj.h"

class AcceptEvent;

class Listener : public IocpObj
{
private:
	SOCKET socket = INVALID_SOCKET;
public:
	Listener() = default;
	virtual ~Listener();
public:
	HANDLE GetHandle() override { return (HANDLE)socket; };
	//��� �ҷ������ϱ� ���漱�� ����
	void ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred) override;

public:
	bool StartAccept(class Service* service);
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);
	void CloseSocket();

};

