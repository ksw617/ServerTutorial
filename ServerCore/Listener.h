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

	//Accept함수에 등록해주는 용도로
	void RegisterAccept(class IocpEvent* acceptEvent);

	void CloseSocket();


};

