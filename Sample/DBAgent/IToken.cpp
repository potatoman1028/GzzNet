#include "IToken.h"
#include "AgentDefine.h"
#include "DBAgent.h"

using namespace std;
using namespace chrono;

IToken::IToken(int id, Network* net, __int64 heartbeat)
	:SessionId(id), Net(net), HeartBeatTime(heartbeat), Heart(HEART_BEAT::BEAT), Connection(false)
{
	HeartBeatStart = steady_clock::now();
}

IToken::~IToken()
{
}

bool IToken::SendMsg(PACKET& packet)
{
	if (Connection)
	{
		Network::SendPacket(SessionId, packet);
		return true;
	}
	else
	{
		return false;
	}
}

void IToken::HeartBeatCheck()
{
	steady_clock::time_point end = steady_clock::now();
	std::chrono::duration<double> result;

	switch (Heart)
	{
	case IToken::HEART_BEAT::STOP:
		Disconnect();
		break;
	case IToken::HEART_BEAT::CHECKING:
		result = HeartBeatStart - end;
		if (duration_cast<seconds>(result).count() > HeartBeatTime)
		{
			Heart = HEART_BEAT::STOP;
		}
		break;
	case IToken::HEART_BEAT::BEAT:
		Req_HeartBeat();
		HeartBeatStart = steady_clock::now();
		Heart = HEART_BEAT::CHECKING;
		break;
	}
}

void IToken::Disconnect()
{
	Network::Disconnect(SessionId);
}

void IToken::OnJob(PACKAGE& package)
{
	FuncMap[package.packet.head.protocol](package);
}

void IToken::Ans_HeartBeat(PACKAGE& package)
{
	Heart = HEART_BEAT::BEAT;
}

void IToken::Req_HeartBeat()
{
	using namespace db::Packet;
	flatbuffers::FlatBufferBuilder builder;
	auto protocol = CreateOneCode(builder, -1);
	builder.Finish(protocol);
	auto buffer = builder.GetBufferPointer();

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<int>(PROTOCOL::HEARTBEAT);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(buffer), builder.GetSize());
	builder.Clear();
	SendMsg(pack);
}