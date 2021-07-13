#include "SimpleDefine.h"
#include "SimpleClient.h"
#include <iostream>

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(172);
#endif
	Dump minidump(L"minidump");

	SimpleClient* simple;
	simple = new SimpleClient();

	simple->Create();
	simple->Init();
	simple->Run();
	simple->Wait();
	delete simple;

	return 0;
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}