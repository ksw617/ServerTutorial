#include "pch.h"
#include <Service.h>


int main()
{
	printf("==== SERVER ====\n");

	Service* service = new Service(L"127.0.0.1", 27015);


	//���ٷ� ���� ����
	thread t([=]()
		{
			while (true)
			{
				service->ObserveIO();
			}
		}
	);

	service->Start();

	t.join();

	delete service;

	return 0;
}
