#pragma once
#include "AgentDefine.h"

namespace debug_table
{
	enum class R_STATE { E = 1, D, G, W };


	class SP_Users : public Query
	{
	public:
		SP_Users();
		virtual ~SP_Users();
	};

	class SP_Rooms : public Query
	{
	public:
		SP_Rooms();
		virtual ~SP_Rooms();
	};

	class SP_SignUp : public Query
	{
	public:
		SP_SignUp() = delete;
		SP_SignUp(const wchar_t* id, const wchar_t* pw);
		virtual ~SP_SignUp();
	};

	class SP_Withdrawal : public Query
	{
	public:
		SP_Withdrawal() = delete;
		SP_Withdrawal(const wchar_t* id, const wchar_t* pw);
		virtual ~SP_Withdrawal();
	};

	class SP_Login : public Query
	{
	public:
		SP_Login() = delete;
		SP_Login(const wchar_t* id, const wchar_t* pw);
		virtual ~SP_Login();
	};

	class SP_Logout : public Query
	{
	public:
		SP_Logout() = delete;
		SP_Logout(const wchar_t* id, const wchar_t* pw);
		virtual ~SP_Logout();
	};

	class SP_ActiveRoom : public Query
	{
	public:
		SP_ActiveRoom() = delete;
		SP_ActiveRoom(const wchar_t* id, int rid, const wchar_t* rname);
		virtual ~SP_ActiveRoom();
	};

	class SP_DeactiveRoom : public Query
	{
	public:
		SP_DeactiveRoom() = delete;
		SP_DeactiveRoom(int rno);
		virtual ~SP_DeactiveRoom();
	};

	class SP_WaitStateRoom : public Query
	{
	public:
		SP_WaitStateRoom() = delete;
		SP_WaitStateRoom(int rno);
		virtual ~SP_WaitStateRoom();
	};

	class SP_GameStateRoom : public Query
	{
	public:
		SP_GameStateRoom() = delete;
		SP_GameStateRoom(int rno);
		virtual ~SP_GameStateRoom();
	};

	class SP_LookUpActiveRooms : public Query
	{
	public:
		SP_LookUpActiveRooms();
		virtual ~SP_LookUpActiveRooms();
	};
}
