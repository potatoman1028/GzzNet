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

	// �ִ� Accept Session ����
	int MaxSession;
	// �ִ� Connect Session ����
	int ExtraSession;
	// Network Thread Count
	int NetworkThreadCount;
	// ���� Connect �� ����
	int ConnectSessionCount;

	// �ѹ��� Echo ��Ŷ�� ���� ��
	int EchoTryCount;
	// Echo ��Ŷ �ּ� ����
	int EchoMinCount;
	// Echo ��Ŷ �ִ� ����
	int EchoMaxCount;
	// Echo ��Ŷ�� ���� ����
	__int64 EchoTerm;

	int BroadCastTryCount;
	int BroadCastMinCount;
	int BroadCastMaxCount;

	__int64 BroadCastTerm;
	// Heart Beat ��Ŷ�� ���� ����
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

