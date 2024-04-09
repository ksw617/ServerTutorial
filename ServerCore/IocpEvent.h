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
	//생성자 호출 될때 부모에 있는 생성자 IocpEvent(EventType type) 호출
	// 매개변수로 나 ACCEPT이라고 넣어줌
	AcceptEvent() : IocpEvent(EventType::ACCEPT) {}
};
