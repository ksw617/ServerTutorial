#pragma once
#include "IocpObj.h"

class Service;

class Session : public IocpObj
{
	friend class Listener;

private:
	//연결관리
	atomic<bool> isConnected = false;
	//서비스 들고 있을 용도로
	Service* service = nullptr;
	SOCKET socket = INVALID_SOCKET;
	SOCKADDR_IN sockAddr = {};
private:
	//RecvEvent 하나 만들어서 재활용
	RecvEvent recvEvent;
public:
	BYTE recvBuffer[1024] = {};
public:
	Session();
	virtual ~Session();
public:
	SOCKET GetSocket() const { return socket; }
	HANDLE GetHandle() override { return (HANDLE)socket; };
	//Get
	Service* GetService() const { return service; }
	bool IsConnected() const { return isConnected; }
public:
	void SetSockAddr(SOCKADDR_IN address) { sockAddr = address; }
	//Set
	void SetService(Service* _service) { service = _service; }
private:
	//Recv등록
	void RegisterRecv();
private:
	void ProcessConnect();
	//Recv진행
	void ProcessRecv(int numOfBytes);
private:
	//에러 관리
	void HandleError(int errorCode);
public:
	//자식이 상속받아서 사용할 용도로
	virtual void OnConnected() {}
	virtual int OnRecv(BYTE* buffer, int len) { return len; }
public:
	void ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred) override;
};

