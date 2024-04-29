#include "pch.h"
#include <ServerService.h>
#include <SendBufferManager.h>

#include "ClientSession.h"
#include "SessionManager.h"

#include "Protocol.pb.h"

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

	while (true)
	{
		Protocol::Login packet;
	
		Protocol::Player* player = new Protocol::Player();
		player->set_id(1);
		player->set_name("곽상휘");
		packet.set_allocated_player(player);

		uint16 dataSize = (uint16)packet.ByteSizeLong();
		uint16 packetSize = dataSize + sizeof(PacketHeader);

		shared_ptr<SendBuffer> sendBuffer = SendBufferManager::Get().Open(4096);
		BYTE* buffer = sendBuffer->GetBuffer();
		((PacketHeader*)buffer)->size = packetSize;
		((PacketHeader*)buffer)->id = 0;

		packet.SerializeToArray(&buffer[4], dataSize);
		if (sendBuffer->Close(packetSize))
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
