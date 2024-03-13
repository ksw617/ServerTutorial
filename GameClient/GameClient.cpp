#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")	
#include <WinSock2.h>	
#include <WS2tcpip.h>  // ���

int main()
{

	WORD wVersionRequested;
	WSAData wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		printf("WSAStartup failed with error\n");
		return 1;
	}

	SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket function failed with error : %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}


	SOCKADDR_IN service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);
	service.sin_port = htons(27015);

	//������ ���� service(������ ���� ����)
	//connect �Լ� ���� �߻���
	if (connect(connectSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		//���� �ڵ� Ȯ��
		printf("connect function failed with error : %d\n", WSAGetLastError());
		//���� �ݾ��ְ�
		closesocket(connectSocket);
		//win sock dll ��� ����
		WSACleanup();
		//���α׷� ����
		return 1;

	}

	printf("Connect to Server\n");

	while (true)
	{
		//Todo
		Sleep(1000);
	}

	closesocket(connectSocket);
	WSACleanup();

}
