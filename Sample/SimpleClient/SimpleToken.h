#pragma once
#include "SimpleDefine.h"

#include <deque>
#include <mutex>
#include <chrono>

static constexpr int MIN_RAND = 20;

struct ECHO_PACKET;
struct ECHO_DATA;

class SimpleToken
{
	friend class SimpleClient;

protected:
	using Network = SimpleClient;
	std::deque<ECHO_DATA*> sendqueue;
	
	int SendTryCount;
	int SendFailCount;

	int DataMissCount;			// recvqueue�� sendqueue�� ������ �ùٸ��� ���� Ƚ��	
	int SeqMissCount;			// Sequence�� �ùٸ��� ���� Ƚ��
	int SeqMissFindSucessCount;	// Sequence�� �ùٸ��� ������ ã��.
	int SeqMissFindFailCount;	// ��Ŷ����
	double Rtt;					// Round Trip Time
	int StringMissCount;		// ��Ʈ�� �����Ͱ� �ùٸ��� ���� Ƚ��	
	int ReceiveCount;

#pragma region Network
	bool Connection;														// ������ �Ǿ��ִ°�?
	bool HadConnected;															// ���� �߾��°�?
	int SessionId;															// ���� ��ū�� ���εǾ��ִ� Sessionid

	RpcMap FuncMap;															// Network Call Func
	Network* Net;															// Network
#pragma endregion

#pragma region Echo Packet Data
	int EchoSeq;															// Echo ��Ŷ�� ������ ���� ������
	int EchoCount;															// �� ���� ���� Echo �õ� Ƚ��
	int EchoReceiveCount;															// �� ���� ���� Echo �õ� Ƚ��

	int EchoFailCount;														// Send �õ� Fail Ƚ��
	int EchoMissCount;														// Seq�� ���� �ȸ´� ���

	int EchoMaxRand;														// Echo ��Ʈ�� Max ī��Ʈ					
	int EchoMinRand;														// Echo ��Ʈ�� Min ī��Ʈ

	__int64 EchoSendTime;													// Echo Packet Send �ֱ�
	std::chrono::steady_clock::time_point EchoSendTimer;					// Echo Pcket Send Timer
#pragma endregion

#pragma region BroadCast Packet Data
	int BroadCastSeq;														// BroadCast ��Ŷ�� ������ ���� ������
	int BroadCastCount;														// �� ���� ���� BroadCast �õ� Ƚ��

	int BroadCastFailCount;													// Send �õ� Fail Ƚ��
	int BroadCastMissCount;													// Seq�� ���� �ȸ´� ���

	int BroadCastMaxRand;													// BroadCast ��Ʈ�� Max ī��Ʈ					
	int BroadCastMinRand;													// BroadCast ��Ʈ�� Min ī��Ʈ

	__int64 BroadCastSendTime;												// BroadCast Packet Send �ֱ�
	std::chrono::steady_clock::time_point BroadCastSendTimer;				// BroadCast Pcket Send Timer
#pragma endregion

#pragma region Util
	Rand random;
#pragma endregion

#pragma region Lock
	std::mutex EchosendLock;
	std::mutex EchoRecvLock;
#pragma endregion


public:
	SimpleToken() = delete;
	SimpleToken(int id, Network* net);
	virtual ~SimpleToken();

public:
	int SendMsg(PACKET& packet);								// �޼��� send
	const int GetSessionId()
	{
		return SessionId;
	}

public:
	void EchoSetting(const int trycnt, const int minrand, const int maxrand, const __int64 time);
	void BroadCastSetting(const int trycnt, const int minrand, const int maxrand, const __int64 time);

	virtual void MappingProcotol();

	template<class C>
	void Mapping(C* this_class, int index, CLASS_MEM_PACKAGE_FUNC<C> func)
	{
		FuncMap.Mapping(func, index, this_class);
	}

public:
	int GetEchoFailCount() { return EchoFailCount; };
	int GetEchoTryCount() { return EchoSeq; };

protected:
	void Disconnect();

	virtual void OnAccept(PACKAGE& package);					// accept sucess
	virtual void OnConnect(PACKAGE& package);					// connect sucess
	virtual void OnConnectFail(PACKAGE& package);				// connect fail
	virtual void OnDisconnect_Accept(PACKAGE& package);			// disconnect session
	virtual void OnDisconnect_Connect(PACKAGE& package);		// disconnect session
	virtual void OnJob(PACKAGE& package);

	void EchoSend();
	void BroadCastSend();
	void EchoReceive(PACKAGE& package);
	void BroadCastReceive(PACKAGE& package);

	void EchoCalcResult();

	void BroadCastCalcResult();

	char* MakeAlphabet(int size);

	void EchoProcess();
	void BroadCastProcess();
};