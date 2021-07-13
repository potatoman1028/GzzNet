#include "Customer.h"

#include "DB_Connection.h"
#include "LoginServer.h"

Customer::Customer(int id, Network* net)
	:IToken(id, net, DEFAULT_TIME), Id(""), Pw(""), Certification(false)
{
}

Customer::~Customer()
{
}

void Customer::Req_Login()
{
	static_cast<DB_Connection*>(Net->Tokens[Net->DbConnectionTokenId])->Req_Login(SessionId, Id.c_str(), Pw.c_str());
}

void Customer::Req_Logout()
{
	if (Certification)
	{
		static_cast<DB_Connection*>(Net->Tokens[Net->DbConnectionTokenId])->Req_Logout(SessionId, Id.c_str(), Pw.c_str());
	}
}

void Customer::Req_Signup()
{
	static_cast<DB_Connection*>(Net->Tokens[Net->DbConnectionTokenId])->Req_Signup(SessionId, Id.c_str(), Pw.c_str());
}

void Customer::Req_Withdrawl()
{
	static_cast<DB_Connection*>(Net->Tokens[Net->DbConnectionTokenId])->Req_Withdrawl(SessionId, Id.c_str(), Pw.c_str());
}

void Customer::Ret_Login(int code)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;
	auto protocol = CreateOneCode(builder, code);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::LOGIN);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void Customer::Ret_Signup(int code)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;
	auto protocol = CreateOneCode(builder, code);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::SIGNUP);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void Customer::Ret_Withdrawl(int code)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;
	auto protocol = CreateOneCode(builder, code);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::WITHDRAWL);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void Customer::Ans_Login(PACKAGE& package)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	auto id_ = login->id();
	auto pw_ = login->pw();

	SetId(id_->c_str());
	SetPw(pw_->c_str());
	Req_Login();
}

void Customer::Ans_Signup(PACKAGE& package)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	auto id_ = login->id();
	auto pw_ = login->pw();

	SetId(id_->c_str());
	SetPw(pw_->c_str());
	Req_Signup();
}

void Customer::Ans_Withdrawl(PACKAGE& package)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	auto id_ = login->id();
	auto pw_ = login->pw();

	SetId(id_->c_str());
	SetPw(pw_->c_str());
	Req_Withdrawl();
}

void Customer::Ans_HeartBeat(PACKAGE& package)
{
	IToken::Ans_HeartBeat(package);
}

void Customer::DB_Ans_Login(const char* id, const char* pw, int code)
{
	if (code >= 0)
	{
		Certification = true;
		NetLogger::GetIns()->Write(L"customer", L"Login Sucess !!      ", id, L"  User No : ", code);
	}
	else
	{
		NetLogger::GetIns()->Write(L"customer", L"Login Fail..		   ", id, L"  User No : ", code);
	}
	Ret_Login(code);
}

void Customer::DB_Ans_Signup(const char* id, const char* pw, int code)
{
	Ret_Signup(code);
}

void Customer::DB_Ans_Withdrawl(const char* id, const char* pw, int code)
{
	Ret_Withdrawl(code);
}

void Customer::OnAccept(PACKAGE& package)
{
	Connection = true;
	NetLogger::GetIns()->Write(L"Customer", L"Connection Sucess Customer");
	Id = "";
	Pw = "";
}

void Customer::OnDisconnect_Accept(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"Customer", L"Try Logout");
	Req_Logout();

	NetLogger::GetIns()->Write(L"Customer", L"Disconnect Customer");
	Id = "";
	Pw = "";
	Connection = false;
}

void Customer::MappingProcotol()
{
	Mapping(this, static_cast<int>(PROTOCOL::LOGIN), &Customer::Ans_Login);
	Mapping(this, static_cast<int>(PROTOCOL::SIGNUP), &Customer::Ans_Signup);
	Mapping(this, static_cast<int>(PROTOCOL::WITHDRAWL), &Customer::Ans_Withdrawl);
	Mapping(this, static_cast<int>(PROTOCOL::HEARTBEAT), &Customer::Ans_HeartBeat);
}
