#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"

namespace gzz
{
	class TcpConnect :
		public IConnect
	{
	public:
		TcpConnect() = default;
		virtual ~TcpConnect() = default;

	public:
		virtual int Connect(const SOCKET sock, const SOCKADDR_IN addr);
		virtual int BindCompletionPort(SOCKET sock, HANDLE iocpHandle, ULONG_PTR key);
	};
}