#include "SimpleToken.h"
#include "SimpleClient.h"

#include "PacketDefine.h"

struct ECHO_PACKET
{
	int Seq;					// Echo 를 한번 보낼 때 마다 증가되는 Sequence 데이터
	__int64 Time;				// Echo 를 언제 보냈는지 확인하는 스탬프
	int Size;					// 문자열의 크기
};

struct ECHO_DATA
{
	int Seq;					// Echo 를 한번 보낼 때 마다 증가되는 Sequence 데이터 
	__int64 Time;				// Round Trip Time SendTime - RecvTime
	char* Data;					// 데이터

	ECHO_DATA(int seq, __int64 rtt, char* data)
	{
		Seq = seq;
		Time = rtt;
		Data = data;
	}

	~ECHO_DATA()
	{
		delete[] Data;
	}
};

SimpleToken::SimpleToken(int id, Network* net)
	:SessionId(id), Net(net), Connection(false), HadConnected(false),
	EchoSeq(0), EchoCount(0), EchoFailCount(0), EchoMissCount(0), EchoMaxRand(0), EchoMinRand(0), EchoSendTime(0), ReceiveCount(0),
	BroadCastSeq(0), BroadCastCount(0), BroadCastFailCount(0), BroadCastMissCount(0), BroadCastMinRand(0), BroadCastMaxRand(0), BroadCastSendTime(0),
	SendTryCount(0), SendFailCount(0)
{
	sendqueue.clear();
}

SimpleToken::~SimpleToken()
{
	if (!sendqueue.empty())
	{
		for (size_t i = 0; i < sendqueue.size(); ++i)
		{
			if (sendqueue[i])
			{
				delete sendqueue[i];
				sendqueue[i] = nullptr;
			}
		}

		sendqueue.clear();
	}
}

int SimpleToken::SendMsg(PACKET& packet)
{
	if (Connection)
	{
		return Network::SendPacket(SessionId, packet);
	}
	else
	{
		return -2;
	}
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

void SimpleToken::MappingProcotol()
{
	Mapping(this, static_cast<int>(PROTOCOL::STRESS_TEST_ECHO_RETURN), &SimpleToken::EchoReceive);
	Mapping(this, static_cast<int>(PROTOCOL::STRESS_TEST_BROADCAST), &SimpleToken::BroadCastReceive);
}

void SimpleToken::Disconnect()
{
	Network::Disconnect(SessionId);
}

void SimpleToken::OnAccept(PACKAGE& package)
{
}

void SimpleToken::OnConnect(PACKAGE& package)
{
	Connection = true;
	HadConnected = true;
	NetLogger::GetIns()->Write(L"Client", "Dummy Is Connected ID : ", package.id);
}

void SimpleToken::OnConnectFail(PACKAGE& package)
{
	Connection = false;
	NetLogger::GetIns()->Write(L"Client", "Dummy Connected Fail... ID :	", package.id);
}

void SimpleToken::OnDisconnect_Accept(PACKAGE& package)
{
}

void SimpleToken::OnDisconnect_Connect(PACKAGE& package)
{
	Connection = false;
	NetLogger::GetIns()->Write(L"Client", "Dummy Is Disconnected ID : ", package.id);
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
	++SendTryCount;

	if (Net->SendPacket(SessionId, pack) < 0)
	{
		++SendFailCount;
		--EchoSeq;
	}
	else
	{
		sendqueue.emplace_back(new ECHO_DATA(echo.Seq, echo.Time, cstr));
	}
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
	using namespace std;
	using namespace chrono;
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

	auto tp = std::chrono::system_clock::now();
	std::time_t now = std::chrono::system_clock::to_time_t(tp);
	echo.Time = now;

	ECHO_DATA recho(echo.Seq, echo.Time, str);
	ECHO_DATA* secho = sendqueue.front();

	++ReceiveCount;;

	// sequence 점검 
	if (secho->Seq != recho.Seq)
	{
		++SeqMissCount;
		secho = nullptr;

		// 데이터를 탐색한다.
		auto iter = sendqueue.begin();
		if ((*iter)->Seq != recho.Seq)
		{
			for (; iter != sendqueue.end(); ++iter)
			{
				if ((*iter)->Seq == recho.Seq)
				{
					++SeqMissFindSucessCount;
					break;
				}
			}

			if (iter == sendqueue.end())
			{
				++SeqMissFindFailCount;
				secho = nullptr;
			}
			else
			{
				secho = (*iter);
				sendqueue.erase(iter);
			}
		}
	}
	else
	{
		sendqueue.pop_front();
	}

	if (secho)
	{
		// stirng 점검
		if (std::strcmp(recho.Data, secho->Data) != 0)
		{
			++StringMissCount;
		}

		// 지연시간 확인
		auto sendtp = system_clock::from_time_t(secho->Time);
		auto recvtp = system_clock::from_time_t(recho.Time);

		duration<double> res = recvtp - sendtp;
		Rtt = gzz::StandardDeviation(res.count(), Rtt);

		delete secho;
	}
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

void SimpleToken::EchoCalcResult()
{
	//using namespace std;
	//using namespace chrono;

	//// 먼저 sendqueue와 recvqueue의 갯수를 비교한다.
	//size_t s_size = sendqueue.size();
	//size_t r_size = recvqueue.size();

	//if (sendqueue.size() != recvqueue.size())
	//{
	//	DataMissCount = (s_size >= r_size ? s_size - r_size : r_size - s_size);
	//}

	//// 다음으로 순차적으로 돌면서 서로의 데이터가 맞는지 확인한다.

	//int size = (s_size >= r_size ? r_size : s_size);

	//// Round Trip Time 계산, 표준편차 사용

	//for (int i = 0; i < size; ++i)
	//{
	//	if (sendqueue[i]->Seq != recvqueue[i]->Seq)
	//	{
	//		++SeqMissCount;
	//	}

	//	if (std::strcmp(sendqueue[i]->Data, recvqueue[i]->Data) != 0)
	//	{
	//		++StringMissCount;
	//	}

	//	auto sendtp = system_clock::from_time_t(sendqueue[i]->Time);
	//	auto recvtp = system_clock::from_time_t(recvqueue[i]->Time);

	//	duration<double> res = recvtp - sendtp;
	//	Rtt = gzz::StandardDeviation(res.count(), Rtt);
	//}

	//// 결과를 계산하고 초기화한다.

	//for (size_t i = 0; i < sendqueue.size(); i++)
	//{
	//	if (sendqueue[i])
	//	{
	//		delete sendqueue[i];
	//		sendqueue[i] = nullptr;
	//	}
	//}

	//for (size_t i = 0; i < recvqueue.size(); i++)
	//{
	//	if (recvqueue[i])
	//	{
	//		delete recvqueue[i];
	//		recvqueue[i] = nullptr;
	//	}
	//}
	//
	//EchoReceiveCount = 0;
	//sendqueue.clear();
	//recvqueue.clear();
}

void SimpleToken::BroadCastCalcResult()
{
}

char* SimpleToken::MakeAlphabet(int size)
{
	char* str = new char[size + 1];

	for (int i = 0; i < size; i++)
	{
		// ascii a ~ z / 65 ~ 90
		str[i] = i % 25;
		str[i] += 65;
	}
	str[size] = '\0';
	return str;
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

#pragma region OldCode


//void SimpleToken::GetStringData()
//{
//	// Receive Queue 에 도착한 Echo 패킷들을 검증한다.
//	std::scoped_lock<std::mutex, std::mutex> lock(SqLock, RqLock);
//	if (SendQueue.empty() || ReceiveQueue.empty())
//	{
//		return;
//	}

	//if (std::strcmp(SendQueue.front(), ReceiveQueue.front()) == 0)
	//{
	//	//NetLogger::GetIns()->Write(L"SUCCESS", L"Good Result  ", SendQueue.front());

	//	auto tmp = ReceiveQueue.front();
	//	if (tmp)
	//	{
	//		delete[] tmp;
	//		tmp = nullptr;
	//	}

	//	tmp = SendQueue.front();
	//	if (tmp)
	//	{
	//		delete[] tmp;
	//		tmp = nullptr;
	//	}

	//	ReceiveQueue.pop_front();
	//	SendQueue.pop_front();
	//}
	//else
	//{
	//	NetLogger::GetIns()->Write(L"ERROR", L"Bad Result  ", ReceiveQueue.front());

	//	auto tmp = ReceiveQueue.front();
	//	if (tmp)
	//	{
	//		delete[] tmp;
	//		tmp = nullptr;
	//	}

	//	tmp = SendQueue.front();
	//	if (tmp)
	//	{
	//		delete[] tmp;
	//		tmp = nullptr;
	//	}

	//	ReceiveQueue.pop_front();
	//	SendQueue.pop_front();
	//}
//}

#pragma endregion
