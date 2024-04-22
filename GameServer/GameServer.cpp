#include "pch.h"
#include <ServerService.h>
#include <Session.h>

class ServerSession : public Session
{
public:
	~ServerSession()
	{
		cout << "ServerSession Destroy" << endl;
	}

	virtual void OnConnected() override
	{
		cout << "On Connect ȣ��" << endl;
	}

	virtual int OnRecv(BYTE* buffer, int len) override
	{
		cout << "OnRecv : " << buffer << ", On Recv Len : " << len << endl;

		//Todo ����

		Send(buffer, len);
		return len;
	}

	virtual void OnSend(int len) override
	{
		cout << "OnSend Len : " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		cout << "On DisConnect ȣ��" << endl;
	}

};


int main()
{
	printf("==== SERVER ====\n");

	//����Ʈ �����ͷ� ��ȯ Service & ServerSession
	shared_ptr<Service> service = make_shared<ServerService>(L"127.0.0.1", 27015, []() {return make_shared<ServerSession>(); });


	if (!service->Start())
	{
		printf("Server Service Error\n");
		return 1;
	}

	thread t([=]()
		{
			while (true)
			{
				service->ObserveIO();
			}
		}
	);


	t.join();

	//delete service;

	return 0;
}
