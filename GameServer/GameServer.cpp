#include "pch.h"
#include <ServerService.h>


int main()
{
	printf("==== SERVER ====\n");

	Service* service = new ServerService(L"127.0.0.1", 27015);

	if (!service->Start())
	{
		printf("Server Service Error\n");
		return 1;
	}


	//람다로 관찰 실행
	thread t([=]()
		{
			while (true)
			{
				service->ObserveIO();
			}
		}
	);


	t.join();

	delete service;

	return 0;
}
