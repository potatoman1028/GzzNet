#pragma once

#include "NetDefine.h"
#include "InterfacesHead.h"

#include "TcpSocket.h"
#include "AsyncTcpSend.h"
#include "AsyncTcpRecv.h"

namespace gzz
{
	class TcpGuest :
		public TcpSocket,
		public TcpConnect,
		public AsyncTcpRecv,
		public AsyncTcpSend
	{
	public:
		TcpGuest() = delete;
		TcpGuest(int id);
		virtual ~TcpGuest() = default;

	public:
		virtual void Release();
		virtual int SetSocketOption();
	};
}