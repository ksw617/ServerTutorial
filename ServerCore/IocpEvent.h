#pragma once

enum class EventType : uint8
{
	CONNECT, 
	DISCONNECT,
	ACCEPT,
	RECV,
	SEND,

};

class IocpEvent	: public OVERLAPPED
{
public:
	EventType eventType;
	//IocpObj 주소 가지고 있게
	class IocpObj* iocpObj;
public:
	IocpEvent(EventType type);
public:
	void Init();
};

class AcceptEvent : public IocpEvent
{
public:
	class Session* session = nullptr;
public:
	AcceptEvent() : IocpEvent(EventType::ACCEPT) {}
};
