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

	int TotalTryCount;			// �� Send �õ� Ƚ��
	int TotalFailCount;			// �� Send ���� Ƚ��
	int TotalReceiveCount;		// �� Recv Ƚ��

	int DataMissCount;			// recvqueue�� sendqueue�� ������ �ùٸ��� ���� Ƚ��	

	int SeqMissFindSucessCount; // Sequence�� �ùٸ��� ���� ���. SendQueue���� �ٽ� ã�� ������ ����.
	int SeqMissCount;			// Sequence�� �ùٸ��� ���� Ƚ��
	int SeqMissFindFailCount;		 // Sequence�� �ùٸ��� ���� Ƚ��
	int StringMissCount;		// ��Ʈ�� �����Ͱ� �ùٸ��� ���� Ƚ��	

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

