#pragma once

enum class EventType : uint8
{
	CONNECT, 
	DISCONNECT,
	ACCEPT,
	RECV,
	SEND,

};

//�ڽ� : [�θ� ������...][��ӹ��� �ڽ� ������...]

class IocpEvent	: public OVERLAPPED
{
public:
	EventType eventType;
public:
	IocpEvent(EventType type);
public:
	void Init();
};

