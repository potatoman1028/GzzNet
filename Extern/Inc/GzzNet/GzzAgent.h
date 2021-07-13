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

#include "GzzObject.h"

namespace gzz
{
	class GzzAgent : public GzzObject
	{
		using NetworkFunc = std::function<void(GzzAgent&, PACKAGE&)>;
	protected:
		GzzAgent();
		virtual ~GzzAgent();

	protected:
		static GzzIOCP_EX*				Net;
		std::thread						WokerThread;
		std::vector<NetworkFunc>		NetworkFuncTable;

		std::wstring					Address;
		unsigned short					Port;

		int								MaxSessionCount;
		bool							IsWorking;

	public:
		virtual void					Initialize(int max_thread, int max_session, IPaper* paper);
		virtual void					Finalize();

		// setter
		void							SetAddress(std::wstring address) { Address = address; }
		void							SetPort(unsigned short port) { Port = port; }
		void							SetMaxSession(int max_session) { MaxSessionCount = max_session; }

		// getter
		std::wstring					GetAddress() { return Address; }
		unsigned short					GetPort() { return Port; }
		int								GetMaxSession() { return MaxSessionCount; }
		int								GetSessionCount() { return Pool.size(); }

		// network base
		virtual void					Listen(unsigned short port);

		static void						CreateNet(int thread_count, int max_session);
		static void						MakeJobHeader(BASE_PACKET& b_packet, int pool_index, unsigned char job_protocol, unsigned char level);
		static void						Connect(std::wstring ip, unsigned short port);
		static void						Disconnect(int session_id);
		static void						SendPacket(int dst, Chunk& stream);
		static void						SendPacket(int dst, PACKET& packet);
		static void						FourceEnque(PACKAGE& package);

		void							Start();

	protected:
		virtual void					CreateRpcTable();
		virtual void					CreateEventTable() {};
		virtual void					UpdateEvent() {};

		// gzz object 를 상속받는 객체를 생성한다.
		virtual void					CreateSession(const int max_session) abstract;

		virtual void					WorkProc();
		virtual bool					ReadPackage();
		virtual void					StartThread();
		virtual void					DestroyThread();

		// network system call back
		virtual void					OnAccept(PACKAGE& package) abstract;				// accept sucess
		virtual void					OnConnect(PACKAGE& package) abstract;				// connect sucess
		virtual void					OnConnectFail(PACKAGE& package) abstract;			// connect fail
		virtual void					OnDisconnect_Accept(PACKAGE& package) abstract;		// disconnect session
		virtual void					OnDisconnect_Connect(PACKAGE& package) abstract;	// disconnect session

		virtual void					OnJob(PACKAGE& package) abstract;					// other Packet;
		virtual void					Play();

	private:
		virtual void					Req_Hello(const int remoteid) {};
		virtual void					Req_GoodBye(const int remoteid) {};
		virtual void					Req_Info(const int remoteid) {};
		virtual void					Req_Update(const int remoteid) {};
		virtual void					Req_Event(const int remoteid, unsigned char event) {};

		virtual void					Ans_Hello(PACKAGE& package) {};
		virtual void					Ans_GoodBye(PACKAGE& package) {};
		virtual void					Ans_Info(PACKAGE& package) {};
		virtual void					Ans_Update(PACKAGE& package) {};

		virtual void					Call(PACKAGE& package) {};
	};
}