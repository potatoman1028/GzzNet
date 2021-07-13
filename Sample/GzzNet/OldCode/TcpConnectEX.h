#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"

namespace gzz
{
	class TcpConnectEX : public
		IConnect
	{
	public:
		TcpConnectEX();
		virtual ~TcpConnectEX();

	public:
		virtual int Connect(const SOCKET sock, const SOCKADDR_IN addr);
		virtual int BindCompletionPort(SOCKET sock, HANDLE iocpHandle, ULONG_PTR key);
	};
}
