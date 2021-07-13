#pragma once
#include "AgentDefine.h"

class DBAgent;
class IToken
{
	enum class HEART_BEAT { STOP, CHECKING, BEAT };			// 실패, 체크중, 성공

protected:
	friend class DBAgent;
	using Network = DBAgent;

	int SessionId;
	bool Connection;

	RpcMap FuncMap;
	Network* Net;

	HEART_BEAT Heart;

	std::chrono::steady_clock::time_point HeartBeatStart;
	__int64 HeartBeatTime;

public:
	IToken() = delete;
	IToken(int id, Network* net, __int64 heartbeat);
	virtual ~IToken();

public:
	bool SendMsg(PACKET& packet);								// 메세지 send
	const int GetSessionId()
	{
		return SessionId;
	}

public:
	virtual void MappingProcotol() = 0;

	template<class C>
	void Mapping(C* this_class, int index, CLASS_MEM_PACKAGE_FUNC<C> func)
	{
		FuncMap.Mapping(func, index, this_class);
	}
	void HeartBeatCheck();
	void Req_HeartBeat();

protected:
	void Disconnect();
	virtual void OnAccept(PACKAGE& package) {};					// accept sucess
	virtual void OnConnect(PACKAGE& package) {};				// connect sucess
	virtual void OnConnectFail(PACKAGE& package) {};			// connect fail
	virtual void OnDisconnect_Accept(PACKAGE& package) {};		// disconnect session
	virtual void OnDisconnect_Connect(PACKAGE& package) {};		// disconnect session
	virtual void OnJob(PACKAGE& package);
	virtual void Ans_HeartBeat(PACKAGE& package);
};