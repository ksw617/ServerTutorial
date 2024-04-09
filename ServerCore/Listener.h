#pragma once

class Listener
{
private:
	SOCKET socket = INVALID_SOCKET;
public:
	Listener() = default;;
	~Listener();
public:
	bool StartAccept(class Service* service);

	//Accept�Լ��� ������ִ� �뵵��
	void RegisterAccept(class IocpEvent* acceptEvent);

	void CloseSocket();


};

