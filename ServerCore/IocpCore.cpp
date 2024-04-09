#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);
}

IocpCore::~IocpCore()
{
	CloseHandle(iocpHandle);
}

void IocpCore::Register(HANDLE socket, ULONG_PTR key)
{
	CreateIoCompletionPort(socket, iocpHandle, key, 0);;
}

bool IocpCore::ObserveIO(DWORD time)
{
	DWORD bytesTransferred = 0;
	ULONG_PTR key = 0;
	//Session을 iocpEvent로 교체
	IocpEvent* iocpEvent = nullptr;

	printf("Waiting...\n");
	//iocpEvent로 교체
	if (GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, &key, (LPOVERLAPPED*)&iocpEvent, time))
	{
		switch (iocpEvent->eventType)
		{
			case EventType::ACCEPT:
				printf("Client connected....\n");
				break;
		default:
			break;
		}
		
	}
	else
	{
		printf("GetQueuedCompletionStatus function failed with error : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}
