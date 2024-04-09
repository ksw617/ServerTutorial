#include "pch.h"
#include "Listener.h"
#include "Service.h"
#include "SocketHelper.h"
#include "IocpCore.h"
#include "Session.h"
#include "IocpEvent.h"


Listener::~Listener()
{
    CloseSocket();
}

bool Listener::StartAccept(Service* service)
{
	socket = SocketHelper::CreateSocket();
	if (socket == INVALID_SOCKET)
		return false;
				  
	
	if (!SocketHelper::SetReuseAddress(socket, true))
		return false;
	
	if (!SocketHelper::SetLinger(socket, 0, 0))
		return false;

	ULONG_PTR key = 0;
	service->GetIocpCore()->Register((HANDLE)socket, key);


	if (!SocketHelper::Bind(socket, service->GetSockAddr()))
		return false;

	if (!SocketHelper::Listen(socket))
		return false;


	printf("listening...\n");

	//AcceptEvent ��ȯ
	AcceptEvent* acceptEvent = new AcceptEvent();
	RegisterAccept(acceptEvent);

	return true;

}

//AcceptEvent ��ȯ
void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	Session* session = new Session;
	acceptEvent->Init();
	//acceptEvent �� session ����
	acceptEvent->session = session;
	session->testNum = 11;


	DWORD dwBytes = 0;
	if (!SocketHelper::AcceptEx(socket, session->GetSocket(), session->recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, (LPOVERLAPPED)acceptEvent))
	{
		//������ ��Ȳ
		if (WSAGetLastError() != ERROR_IO_PENDING)
			RegisterAccept(acceptEvent);
	}
}


void Listener::CloseSocket()
{
	SocketHelper::CloseSocket(socket);
}

