#pragma once
#include "IToken.h"

class LoginServer;
class DB_Connection : public IToken
{
	static constexpr int DEFAULT_TIME = 360;
	friend class LoginServer;

private:

public:
	DB_Connection() = delete;
	DB_Connection(int id, Network* net);
	virtual ~DB_Connection();

public:
	virtual void OnConnect(PACKAGE& package) override;				// connect sucess
	virtual void OnConnectFail(PACKAGE& package) override;			// connect fail
	virtual void OnDisconnect_Connect(PACKAGE& package) override;		// disconnect session
	virtual void MappingProcotol() override;

	// FROM CUSTOMER TO DBA
	void Req_Login(int sessionid, const char* id, const char* pw);
	void Req_Logout(int sessionid, const char* id, const char* pw);
	void Req_Signup(int sessionid, const char* id, const char* pw);
	void Req_Withdrawl(int sessionid, const char* id, const char* pw);

private:
	// FROM DBA TO CUSTOMER
	void Ans_Login(PACKAGE& package);
	void Ans_Logout(PACKAGE& package);
	void Ans_Signup(PACKAGE& package);
	void Ans_Withdrawl(PACKAGE& package);
	void Ans_HeartBeat(PACKAGE& package) override;
};