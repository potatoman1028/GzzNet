#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"

namespace gzz
{
	class TcpHost : public ISocket
	{
	public:
		TcpHost();
		virtual ~TcpHost();

	public:
		virtual int Open(SOCKET sock);
		virtual int Bind(const char* ip, const short port);
		virtual int Close();
		virtual int ForceClose();
		virtual void Release() {};
		virtual int SetSocketOption();
		virtual int Listen();

		// 동기 Accept()
		SOCKET Accept();
		// 비동기 AcceptEX()
	};
}