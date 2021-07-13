#include "Game.h"

#include "Tables.h"
#include "DBAgent.h"

Game::Game(int id, Network* net)
	:IToken(id, net, 0)
{

}

Game::~Game()
{

}

void Game::MappingProcotol()
{
	Mapping(this, static_cast<int>(PROTOCOL::DB_CERTIFICATION), &Game::Ans_Certification);
	Mapping(this, static_cast<int>(PROTOCOL::DB_ROOM_ACTIVE), &Game::Ans_ActiveRoom);
	Mapping(this, static_cast<int>(PROTOCOL::DB_ROOM_IN), &Game::Ans_UserInRoom);

	Mapping(this, static_cast<int>(PROTOCOL::DB_ROOM_DEACTIVE), &Game::Ans_DeActiveRoom);
	Mapping(this, static_cast<int>(PROTOCOL::DB_ROOM_STATE_WAIT), &Game::Ans_StateUpdateWaitRoom);
	Mapping(this, static_cast<int>(PROTOCOL::DB_ROOM_STATE_GAME), &Game::Ans_StateUpdateGameRoom);
	Mapping(this, static_cast<int>(PROTOCOL::DB_GAME_CLOSE_INFO), &Game::Ans_GameInfoUpdate);
}

void Game::Ret_Certification(int sessionid, int uid, const char* id)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int no_ = sessionid;
	int code_ = uid;
	auto id_ = builder.CreateString(id);
	auto pw_ = builder.CreateString("");

	auto protocol = CreateIdPw(builder, no_, id_, pw_, code_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_CERTIFICATION);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void Game::Ret_ActiveRoom(int sessionid, const char* id, int room_no, const char* room_name)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = sessionid;
	int roomno_ = room_no;
	auto id_ = builder.CreateString(id);
	auto roomname_ = builder.CreateString(room_name);

	auto protocol = CreateRoomSetting(builder, sid_, roomno_, id_, roomname_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_ROOM_ACTIVE);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}

void Game::Ret_UserInRoom(int sessionid, const char* id, int room_no, const char* room_name)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = sessionid;
	int roomno_ = room_no;
	auto id_ = builder.CreateString(id);
	auto roomname_ = builder.CreateString(room_name);

	auto protocol = CreateRoomSetting(builder, sid_, roomno_, id_, roomname_);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	int size = builder.GetSize();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_ROOM_IN);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), size);
	builder.Clear();
	SendMsg(pack);
}

void Game::Ret_GameInfoUpdate(int sessionid, int myscore, int bestscore)
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;

	int sid_ = sessionid;
	auto userid = builder.CreateString("test");

	auto protocol = CreateGameInfo(builder, sid_, userid, myscore, bestscore);

	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	int size = builder.GetSize();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::DB_GAME_CLOSE_INFO);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), size);
	builder.Clear();
	SendMsg(pack);
}

void Game::OnAccept(PACKAGE& package)
{
	Connection = true;
	NetLogger::GetIns()->Write(L"DBA", L"Connect Game Server");
	DBAgent::DBM->AddQuery(new table::SP_ClearRooms(this, -1));
}

void Game::OnDisconnect_Accept(PACKAGE& package)
{
	Connection = false;
	NetLogger::GetIns()->Write(L"DBA", L"Disconnect Game Server");
}

void Game::Ans_Certification(PACKAGE& package)
{
	using namespace db::Packet;
	auto cert = GetIdPw(package.packet.memory + PACKET_HEADER_SIZE);

	int no_ = cert->session_id();
	int code_ = cert->uuid();

	std::wstring id_ = L"'";

	id_ += Util::AnsiToUTF16(cert->id()->c_str());
	id_ += L"'";

	table::SP_Certification* cert_query = new table::SP_Certification(this, no_, id_.c_str());
	DBAgent::DBM->AddQuery(cert_query);
}

void Game::Ans_ActiveRoom(PACKAGE& package)
{
	using namespace db::Packet;
	auto active = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);

	int roomno_ = active->roomid();
	int session_ = active->session_id();

	std::wstring roomname_ = L"'";
	std::wstring userid_ = L"'";

	roomname_ += Util::AnsiToUTF16(active->roomname()->c_str());
	userid_ += Util::AnsiToUTF16(active->userid()->c_str());

	roomname_ += L"'";
	userid_ += L"'";

	table::SP_ActiveRoom* active_query = new table::SP_ActiveRoom(this, session_, userid_.c_str(), roomno_, roomname_.c_str());
	DBAgent::DBM->AddQuery(active_query);
}

void Game::Ans_UserInRoom(PACKAGE& package)
{
	using namespace db::Packet;
	auto roomin = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);

	int roomno_ = roomin->roomid();
	int session_ = roomin->session_id();

	std::wstring roomname_ = L"'";
	std::wstring userid_ = L"'";

	roomname_ += Util::AnsiToUTF16(roomin->roomname()->c_str());
	userid_ += Util::AnsiToUTF16(roomin->userid()->c_str());

	roomname_ += L"'";
	userid_ += L"'";

	table::SP_UserInRoom* roomin_query = new table::SP_UserInRoom(this, session_, userid_.c_str(), roomname_.c_str());
	DBAgent::DBM->AddQuery(roomin_query);
}

void Game::Ans_DeActiveRoom(PACKAGE& package)
{
	using namespace db::Packet;
	auto roomset = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);
	int roomno_ = roomset->roomid();

	table::SP_DeactiveRoom* roomset_query = new table::SP_DeactiveRoom(this, 0, roomno_);
	DBAgent::DBM->AddQuery(roomset_query);
}

void Game::Ans_StateUpdateWaitRoom(PACKAGE& package)
{
	using namespace db::Packet;
	auto roomset = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);
	int roomno_ = roomset->roomid();

	table::SP_WaitStateRoom* roomset_query = new table::SP_WaitStateRoom(this, 0, roomno_);
	DBAgent::DBM->AddQuery(roomset_query);
}

void Game::Ans_StateUpdateGameRoom(PACKAGE& package)
{
	using namespace db::Packet;
	auto roomset = GetRoomSetting(package.packet.memory + PACKET_HEADER_SIZE);
	int roomno_ = roomset->roomid();

	table::SP_GameStateRoom* roomset_query = new table::SP_GameStateRoom(this, 0, roomno_);
	DBAgent::DBM->AddQuery(roomset_query);
}

void Game::Ans_GameInfoUpdate(PACKAGE& package)
{
	using namespace db::Packet;
	auto g_info = GetGameInfo(package.packet.memory + PACKET_HEADER_SIZE);

	std::wstring userid_ = L"'";
	userid_ += Util::AnsiToUTF16(g_info->userid()->c_str());
	userid_ += L"'";

	table::SP_GameInfoUpdate* gameinfo = new table::SP_GameInfoUpdate(this, g_info->session_id(), userid_.c_str(), g_info->myscore());
	DBAgent::DBM->AddQuery(gameinfo);
}