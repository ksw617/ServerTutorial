#include "pch.h"
#include "SocketHelper.h"

//�ʱ�ȭ
LPFN_CONNECTEX SocketHelper::ConnectEx = nullptr;
LPFN_ACCEPTEX SocketHelper::AcceptEx = nullptr;
LPFN_DISCONNECTEX SocketHelper::DisconnectEx = nullptr;

bool SocketHelper::StartUp()
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return false;

    SOCKET tempSocket = CreateSocket();
    //�Լ� �����Ϳ� �ּҰ� �־���
    SetIoControl(tempSocket, WSAID_CONNECTEX, (LPVOID*)&ConnectEx);
    SetIoControl(tempSocket, WSAID_ACCEPTEX, (LPVOID*)&AcceptEx);
    SetIoControl(tempSocket, WSAID_DISCONNECTEX, (LPVOID*)&DisconnectEx);

    CloseSocket(tempSocket);
}

bool SocketHelper::CleanUp()
{
    return WSACleanup();
}

bool SocketHelper::SetIoControl(SOCKET socket, GUID guid, LPVOID* func)
{
    DWORD dwBytes = 0;
    return WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), func, sizeof(*func), &dwBytes, NULL, NULL) != SOCKET_ERROR;

}

SOCKET SocketHelper::CreateSocket()
{
    return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketHelper::SetReuseAddress(SOCKET socket, bool enable)
{
    return SetSocketOpt(socket, SOL_SOCKET, SO_REUSEADDR, enable);
}

bool SocketHelper::SetLinger(SOCKET socket, u_short onOff, u_short time)
{
    LINGER linger;
    linger.l_onoff = onOff;
    linger.l_linger = time;
    return SetSocketOpt(socket, SOL_SOCKET, SO_LINGER, linger);
}

bool SocketHelper::SetUpdateAcceptSocket(SOCKET acceptSocket, SOCKET ListenSocket)
{
    return SetSocketOpt(acceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, ListenSocket);
}

bool SocketHelper::Bind(SOCKET socket, SOCKADDR_IN sockAddr)
{
    return bind(socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != SOCKET_ERROR;
}

//�ƹ� �ּ� ���̵�
bool SocketHelper::BindAny(SOCKET socket, uint16 port)
{
    SOCKADDR_IN sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(port);

    return bind(socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != SOCKET_ERROR;
}

bool SocketHelper::Listen(SOCKET socket, int backlog)
{
    return listen(socket, backlog) != SOCKET_ERROR;
}

void SocketHelper::CloseSocket(SOCKET& socket)
{
    if (socket != INVALID_SOCKET)
    {
        closesocket(socket);
        socket = INVALID_SOCKET;
    }
}
