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

	//AcceptEvent·Î ¹Ù²Þ
	void RegisterAccept(class AcceptEvent* acceptEvent);

	void CloseSocket();


};

