#include "pch.h"
#include "Session.h"
#include "SocketHelper.h"
#include "Service.h"


Session::Session() : recvBuffer(BUFFER_SIZE)
{
	socket = SocketHelper::CreateSocket();
}

Session::~Session()
{
	SocketHelper::CloseSocket(socket);
}


bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Send(shared_ptr<SendBuffer> sendData)
{
	if (!IsConnected())
		return;

	bool registerSend = false;

	{
		unique_lock<shared_mutex> lock(rwLock);
		sendQueue.push(sendData);
		if (sendRegistered.exchange(true) == false)
		{
			registerSend = true;
		}
	}

	if (registerSend)
	{
		RegisterSend();
	}
}

void Session::Disconnect(const WCHAR* cause)
{

	if (isConnected.exchange(false) == false)
		return;

	wprintf(L"Disconnect reason : %ls\n", cause);

	//OnDisconnected();
	//GetService()->RemoveSession(GetSession());

	RegisterDisconnect();
}


bool Session::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != ServiceType::CLIENT)
		return false;

	if (!SocketHelper::SetReuseAddress(socket, true))
		return false;

	if (SocketHelper::BindAny(socket, 0) == false)
		return false;

	connectEvent.Init();
	connectEvent.iocpObj = shared_from_this();

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetSockAddr();
	if (SocketHelper::ConnectEx(socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr), nullptr, 0, &numOfBytes, &connectEvent))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != ERROR_IO_PENDING)
		{
			HandleError(errorCode);
			connectEvent.iocpObj = nullptr;
			return false;
		}
	}

	return true;
}

void Session::RegisterSend()
{
	if (!IsConnected())
		return;

	sendEvent.Init();
	sendEvent.iocpObj = shared_from_this();


	int writeSize = 0;
	while (!sendQueue.empty())
	{

		shared_ptr<SendBuffer> sendData = sendQueue.front();
		writeSize += sendData->WriteSize();

		sendQueue.pop();

		sendEvent.sendBuffers.push_back(sendData);
	}

	vector<WSABUF> wsaBufs;
	wsaBufs.reserve(sendEvent.sendBuffers.size());
	for (auto sendData : sendEvent.sendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = (char*)sendData->GetBuffer();
		wsaBuf.len = sendData->WriteSize();
		wsaBufs.push_back(wsaBuf);
	}

	DWORD sendLen = 0;
	DWORD flags = 0;

	if (WSASend(socket, wsaBufs.data(), (DWORD)wsaBufs.size(), &sendLen, flags, &sendEvent, nullptr) == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			sendEvent.iocpObj = nullptr;
			sendEvent.sendBuffers.clear();
			sendRegistered.store(false);
		}

	}
}

void Session::RegisterRecv()
{
	if (!IsConnected())
		return;

	recvEvent.Init();
	recvEvent.iocpObj = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)recvBuffer.WritePos();
	wsaBuf.len = recvBuffer.FreeSize();

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



bool Session::RegisterDisconnect()
{
	disconnectEvent.Init();
	disconnectEvent.iocpObj = shared_from_this();

	if (SocketHelper::DisconnectEx(socket, &disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			disconnectEvent.iocpObj = nullptr;
			return false;

		}
	}
	return true;
}

void Session::ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred)
{
	switch (iocpEvent->eventType)
	{
	case EventType::CONNECT:
		ProcessConnect();
		break;
	case EventType::RECV:
		ProcessRecv(bytesTransferred);
		break;
	case EventType::SEND:
		//보낼 크기만 보내기
		ProcessSend(bytesTransferred);
		break;
	case EventType::DISCONNECT:
		ProcessDisconnect();
		break;
	default:
		break;
	}
}


void Session::ProcessConnect()
{
	connectEvent.iocpObj = nullptr;

	isConnected.store(true);

	GetService()->AddSession(GetSession());

	OnConnected();

	RegisterRecv();
}

void Session::ProcessSend(int numOfBytes)
{
	//null로 밀고
	sendEvent.iocpObj = nullptr;
	// SendEvent의 sendBuffers 깨끗이
	sendEvent.sendBuffers.clear();

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0 bytes");
	}


	OnSend(numOfBytes);

	unique_lock<shared_mutex> lock(rwLock);
	if (sendQueue.empty())
		//sendRegistered 상태를 false
		sendRegistered.store(false);
	else
		//RegisterSend를 한번 더 실행
		RegisterSend();
}

void Session::ProcessRecv(int numOfBytes)
{
	recvEvent.iocpObj = nullptr;

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0 bytes");
		return;
	}

	if (recvBuffer.OnWrite(numOfBytes) == false)
	{
		Disconnect(L"On write overflow");
		return;
	}

	int dataSize = recvBuffer.DataSize();

	int processLen = OnRecv(recvBuffer.ReadPos(), numOfBytes);
	if (processLen < 0 || dataSize < processLen || recvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"On read overflow");
		return;
	}

	recvBuffer.Clear();

	RegisterRecv();

}


void Session::ProcessDisconnect()
{
	disconnectEvent.iocpObj = nullptr;

	//이동
	OnDisconnected();
	GetService()->RemoveSession(GetSession());
}


void Session::HandleError(int errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"Handle Error");
		break;
	default:
		printf("Error Code : %d\n", errorCode);
		break;
	}
}
