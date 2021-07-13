#pragma once
#include "IToken.h"

class DBAgent;
class Login : public IToken
{
	static constexpr int DEFAULT_TIME = 360;
	friend class DBAgent;
private:

public:
	Login() = delete;
	Login(int id, Network* net);
	virtual ~Login();

	virtual void MappingProcotol() override;

public:
	void Ret_Login(int sessionid, int uid, const char* id, const char* pw);
	void Ret_Logout(int sessionid, int uid, const char* id, const char* pw);
	void Ret_Signup(int sessionid, int uid, const char* id, const char* pw);
	void Ret_Withdrawl(int sessionid, int uid, const char* id, const char* pw);

private:
	virtual void OnAccept(PACKAGE& package) override;					// accept sucess
	virtual void OnDisconnect_Accept(PACKAGE& package) override;		// disconnect session

	// FROM DBA TO CUSTOMER
	void Ans_Login(PACKAGE& package);
	void Ans_Logout(PACKAGE& package);
	void Ans_Signup(PACKAGE& package);
	void Ans_Withdrawl(PACKAGE& package);

	void Ans_HeartBeat(PACKAGE& package) override;
};