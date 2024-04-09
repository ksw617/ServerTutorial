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
	//������ ȣ�� �ɶ� �θ� �ִ� ������ IocpEvent(EventType type) ȣ��
	// �Ű������� �� ACCEPT�̶�� �־���
	AcceptEvent() : IocpEvent(EventType::ACCEPT) {}
};
