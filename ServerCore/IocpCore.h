#pragma once
class IocpCore
{
private:
	HANDLE iocpHandle = NULL;
public:
	IocpCore();
	~IocpCore();
public:
	HANDLE GetHandle() const { return iocpHandle; }
public:
	//bool�� �ٲ�
	bool Register(class IocpObj* iocpObj);
	bool ObserveIO(DWORD time = INFINITE);
};

