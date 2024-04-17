#pragma once
#include "IocpObj.h"

class Service;

class Session : public IocpObj
{
	friend class Listener;

private:
	shared_mutex rwLock; 
	atomic<bool> isConnected = false;
	//����Ʈ �����ͷ� ��ȯ
	shared_ptr<Service> service = nullptr;
	SOCKET socket = INVALID_SOCKET;
	SOCKADDR_IN sockAddr = {};
private:
	ConnectEvent connectEvent;
	RecvEvent recvEvent;
	DisconnectEvent disconnectEvent;
public:
	BYTE recvBuffer[1024] = {};
public:
	Session();
	virtual ~Session();
public:
	SOCKET GetSocket() const { return socket; }
	HANDLE GetHandle() override { return (HANDLE)socket; };
	shared_ptr<Service>  GetService() const { return service; }
	//�� ����Ʈ �����Ϳ� �ּ� ��ȯ
	shared_ptr<Session> GetSession() { return static_pointer_cast<Session>(shared_from_this()); }
public:
	bool IsConnected() const { return isConnected; }
public:
	void SetSockAddr(SOCKADDR_IN address) { sockAddr = address; }
	void SetService(shared_ptr<Service>  _service) { service = _service; }
private:
	bool RegisterConnect();
	void RegisterSend(SendEvent* sendEvent);
	void RegisterRecv();
	bool RegisterDisconnect();
private:
	void ProcessConnect();
	void ProcessSend(SendEvent* sendEvent, int numOfBytes);
	void ProcessRecv(int numOfBytes);
	void ProcessDisconnect();
private:
	void HandleError(int errorCode);
public:
	virtual void OnConnected() {}
	virtual void OnSend(int len) {}
	virtual int OnRecv(BYTE* buffer, int len) { return len; }
	virtual void OnDisconnected() {}
public:
	bool Connect();
	void Send(BYTE* buffer, int len);
	void Disconnect(const WCHAR* cause);
public:
	void ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred) override;
};

