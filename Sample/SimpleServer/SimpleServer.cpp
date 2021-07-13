#include "SimpleServer.h"
#include "SimpleToken.h"

#include "PacketDefine.h"

#include <filesystem>

IOCP* SimpleServer::Net = nullptr;

SimpleServer::SimpleServer()
	:MainBtn(false), ExitEvent(NULL), CheckTime(0), MonitoringTime(1.0), Time(nullptr),
	MaxSession(0), ExtraSession(0), Ip(L""), Port(0), NetworkThreadCount(0), ConnectSessionCount(0),
	EchoTryCount(0), EchoMinCount(0), EchoMaxCount(0), EchoTerm(0),
	BroadCastTryCount(0), BroadCastMinCount(0), BroadCastMaxCount(0), BroadCastTerm(0), HeartBeatTime(0),
	TotalTryCount(0), TotalFailCount(0)
{
	NetLogger::GetIns()->AddPage(new PConsole());
}

SimpleServer::~SimpleServer()
{
	Release();

	MainBtn.store(false);
	if (MainThread.joinable() == true)
	{
		MainThread.join();
	}
}

void SimpleServer::Create()
{
	wchar_t szSrc[MAX_PATH];
	ZeroMemory(szSrc, MAX_PATH);

	std::wstring dirpath = std::filesystem::current_path();
	std::wstring iniName = L"\\simple_server.ini";
	
	// ini 파일에서 데이터를 가져옵니다.

	// 오픈 할 IP, IP, Port를  가져옵니다.
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

	for (int i = 0; i < MaxSession + ExtraSession; ++i)
	{
		Clients[i]->EchoSetting(EchoTryCount, EchoMinCount, EchoMaxCount, EchoTerm);
		Clients[i]->BroadCastSetting(BroadCastTryCount, BroadCastMinCount, BroadCastMaxCount, BroadCastTerm);
		Clients[i]->MappingProcotol();
	}

	for (int i = 0; i < static_cast<int>(PROTOCOL::END); i++)
	{
		FuncTable.push_back(nullptr);
		FuncTable[i] = &SimpleServer::NotImplemented;
	}

	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::ACCEPT)] = &SimpleServer::OnAccept;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT)] = &SimpleServer::OnConnect;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT_FAIL)] = &SimpleServer::OnConnectFail;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_ACCEPT)] = &SimpleServer::OnDisconnect_Accept;
	FuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_CONNECT)] = &SimpleServer::OnDisconnect_Connect;
}

void SimpleServer::Init()
{
	Net->Init();
}

void SimpleServer::Wait()
{
	WaitForSingleObject(ExitEvent, INFINITE);
}

void SimpleServer::Run()
{
	Time->Start();
	Net->Open(MaxSession, Port);
	Net->Run();

	MainThread = std::thread([=]()
		{
			MainBtn.store(true);
			while (MainBtn.load())
			{
				if (GetAsyncKeyState(VK_ESCAPE))
				{
					MainBtn.store(false);
				}

				Time->Tick();
				ReadPackage(100);
			
				Monitoring();
				Process();

				if (GetAsyncKeyState('A'))
				{
					NetLogger::GetIns()->Write(L"Echo Result", "Total Send FailCount :", TotalFailCount, L" / ", TotalTryCount);
					TotalFailCount = 0;
					TotalTryCount = 0;
				}
			}
			SetEvent(ExitEvent);
		});
}

void SimpleServer::Release()
{
	Net->Stop();
	int size = static_cast<int>(Clients.size());

	for (int i = 0; i < size; ++i)
	{
		SafeDelete(Clients[i]);
	}
	delete Net;
	NetLogger::GetIns()->Destory();
	if (Time)
	{
		delete Time;
		Time = nullptr;
	}
}

void SimpleServer::Connect(std::wstring ip, unsigned short port)
{
	std::string st = Util::UTF16ToUTF8(ip);
	Net->Connect(st.c_str(), port);
}

bool SimpleServer::Connect(int id, std::wstring ip, unsigned short port)
{
	std::string st = Util::UTF16ToUTF8(ip);
	return Net->Connect(id, st.c_str(), port);
}

void SimpleServer::Disconnect(int session_id)
{
	Net->Disconnect(session_id);
}

int SimpleServer::SendPacket(int dst, PACKET& packet)
{
	return Net->SendMsg(dst, packet);
}

void SimpleServer::FourceEnque(PACKAGE& package)
{
	Net->FourceEnquePackage(package);
}

bool SimpleServer::ReadPackage(int count)
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
			return true;
		}
	}
	return false;
}

void SimpleServer::Process()
{
	for (auto client : Clients)
	{
		if (client->Connection)
		{
			client->Process(0);
			TotalTryCount += client->SendTryCount;
			TotalFailCount += client->SendFailCount;
		}
	}
}

void SimpleServer::Monitoring()
{
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
}

void SimpleServer::NotImplemented(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"SimpleServer", __FUNCTIONW__, L"Not Implemented");
}

void SimpleServer::OnAccept(PACKAGE& package)
{
	Clients[package.id]->OnAccept(package);
}

void SimpleServer::OnConnect(PACKAGE& package)
{
	Clients[package.id]->OnConnect(package);
}

void SimpleServer::OnConnectFail(PACKAGE& package)
{
	Clients[package.id]->OnConnectFail(package);
}

void SimpleServer::OnDisconnect_Accept(PACKAGE& package)
{
	Clients[package.id]->OnDisconnect_Accept(package);
}

void SimpleServer::OnDisconnect_Connect(PACKAGE& package)
{
	Clients[package.id]->OnDisconnect_Connect(package);
}

void SimpleServer::OnJob(PACKAGE& package)
{
	Clients[package.id]->OnJob(package);
}
