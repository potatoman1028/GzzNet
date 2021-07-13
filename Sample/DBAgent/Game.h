#pragma once
#include "IToken.h"

class DBAgent; 

class Game : public IToken
{
	static constexpr int DEFAULT_TIME = 360;
	friend class DBAgent;

private:

public:
	Game() = delete;
	Game(int id, Network* net);
	virtual ~Game();

	virtual void MappingProcotol() override;

public:
	// FROM DBA TO GAMESERVER
	void Ret_Certification(int sessionid, int uid, const char* id);
	void Ret_ActiveRoom(int sessionid, const char* id, int room_no, const char* room_name);
	void Ret_UserInRoom(int sessionid, const char* id, int room_no, const char* room_name);
	void Ret_GameInfoUpdate(int sessionid, int myscore, int bestscore);

private:
	virtual void OnAccept(PACKAGE& package) override;					// accept sucess
	virtual void OnDisconnect_Accept(PACKAGE& package) override;		// disconnect session

	// FROM GAMESERVER TO DBA
	void Ans_Certification(PACKAGE& package);
	void Ans_ActiveRoom(PACKAGE& package);
	void Ans_UserInRoom(PACKAGE& package);

	// FROM DBA TO GAMESERVER
	void Ans_DeActiveRoom(PACKAGE& package);
	void Ans_StateUpdateWaitRoom(PACKAGE& package);
	void Ans_StateUpdateGameRoom(PACKAGE& package);
	void Ans_GameInfoUpdate(PACKAGE& package);
};