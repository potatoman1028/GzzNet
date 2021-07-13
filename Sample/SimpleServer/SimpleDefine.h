#pragma once
#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"GzzNet_x64D")
#else
#pragma comment(lib,"GzzNet_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"GzzNet_x64R")
#else
#pragma comment(lib,"GzzNet_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)

#define WIN32_LEAN_AND_MEAN

#include "../GzzNet/NetDefine.h"
#include "../GzzNet/RpcTable.h"
#include "../GzzNet/RpcMap.h"
#include "../GzzNet/Note.h"
#include "../GzzNet/DBManager.h"
#include "../GzzNet/GzzIOCP_EX.h"
#include "../GzzNet/FormatUtils.h"
#include "../GzzNet/PConsole.h"
#include "../GzzNet/Clock.h"
#include "../GzzNet/Monitoring.h"
#include "../GzzNet/Query.h"
#include "../GzzNet/Random.h"
#include "../GzzNet/Dump.h"

//#include "PacketDefine.h"

static constexpr int MAX_SESSION_COUNT = 5000;
static constexpr int EXTRA_SESSION_COUNT = 500;
static constexpr int PACKET_HEADER_SIZE = gzz::PACKET_HEADER_SIZE;

using PACKET_HEAD = gzz::PACKET_HEADER;
using PACKET = gzz::PACKET;
using PACKAGE = gzz::PACKAGE;
using RpcMap = gzz::RpcMap<void, PACKAGE&>;
using RpcTable = gzz::RpcTable<void, PACKAGE&>;

using IOCP = gzz::GzzIOCP_EX;
using SYSTEM_PROTOCOL = gzz::SYSTEM_PROTOCOL;
using NetLogger = gzz::Note;
using DBManager = gzz::DBManager;
using Util = gzz::FormatUtils;
using PConsole = gzz::PConsole;
using Timer = gzz::Clock;
using Monitor = gzz::Monitoring;
using Query = gzz::Query;
using QueryCommand = gzz::QueryCommand;
using QueryRecord = gzz::QueryRecord;
using Rand = gzz::Random;

template<class C>
using CLASS_MEM_PACKAGE_FUNC = void(C::*)(PACKAGE&);

class SimpleToken;
class SimpleServer;

template<class T>
__forceinline void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

