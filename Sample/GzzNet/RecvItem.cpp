#include "RecvItem.h"

gzz::RecvItemEX::RecvItemEX(int id)
	: ICompletionItemEX(id), Socket(nullptr), Recv(nullptr), Queue(nullptr)
{
	Buf = new RingBuffer(MAX_RECV_BUF_SIZE * 2);
}

gzz::RecvItemEX::RecvItemEX(int id, TcpSocketEX* sock, IReceive* item, JobQueue<PACKAGE>* queue)
	: ICompletionItemEX(id), Socket(sock), Recv(item), Queue(queue)
{
	Buf = new RingBuffer(MAX_RECV_BUF_SIZE * 2);
}

gzz::RecvItemEX::~RecvItemEX()
{
	if (Buf)
	{
		delete Buf;
	}
}

void gzz::RecvItemEX::SettingItem(TcpSocketEX* sock, IReceive* item, JobQueue<PACKAGE>* queue)
{
	Socket = sock;
	SockId = Socket->GetId();
	Recv = item;
	Queue = queue;
}

void gzz::RecvItemEX::AddMessage(PACKAGE pack)
{
	Queue->Enqueue(pack);
}

int gzz::RecvItemEX::Process(const int completionBytes, LPOVERLAPPED other)
{
	if (completionBytes <= 0)
	{
		return 	Socket->Close(other);
	}

	int size = completionBytes;
	int readbytes = 0;
	bool IsPackage = true;
	char* buf = Recv->GetBuf();
	PACKET packet;
	unsigned short packet_size = 0;
	int ret = 0;

	while (size > 0)
	{
		 ret = Buf->Push(buf + ret, completionBytes);
		// 링버퍼의 최대치를 넘은 경우.
		if (ret > -1)
		{
			if (ret == 0)
			{
				// 링버퍼가 꽉차고 처리도 못했다는 판단, 링버퍼의 크기보다 패킷의 크기가 큰 경우 생길 가능성이 있다.
				// 현재는 Close 시킴.
				Socket->Close(other);
			}
			else
			{
				ret = Buf->Push(buf, ret);
			}
			size -= ret;
		}
		else
		{
			size = 0;
		}

		while (true)
		{
			if (Buf->CurrentSize() < PACKET_HEADER_SIZE)
			{
				break;
			}

			Buf->Get(reinterpret_cast<char*>(&packet_size), sizeof(unsigned short));

			if (Buf->CurrentSize() < packet_size)
			{
				break;
			}
			else
			{
				Buf->Pop(packet.memory, packet_size);
				PACKAGE pack(Socket->GetId(), packet);
				Queue->Enqueue(pack);
			}
		}
	}

	Recv->Receive(Socket->GetSock(), static_cast<LPOVERLAPPED>(this));
	return 0;
}
