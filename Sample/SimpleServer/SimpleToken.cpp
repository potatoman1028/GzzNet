#include "SimpleToken.h"
#include "SimpleServer.h"

//#include <flatbuffers/flexbuffers.h>
#include "PacketDefine.h"

struct ECHO_PACKET
{
	int Seq;					// Echo 를 한번 보낼 때 마다 증가되는 Sequence 데이터
	__int64 Time;				// Echo 를 언제 보냈는지 확인하는 스탬프
	int Size;					// 문자열의 크기
};

SimpleToken::SimpleToken(int id, Network* net)
	:SessionId(id), Net(net), Connection(false), HadConnected(false),
	EchoSeq(0), EchoCount(0), EchoFailCount(0), EchoMissCount(0), EchoMaxRand(0), EchoMinRand(0), EchoSendTime(0), SendTryCount(0), SendFailCount(0),
	BroadCastSeq(0), BroadCastCount(0), BroadCastFailCount(0), BroadCastMissCount(0), BroadCastMinRand(0), BroadCastMaxRand(0),  BroadCastSendTime(0)
{
}

SimpleToken::~SimpleToken()
{

}

bool SimpleToken::SendMsg(PACKET& packet)
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

void SimpleToken::MappingProcotol()
{
	Mapping(this, static_cast<int>(PROTOCOL::STRESS_TEST_ECHO), &SimpleToken::EchoReceive);
	Mapping(this, static_cast<int>(PROTOCOL::STRESS_TEST_BROADCAST_RETURN), &SimpleToken::BroadCastReceive);
}

void SimpleToken::EchoSetting(const int trycnt, const int minrand, const int maxrand, const __int64 time)
{
	EchoCount = trycnt;
	EchoSendTime = time;
	EchoMinRand = minrand;
	EchoMaxRand = maxrand;
}

void SimpleToken::BroadCastSetting(const int trycnt, const int minrand, const int maxrand, const __int64 time)
{
	BroadCastCount = trycnt;
	BroadCastSendTime = time;
	BroadCastMinRand = minrand;
	BroadCastMaxRand = maxrand;
}

void SimpleToken::Disconnect()
{
	Network::Disconnect(SessionId);
}

void SimpleToken::OnAccept(PACKAGE& package)
{
	Connection = true;
	HadConnected = true;
	NetLogger::GetIns()->Write(L"Server", "Dummy Is Connected ID : ", package.id);
}

void SimpleToken::OnConnect(PACKAGE& package)
{
}

void SimpleToken::OnConnectFail(PACKAGE& package)
{
}

void SimpleToken::OnDisconnect_Accept(PACKAGE& package)
{
	Connection = false;
	EchoSeq = 0;
	NetLogger::GetIns()->Write(L"Server", "Dummy Is Disconnected ID : ", package.id);
}

void SimpleToken::OnDisconnect_Connect(PACKAGE& package)
{
}

void SimpleToken::OnJob(PACKAGE& package)
{
	FuncMap[package.packet.head.protocol](package);
}

void SimpleToken::EchoSend()
{
	int size = random.GetRand(EchoMinRand, EchoMaxRand);
	char* cstr;
	cstr = MakeAlphabet(size);

	ECHO_PACKET echo;

	echo.Seq = EchoSeq++;
	echo.Size = size;
	auto tp = std::chrono::system_clock::now();
	std::time_t now = std::chrono::system_clock::to_time_t(tp);
	echo.Time = now;

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::STRESS_TEST_ECHO);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(&echo), sizeof(ECHO_PACKET));
	pack.Make(pack, pack.head, cstr, size);

	if (Net->SendPacket(SessionId, pack) < 0)
	{
		EchoFailCount++;
	};

	delete[] cstr;
}

void SimpleToken::BroadCastSend()
{
	int size = random.GetRand(BroadCastMinRand, BroadCastMaxRand);
	char* cstr;
	cstr = MakeAlphabet(size);

	ECHO_PACKET broadcast;

	broadcast.Seq = BroadCastSeq++;
	broadcast.Size = size;
	auto tp = std::chrono::system_clock::now();
	std::time_t now = std::chrono::system_clock::to_time_t(tp);
	broadcast.Time = now;

	PACKET pack;
	pack.head.size = PACKET_HEADER_SIZE;
	pack.head.protocol = static_cast<unsigned short>(PROTOCOL::STRESS_TEST_BROADCAST);
	pack.Make(pack, pack.head, reinterpret_cast<char*>(&broadcast), sizeof(ECHO_PACKET));
	pack.Make(pack, pack.head, cstr, size);

	if (Net->SendPacket(SessionId, pack) < 0)
	{
		BroadCastFailCount++;
	};
	delete[] cstr;
}

void SimpleToken::EchoReceive(PACKAGE& package)
{
	auto packet = package.packet;

	int offset = 0;

	PACKET_HEAD head;
	ECHO_PACKET echo;

	memcpy_s(&head, sizeof(ECHO_PACKET), packet.memory + offset, sizeof(PACKET_HEAD));
	offset += sizeof(PACKET_HEAD);

	memcpy_s(&echo, sizeof(ECHO_PACKET), packet.memory + offset, sizeof(ECHO_PACKET));
	offset += sizeof(ECHO_PACKET);

	char* str = new char[echo.Size + 1];

	memcpy_s(str, echo.Size, packet.memory + offset, echo.Size);
	offset += echo.Size;
	str[echo.Size] = '\0';
	//std::printf("%s\n", str);

	auto tp = std::chrono::system_clock::now();
	std::time_t now = std::chrono::system_clock::to_time_t(tp);
	echo.Time = now;

	packet.head.protocol = static_cast<unsigned short>(PROTOCOL::STRESS_TEST_ECHO_RETURN);
	++SendTryCount;
	if (Net->SendPacket(package.id, packet) < 0)
	{
		++SendFailCount;
	}

	delete[] str;
}

void SimpleToken::BroadCastReceive(PACKAGE& package)
{
	auto packet = package.packet;

	int offset = 0;

	PACKET_HEAD head;
	ECHO_PACKET broadcast;

	memcpy_s(&head, sizeof(ECHO_PACKET), packet.memory + offset, sizeof(PACKET_HEAD));
	offset += sizeof(PACKET_HEAD);

	memcpy_s(&broadcast, sizeof(ECHO_PACKET), packet.memory + offset, sizeof(ECHO_PACKET));
	offset += sizeof(ECHO_PACKET);

	char* str = new char[broadcast.Size + 1];

	memcpy_s(str, broadcast.Size, packet.memory + offset, broadcast.Size);
	offset += broadcast.Size;
	str[broadcast.Size] = '\0';
	std::printf("%s\n", str);

	delete[] str;
}

char* SimpleToken::MakeAlphabet(int size)
{
	char* str = new char[size];

	for (int i = 0; i < size; i++)
	{
		// ascii a ~ z / 65 ~ 90
		str[i] = size / 25;
		str[i] += 65;
	}
	return str;
}

void SimpleToken::Process(const double delta)
{
	// EchoProcess();
	// BroadCastProcess();
}

void SimpleToken::EchoProcess()
{
	using namespace std;
	using namespace chrono;

	auto delta = duration_cast<milliseconds>(steady_clock::now() - EchoSendTimer);

	if (delta.count() >= EchoSendTime)
	{
		for (int i = 0; i < EchoCount; ++i)
		{
			EchoSend();
		}
		EchoSendTimer = std::chrono::steady_clock::now();
	}
}

void SimpleToken::BroadCastProcess()
{
	using namespace std;
	using namespace chrono;

	auto delta = duration_cast<milliseconds>(steady_clock::now() - BroadCastSendTimer);
	if (delta.count() >= BroadCastSendTime)
	{
		for (int i = 0; i < BroadCastCount; ++i)
		{
			BroadCastSend();
		}
		BroadCastSendTimer = std::chrono::steady_clock::now();
	}
}

//void SimpleToken::GetStringData()
//{
//	// Receive Queue 에 도착한 Echo 패킷들을 검증한다.
//	std::scoped_lock<std::mutex, std::mutex> lock(SqLock, RqLock);
//	if (SendQueue.empty() || ReceiveQueue.empty())
//	{
//		return;
//	}
//
//	if (std::strcmp(SendQueue.front(), ReceiveQueue.front()) == 0)
//	{
//		NetLogger::GetIns()->Write(L"GOOD RESULT", SendQueue.front());
//
//		delete[] ReceiveQueue.front();
//		delete[] SendQueue.front();
//
//		ReceiveQueue.pop_front();
//		SendQueue.pop_front();
//	}
//	else
//	{
//		NetLogger::GetIns()->Write(L"ERROR", L"Bad Result");
//	}
//}
