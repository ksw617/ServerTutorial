#include "pch.h"
#include "Session.h"
#include "SocketHelper.h"
#include "Service.h"

Session::Session()
{
	socket = SocketHelper::CreateSocket();
}

Session::~Session()
{
	SocketHelper::CloseSocket(socket);
}

void Session::ProcessConnect()
{
	isConnected.store(true);

	GetService()->AddSession(this);

	OnConnected();

	RegisterRecv();
}


void Session::RegisterRecv()
{
	if (!IsConnected())
		return;

	recvEvent.Init();
	recvEvent.iocpObj = this; 

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)recvBuffer;
	wsaBuf.len = sizeof(recvBuffer);

	DWORD recvLen = 0;
	DWORD flags = 0;

	if (WSARecv(socket, &wsaBuf, 1, &recvLen, &flags, &recvEvent, nullptr) == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			recvEvent.iocpObj = nullptr;
		}

	}

}

//
void Session::ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred)
{
	//iocpEvent�� RecvEvent �ϲ��ϱ� eventType�� RECV
	switch (iocpEvent->eventType)
	{
	case EventType::RECV:
		//ProcessRecv ȣ��
		ProcessRecv(bytesTransferred);
	default:
		break;
	}
}


void Session::ProcessRecv(int numOfBytes)
{
	//nullptr�� �б�
	recvEvent.iocpObj = nullptr;
	//���� ������ ũ�� Ȯ��
	printf("Recv Data : %d\n", numOfBytes);
	//����� ��� ������ �ٽ� ������
	RegisterRecv();


}

void Session::HandleError(int errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		printf("Handle Eror\n");
		break;
	default:
		break;
	}
}
