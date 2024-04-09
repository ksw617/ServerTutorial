#pragma once

enum class EventType : uint8
{
	CONNECT, 
	DISCONNECT,
	ACCEPT,
	RECV,
	SEND,

};

//자식 : [부모 데이터...][상속받은 자식 데이터...]

class IocpEvent	: public OVERLAPPED
{
public:
	EventType eventType;
public:
	IocpEvent(EventType type);
public:
	void Init();
};

