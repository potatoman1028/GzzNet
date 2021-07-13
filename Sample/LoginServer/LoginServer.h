#pragma once
#define DB_DEBUGING

#include "LoginDefine.h"
#include "Customer.h"
#include "DB_Connection.h"

class LoginServer
{
	using LoginFunc = std::function<void(LoginServer&, PACKAGE&)>;

public:
	int DbConnectionTokenId;
	std::vector<IToken*> Tokens;
	static IOCP* Net;

private:
	HANDLE ExitEvent;

	std::atomic<bool> MainBtn;
	std::atomic<bool> NetBtn;

	std::thread MainThread;
	std::thread NetWoker;

	std::vector<LoginFunc> LoginFuncTable;

	Monitor Mon;
	Timer* Time;

	double MonitorCheckTime;
	double MonitoringTime;

	double HeartCheckTime;
	double HeartBeatTime;

public:
	LoginServer();
	virtual ~LoginServer();

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
	void Process();
	void Monitoring();
	void HeartBeatChk();

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

