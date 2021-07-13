#include "DBAgent.h"
#include "Querys.h"
#include "Tables.h"

// #define DB_DEBUG


using namespace std;
using namespace chrono;

IOCP* DBAgent::Net = nullptr;
DBManager* DBAgent::DBM = nullptr;
int DBAgent::BestScore = 0;

DBAgent::DBAgent()
	:OnBtn(false), ExitEvent(NULL), CheckTime(0), MonitoringTime(60), Time(nullptr)
{
	NetLogger::GetIns()->AddPage(new PConsole());
}

DBAgent::~DBAgent()
{
	Release();
	OnBtn.store(false);
	if (MainThread.joinable() == true)
	{
		MainThread.join();
	}
}

void DBAgent::Create()
{
	ExitEvent = CreateEvent(NULL, true, false, nullptr);
	Net = new IOCP(5, 2);
	DBM = new DBManager();
	Time = new Timer();
	Clients.emplace_back(new Game(0, this));
	Clients.emplace_back(new Login(1, this));

	for (int i = 0; i < 2; ++i)
	{
		Clients[i]->MappingProcotol();
	}

	for (int i = 0; i < static_cast<int>(PROTOCOL::END); i++)
	{
		DBFuncTable.push_back(nullptr);
		DBFuncTable[i] = &DBAgent::NotImplemented;
	}

	DBFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::ACCEPT)] = &DBAgent::OnAccept;
	DBFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT)] = &DBAgent::OnConnect;
	DBFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::CONNECT_FAIL)] = &DBAgent::OnConnectFail;
	DBFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_ACCEPT)] = &DBAgent::OnDisconnect_Accept;
	DBFuncTable[static_cast<unsigned short>(SYSTEM_PROTOCOL::DISCONNECT_CONNECT)] = &DBAgent::OnDisconnect_Connect;
}

void DBAgent::Init()
{
	Net->Init();
	DBM->Init(5, "DSN=GonDB; UID=root; PWD=910540; DATABASE=gondb; TCPIP=1");
}

void DBAgent::Wait()
{
	WaitForSingleObject(ExitEvent, INFINITE);
}

void DBAgent::Run()
{
	Net->Run();
	DBM->Run();
	Time->Start();
	MainThread = std::thread([=]()
		{
			Net->Open(2, 9123);
			OnBtn.store(true);
			while (OnBtn.load())
			{
				ReadPackage();
				if (GetAsyncKeyState(VK_ESCAPE))
				{
					OnBtn.store(false);
				}
#ifdef DB_DEBUG
				DB_Debuging_Process();
#endif // DB_DEBUGW
				
				Process();
				Monitoring();
			}
			SetEvent(ExitEvent);
		});
}

void DBAgent::Release()
{
	Net->Stop();
	DBM->Release();

	for (size_t i = 0; i < Clients.size(); i++)
	{
		SafeDelete(Clients[i]);
	}
	delete Net;
	delete DBM;
	NetLogger::GetIns()->Destory();
	if (Time)
	{
		delete Time;
		Time = nullptr;
	}
}

void DBAgent::Connect(std::wstring ip, unsigned short port)
{
	std::string st = Util::UTF16ToUTF8(ip);
	Net->Connect(st.c_str(), port);
}

bool DBAgent::Connect(int id, std::wstring ip, unsigned short port)
{
	std::string st = Util::UTF16ToUTF8(ip);
	return Net->Connect(id, st.c_str(), port);
}

void DBAgent::Disconnect(int session_id)
{
	Net->Disconnect(session_id);
}

void DBAgent::SendPacket(int dst, PACKET& packet)
{
	Net->SendMsg(dst, packet);
}

void DBAgent::FourceEnque(PACKAGE& package)
{
	Net->FourceEnquePackage(package);
}

void DBAgent::DB_Debuging_Process()
{
	// test 할 정보 입력 


	if (GetAsyncKeyState('Q'))
	{
		if (toggleQ == false)
		{
			toggleQ = true;
			DBM->AddQuery(new debug_table::SP_Users());
		}
	}
	else
	{
		if (toggleQ == true)
		{
			toggleQ = false;
		}
	}

	if (GetAsyncKeyState('W'))
	{
		if (toggleW == false)
		{
			toggleW = true;
			DBM->AddQuery(new debug_table::SP_Rooms());
		}
	}
	else
	{
		if (toggleW == true)
		{
			toggleW = false;
		}
	}
	if (GetAsyncKeyState('E'))
	{
		if (toggleE == false)
		{
			toggleE = true;
			DBM->AddQuery(new debug_table::SP_SignUp(L"'gongongon1'", L"'Gqwe123!@#'"));
		}
	}
	else
	{
		if (toggleE == true)
		{
			toggleE = false;
		}
	}

	if (GetAsyncKeyState('R'))
	{
		if (toggleR == false)
		{
			toggleR = true;
			DBM->AddQuery(new debug_table::SP_Withdrawal(L"'gongon1'", L"'qwe123!@#'"));
		}
	}
	else
	{
		if (toggleR == true)
		{
			toggleR = false;
		}
	}
	/// <summary>
	/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// </summary>
	if (GetAsyncKeyState(VK_0))
	{
		if (toggle0 == false)
		{
			toggle0 = true;
			DBM->AddQuery(new debug_table::SP_Login(L"'gongon1'", L"'qwe123!@#'"));
		}
	}
	else
	{
		if (toggle0 == true)
		{
			toggle0 = false;
		}
	}
	if (GetAsyncKeyState(VK_1))
	{
		if (toggle1 == false)
		{
			toggle1 = true;
			DBM->AddQuery(new debug_table::SP_Logout(L"'gongon1'", L"'qwe123!@#'"));
		}
	}
	else
	{
		if (toggle1 == true)
		{
			toggle1 = false;
		}
	}
	if (GetAsyncKeyState(VK_2))
	{
		if (toggle2 == false)
		{
			toggle2 = true;
			DBM->AddQuery(new debug_table::SP_ActiveRoom(L"'gongon1'", 6, L"'절대 고수만3'"));
		}
	}
	else
	{
		if (toggle2 == true)
		{
			toggle2 = false;
		}
	}

	if (GetAsyncKeyState(VK_3))
	{
		if (toggle3 == false)
		{
			toggle3 = true;
			DBM->AddQuery(new debug_table::SP_DeactiveRoom(6));
		}
	}
	else
	{
		if (toggle3 == true)
		{
			toggle3 = false;
		}
	}

	if (GetAsyncKeyState(VK_4))
	{
		if (toggle4 == false)
		{
			toggle4 = true;
			DBM->AddQuery(new debug_table::SP_WaitStateRoom(6));
		}
	}
	else
	{
		if (toggle4 == true)
		{
			toggle4 = false;
		}
	}

	if (GetAsyncKeyState(VK_5))
	{
		if (toggle5 == false)
		{
			toggle5 = true;
			DBM->AddQuery(new debug_table::SP_GameStateRoom(6));
		}
	}
	else
	{
		if (toggle5 == true)
		{
			toggle5 = false;
		}
	}

	if (GetAsyncKeyState(VK_6))
	{
		if (toggle6 == false)
		{
			toggle6 = true;
			DBM->AddQuery(new debug_table::SP_LookUpActiveRooms());
		}
	}
	else
	{
		if (toggle6 == true)
		{
			toggle6 = false;
		}
	}
}

void DBAgent::Process()
{
}

void DBAgent::Monitoring()
{
#ifdef _DEBUG
	Time->Tick();
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
#endif // _DEBUG
}

bool DBAgent::ReadPackage()
{
	bool result = false;
	PACKAGE pack = { 0, {0} };
	if (Net->GetPackage(pack) == true)
	{
		if (pack.packet.head.protocol < static_cast<unsigned char>(SYSTEM_PROTOCOL::JOB))
		{
			DBFuncTable[static_cast<unsigned short>(pack.packet.head.protocol)](*this, pack);
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

void DBAgent::NotImplemented(PACKAGE& package)
{
	NetLogger::GetIns()->Write(L"dba", __FUNCTIONW__, L"Not Implemented");
}

void DBAgent::OnAccept(PACKAGE& package)
{
	Clients[package.id]->OnAccept(package);
}

void DBAgent::OnConnect(PACKAGE& package)
{
	NotImplemented(package);
}

void DBAgent::OnConnectFail(PACKAGE& package)
{
	NotImplemented(package);
}

void DBAgent::OnDisconnect_Accept(PACKAGE& package)
{
	Clients[package.id]->OnDisconnect_Accept(package);
}

void DBAgent::OnDisconnect_Connect(PACKAGE& package)
{
	NotImplemented(package);
}

void DBAgent::OnJob(PACKAGE& package)
{
	Clients[package.id]->OnJob(package);
}
