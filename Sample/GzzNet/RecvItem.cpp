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
		// �������� �ִ�ġ�� ���� ���.
		if (ret > -1)
		{
			if (ret == 0)
			{
				// �����۰� ������ ó���� ���ߴٴ� �Ǵ�, �������� ũ�⺸�� ��Ŷ�� ũ�Ⱑ ū ��� ���� ���ɼ��� �ִ�.
				// ����� Close ��Ŵ.
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
