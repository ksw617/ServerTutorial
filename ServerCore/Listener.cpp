#include "pch.h"
#include "Listener.h"
#include "ServerService.h" 
#include "SocketHelper.h"
#include "IocpCore.h"
#include "Session.h"
#include "IocpEvent.h"


Listener::~Listener()
{
    CloseSocket();
}

//스마트 포인터로 변환
bool Listener::StartAccept(shared_ptr<ServerService> service)
{
	serverService = service;

	socket = SocketHelper::CreateSocket();
	if (socket == INVALID_SOCKET)
		return false;
				  
	if (!SocketHelper::SetReuseAddress(socket, true))
		return false;
	
	if (!SocketHelper::SetLinger(socket, 0, 0))
		return false;

	ULONG_PTR key = 0;
	//this ->  shared_from_this()로 변환 : 스마트 포인터로 레퍼 관리
	if (service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;


	if (!SocketHelper::Bind(socket, service->GetSockAddr()))
		return false;

	if (!SocketHelper::Listen(socket))
		return false;


	printf("listening...\n");

	AcceptEvent* acceptEvent = new AcceptEvent();
	//스마트 포인터로 레퍼 관리
	acceptEvent->iocpObj = shared_from_this();
	RegisterAccept(acceptEvent);

	return true;

}


void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	//스마트 포인터로 변환
	shared_ptr<Session> session = serverService->CreateSession();

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD dwBytes = 0;
	if (!SocketHelper::AcceptEx(socket, session->GetSocket(), session->recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, (LPOVERLAPPED)acceptEvent))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
			RegisterAccept(acceptEvent);
	}
}

void Listener::ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred)
{
	ProcessAccept((AcceptEvent*)iocpEvent);
}


void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	//스마트 포인터로 변환
	shared_ptr<Session> session = acceptEvent->session;
	if (!SocketHelper::SetUpdateAcceptSocket(session->GetSocket(), socket))
	{
		printf("SetUpdateAcceptSocket Error\n");
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddr;
	int sockAddrSize = sizeof(sockAddr);
	if (getpeername(session->GetSocket(),(SOCKADDR*)&sockAddr, &sockAddrSize) == SOCKET_ERROR)
	{
		printf("getpeername Error\n");
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetSockAddr(sockAddr);
	session->ProcessConnect();

	RegisterAccept(acceptEvent);
}


void Listener::CloseSocket()
{
	SocketHelper::CloseSocket(socket);
}
