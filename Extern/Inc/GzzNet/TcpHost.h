#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"
#include "TcpSocketEX.h"
#include "AsyncTcpSend.h"
#include "AsyncTcpRecv.h"

namespace gzz
{
	class TcpHost :
		public TcpSocketEX,
		public AsyncTcpRecv,
		public AsyncTcpSend
	{
	public:
		TcpHost() = delete;
		TcpHost(int id, SOCKET listen, HANDLE iocpHandle, LPOVERLAPPED overLapped);
		virtual ~TcpHost();

	public:
		virtual int Open();
		virtual int Bind(SOCKET sock, HANDLE iocpHandle);
		virtual void Release();

	public:
		virtual ISend* GetInterfaceSend() override;
		virtual IReceive* GetInterfaceRecv() override;

	public:
		int Accept(SOCKET listen, LPOVERLAPPED overLapped);
		int PostAcception();
		int SetSocketOption(SOCKET listen);
	private:
	};
}