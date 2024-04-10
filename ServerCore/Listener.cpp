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
	service->GetIocpCore()->Register(this);


	if (!SocketHelper::Bind(socket, service->GetSockAddr()))
		return false;

	if (!SocketHelper::Listen(socket))
		return false;


	printf("listening...\n");

	AcceptEvent* acceptEvent = new AcceptEvent();
	acceptEvent->iocpObj = this;
	RegisterAccept(acceptEvent);

	return true;

}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	Session* session = new Session;
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
	Session* session = acceptEvent->session;
	if (!SocketHelper::SetUpdateAcceptSocket(session->GetSocket(), socket))
	{
		printf("SetUpdateAcceptSocket Error\n");
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddr;
	int sockAddrSize = sizeof(sockAddr);
	//AcceptSocket�� �־��ָ� sockAddr���ٰ� Ŭ���� �ּ� ������ �־���
	//SOCKET_ERROR��� ���� �ִ°Ŵϱ� ���� ó��
	if (getpeername(session->GetSocket(),(SOCKADDR*)&sockAddr, &sockAddrSize) == SOCKET_ERROR)
	{
		//���� ó��
		printf("getpeername Error\n");
		RegisterAccept(acceptEvent);
		return;
	}

	//Session�� Ŭ���� �ּҸ� ������Ʈ
	session->SetSockAddr(sockAddr);
	//����ȰŴϱ� ���������
	session->ProcessConnect();

	//�ٽ� �������� ������ == �ٸ� Ŭ�� �����Ҽ� �ֵ��� ���
	//����� == Ŭ��
	RegisterAccept(acceptEvent);
}


void Listener::CloseSocket()
{
	SocketHelper::CloseSocket(socket);
}
