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
	void Register(HANDLE socket, ULONG_PTR key);
	bool ObserveIO(DWORD time = INFINITE); // INFINITE : ���� INFINITY : �Ǽ�
};

