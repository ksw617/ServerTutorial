#include "pch.h"
#include "ServerService.h"
#include "Listener.h"


ServerService::ServerService(wstring ip, uint16 port, SessionFactory factory) : Service(ServiceType::SERVER, ip, port, factory)
{
}

bool ServerService::Start()
{
	listener = make_shared<Listener>();
	//shared_from_this()-> Service狼 林家 -> static_pointer_cast<ServerService>(Service狼 林家) -> ServerService狼 林家 
	return listener->StartAccept(static_pointer_cast<ServerService>(shared_from_this()));
}

