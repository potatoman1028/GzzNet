#include "Querys.h"


debug_table::SP_Users::SP_Users() : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Users");
}

debug_table::SP_Users::~SP_Users()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}

	while (!Rec.IsEof())
	{
		int no;
		char id[DB_PARAM_SIZE];
		char pw[DB_PARAM_SIZE];
		int rno;
		char auth[DB_PARAM_SIZE];
		char cert[DB_PARAM_SIZE];

		Rec.Get("u_no", no);
		Rec.Get("u_id", id);
		Rec.Get("u_pw", pw);
		Rec.Get("room_no", rno);
		Rec.Get("authority", auth);
		Rec.Get("cert", cert);

		NetLogger::GetIns()->Write(L"ADO <CALL SP USERS> : ", L"  NO : ", no,
			L"  Id : ", id,
			L"  Pw : ", pw,
			L"  RoomNo : ", rno,
			L"  Authority : ", auth,
			L"  Cert : ", cert);
		Rec.MoveNext();
	}
}

debug_table::SP_Rooms::SP_Rooms() : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Rooms");
}

debug_table::SP_Rooms::~SP_Rooms()
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
		char manager[DB_PARAM_SIZE];
		char state[DB_PARAM_SIZE];

		Rec.Get("r_no", no);
		Rec.Get("r_name", name);
		Rec.Get("r_code", code);
		Rec.Get("manager", manager);
		Rec.Get("r_state", state);

		NetLogger::GetIns()->Write(L"ADO <CALL SP ROOMS> : ", L"  NO : ", no,
			L"  Name : ", name,
			L"  Code : ", code,
			L"  Manager : ", manager,
			L"  State : ", state);
		Rec.MoveNext();
	}
}

debug_table::SP_SignUp::SP_SignUp(const wchar_t* id, const wchar_t* pw) : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Signup", id, pw);
}

debug_table::SP_SignUp::~SP_SignUp()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char id[DB_PARAM_SIZE];

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Signup Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_id", id);
		NetLogger::GetIns()->Write(L"ADO : ", L"Signup Sucess !!" L"  NO : ", no, L"  Id : ", id);
		Rec.MoveNext();
	}
}

debug_table::SP_Withdrawal::SP_Withdrawal(const wchar_t* id, const wchar_t* pw) : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Withdrawal", id, pw);
}

debug_table::SP_Withdrawal::~SP_Withdrawal()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char id[DB_PARAM_SIZE];

		Rec.Get("u_no", no);

		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Withdrawal Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_id", id);

		NetLogger::GetIns()->Write(L"ADO : ", L"Withdrawal Sucess" L"  NO : ", no, L"  Id : ", id);
		Rec.MoveNext();
	}
}

debug_table::SP_Login::SP_Login(const wchar_t* id, const wchar_t* pw) : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Login", id, pw);
}

debug_table::SP_Login::~SP_Login()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char id[DB_PARAM_SIZE];
		char date[DB_PARAM_SIZE];

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Login Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_id", id);
		Rec.Get("login_date", date);

		NetLogger::GetIns()->Write(L"ADO : ", L"Login Sucess" L"  NO : ", no, L"  Id : ", id, L" Date : ", date);
		Rec.MoveNext();
	}
}

debug_table::SP_Logout::SP_Logout(const wchar_t* id, const wchar_t* pw) : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_Logout", id, pw);
}

debug_table::SP_Logout::~SP_Logout()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char id[DB_PARAM_SIZE];
		char date[DB_PARAM_SIZE];

		Rec.Get("u_no", no);
		if (no < 0)
		{
			NetLogger::GetIns()->Write(L"ADO : ", L"Logout Fail.. CODE : ", no);
			break;
		}

		Rec.Get("u_id", id);
		Rec.Get("logout_date", date);

		NetLogger::GetIns()->Write(L"ADO : ", L"Logout Sucess" L"  NO : ", no, L"  Id : ", id, L" Date : ", date);
		Rec.MoveNext();
	}
}

debug_table::SP_ActiveRoom::SP_ActiveRoom(const wchar_t* id, int rid, const wchar_t* rname) : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_ActiveRoom", id, rid, rname);
}

debug_table::SP_ActiveRoom::~SP_ActiveRoom()
{
	if (!Rec.IsEof())
	{
		Rec.MoveFirst();
	}
	while (!Rec.IsEof())
	{
		int no;
		char name[DB_PARAM_SIZE];
		char manager[DB_PARAM_SIZE];
		char code[DB_PARAM_SIZE];

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
}

debug_table::SP_DeactiveRoom::SP_DeactiveRoom(int rno) : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_RoomStateUpdate", rno, static_cast<int>(R_STATE::D));
}

debug_table::SP_DeactiveRoom::~SP_DeactiveRoom()
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

debug_table::SP_WaitStateRoom::SP_WaitStateRoom(int rno)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_RoomStateUpdate", rno, static_cast<int>(R_STATE::W));
}

debug_table::SP_WaitStateRoom::~SP_WaitStateRoom()
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

debug_table::SP_GameStateRoom::SP_GameStateRoom(int rno)
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_RoomStateUpdate", rno, static_cast<int>(R_STATE::G));
}

debug_table::SP_GameStateRoom::~SP_GameStateRoom()
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

debug_table::SP_LookUpActiveRooms::SP_LookUpActiveRooms() : Query()
{
	Cmd->Call_Procedure(QueryCommand::TYPE::CALL_BACK, L"SP_LookUpActiveRooms");
}

debug_table::SP_LookUpActiveRooms::~SP_LookUpActiveRooms()
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
		NetLogger::GetIns()->Write(L"ADO : ", L"SP_GameStateRoom Sucess" L"  NO : ", no, L"  Name : ", name, L"  Code : ", code);
		Rec.MoveNext();
	}
}
