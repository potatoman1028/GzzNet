#pragma once
#include "IoDefine.h"
#include "JobQueue.h"
#include "Note.h"

#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
#include <deque>
#include <mutex>

namespace gzz
{
	class GzzIOCP_EX
	{
	private:

		int MaxIoThreadCount;
		int MaxSessionCount;

		// message Queue
		JobQueue<PACKAGE> MessageQueue;

		// Completion Handle
		HANDLE IocpHandle;

		// Listener
		TcpListen* Host;

		// Works
		std::vector<std::thread> IoWorkers;

		std::atomic<int> CloseCount;

		// Session 관리 Pool
		std::vector<TcpSocketEX*> SessionPool;

		// Send Overlapped Pool 입니다. Send의 완료처리를 담당합니다.
		std::vector<LPOVERLAPPED> SendItemPool;

		// Receive Overlapped Pool 입니다. Receive의 완료처리를 담당합니다.
		std::vector<LPOVERLAPPED> RecvItemPool;

		// Session Overlapped Pool 입니다. 세션의 연결과 종료 후 완료처리를 담당합니다.
		std::vector<LPOVERLAPPED> SessionItemPool;

	public:
		// 기본 생성자는 삭제합니다.
		GzzIOCP_EX() = delete;

		// 최대 연결 가능한 세션과 Create 할 IOCP Thread 갯수를 정합니다.
		GzzIOCP_EX(const int maxIoThreadCount, const int maxSessionCount);
		~GzzIOCP_EX();

	public:
		// 설정된 네트워크 라이브러리 정보를 가지고 Init 합니다.
		virtual int	 Init();

		// IO Woker을 생성해줍니다.
		virtual int	 Run();

		// 전체 서버의 Stop 명령어 입니다.
		virtual int	 Stop();

		// AcceptEx를 할 Socket의 갯수만큼 소켓을 생성 후, 대기 시킵니다. 그 후 Listen Port를 열어줍니다.
		virtual int	 Open(unsigned short maxConnection, unsigned short port);

		// ConnectEx 로 동작하는 Connect 입니다.
		virtual int	 Connect(const char* dst, const short port);

		// 소켓이 이미 생성된 경우 사용하는 ConnectEx 입니다.
		virtual bool Connect(int id, const char* dst, const short port);

		// 일반 connect 함수로 동작하는 Connect 입니다.
		virtual int	 UnExConnect(const char* dst, const short port);

		// 소켓이 이미 생성된 경우 사용하는 connect 입니다.
		virtual bool UnExConnect(int id, const char* dst, const short port);

		// 소켓을 종료합니다.
		virtual int	 Disconnect(const int Id);

		// 강제로 JobQueue에 패킷 데이터를 넣습니다.
		void FourceEnquePackage(PACKAGE& package);

		// JobQueue에서 하나의 패킷을 Enqueue 합니다.
		virtual bool GetPackage(PACKAGE& package);

		// 메세지를 보냅니다.
		virtual int	 SendMsg(const int id, PACKET packet);

	private:
		int  CreateCompletionPort();
		void CreateIoWoker();

		// IO Woker에서 사용 될 함수 입니다.
		int  IoFunc();

		// 스레드를 제거할 때 사용합니다.
		int  DestroyThread();

		// PostQueueMessage를 활용하여 IO Woker 스레드를 종료 시킵니다.
		void IoCloseFlag();

		// 전체 세션을 강제종료 시킵니다.
		void CloseAllSession();

		// 소켓과 Overlapped Pool을 연동합니다.
		void ReservationSession(SESSION_TYPE type, const int id, IReceive* recv, ISend* send);

		void CreateCompletionItem(const int size);
	};
}
