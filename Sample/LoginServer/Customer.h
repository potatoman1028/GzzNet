#pragma once
#include "IToken.h"

class LoginServer;
class Customer : public IToken
{
	static constexpr int DEFAULT_TIME = 30;
	friend class DB_Connection;
	friend class LoginServer;

private:
	std::string Id;
	std::string Pw;
	
	bool Certification;

public:
	Customer() = delete;
	Customer(int id, Network* net);
	virtual ~Customer();
	
public:
	virtual void OnAccept(PACKAGE& package) override;					// accept sucess
	virtual void OnDisconnect_Accept(PACKAGE& package) override;		// disconnect session
	virtual void MappingProcotol() override;

public:
	const char* GetId() { return Id.c_str(); }
	void SetId(std::string_view id) { Id = id.data(); }
	const char* GetPw() { return Pw.c_str(); }
	void SetPw(std::string_view pw) { Pw = pw.data(); }

	// TO DBC
	void Req_Login();
	void Req_Logout();
	void Req_Signup();
	void Req_Withdrawl();

private:
	// TO CLIENT
	void Ret_Login(int code);
	void Ret_Signup(int code);
	void Ret_Withdrawl(int code);

	// FROM CLINET
	void Ans_Login(PACKAGE& package);
	void Ans_Signup(PACKAGE& package);
	void Ans_Withdrawl(PACKAGE& package);
	void Ans_HeartBeat(PACKAGE& package) override;

	// FROM DBC
	void DB_Ans_Login(const char* id, const char* pw, int code);
	void DB_Ans_Signup(const char* id, const char* pw, int code);
	void DB_Ans_Withdrawl(const char* id, const char* pw, int code);
};

