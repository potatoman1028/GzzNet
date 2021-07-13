#pragma once
#include "SimpleDefine.h"

#include <deque>
#include <mutex>

class SimpleToken
{
	friend class SimpleServer;

protected:
	using Network = SimpleServer;

#pragma region Network
	bool Connection;
	bool HadConnected;
	int SessionId;

	RpcMap FuncMap;
	Network* Net;
#pragma endregion

#pragma region Echo Packet Data
	int EchoSeq;															// Echo ��Ŷ�� ������ ���� ������
	int EchoCount;													// �� ���� ���� Echo �õ� Ƚ��
	int SendTryCount;
	int SendFailCount;

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

public:
	SimpleToken() = delete;
	SimpleToken(int id, Network* net);
	virtual ~SimpleToken();

public:
	bool SendMsg(PACKET& packet);								// �޼��� send
	const int GetSessionId()
	{
		return SessionId;
	}

public:
	virtual void MappingProcotol();

	template<class C>
	void Mapping(C* this_class, int index, CLASS_MEM_PACKAGE_FUNC<C> func)
	{
		FuncMap.Mapping(func, index, this_class);
	}

public:
	void EchoSetting(const int trycnt, const int minrand, const int maxrand, const __int64 time);
	void BroadCastSetting(const int trycnt, const int minrand, const int maxrand, const __int64 time);

protected:
	void Disconnect();
	virtual void OnAccept(PACKAGE& package);					// accept sucess
	virtual void OnConnect(PACKAGE& package);				// connect sucess
	virtual void OnConnectFail(PACKAGE& package);			// connect fail
	virtual void OnDisconnect_Accept(PACKAGE& package);		// disconnect session
	virtual void OnDisconnect_Connect(PACKAGE& package);		// disconnect session
	virtual void OnJob(PACKAGE& package);

	void EchoSend();
	void BroadCastSend();

	void EchoReceive(PACKAGE& package);
	void BroadCastReceive(PACKAGE& package);

	char* MakeAlphabet(int size);

	void Process(const double delta);

	void EchoProcess();
	void BroadCastProcess();
};
