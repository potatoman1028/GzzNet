#include "LoginServer.h"
#include <iostream>

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(377);
#endif
	LoginServer* server;
	server = new LoginServer();
	server->Create();
	server->Init();
	server->Run();
	server->Wait();
	delete server;
	return 0;
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}





