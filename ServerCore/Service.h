#pragma once
class IocpCore;

enum class ServiceType : uint8
{
	NONE,
	SERVER,
	CLIENT,
};

class Service
{
private:
	ServiceType serviceType = ServiceType::NONE;
	SOCKADDR_IN sockAddr = {};
	IocpCore* iocpCore = nullptr;
protected:
	shared_mutex rwLock;
public:
	Service(ServiceType type, wstring ip, uint16 port);
	virtual ~Service();
public:
	SOCKADDR_IN& GetSockAddr() { return sockAddr; }
	IocpCore* GetIocpCore() const { return iocpCore; }
public:
	virtual bool Start() abstract;
	bool ObserveIO(DWORD time = INFINITE);
};
