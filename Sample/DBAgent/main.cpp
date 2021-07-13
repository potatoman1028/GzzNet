#include "DBAgent.h"
#include <iostream>

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(172);
#endif
	DBAgent* agent;
	agent = new DBAgent();

	agent->Create();
	agent->Init();
	agent->Run();
	agent->Wait();
	delete agent;

	return 0;
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}





