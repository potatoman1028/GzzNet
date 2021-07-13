#pragma once
#pragma comment(lib, "GzzNet")
#include <vector>	
#include <string>
#include <thread>

#include "Note.h"
#include "Singleton.h"
#include "GzzIOCP_EX.h"
#include "FormatUtils.h"
#include "Chunk.h"

namespace gzz
{
#pragma pack(push, 1)
	struct GzzToken
	{
		GzzToken() = delete;
		GzzToken(int id) { this->id = id; Connection = false; }
	
		int id;
		bool Connection;
	};
#pragma pack(pop)

	class GzzRoot
	{
		using NetworkFunc = std::function<void(GzzRoot&, PACKAGE&)>;
	protected:
		GzzRoot();
		virtual ~GzzRoot();

	protected:
		static GzzIOCP_EX* Net;
		std::thread	WokerThread;
		std::vector<NetworkFunc> NetworkFuncTable;
		std::vector<GzzToken*> ConnectionPool;

		unsigned short Port;

		int	MaxSessionCount;
		int	CurrentSessionCount;
		bool IsWorking;

	public:
		virtual void Initialize(int max_thread, int max_session, IPaper* paper);
		virtual void Finalize();

		// setter
		void SetPort(unsigned short port) { Port = port; }
		void SetMaxSession(int max_session) { MaxSessionCount = max_session; }

		// getter
		unsigned short GetPort() { return Port; }
		int	GetMaxSession() { return MaxSessionCount; }

		// network base
		void Listen(unsigned short port);

		static void CreateNet(int thread_count, int max_session);
		static void Connect(std::wstring ip, unsigned short port);
		static void Disconnect(int session_id);
		static void SendPacket(int dst, PACKET& packet);
		static void FourceEnque(PACKAGE& package);

		void Start();
		void Run();	

	protected:
		void CreateRpcTable();

		// gzz object 를 상속받는 객체를 생성한다.
		void CreateSession(const int max_session);
		void WorkProc();
		bool ReadPackage();
		void StartThread();
		void DestroyThread();

		// network system call back
		virtual void OnAccept(PACKAGE& package);				// accept sucess
		virtual void OnConnect(PACKAGE& package);				// connect sucess
		virtual void OnConnectFail(PACKAGE& package);			// connect fail
		virtual void OnDisconnect_Accept(PACKAGE& package);		// disconnect session
		virtual void OnDisconnect_Connect(PACKAGE& package);	// disconnect session
		virtual void OnJob(PACKAGE& package) abstract;			// other Packet;
	};
}