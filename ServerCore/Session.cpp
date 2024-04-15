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
	//iocpEvent는 RecvEvent 일꺼니까 eventType은 RECV
	switch (iocpEvent->eventType)
	{
	case EventType::RECV:
		//ProcessRecv 호출
		ProcessRecv(bytesTransferred);
	default:
		break;
	}
}


void Session::ProcessRecv(int numOfBytes)
{
	//nullptr로 밀기
	recvEvent.iocpObj = nullptr;
	//받은 데이터 크기 확인
	printf("Recv Data : %d\n", numOfBytes);
	//물고기 잡고 낚시줄 다시 던지기
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
