#include "LoginServer.h"

#include <thread>
#include <mutex>	
#include <vector>
#include <atomic>
#include <deque>

IOCP* LoginServer::Net = nullptr;
LoginServer::LoginServer()
	:MainBtn(false), NetBtn(false), ExitEvent(NULL), Time(nullptr),
	MonitorCheckTime(0), MonitoringTime(60), HeartCheckTime(0), HeartBeatTime(300), DbConnectionTokenId(0)

{
	NetLogger::GetIns()->AddPage(new PConsole());
}

LoginServer::~LoginServer()
{
	NetBtn.store(false);
	if (NetWoker.joinable() == true)
	{
		NetWoker.join();
	}

	Release();

	MainBtn.store(false);
	if (MainThread.joinable() == true)
	{
		MainThread.join();
	}
}

void LoginServer::Create()
{
	ExitEvent = CreateEvent(NULL, true, false, nullptr);

	for (int i = 0; i < MAX_SESSION_COUNT + EXTRA_SESSION_COUNT; i++)
	{
		if (MAX_SESSION_COUNT != i)
			Tokens.emplace_back(new Customer(i, this));
		else
		{
			Tokens.emplace_back(new DB_Connection(i, this));
			DbConnectionTokenId = MAX_SESSION_COUNT;
		}
	}

	for (int i = 0; i < static_cast<unsigned short>(SYSTEM_PROTOCOL::JOB); i++)
	{
		LoginFuncTable.push_back(nullptr);
		LoginFuncTable[i] = &LoginServer::NotImplemented;
	}
	LoginFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::ACCEPT)] = &LoginServer::OnAccept;
	LoginFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT)] = &LoginServer::OnConnect;
	LoginFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT_FAIL)] = &LoginServer::OnConnectFail;
	LoginFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_ACCEPT)] = &LoginServer::OnDisconnect_Accept;
	LoginFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_CONNECT)] = &LoginServer::OnDisconnect_Connect;

	for (int i = 0; i < MAX_SESSION_COUNT + EXTRA_SESSION_COUNT; i++)
	{
		Tokens[i]->MappingProcotol();
	}

	Net = new IOCP(5, MAX_SESSION_COUNT + EXTRA_SESSION_COUNT);
	Time = new Timer();
}

void LoginServer::Init()
{
	Net->Init();
}

void LoginServer::Wait()
{
	WaitForSingleObject(ExitEvent, INFINITE);
}

void LoginServer::Run()
{
	Time->Start();	
	NetBtn.store(true);
	NetWoker = std::thread([=]()
		{
			Net->Open(MAX_SESSION_COUNT, 9547);
			Net->Run();
			Net->Connect("127.0.0.1", 9123);
			while (NetBtn.load())
			{
				ReadPackage();
			}
		}
	);
	MainThread = std::thread([=]()
		{
			Time->Tick();
			MainBtn.store(true);
			while (MainBtn.load())
			{
				Process();
				Monitoring();
				HeartBeatChk();
			}
			SetEvent(ExitEvent);
		}
	);
}

void LoginServer::Release()
{
	Net->Stop();
	size_t size = Tokens.size();
	for (int i = 0; i < size; i++)
	{
		SafeDelete(Tokens[i]);
	}
	SafeDelete(Net);
	SafeDelete(Time);
	NetLogger::GetIns()->Destory();
}

void LoginServer::Connect(std::wstring ip, unsigned short port)
{	
	std::string st = Util::UTF16ToUTF8(ip);
	Net->Connect(st.c_str(), port);
}

bool LoginServer::Connect(int id, std::wstring ip, unsigned short port)
{
	std::string st = Util::UTF16ToUTF8(ip);
	return Net->Connect(id, st.c_str(), port);
}

void LoginServer::Disconnect(int session_id)
{
	Net->Disconnect(session_id);
}

void LoginServer::SendPacket(int dst, PACKET& packet)
{
	Net->SendMsg(dst, packet);
}

void LoginServer::FourceEnque(PACKAGE& package)
{
	Net->FourceEnquePackage(package);
}

void LoginServer::Process()
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		MainBtn.store(false);
	}

	for (IToken* tok : Tokens)
	{
		// tok->HeartBeatCheck();
	}
}

void LoginServer::Monitoring()
{
#ifdef _DEBUG
	Time->Tick();
	MonitorCheckTime += Time->DeltaTime();		
	if (MonitorCheckTime >= MonitoringTime)
	{
		MonitorCheckTime = 0;
			NetLogger::GetIns()->Write(L"system", L"", L"",
			L"CPU USE : ", Mon.CpuUsage(),
			L"  PROCESS CPU USE :", Mon.ThisProcessCpuUsage(),
			L"  PHYSIC MEMORY : ", Mon.PhysicMemory(), L"  bytes"
			L"  WORKING SIZE : ", Mon.WorkingSet(), L"  bytes");
	}
#endif // _DEBUG
}

void LoginServer::HeartBeatChk()
{
	for (IToken* tok : Tokens)
	{
		tok->HeartBeatCheck();
	}
}

bool LoginServer::ReadPackage()
{
	bool result = false;
	PACKAGE pack = { 0, {0} };
	if (Net->GetPackage(pack) == true)
	{
		if (pack.packet.head.protocol < static_cast<unsigned char>(SYSTEM_PROTOCOL::JOB))
		{
			LoginFuncTable[static_cast<unsigned short>(pack.packet.head.protocol)](*this, pack);
		}
		else
		{
			OnJob(pack);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void LoginServer::NotImplemented(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"login_svr", __FUNCTIONW__, L"Not Implemented");
	// Disconnect(package.id);
}

void LoginServer::OnAccept(PACKAGE& package)
{
	Tokens[package.id]->OnAccept(package);
}

void LoginServer::OnConnect(PACKAGE& package)
{
	Tokens[package.id]->OnConnect(package);
}

void LoginServer::OnConnectFail(PACKAGE& package)
{
	Tokens[package.id]->OnConnectFail(package);
}

void LoginServer::OnDisconnect_Accept(PACKAGE& package)
{
	Tokens[package.id]->OnDisconnect_Accept(package);
}

void LoginServer::OnDisconnect_Connect(PACKAGE& package)
{
	Tokens[package.id]->OnDisconnect_Connect(package);
}

void LoginServer::OnJob(PACKAGE& package)
{
	Tokens[package.id]->OnJob(package);
}
