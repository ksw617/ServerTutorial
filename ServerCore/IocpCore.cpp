#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"
#include "Session.h"
#include "IocpObj.h"

IocpCore::IocpCore()
{
	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);
}

IocpCore::~IocpCore()
{
	CloseHandle(iocpHandle);
}

void IocpCore::Register(IocpObj* iocpObj)
{
	//iocpObj->GetHandle() : return (HANDLE)socket
	//key는 안사용할꺼라 0
	CreateIoCompletionPort(iocpObj->GetHandle(), iocpHandle, 0, 0);
}

bool IocpCore::ObserveIO(DWORD time)
{
	DWORD bytesTransferred = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	printf("Waiting...\n");
	if (GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, &key, (LPOVERLAPPED*)&iocpEvent, time))
	{
		//Session과 Listener는 IocpObj 상속받을꺼임
		IocpObj* iocpObj = iocpEvent->iocpObj;
		//iocpObj의 ObserveIO는 가상함수이기 때문에
		//할당된 자식이 Session이라면 Session->ObserveIO
		//할당된 자식이 Listener라면  Listener->ObserveIO
		iocpObj->ObserveIO(iocpEvent, bytesTransferred);
	}
	else
	{
							  
		//에러 코드 확인
		switch (GetLastError())
		{
		case WAIT_TIMEOUT:
			//GetQueuedCompletionStatus
			//기다리는 시간 넘어간거니까 
			//return false
			return false;
		default:
			break;
		}
		return false;
	}

	return true;
}
