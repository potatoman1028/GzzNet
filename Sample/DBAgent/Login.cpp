#include "Login.h"

#include "Tables.h"
#include "DBAgent.h"

Login::Login(int id, Network* net)
	:IToken(id, net, 0)
{
}

Login::~Login()
{
}

void Login::MappingProcotol()
{
	Mapping(this, static_cast<int>(PROTOCOL::DB_LOGIN), &Login::Ans_Login);
	Mapping(this, static_cast<int>(PROTOCOL::DB_SIGNUP), &Login::Ans_Signup);
	Mapping(this, static_cast<int>(PROTOCOL::DB_WITHDRAWL), &Login::Ans_Withdrawl);
	Mapping(this, static_cast<int>(PROTOCOL::DB_LOGOUT), &Login::Ans_Logout);
	Mapping(this, static_cast<int>(PROTOCOL::HEARTBEAT), &Login::Ans_HeartBeat);
}

void Login::Ret_Login(int sessionid, int uid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = uid;
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

void Login::Ret_Logout(int sessionid, int uid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = uid;
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

void Login::Ret_Signup(int sessionid, int uid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = uid;
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

void Login::Ret_Withdrawl(int sessionid, int uid, const char* id, const char* pw)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = uid;
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

void Login::OnAccept(PACKAGE& package)
{
	Connection = true;
	NetLogger::GetIns()->Write(L"DBA", L"Connect Login Server");
	DBAgent::DBM->AddQuery(new table::SP_ALL_Logout(this, -1));
	DBAgent::DBM->AddQuery(new table::SP_ClearRooms(this, -1));
}

void Login::OnDisconnect_Accept(PACKAGE& package)
{
	Connection = false;
	NetLogger::GetIns()->Write(L"DBA", L"Disconnect Login Server");
}

void Login::Ans_Login(PACKAGE& package)
{
	using namespace db::Packet;
	auto login = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = login->session_id();
	int code_ = login->uuid();

	std::wstring id_ = L"'";
	std::wstring pw_ = L"'";

	id_ += Util::AnsiToUTF16(login->id()->c_str());
	pw_ += Util::AnsiToUTF16(login->pw()->c_str());

	id_ += L"'";
	pw_ += L"'";

	table::SP_Login* login_query = new table::SP_Login(this, no_, id_.c_str(), pw_.c_str());
	DBAgent::DBM->AddQuery(login_query);
}

void Login::Ans_Logout(PACKAGE& package)
{
	using namespace db::Packet;
	auto logout = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = logout->session_id();
	int code_ = logout->uuid();

	std::wstring id_ = L"'";
	std::wstring pw_ = L"'";

	id_ += Util::AnsiToUTF16(logout->id()->c_str());
	pw_ += Util::AnsiToUTF16(logout->pw()->c_str());

	id_ += L"'";
	pw_ += L"'";

	table::SP_Logout* logout_query = new table::SP_Logout(this, no_, id_.c_str(), pw_.c_str());
	DBAgent::DBM->AddQuery(logout_query);
}

void Login::Ans_Signup(PACKAGE& package)
{
	using namespace db::Packet;
	auto signup = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = signup->session_id();
	int code_ = signup->uuid();

	std::wstring id_ = L"'";
	std::wstring pw_ = L"'";

	id_ += Util::AnsiToUTF16(signup->id()->c_str());
	pw_ += Util::AnsiToUTF16(signup->pw()->c_str());

	id_ += L"'";
	pw_ += L"'";

	table::SP_SignUp* signup_query = new table::SP_SignUp(this, no_, id_.c_str(), pw_.c_str());
	DBAgent::DBM->AddQuery(signup_query);
}

void Login::Ans_Withdrawl(PACKAGE& package)
{
	using namespace db::Packet;
	auto withdrawl = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = withdrawl->session_id();
	int code_ = withdrawl->uuid();

	std::wstring id_ = L"'";
	std::wstring pw_ = L"'";

	id_ += Util::AnsiToUTF16(withdrawl->id()->c_str());
	pw_ += Util::AnsiToUTF16(withdrawl->pw()->c_str());

	id_ += L"'";
	pw_ += L"'";

	table::SP_Withdrawal* withdrawal_query = new table::SP_Withdrawal(this, no_, id_.c_str(), pw_.c_str());
	DBAgent::DBM->AddQuery(withdrawal_query);
}

void Login::Ans_HeartBeat(PACKAGE& package)
{
	IToken::Ans_HeartBeat(package);
}
