#pragma once
#include "IoDefine.h"
#include "JobQueue.h"
#include "IReceive.h"
#include "TcpSocketEX.h"
#include "CompletionItemDefine.h"
#include "RingBuffer.h"

namespace gzz
{
	class RecvItemEX : public ICompletionItemEX
	{
	private:
		TcpSocketEX* Socket;
		IReceive* Recv;
		RingBuffer* Buf;
		JobQueue<PACKAGE>* Queue;

	public:
		RecvItemEX() = delete;
		RecvItemEX(int id);
		RecvItemEX(int id, TcpSocketEX* sock, IReceive* item, JobQueue<PACKAGE>* queue);
		virtual ~RecvItemEX();

	public:
		void SettingItem(TcpSocketEX* sock, IReceive* item, JobQueue<PACKAGE>* queue);
		void AddMessage(PACKAGE protocol);

		virtual int Process(const int completionBytes, LPOVERLAPPED other);
	};
}
