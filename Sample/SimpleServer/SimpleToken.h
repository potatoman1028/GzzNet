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
	int EchoSeq;															// Echo 패킷을 날리는 순서 데이터
	int EchoCount;													// 한 루프 내의 Echo 시도 횟수
	int SendTryCount;
	int SendFailCount;

	int EchoFailCount;														// Send 시도 Fail 횟수
	int EchoMissCount;														// Seq가 서로 안맞는 경우
	
	int EchoMaxRand;														// Echo 스트링 Max 카운트
	int EchoMinRand;														// Echo 스트링 Min 카운트
	
	__int64 EchoSendTime;													// Echo Packet Send 주기
	std::chrono::steady_clock::time_point EchoSendTimer;					// Echo Pcket Send Timer
#pragma endregion

#pragma region BroadCast Packet Data
	int BroadCastSeq;														// BroadCast 패킷을 날리는 순서 데이터
	int BroadCastCount;														// 한 루프 내의 BroadCast 시도 횟수

	int BroadCastFailCount;													// Send 시도 Fail 횟수
	int BroadCastMissCount;													// Seq가 서로 안맞는 경우

	int BroadCastMaxRand;													// BroadCast 스트링 Max 카운트					
	int BroadCastMinRand;													// BroadCast 스트링 Min 카운트

	__int64 BroadCastSendTime;												// BroadCast Packet Send 주기
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
	bool SendMsg(PACKET& packet);								// 메세지 send
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
