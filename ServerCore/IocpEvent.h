#pragma once

enum class EventType : uint8
{
	CONNECT, 
	DISCONNECT,
	ACCEPT,
	RECV,
	SEND,

};

//전방선언
class IocpObj;
class Session;

class IocpEvent	: public OVERLAPPED
{
public:
	EventType eventType;
	//스마트 포인터로 전환
	shared_ptr<IocpObj> iocpObj;
public:
	IocpEvent(EventType type);
public:
	void Init();
};

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::CONNECT) {}
};

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::DISCONNECT) {}
};

class AcceptEvent : public IocpEvent
{
public:
	//스마트 포인터로 전환
	shared_ptr<Session> session = nullptr;
public:
	AcceptEvent() : IocpEvent(EventType::ACCEPT) {}
};


class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::RECV) {}
};

class SendEvent : public IocpEvent
{
public:
	vector<BYTE> sendBuffer;
public:
	SendEvent() : IocpEvent(EventType::SEND) {}
};