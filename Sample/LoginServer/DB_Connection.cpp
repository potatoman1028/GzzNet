#include "DB_Connection.h"

#include "Customer.h"
#include "LoginServer.h"

DB_Connection::DB_Connection(int id, Network* net)
	:IToken(id, net, DEFAULT_TIME)
{
}

DB_Connection::~DB_Connection()
{
}

void DB_Connection::OnConnect(PACKAGE& package)
{
	Connection = true;
	NetLogger::GetIns()->Write(L"DB_Connector", L"Connection Sucess DBAgent");
}

void DB_Connection::OnConnectFail(PACKAGE& package)
{
	Connection = false;
	NetLogger::GetIns()->Write(L"DB_Connector", L"Connection Fail DBAgent");
}

void DB_Connection::OnDisconnect_Connect(PACKAGE& package)
{
	Connection = false;
	NetLogger::GetIns()->Write(L"DB_Connector", L"Disconnect DBAgent");
}

void DB_Connection::Req_Login(int sessionid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = -1;
	auto id_ = builder.CreateString(id);
	auto pw_ = builder.CreateString(pw);

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_LOGIN);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_Logout(int sessionid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = -1;
	auto id_ = builder.CreateString(id);
	auto pw_ = builder.CreateString(pw);

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_LOGOUT);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_Signup(int sessionid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = -1;
	auto id_ = builder.CreateString(id);
	auto pw_ = builder.CreateString(pw);

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_SIGNUP);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Req_Withdrawl(int sessionid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = -1;
	auto id_ = builder.CreateString(id);
	auto pw_ = builder.CreateString(pw);

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_WITHDRAWL);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void DB_Connection::Ans_Login(PACKAGE& package)
{
	using namespace db::Packet;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	auto id_ = login->id();
	auto pw_ = login->pw();
	
	size_t size = Net->Tokens.size();
	if (no_ < 0 || no_ > size || no_ == SessionId) return;	

	static_cast<Customer*>(Net->Tokens[no_])->DB_Ans_Login(id_->c_str(), pw_->c_str(), code_);
}

void DB_Connection::Ans_Logout(PACKAGE& package)
{
	using namespace db::Packet;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	auto id_ = login->id();
	auto pw_ = login->pw();

	size_t size = Net->Tokens.size();
	if (no_ < 0 || no_ > size || no_ == SessionId) return;

	NetLogger::GetIns()->Write(L"DB_Connector", "Logout Sucess ID : ", Util::AnsiToUTF16(id_->c_str()));
}

void DB_Connection::Ans_Signup(PACKAGE& package)
{
	using namespace db::Packet;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	auto id_ = login->id();
	auto pw_ = login->pw();

	size_t size = Net->Tokens.size();
	if (no_ < 0 || no_ > size || no_ == SessionId) return;
	static_cast<Customer*>(Net->Tokens[no_])->DB_Ans_Signup(id_->c_str(), pw_->c_str(), code_);
}

void DB_Connection::Ans_Withdrawl(PACKAGE& package)
{
	using namespace db::Packet;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	auto id_ = login->id();
	auto pw_ = login->pw();

	size_t size = Net->Tokens.size();
	if (no_ < 0 || no_ > size || no_ == SessionId) return;
	static_cast<Customer*>(Net->Tokens[no_])->DB_Ans_Withdrawl(id_->c_str(), pw_->c_str(), code_);
}

void DB_Connection::Ans_HeartBeat(PACKAGE& package)
{
	IToken::Ans_HeartBeat(package);
}

void DB_Connection::MappingProcotol()
{
	Mapping(this, static_cast<int>(PROTOCOL::DB_LOGIN), &DB_Connection::Ans_Login);
	Mapping(this, static_cast<int>(PROTOCOL::DB_LOGOUT), &DB_Connection::Ans_Logout);
	Mapping(this, static_cast<int>(PROTOCOL::DB_SIGNUP), &DB_Connection::Ans_Signup);
	Mapping(this, static_cast<int>(PROTOCOL::DB_WITHDRAWL), &DB_Connection::Ans_Withdrawl);
	Mapping(this, static_cast<int>(PROTOCOL::HEARTBEAT), &DB_Connection::Ans_HeartBeat);
}