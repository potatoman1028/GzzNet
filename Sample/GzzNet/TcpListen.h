#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"
#include "TcpSocketEX.h"

namespace gzz
{
	class TcpListen : public TcpSocketEX
	{
	private:
		unsigned short		MaxConnect;
		unsigned short		Port;

	public:
		TcpListen() = delete;
		TcpListen(unsigned short maxConnection, unsigned short port, HANDLE iocpHandle);
		virtual ~TcpListen();

	public:
		int SetSocketOption();
		virtual int Listen();
		virtual int Bind(SOCKET sock, HANDLE iocpHandle);

		virtual int Close(LPOVERLAPPED overlapped);
		virtual void Release();

		int GetSize() { return MaxConnect; }
	};
}