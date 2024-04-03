#pragma once

class Listener
{
private:
	SOCKET socket = INVALID_SOCKET;
	//지우고
public:
	Listener() = default;;
	~Listener();
public:
	//포인터 변경
	bool StartAccept(class Service* service);
	void CloseSocket();

};

