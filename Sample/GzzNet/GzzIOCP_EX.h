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

		// Session ���� Pool
		std::vector<TcpSocketEX*> SessionPool;

		// Send Overlapped Pool �Դϴ�. Send�� �Ϸ�ó���� ����մϴ�.
		std::vector<LPOVERLAPPED> SendItemPool;

		// Receive Overlapped Pool �Դϴ�. Receive�� �Ϸ�ó���� ����մϴ�.
		std::vector<LPOVERLAPPED> RecvItemPool;

		// Session Overlapped Pool �Դϴ�. ������ ����� ���� �� �Ϸ�ó���� ����մϴ�.
		std::vector<LPOVERLAPPED> SessionItemPool;

	public:
		// �⺻ �����ڴ� �����մϴ�.
		GzzIOCP_EX() = delete;

		// �ִ� ���� ������ ���ǰ� Create �� IOCP Thread ������ ���մϴ�.
		GzzIOCP_EX(const int maxIoThreadCount, const int maxSessionCount);
		~GzzIOCP_EX();

	public:
		// ������ ��Ʈ��ũ ���̺귯�� ������ ������ Init �մϴ�.
		virtual int	 Init();

		// IO Woker�� �������ݴϴ�.
		virtual int	 Run();

		// ��ü ������ Stop ��ɾ� �Դϴ�.
		virtual int	 Stop();

		// AcceptEx�� �� Socket�� ������ŭ ������ ���� ��, ��� ��ŵ�ϴ�. �� �� Listen Port�� �����ݴϴ�.
		virtual int	 Open(unsigned short maxConnection, unsigned short port);

		// ConnectEx �� �����ϴ� Connect �Դϴ�.
		virtual int	 Connect(const char* dst, const short port);

		// ������ �̹� ������ ��� ����ϴ� ConnectEx �Դϴ�.
		virtual bool Connect(int id, const char* dst, const short port);

		// �Ϲ� connect �Լ��� �����ϴ� Connect �Դϴ�.
		virtual int	 UnExConnect(const char* dst, const short port);

		// ������ �̹� ������ ��� ����ϴ� connect �Դϴ�.
		virtual bool UnExConnect(int id, const char* dst, const short port);

		// ������ �����մϴ�.
		virtual int	 Disconnect(const int Id);

		// ������ JobQueue�� ��Ŷ �����͸� �ֽ��ϴ�.
		void FourceEnquePackage(PACKAGE& package);

		// JobQueue���� �ϳ��� ��Ŷ�� Enqueue �մϴ�.
		virtual bool GetPackage(PACKAGE& package);

		// �޼����� �����ϴ�.
		virtual int	 SendMsg(const int id, PACKET packet);

	private:
		int  CreateCompletionPort();
		void CreateIoWoker();

		// IO Woker���� ��� �� �Լ� �Դϴ�.
		int  IoFunc();

		// �����带 ������ �� ����մϴ�.
		int  DestroyThread();

		// PostQueueMessage�� Ȱ���Ͽ� IO Woker �����带 ���� ��ŵ�ϴ�.
		void IoCloseFlag();

		// ��ü ������ �������� ��ŵ�ϴ�.
		void CloseAllSession();

		// ���ϰ� Overlapped Pool�� �����մϴ�.
		void ReservationSession(SESSION_TYPE type, const int id, IReceive* recv, ISend* send);

		void CreateCompletionItem(const int size);
	};
}
