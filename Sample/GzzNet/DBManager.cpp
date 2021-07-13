#include "DBManager.h"
#include "Ado.h"
//#include "Note.h"

gzz::DBManager::DBManager()
	:ConnectString(""), DBCount(0)
{
}

gzz::DBManager::~DBManager()
{
}

void gzz::DBManager::Init(int count, std::string constr)
{
	DBCount = count;
	ConnectString = constr;
	DbPool.reserve(count);

	for (int i = 0; i < count; i++)
	{
		DbPool.emplace_back(new Ado(i, this));
		DbPool[i]->SetConnectString(ConnectString.c_str());
		DbPool[i]->Connect();
	}
}

void gzz::DBManager::Release()
{
	int size = static_cast<int>(DbPool.size());
	for (int i = 0; i < size; ++i)
	{
		if (DbPool[i])
		{
			delete DbPool[i];
			DbPool[i] = nullptr;
		}
	}
}

void gzz::DBManager::Run()
{
	int size = static_cast<int>(DbPool.size());
	for (int i = 0; i < size; ++i)
	{
		if (DbPool[i])
		{
			DbPool[i]->StartThread();
		}
	}
}

bool gzz::DBManager::AddQuery(Query* query)
{
	return QueryJobQueue.Enqueue(query);
}

bool gzz::DBManager::PopQuery(Query** query)
{
	return QueryJobQueue.Dequeue(*query);
}

int gzz::DBManager::QueryCount()
{
	return QueryJobQueue.Size();
}
