#pragma once
#define WIN32_LEAN_AND_MEAN

#include "TcpSocketEX.h"
#include "ISend.h"
#include "CompletionItemDefine.h"

#include "LockedCQ.hpp"

namespace gzz
{
	class SendItemEX :
		public ICompletionItemEX
	{
	protected:
		TcpSocketEX* Socket;
		ISend* Send;

	public:
		SendItemEX() = delete;
		SendItemEX(int id);
		SendItemEX(int id, TcpSocketEX* sock, ISend* item);
		virtual ~SendItemEX();
	public:
		virtual int SendMsg(PACKET pack);
		void SettingItem(TcpSocketEX* sock, ISend* item);

		virtual int Process(const int completionBytes, LPOVERLAPPED other);
	};
}