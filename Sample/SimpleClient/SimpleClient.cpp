#include "SimpleClient.h"
#include "SimpleToken.h"

#include "PacketDefine.h"

#include <filesystem>

IOCP* SimpleClient::Net = nullptr;

SimpleClient::SimpleClient()
	:MainBtn(false), ExitEvent(NULL), CheckTime(0), MonitoringTime(1.0), Time(nullptr), IsConnectTry(false), IsConnectReTry(false), IsDisconnectTry(false),
	MaxSession(0), ExtraSession(0), Ip(L""), Port(0), NetworkThreadCount(0), ConnectSessionCount(0),
	EchoTryCount(0), EchoMinCount(0), EchoMaxCount(0), EchoTerm(0), EchoCalcResetCount(0),
	BroadCastTryCount(0), BroadCastMinCount(0), BroadCastMaxCount(0), BroadCastTerm(0), HeartBeatTime(0),
	TotalTryCount(0), TotalFailCount(0), IsGetResult(false), TotalReceiveCount(0),
	Mode(-1), PreMode(-1),
	DataMissCount(0), SeqMissCount(0), SeqMissFindSucessCount(0), SeqMissFindFailCount(0), StringMissCount(0), AverageRtt(0.0), MaximumRtt(0.0)
{
	NetLogger::GetIns()->AddPage(new PConsole());
}

SimpleClient::~SimpleClient()
{
	Release();

	MainBtn.store(false);
	if (MainThread.joinable() == true)
	{
		MainThread.join();
	}
}

void SimpleClient::Create()
{
	wchar_t szSrc[MAX_PATH];
	ZeroMemory(szSrc, MAX_PATH);

	std::wstring dirpath = std::filesystem::current_path();
	std::wstring iniName = L"\\simple_client.ini";

	// ini 파일에서 데이터를 가져옵니다.

	// 서버에 접속할 IP, Port를 가져옵니다.
	GetPrivateProfileString(L"Simple", L"IP", L"NONE", szSrc, MAX_PATH, (dirpath + iniName).c_str());
	Ip = szSrc;

	Port = GetPrivateProfileInt(L"Simple", L"PORT", 9000, (dirpath + iniName).c_str());

	// 한번에 연결할 수 있는, 그리고 확장 세션의 갯수를 가져옵니다.
	MaxSession = GetPrivateProfileInt(L"Simple", L"MAX_SESSION", 100, (dirpath + iniName).c_str());
	ExtraSession = GetPrivateProfileInt(L"Simple", L"EXTRA_SESSION", 500, (dirpath + iniName).c_str());

	// 세션 중 커넥트할 세션의 갯수를 가져옵니다.
	ConnectSessionCount = GetPrivateProfileInt(L"Simple", L"CONNECT_SESSION_COUNT", 1, (dirpath + iniName).c_str());

	// 세션 한번에 시도할 에코 패킷의 수를 가져옵니다.
	EchoTryCount = GetPrivateProfileInt(L"Simple", L"ECHO_COUNT", 1, (dirpath + iniName).c_str());
	EchoMinCount = GetPrivateProfileInt(L"Simple", L"ECHO_MIN_SIZE", 1, (dirpath + iniName).c_str());
	EchoMaxCount = GetPrivateProfileInt(L"Simple", L"ECHO_MAX_SIZE", 1, (dirpath + iniName).c_str());

	// 에코 패킷의 송신 주기를 가져옵니다.
	EchoTerm = GetPrivateProfileInt(L"Simple", L"ECHO_TERM", 1, (dirpath + iniName).c_str());

	// 세션 한번에 시도할 브로드캐스트 패킷의 수를 가져옵니다.
	BroadCastTryCount = GetPrivateProfileInt(L"Simple", L"BROADCAST_COUNT", 1, (dirpath + iniName).c_str());
	BroadCastMinCount = GetPrivateProfileInt(L"Simple", L"BROADCAST_MIN_SIZE", 1, (dirpath + iniName).c_str());
	BroadCastMaxCount = GetPrivateProfileInt(L"Simple", L"BROADCAST_MAX_SIZE", 1, (dirpath + iniName).c_str());

	// 브로드캐스트 패킷의 송신 주기를 가져옵니다.
	BroadCastTerm = GetPrivateProfileInt(L"Simple", L"BROADCAST_TERM", 1, (dirpath + iniName).c_str());

	// HeartBeat 패킷의 송신 주기를 가져옵니다.
	HeartBeatTime = GetPrivateProfileInt(L"Simple", L"HEATBEAT_TIME", 1, (dirpath + iniName).c_str());

	NetworkThreadCount = GetPrivateProfileInt(L"Simple", L"NETWORK_THREAD_COUNT", 1, (dirpath + iniName).c_str());
	ExitEvent = CreateEvent(NULL, true, false, nullptr);

	Net = new IOCP(NetworkThreadCount, MaxSession + ExtraSession);
	Time = new Timer();

	for (int i = 0; i < MaxSession + ExtraSession; i++)
	{
		Clients.emplace_back(new SimpleToken(i, this));
	}

	for (int i = 0; i < MaxSession; ++i)
	{
		Clients[i]->MappingProcotol();
		Clients[i]->EchoSetting(EchoTryCount, EchoMinCount, EchoMaxCount, EchoTerm);
		Clients[i]->BroadCastSetting(BroadCastTryCount, BroadCastMinCount, BroadCastMaxCount, BroadCastTerm);
	}

	for (int i = 0; i < static_cast<int>(PROTOCOL::END); i++)
	{
		FuncTable.push_back(nullptr);
		FuncTable[i] = &SimpleClient::NotImplemented;
	}

	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::ACCEPT)] = &SimpleClient::OnAccept;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT)] = &SimpleClient::OnConnect;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT_FAIL)] = &SimpleClient::OnConnectFail;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_ACCEPT)] = &SimpleClient::OnDisconnect_Accept;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_CONNECT)] = &SimpleClient::OnDisconnect_Connect;
}

void SimpleClient::Init()
{
	Net->Init();
}

void SimpleClient::Wait()
{
	WaitForSingleObject(ExitEvent, INFINITE);
}

void SimpleClient::Run()
{
	Time->Start();
	Net->Run();

	MainThread = std::thread([=]()
		{
			int ret = 0;
			MainBtn.store(true);
			while (MainBtn.load())
			{
				if (GetAsyncKeyState('C'))	// C를 누르면 커넥트를 시작합니다.
				{
					ConnectTry(ConnectSessionCount);
				}

				if (GetAsyncKeyState('R'))	// C를 누르면 커넥트를 시작합니다.
				{
					ConnectReTry();
				}

				if (GetAsyncKeyState('D'))	// D를 누르면 디스 커넥트를 시작합니다.
				{
					DisconnectTry();
				}

				if (GetAsyncKeyState(VK_ESCAPE)) // ESC를 누르면 더미 클라이언트를 종료합니다.
				{
					MainBtn.store(false);
				}

				ReadPackage(100);

				Time->Tick();
				Monitoring();
				ModeSelect();

				switch (Mode)
				{
				case SimpleClient::ECHO:
					EchoProcess();
					break;
				case SimpleClient::BROACAST:
					BroadCastProcess();
					break;
				case SimpleClient::STOP:
					Stop_GetResult();
					break;

				default:
					break;
				}
			}
			SetEvent(ExitEvent);
		});
}

void SimpleClient::Release()
{
	Net->Stop();
	size_t size = Clients.size();

	for (int i = 0; i < size; i++)
	{
		TotalFailCount += Clients[i]->GetEchoFailCount();
		TotalTryCount += Clients[i]->GetEchoTryCount();

		SafeDelete(Clients[i]);
	}

	SafeDelete(Net);
	NetLogger::GetIns()->Write(L"System", L"Total Send Count : ", TotalTryCount, L" / ", TotalFailCount);

	NetLogger::GetIns()->Destory();
	if (Time)
	{
		delete Time;
		Time = nullptr;
	}
}

void SimpleClient::Connect(std::wstring ip, unsigned short port)
{
	std::string st = Util::UTF16ToUTF8(ip);
	Net->Connect(st.c_str(), port);
}

bool SimpleClient::Connect(int id, std::wstring ip, unsigned short port)
{
	std::string st = Util::UTF16ToUTF8(ip);
	return Net->Connect(id, st.c_str(), port);
}

void SimpleClient::Disconnect(int session_id)
{
	Net->Disconnect(session_id);
}

int SimpleClient::SendPacket(int dst, PACKET& packet)
{
	return Net->SendMsg(dst, packet);
}

void SimpleClient::FourceEnque(PACKAGE& package)
{
	Net->FourceEnquePackage(package);
}

void SimpleClient::ConnectTry(int size)
{
	if (!IsConnectTry)
	{
		NetLogger::GetIns()->Write(L"Client", L"Connecting Try..");

		IsConnectReTry = true;
		IsConnectTry = true;
		IsDisconnectTry = false;

		for (int i = 0; i < size; i++)
		{
			Net->Connect(Util::UTF16ToAnsi(Ip.c_str()).c_str(), Port);
		}
	}
}

void SimpleClient::ConnectReTry()
{
	if (!IsConnectReTry)
	{
		IsConnectReTry = true;
		IsConnectTry = true;
		IsDisconnectTry = false;

		for (auto client : Clients)
		{
			if (client->HadConnected)
			{
				Net->Connect(client->GetSessionId(), Util::UTF16ToAnsi(Ip.c_str()).c_str(), Port);
			}
		}
	}
}

void SimpleClient::DisconnectTry()
{
	if (!IsDisconnectTry)
	{
		NetLogger::GetIns()->Write(L"Client", L"Disconnect Try..");

		IsDisconnectTry = true;
		IsConnectTry = false;
		IsConnectReTry = false;

		for (auto client : Clients)
		{
			if (client->Connection)
			{
				client->Disconnect();
			}
		}
	}
}

bool SimpleClient::ReadPackage(int count)
{
	bool result = false;
	PACKAGE pack = { 0, {0} };

	for (int i = 0; i < count; ++i)
	{
		if (Net->GetPackage(pack) == true)
		{
			if (pack.packet.head.protocol < static_cast<unsigned char>(SYSTEM_PROTOCOL::JOB))
			{
				FuncTable[static_cast<unsigned short>(pack.packet.head.protocol)](*this, pack);
			}
			else
			{
				OnJob(pack);
			}
		}
	}
	return false;
}

void SimpleClient::ModeSelect()
{
	IsGetResult = true;
	if (GetAsyncKeyState('Q'))					// Q를 누르면 Echo 테스트를 시작합니다.
	{
		if (Mode != ECHO)
		{
			PreMode = Mode;
			Mode = ECHO;
		}
	}

	if (GetAsyncKeyState('W'))					// W를 누르면 .. 미완성
	{
		if (Mode != BROACAST)
		{
			PreMode = Mode;
			Mode = BROACAST;
		}
	}

	if (GetAsyncKeyState('E'))					// E를 누르면 ..
	{
	}

	if (GetAsyncKeyState(VK_SPACE))				// SPACE를 누르면 멈춥니다.
	{
		if (Mode != STOP)
		{
			IsGetResult = false;
			PreMode = Mode;
			Mode = STOP;
		}
	}
}

void SimpleClient::EchoProcess()
{
	for (auto client : Clients)
	{
		if (client->Connection)
		{
			client->EchoProcess();

			if (++EchoCalcResetCount > 100)
			{

			}
		}
	}
}

void SimpleClient::BroadCastProcess()
{
	for (auto client : Clients)
	{
		if (client->Connection)
		{
			client->BroadCastProcess();
		}
	}
}

void SimpleClient::Stop_GetResult()
{
	switch (PreMode)
	{
	case SimpleClient::ECHO:
		{
			for (auto client : Clients)
			{
				if (client->Connection)
				{
					TotalFailCount += client->SendFailCount;
					TotalTryCount += client->SendTryCount;
					client->EchoCalcResult();

					// data miss를 센다.
					DataMissCount += client->DataMissCount;

					// sequence miss를 센다.
					SeqMissCount += client->SeqMissCount;

					// string miss를 센다.
					StringMissCount += client->StringMissCount;

					SeqMissFindFailCount += client->SeqMissFindFailCount;

					SeqMissFindSucessCount += client->SeqMissFindSucessCount;

					TotalReceiveCount += client->ReceiveCount;


					// Rtt 계산.

					// 평균 Rtt를 계산한다.
					AverageRtt += client->Rtt;

					// 가장 느린 Rtt를 가져온다.
					MaximumRtt = client->Rtt > MaximumRtt ? client->Rtt : MaximumRtt;
				}
			}
			IsGetResult = true;
		}

		if (GetAsyncKeyState('A'))
		{
			NetLogger::GetIns()->Write(L"Echo Result", "Total Send FailCount :", TotalFailCount, L" / ", TotalTryCount);
			NetLogger::GetIns()->Write(L"Echo Result", "Total Recv Count :", TotalReceiveCount);
			NetLogger::GetIns()->Write(L"Echo Result", "Data Miss Count :", DataMissCount);
			NetLogger::GetIns()->Write(L"Echo Result", "Sequence Miss Count :", SeqMissFindSucessCount, " / ", SeqMissCount,
				" Packet Loss : ", SeqMissFindFailCount);
			NetLogger::GetIns()->Write(L"Echo Result", "String Miss Count :", StringMissCount);
			NetLogger::GetIns()->Write(L"Echo Result", "Average Rtt :", AverageRtt / Clients.size());
			NetLogger::GetIns()->Write(L"Echo Result", "Maximum Rtt :", MaximumRtt);

			TotalTryCount = 0;
			TotalFailCount = 0;
			TotalReceiveCount = 0;
			DataMissCount = 0;
			SeqMissCount = 0;
			SeqMissFindSucessCount = 0;
			SeqMissFindFailCount = 0;
			StringMissCount = 0;
			MaximumRtt = 0.0;
			AverageRtt = 0.0;
		}

		break;
	case SimpleClient::BROACAST:
		break;
	case SimpleClient::STOP:
		break;
	}
}

void SimpleClient::Monitoring()
{
	//#ifdef _DEBUG
	CheckTime += Time->DeltaTime();

	if (CheckTime >= MonitoringTime)
	{
		CheckTime = 0;
		NetLogger::GetIns()->Write(L"system", L"", L"",
			L"CPU USE : ", Mon.CpuUsage(),
			L"  PROCESS CPU USE :", Mon.ThisProcessCpuUsage(),
			L"  PHYSIC MEMORY : ", Mon.PhysicMemory(), L"  bytes"
			L"  WORKING SIZE : ", Mon.WorkingSet(), L"  bytes");
	}
	//#endif // _DEBUG
}

void SimpleClient::NotImplemented(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"dba", L"Not Implemented");
}

void SimpleClient::OnAccept(PACKAGE& package)
{
	Clients[package.id]->OnAccept(package);
}

void SimpleClient::OnConnect(PACKAGE& package)
{
	Clients[package.id]->OnConnect(package);
}

void SimpleClient::OnConnectFail(PACKAGE& package)
{
	Clients[package.id]->OnConnectFail(package);
}

void SimpleClient::OnDisconnect_Accept(PACKAGE& package)
{
	Clients[package.id]->OnDisconnect_Accept(package);
}

void SimpleClient::OnDisconnect_Connect(PACKAGE& package)
{
	Clients[package.id]->OnDisconnect_Connect(package);
}

void SimpleClient::OnJob(PACKAGE& package)
{
	Clients[package.id]->OnJob(package);
}
