#pragma once
#include "pch.h"
#include <ClientService.h>

#include "ServerSession.h"
#include "ServerPacketHandler.h"

int main()
{
	this_thread::sleep_for(1s);

	printf("==== CLIENT ====\n");

	ServerPacketHandler::Init();

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
