#pragma once
#include "SimpleDefine.h"


class SimpleClient
{
	using SimpleFunc = std::function<void(SimpleClient&, PACKAGE&)>;

public:
	static IOCP* Net;
	std::vector<SimpleToken*> Clients;

	enum MODE {ECHO, BROACAST, STOP};

private:
	// network init

	std::wstring Ip;
	int Port;

	int MaxSession;
	int ExtraSession;
	int NetworkThreadCount;
	int ConnectSessionCount;
	
	int EchoTryCount;
	int EchoMinCount;
	int EchoMaxCount;

	__int64 EchoTerm;

	int BroadCastTryCount;
	int BroadCastMinCount;
	int BroadCastMaxCount;

	__int64 BroadCastTerm;

	__int64 HeartBeatTime;

	int TotalTryCount;			// 총 Send 시도 횟수
	int TotalFailCount;			// 총 Send 실패 횟수
	int TotalReceiveCount;		// 총 Recv 횟수

	int DataMissCount;			// recvqueue와 sendqueue의 갯수가 올바르지 않은 횟수	

	int SeqMissFindSucessCount; // Sequence가 올바르지 않을 경우. SendQueue에서 다시 찾아 원복한 갯수.
	int SeqMissCount;			// Sequence가 올바르지 않은 횟수
	int SeqMissFindFailCount;		 // Sequence가 올바르지 않은 횟수
	int StringMissCount;		// 스트링 데이터가 올바르지 않은 횟수	

	double AverageRtt;			// Round Trip Time
	double MaximumRtt;			// Maxnum Slow Trip Time

	int Mode;
	int PreMode;
	int EchoCalcResetCount;

	bool IsGetResult;

private:
	HANDLE ExitEvent;

	std::atomic<bool> MainBtn;
	
	std::thread MainThread;

	std::vector<SimpleFunc> FuncTable;

	Monitor Mon;
	Timer* Time;

	double CheckTime;
	double MonitoringTime;

	bool IsConnectTry;	
	bool IsConnectReTry;
	bool IsDisconnectTry;

public:
	SimpleClient();
	~SimpleClient();

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
	static void	FourceEnque(PACKAGE& package);
	
	static int SendPacket(int dst, PACKET& packet);

private:
	void ConnectTry(int size);
	void ConnectReTry();

	void DisconnectTry();
	bool ReadPackage(int count);

	void ModeSelect();

	void EchoProcess();
	void BroadCastProcess();
	void Stop_GetResult();

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

