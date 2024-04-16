#pragma once
class SocketHelper
{
public:
	//비동기 connect함수 포인터
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_ACCEPTEX AcceptEx;
	//비동기 Disconnect함수 포인터
	static LPFN_DISCONNECTEX DisconnectEx;
public:
	static bool StartUp();
	static bool CleanUp();
public:
	static bool SetIoControl(SOCKET socket, GUID guid, LPVOID* func);
	static SOCKET CreateSocket();
public:
	static bool SetReuseAddress(SOCKET socket, bool enable);
	static bool SetLinger(SOCKET socket, u_short onOff, u_short time);
	static bool SetUpdateAcceptSocket(SOCKET acceptSocket, SOCKET ListenSocket);
public:
	static bool Bind(SOCKET socket, SOCKADDR_IN sockAddr);
	//아무 주소 바인딩
	static bool BindAny(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int backlog = SOMAXCONN);
	static void CloseSocket(SOCKET& socket);
};

template<typename T>
static inline bool SetSocketOpt(SOCKET socket, int level, int optName, T optVal)
{
	return setsockopt(socket, level, optName, (char*)&optVal, sizeof(T)) != SOCKET_ERROR;
}

