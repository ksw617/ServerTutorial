#pragma once
#include "IocpEvent.h" // ������ ������ ���漱�� IocpEvent�� IocpObj ���漱�� ��
class IocpObj
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void ObserveIO(IocpEvent* iocpEvent, DWORD bytesTransferred = 0) abstract;
};

