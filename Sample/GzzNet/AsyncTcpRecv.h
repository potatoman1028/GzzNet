#pragma once

#include "NetDefine.h"
#include "InterfacesHead.h"

namespace gzz
{
	class AsyncTcpRecv : public IReceive
	{
	private:
		char* RecvBuf;
		int	RecvPosition;

		WSABUF	Wsabuf;

	public:
		AsyncTcpRecv();
		virtual ~AsyncTcpRecv();

	public:
		virtual int Receive(const SOCKET& sock, const LPOVERLAPPED& overraped);
		virtual char* GetBuf() { return RecvBuf; }
		virtual void Release();

	private:
		int RecvIO(const SOCKET& sock, const LPOVERLAPPED& overraped);
	};
}