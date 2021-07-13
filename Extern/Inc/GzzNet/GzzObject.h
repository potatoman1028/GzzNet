#pragma once
#include <vector>
#include <functional>	
#include "IoDefine.h"
#include "Chunk.h"
#include "PackageQueue.h"

namespace gzz
{
#pragma pack(push, 1)
	struct base_protocol
	{
		PACKET_HEADER   base_header;
		unsigned short  index;					// pool object index
		unsigned char	type;					// type
		unsigned char	func_table_index;		// rpc table number
		unsigned char	err_code;				// err code
		unsigned char	level;					// high(0) low(1)
	};
#pragma pack(pop)

	union BASE_PACKET
	{
		base_protocol protocol;
		char memory[MAX_PACKET_SIZE];
	};

	enum class WORK_STATE : unsigned char
	{
		NONE = 0,
		SLEEP,
		WAIT,			 // 유저가 들어온 상태 첫번 째 데이터 송수신.
		READY,		     // 모든 유저가 레디한 상태 플레이를 위한 데이터 송수신
		STANBYE,		 // 모든 유저가 모든 데이터를 송수신 받았음 플레이 직전
		PLAY,			 // 게임을 시작한 상태
		END,
		MAX = 0xff
	};

	constexpr int MAX_PROTOCOL_SIZE = 0xff;
	constexpr int B_HEADER_SIZE = sizeof(base_protocol);

	class GzzObject
	{
		using WorkingFunc = std::function<void(GzzObject&)>;

	protected:
		int									Id;
		int									RemoteId;

		GzzObject*							Boss;
		WORK_STATE							State;

		std::vector<WorkingFunc>			WorkingTable;	
		Chunk								Stream;
		char								Type;

		std::vector<GzzObject*>				Pool;

	public:
		GzzObject() = delete;
		GzzObject(int id, int remoteId, unsigned char type, GzzObject* boss, WORK_STATE state);
		virtual ~GzzObject();

	public:
		virtual void						MakeHeader(BASE_PACKET& b_packet, int pool_index) abstract;	// 상위 오브젝트에 헤더생성 요청할 때 사용.
		virtual void						MakeHeader(BASE_PACKET& b_packet, unsigned char protocol, unsigned char, unsigned char level) abstract;
		virtual void						Call(PACKAGE& pack) abstract;
		virtual void						Serialize() abstract;
		virtual void						Deserialize(Chunk& stream) abstract;
		virtual void						Clear();

		GzzObject*							WhoIsYourBoss() { return Boss; }
		void								NewBoss(GzzObject* boss) { Boss = boss; }
		GzzObject*							GetPoolItem(int index) { return Pool[index]; }
		Chunk*								GetStream() { return &Stream; }

		int									GetId() { return Id; }
		void								SetId(int id) { Id = id; }
		int									GetRemoteId() { return RemoteId; }
		void								SetRemoteId(int id) { RemoteId = id; }

		void								SetState(WORK_STATE state) { State = state; }
		WORK_STATE							GetState() { return State; }

		void								Run();

		virtual	void						UpdateEvent() {};
		virtual void						Req_Hello(const int dst, unsigned char ok) abstract;
		virtual void						Req_GoodBye(const int dst, unsigned char fource) abstract;
		virtual void						Req_Update(const int dst) abstract;
		virtual void						Req_Info(const int dst, unsigned char err_code) abstract;
		virtual void						Req_Event(const int remoteid, unsigned char event) abstract;
	private:
		// custom
		virtual void						CreateRpcTable() {};
		virtual void						CreateEventTable() {};

		virtual void						Ans_Hello(PACKAGE& package) abstract;
		virtual void						Ans_GoodBye(PACKAGE& package) abstract;
		virtual void						Ans_Update(PACKAGE& package) abstract;
		virtual void						Ans_Info(PACKAGE& package) abstract;		
		//virtual void						Call(PACKAGE& package) abstract;

		void								CreateWorkingTable();
		void								ClearPool();

	private:
		virtual void						None() {};
		virtual void						Sleep() {};
		virtual void						Wait() {};
		virtual void						Ready() {};
		virtual void						Stanbye() {};
		virtual void						Play() {};
	};
}