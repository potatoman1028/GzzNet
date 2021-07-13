#include "IoDefine.h"
#include "SendItem.h"
#include "AsyncTcpSend.h"

gzz::SendItemEX::SendItemEX(int id)
	:ICompletionItemEX(id), Socket(nullptr), Send(nullptr)
{
}

gzz::SendItemEX::SendItemEX(int id, TcpSocketEX* sock, ISend* item)
	: ICompletionItemEX(id), Socket(sock), Send(item)
{
}

gzz::SendItemEX::~SendItemEX()
{
}

int gzz::SendItemEX::SendMsg(PACKET pack)
{
	return Send->SendMsg(Socket->GetSock(), static_cast<OVERLAPPED*>(this), pack);
}

int gzz::SendItemEX::Process(const int completionBytes, LPOVERLAPPED other)
{
	if (completionBytes <= 0)
	{
		Socket->Close(other);
	}
	else
	{
		int err = Send->SendProcess(Socket->GetSock(), completionBytes, static_cast<OVERLAPPED*>(this));

		switch (static_cast<NET_ERR>(err))
		{
		case gzz::NET_ERR::SUCESS:
			break;
		case gzz::NET_ERR::SEND_ERROR_NEED_MORE_DATA:
			break;
		case gzz::NET_ERR::SEND_ERROR_MORE_DATA:
			Socket->Close(other);
			break;
		}
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

void gzz::SendItemEX::SettingItem(TcpSocketEX* sock, ISend* item)
{
	Socket = sock;
	Send = item;
}
