#include "pch.h"
#include <ServerService.h>
#include <SendBufferManager.h>

#include "ClientSession.h"
#include "SessionManager.h"

#define THREAD_COUNT 5

int main()
{
	printf("==== SERVER ====\n");
	shared_ptr<Service> service = make_shared<ServerService>(L"127.0.0.1", 27015, []() {return make_shared<ClientSession>(); });

	if (!service->Start())
	{
		printf("Server Service Error\n");
		return 1;
	}

	vector<thread> threads;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		threads.push_back(thread([=]()
			{
				while (true)
				{
					service->ObserveIO();
				}
			}
		));
	}

	BYTE sendData[1000] = "Hello world";

	//메인 스레드에서 접속한 애들한테 메세지 보내기
	while (true)
	{
		shared_ptr<SendBuffer> sendBuffer = SendBufferManager::Get().Open(4096);

		BYTE* buffer = sendBuffer->GetBuffer();

		int sendSize = sizeof(PacketHeader) + sizeof(sendData);
		((PacketHeader*)buffer)->size = sendSize;
		((PacketHeader*)buffer)->id = 0;
		memcpy(&buffer[4], sendData, sizeof(sendData));
		if (sendBuffer->Close(sendSize))
		{
			//접속한 애들 전체 다 보내기
			SessionManager::Get().Broadcast(sendBuffer);
		}

		//1초에 4번정도 전체 메세지
		this_thread::sleep_for(250ms);

	}


	for (int i = 0; i < THREAD_COUNT; i++)
	{
		if (threads[i].joinable())
		{
			threads[i].join();
		}
	}

	return 0;
}
