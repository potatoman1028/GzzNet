#include "Tables.h"
#include "Login.h"
#include "Game.h"
#include "DBAgent.h"

table::Table::Table(IToken* net, int sid) : Query(), Net(net), SessionId(sid)
{
}

table::Table::~Table()
{
}

table::SP_SignUp::SP_SignUp(IToken* net, int sid, const wchar_t* id, const wchar_t* pw) : Table(net, sid), Id(id), Pw(pw)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Signup", Id, Pw);
}

table::SP_SignUp::~SP_SignUp()
{
	int no = 0;
	char id[DB_PARAM_SIZE];
	char pw[DB_PARAM_SIZE];

	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		memset(&no, 0, sizeof(no));
		memset(&id, 0, DB_PARAM_SIZE);
		memset(&pw, 0, DB_PARAM_SIZE);

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Signup Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_id", id);
		Rec.Get("u_pw", pw);

		NetLogger::GetIns()->Write(L"ADO : ", L"Signup Sucess !!" L"  NO : ", no, L"  Id : ", id);
		Rec.MoveNext();
	}
	static_cast<Login*>(Net)->Ret_Signup(SessionId, no, id, pw);
}

table::SP_Withdrawal::SP_Withdrawal(IToken* net, int sid, const wchar_t* id, const wchar_t* pw) : Table(net, sid), Id(id), Pw(pw)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Withdrawal", Id, Pw);
}

table::SP_Withdrawal::~SP_Withdrawal()
{
	int no = 0;
	char id[DB_PARAM_SIZE];
	char pw[DB_PARAM_SIZE];

	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		memset(&no, 0, sizeof(no));
		memset(&id, 0, DB_PARAM_SIZE);
		memset(&pw, 0, DB_PARAM_SIZE);

		Rec.Get("u_no", no);

		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Withdrawal Fail.. CODE : ", no);
			break;
		}
		Rec.Get("u_id", id);
		Rec.Get("u_pw", pw);
		NetLogger::GetIns()->Write(L"ADO : ", L"Withdrawal Sucess" L"  NO : ", no, L"  Id : ", id);
		Rec.MoveNext();
	}
	static_cast<Login*>(Net)->Ret_Withdrawl(SessionId, no, id, pw);
}

table::SP_Login::SP_Login(IToken* net, int sid, const wchar_t* id, const wchar_t* pw) : Table(net, sid), Id(id), Pw(pw)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Login", Id, Pw);
}

table::SP_Login::~SP_Login()
{
	int no = 0;
	char id[DB_PARAM_SIZE];
	char pw[DB_PARAM_SIZE];
	char date[DB_PARAM_SIZE];

	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		memset(&no, 0, sizeof(no));
		memset(&id, 0, DB_PARAM_SIZE);
		memset(&date, 0, DB_PARAM_SIZE);
		memset(&pw, 0, DB_PARAM_SIZE);

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Login Fail.. CODE : ", no);
			break;
		}
		Rec.Get("u_id", id);
		Rec.Get("u_pw", pw);
		Rec.Get("login_date", date);
		NetLogger::GetIns()->Write(L"ADO : ", L"Login Sucess" L"  NO : ", no, L"  Id : ", id, L" Date : ", date);
		Rec.MoveNext();
	}
	static_cast<Login*>(Net)->Ret_Login(SessionId, no, id, pw);
}

table::SP_Logout::SP_Logout(IToken* net, int sid, const wchar_t* id, const wchar_t* pw) : Table(net, sid), Id(id), Pw(pw)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Logout", Id, Pw);
}

table::SP_Logout::~SP_Logout()
{
	int no = 0;
	char id[DB_PARAM_SIZE];
	char date[DB_PARAM_SIZE];
	char pw[DB_PARAM_SIZE];

	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		memset(&no, 0, sizeof(no));
		memset(&id, 0, DB_PARAM_SIZE);
		memset(&date, 0, DB_PARAM_SIZE);
		memset(&pw, 0, DB_PARAM_SIZE);

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Logout Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_id", id);
		Rec.Get("u_pw", pw);
		Rec.Get("logout_date", date);

		NetLogger::GetIns()->Write(L"ADO : ", L"Logout Sucess" L"  NO : ", no, L"  Id : ", id, L" Date : ", date);
		Rec.MoveNext();
	}
	static_cast<Login*>(Net)->Ret_Logout(SessionId, no, id, pw);
}

table::SP_ActiveRoom::SP_ActiveRoom(IToken* net, int sid, const wchar_t* id, int rid, const wchar_t* rname) : Table(net, sid), Id(id), RoomId(rid), RoomName(rname)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_ActiveRoom", Id, RoomId, RoomName);
}

table::SP_ActiveRoom::~SP_ActiveRoom()
{
	int no;
	char name[DB_PARAM_SIZE];
	char manager[DB_PARAM_SIZE];
	char code[DB_PARAM_SIZE];

	memset(name, 0, DB_PARAM_SIZE);
	memset(manager, 0, DB_PARAM_SIZE);
	memset(code, 0, DB_PARAM_SIZE);

	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{

		Rec.Get("r_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_ActiveRoom Fail.. CODE : ", no);
			break;
		}

		Rec.Get("r_name", name);
		Rec.Get("manager", manager);
		Rec.Get("r_code", code);

		NetLogger::GetIns()->Write(L"ADO : ", L"SP_ActiveRoom Sucess" L"  NO : ", no, L"  Name : ", name,
			L" Code : ", code, L"  Master : ", manager);
		Rec.MoveNext();
	}
	static_cast<Game*>(Net)->Ret_ActiveRoom(SessionId, manager, no, name);
}

table::SP_DeactiveRoom::SP_DeactiveRoom(IToken* net, int sid, int rid) : Table(net, sid), RoomId(rid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_RoomStateUpdate", RoomId, static_cast<int>(R_STATE::D));
}

table::SP_DeactiveRoom::~SP_DeactiveRoom()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char state[DB_PARAM_SIZE];

		Rec.Get("r_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_DeactiveRoom Fail.. CODE : ", no);
			break;
		}
		else
		{
			Rec.Get("r_state", state);
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_DeactiveRoom Sucess" L"  NO : ", no, L"  State : ", state);
		}
		Rec.MoveNext();
	}
}

table::SP_WaitStateRoom::SP_WaitStateRoom(IToken* net, int sid, int rid) : Table(net, sid), RoomId(rid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_RoomStateUpdate", RoomId, static_cast<int>(R_STATE::W));
}

table::SP_WaitStateRoom::~SP_WaitStateRoom()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char state[DB_PARAM_SIZE];

		Rec.Get("r_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_WaitStateRoom Fail.. CODE : ", no);
			break;
		}
		else
		{
			Rec.Get("r_state", state);
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_WaitStateRoom Sucess" L"  NO : ", no, L"  State : ", state);
		}
		Rec.MoveNext();
	}
}

table::SP_GameStateRoom::SP_GameStateRoom(IToken* net, int sid, int rid) : Table(net, sid), RoomId(rid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_RoomStateUpdate", RoomId, static_cast<int>(R_STATE::G));
}

table::SP_GameStateRoom::~SP_GameStateRoom()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char state[DB_PARAM_SIZE];

		Rec.Get("r_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_GameStateRoom Fail.. CODE : ", no);
			break;
		}
		else
		{
			Rec.Get("r_state", state);
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_GameStateRoom Sucess" L"  NO : ", no, L"  State : ", state);
		}
		Rec.MoveNext();
	}
}

table::SP_LookUpActiveRooms::SP_LookUpActiveRooms(IToken* net, int sid) : Table(net, sid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_LookUpActiveRooms");
}

table::SP_LookUpActiveRooms::~SP_LookUpActiveRooms()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char name[DB_PARAM_SIZE];
		char code[DB_PARAM_SIZE];

		Rec.Get("r_no", no);
		Rec.Get("r_name", name);
		Rec.Get("r_code", code);
		NetLogger::GetIns()->Write(L"ADO : ", L"SP_LookUpActiveRooms Sucess" L"  NO : ", no, L"  Name : ", name, L"  Code : ", code);
		Rec.MoveNext();
	}
}

table::SP_LookUpActiveFirstRoom::SP_LookUpActiveFirstRoom(IToken* net, int sid) : Table(net, sid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_LookUpActiveFirstRoom");
}

table::SP_LookUpActiveFirstRoom::~SP_LookUpActiveFirstRoom()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char name[DB_PARAM_SIZE];
		char code[DB_PARAM_SIZE];

		Rec.Get("r_no", no);
		Rec.Get("r_name", name);
		Rec.Get("r_code", code);
		NetLogger::GetIns()->Write(L"ADO : ", L"SP_LookUpActiveFirstRoom Sucess" L"  NO : ", no, L"  Name : ", name, L"  Code : ", code);
		break;
	}
}

table::SP_Certification::SP_Certification(IToken* net, int sid, const wchar_t* id) : Table(net, sid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Certification", id);
}

table::SP_Certification::~SP_Certification()
{
	int no = 0;
	char id[DB_PARAM_SIZE];

	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}

	while (!Rec.IsEof())
	{
		memset(&no, 0, sizeof(no));
		memset(&id, 0, DB_PARAM_SIZE);

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Signup Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_id", id);

		NetLogger::GetIns()->Write(L"ADO : ", L"Certification Sucess !!" L"  NO : ", no, L"  Id : ", id);
		Rec.MoveNext();
	}
	static_cast<Game*>(Net)->Ret_Certification(SessionId, no, id);
}

table::SP_UserInRoom::SP_UserInRoom(IToken* net, int sid, const wchar_t* id, const wchar_t* name) : Table(net, sid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_UserInRoom", id, name);
}

table::SP_UserInRoom::~SP_UserInRoom()
{
	int no = 0;
	char id[DB_PARAM_SIZE];
	char manager[DB_PARAM_SIZE];
	int roomno = 0;

	memset(&no, 0, sizeof(no));
	memset(&id, 0, DB_PARAM_SIZE);
	memset(&manager, 0, DB_PARAM_SIZE);
	memset(&roomno, 0, sizeof(roomno));

	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}

	while (!Rec.IsEof())
	{
		memset(&no, 0, sizeof(no));
		memset(&id, 0, DB_PARAM_SIZE);
		memset(&roomno, 0, sizeof(roomno));
		memset(&manager, 0, DB_PARAM_SIZE);

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_UserInRoom Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_no", no);
		Rec.Get("u_id", id);
		Rec.Get("room_no", roomno);
		Rec.Get("manager", manager);

		NetLogger::GetIns()->Write(L"ADO : ", L"UserIn Room Sucess !!" L"  NO : ", no, L"  Id : ", id, L" RoomName : ", manager);
		Rec.MoveNext();
	}
	static_cast<Game*>(Net)->Ret_UserInRoom(SessionId, id, roomno, manager);
}

table::SP_ALL_Logout::SP_ALL_Logout(IToken* net, int sid) : Table(net, sid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::NO_RETURN, L"SP_ALL_Logout");
}

table::SP_ALL_Logout::~SP_ALL_Logout()
{
	NetLogger::GetIns()->Write(L"ADO : ", L"All LogOut Sucess !!");
}

table::SP_ClearRooms::SP_ClearRooms(IToken* net, int sid) : Table(net, sid)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::NO_RETURN, L"SP_ClearRooms");
}

table::SP_ClearRooms::~SP_ClearRooms()
{
	NetLogger::GetIns()->Write(L"ADO : ", L"ClearRooms Sucess !!");
}

table::SP_GameInfoUpdate::SP_GameInfoUpdate(IToken* net, int sid, const wchar_t* id_, int score_) : Table(net, sid),
id(id_), score(score_)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_GameInfoRecording", id, "'00:00:00'", score);
}

table::SP_GameInfoUpdate::~SP_GameInfoUpdate()
{
	NetLogger::GetIns()->Write(L"ADO : ", L"Game Info Update ID : ", id, L" PlayTime : ", time, L"Score : ", score);

	int my_uuid = -1;
	int best_uuid = -1;

	int my_score = 0;
	int best_score = 0;

	// 칼럼을 첫번째로 옮깁니다.
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}

	int index = 0;

	while (!Rec.IsEof())
	{
		if (index++ > 1)
		{
			break;
		}

		if (index == 0)
		{
			memset(&my_uuid, 0, sizeof(int));
			memset(&my_score, 0, sizeof(int));

			Rec.Get("u_no", my_uuid);
			if (my_uuid < 0)
			{
				NetLogger::GetIns()->Write(L"ADO : ", L"SP_GameInfoUpdate Fail.. CODE : ", my_uuid);
				break;
			}

			Rec.Get("score", my_score);
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_GameInfoUpdate Sucess ID : ", my_uuid, L" Score : ", my_score);
			Rec.MoveNext();
		}
		else
		{
			memset(&best_uuid, 0, sizeof(int));
			memset(&best_score, 0, sizeof(int));

			Rec.Get("u_no", best_uuid);
			if (best_uuid < 0)
			{
				NetLogger::GetIns()->Write(L"ADO : ", L"SP_GameInfoUpdate Fail.. CODE : ", my_uuid);
				break;
			}

			Rec.Get("score", best_score);
			DBAgent::BestScore = best_score;
			NetLogger::GetIns()->Write(L"ADO : ", L"SP_GameInfoUpdate Sucess Best User ID : ", best_uuid, L"Best Score : ", best_score);
			Rec.MoveNext();
		}
	}
	static_cast<Game*>(Net)->Ret_GameInfoUpdate(SessionId, my_score, best_score);
}
