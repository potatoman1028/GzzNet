#pragma once
#include "AgentDefine.h"
#include "IToken.h"
namespace table
{
	struct Table : public Query
	{
		IToken* Net;
		int SessionId;
	public:
		Table() = delete;
		Table(IToken* net, int sid);
		virtual ~Table();
	};

	enum class R_STATE { E = 1, D, G, W };

	struct SP_SignUp : public Table
	{
		std::wstring Id;
		std::wstring Pw;

		SP_SignUp() = delete;
		SP_SignUp(IToken* net, int sid, const wchar_t* id, const wchar_t* pw);
		virtual ~SP_SignUp();
	};

	struct SP_Withdrawal : public Table
	{
		std::wstring Id;
		std::wstring Pw;

		SP_Withdrawal() = delete;
		SP_Withdrawal(IToken* net, int sid, const wchar_t* id, const wchar_t* pw);
		virtual ~SP_Withdrawal();
	};

	struct SP_Login : public Table
	{
		std::wstring Id;
		std::wstring Pw;
		 
		SP_Login() = delete;
		SP_Login(IToken* net, int sid, const wchar_t* id, const wchar_t* pw);
		virtual ~SP_Login();
	};

	struct SP_Logout : public Table
	{
		std::wstring Id;
		std::wstring Pw;

		SP_Logout() = delete;
		SP_Logout(IToken* net, int sid, const wchar_t* id, const wchar_t* pw);
		virtual ~SP_Logout();
	};

	struct SP_ActiveRoom : public Table
	{
		std::wstring Id;
		int RoomId;
		std::wstring RoomName;

		SP_ActiveRoom() = delete;
		SP_ActiveRoom(IToken* net, int sid, const wchar_t* id, int rid, const wchar_t* rname);
		virtual ~SP_ActiveRoom();
	};

	struct SP_DeactiveRoom : public Table
	{
		int RoomId;
		SP_DeactiveRoom() = delete;
		SP_DeactiveRoom(IToken* net, int sid, int rid);
		virtual ~SP_DeactiveRoom();
	};

	struct SP_WaitStateRoom : public Table
	{
		int RoomId;
		SP_WaitStateRoom() = delete;
		SP_WaitStateRoom(IToken* net, int sid, int rid);
		virtual ~SP_WaitStateRoom();
	};

	struct SP_GameStateRoom : public Table
	{
		int RoomId;
		SP_GameStateRoom() = delete;
		SP_GameStateRoom(IToken* net, int sid, int rid);
		virtual ~SP_GameStateRoom();
	};

	struct SP_LookUpActiveRooms : public Table
	{
		SP_LookUpActiveRooms() = delete;
		SP_LookUpActiveRooms(IToken* net, int sid);
		virtual ~SP_LookUpActiveRooms();
	};

	struct SP_LookUpActiveFirstRoom : public Table
	{
		SP_LookUpActiveFirstRoom() = delete;
		SP_LookUpActiveFirstRoom(IToken* net, int sid);
		virtual ~SP_LookUpActiveFirstRoom();
	};

	struct SP_Certification : public Table
	{
		std::wstring Id;

		SP_Certification() = delete;
		SP_Certification(IToken* net, int sid, const wchar_t* id);
		virtual ~SP_Certification();
	};

	struct SP_UserInRoom : public Table
	{
		SP_UserInRoom() = delete;
		SP_UserInRoom(IToken* net, int sid, const wchar_t* id, const wchar_t* name);
		virtual ~SP_UserInRoom();
	};

	struct SP_ALL_Logout : public Table
	{
		SP_ALL_Logout() = delete;
		SP_ALL_Logout(IToken* net, int sid);
		virtual ~SP_ALL_Logout();
	};

	struct SP_ClearRooms : public Table
	{
		SP_ClearRooms() = delete;
		SP_ClearRooms(IToken* net, int sid);
		virtual ~SP_ClearRooms();
	};

	struct SP_GameInfoUpdate : public Table
	{
		std::wstring id;
		std::wstring time;
		int score;

		SP_GameInfoUpdate() = delete;
		SP_GameInfoUpdate(IToken* net, int sid, const wchar_t* id_, int score_);
		virtual ~SP_GameInfoUpdate();
	};
}
