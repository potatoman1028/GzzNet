#pragma once
#include "NetDefine.h"
#include "InterfacesHead.h"

namespace gzz
{
	class TcpSocket : public ISocket
	{
	private:
		int Id;
	public:
		TcpSocket() = delete;
		TcpSocket(int id) { Id = id; }
		virtual ~TcpSocket();

	public:
		virtual int Create();
		virtual int Open(SOCKET sock);
		virtual int Bind(const char* ip, const short port);

		virtual int SetSocketOption();
		virtual int Close();
		virtual int ForceClose();
		virtual void Release() abstract;

		int GetID() { return Id; }

	private:
	};
}
