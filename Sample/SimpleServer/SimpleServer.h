#pragma once
#include "SimpleDefine.h"

class SimpleServer
{
	using SimpleFunc = std::function<void(SimpleServer&, PACKAGE&)>;

public:
	static IOCP* Net;
	std::vector<SimpleToken*> Clients;

private:
	// network init

	std::wstring Ip;
	int Port;

	// 최대 Accept Session 갯수
	int MaxSession;
	// 최대 Connect Session 갯수
	int ExtraSession;
	// Network Thread Count
	int NetworkThreadCount;
	// 현재 Connect 된 갯수
	int ConnectSessionCount;

	// 한번에 Echo 패킷을 보낼 양
	int EchoTryCount;
	// Echo 패킷 최소 길이
	int EchoMinCount;
	// Echo 패킷 최대 길이
	int EchoMaxCount;
	// Echo 패킷을 보낼 간격
	__int64 EchoTerm;

	int BroadCastTryCount;
	int BroadCastMinCount;
	int BroadCastMaxCount;

	__int64 BroadCastTerm;
	// Heart Beat 패킷을 보낼 간격
	__int64 HeartBeatTime;

	int TotalTryCount;
	int TotalFailCount;

private:
	HANDLE ExitEvent;

	std::atomic<bool> MainBtn;
	std::thread MainThread;

	std::vector<SimpleFunc> FuncTable;

	Monitor Mon;
	Timer* Time;

	double CheckTime;
	double MonitoringTime;

public:
	SimpleServer();
	~SimpleServer();

private:
	
public:
	void Create();
	void Init();
	void Wait();
	void Run();
	void Release();

	static void	Connect(std::wstring ip, unsigned short port);
	static bool	Connect(int id, std::wstring ip, unsigned short port);

	static void	Disconnect(int session_id);
	static int SendPacket(int dst, PACKET& packet);
	static void	FourceEnque(PACKAGE& package);

private:
	bool ReadPackage(int count);

	void Process();
	void Monitoring();

	// network system call back
	void NotImplemented(PACKAGE& package);
	void OnAccept(PACKAGE& package);				// accept sucess
	void OnConnect(PACKAGE& package);				// connect sucess
	void OnConnectFail(PACKAGE& package);			// connect fail
	void OnDisconnect_Accept(PACKAGE& package);		// disconnect session
	void OnDisconnect_Connect(PACKAGE& package);	// disconnect session
	void OnJob(PACKAGE& package);					// other packet
};

