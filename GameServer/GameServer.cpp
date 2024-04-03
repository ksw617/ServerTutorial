#include "pch.h"
#include <Service.h>
#include <Listener.h>
#include <IocpCore.h>

static void AcceptThrad(HANDLE iocpHandle)
{
	DWORD bytesTransferred = 0;
	ULONG_PTR key = 0;
	WSAOVERLAPPED* overlapped = {};

	while (true)
	{
		printf("Waiting...\n");
		if (GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, &key, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			printf("Client connected....\n");
		}
	}
}

int main()
{
	printf("==== SERVER ====\n");

	//동적할당으로 변경
	Service* service = new Service(L"127.0.0.1", 27015);

	Listener listener;
	
	thread t(AcceptThrad, service->GetIocpCore()->GetHandle());

	listener.StartAccept(service);

	t.join();


	return 0;
}
