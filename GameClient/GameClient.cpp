#pragma once
#include "pch.h"
#include <ClientService.h>

#include "ServerSession.h"

int main()
{
	printf("==== CLIENT ====\n");

	shared_ptr<Service> service = make_shared<ClientService>(L"127.0.0.1", 27015, []() {return make_shared<ServerSession>(); });

	for (int i = 0; i < 1; i++)
	{
		if (!service->Start())
		{
			printf("Server Service Error\n");
			return 1;
		}
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


	return 0;
}
