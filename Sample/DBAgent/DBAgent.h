#pragma once
// #define DB_DEBUGING
#include "AgentDefine.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include <functional>
#include "Login.h"
#include "Game.h"

class DBAgent 
{
	using DBFunc = std::function<void(DBAgent&, PACKAGE&)>;

public:
	static IOCP* Net;
	static DBManager* DBM;
	static int BestScore;
	std::vector<IToken*> Clients;

private:
	HANDLE ExitEvent;

	std::thread MainThread;
	std::thread NetWoker;

	std::atomic<bool> OnBtn;
	std::vector<DBFunc> DBFuncTable;

	Monitor Mon;
	Timer* Time;

	double CheckTime;
	double MonitoringTime;
private:
	bool toggleQ = false;
	bool toggleW = false;
	bool toggleE = false;
	bool toggleR = false;

	bool toggle0 = false;
	bool toggle1 = false;
	bool toggle2 = false;
	bool toggle3 = false;
	bool toggle4 = false;
	bool toggle5 = false;
	bool toggle6 = false;
	bool toggle7 = false;
	bool toggle8 = false;
	bool toggle9 = false;

public:
	DBAgent();
	~DBAgent();

public:
	void Create();
	void Init();
	void Wait();
	void Run();
	void Release();

	static void	Connect(std::wstring ip, unsigned short port);
	static bool	Connect(int id, std::wstring ip, unsigned short port);

	static void	Disconnect(int session_id);
	static void SendPacket(int dst, PACKET& packet);
	static void	FourceEnque(PACKAGE& package);

private:
	void DB_Debuging_Process();
	void Process();
	void Monitoring();

private:
	bool ReadPackage();
	
	// network system call back
	void NotImplemented(PACKAGE& package);
	void OnAccept(PACKAGE& package);				// accept sucess
	void OnConnect(PACKAGE& package);				// connect sucess
	void OnConnectFail(PACKAGE& package);			// connect fail
	void OnDisconnect_Accept(PACKAGE& package);		// disconnect session
	void OnDisconnect_Connect(PACKAGE& package);	// disconnect session
	void OnJob(PACKAGE& package);					// other packet
};

