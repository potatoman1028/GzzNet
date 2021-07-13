#include "TcpHost.h"

gzz::TcpHost::TcpHost()
	:ISocket()
{
}

gzz::TcpHost::~TcpHost()
{
	Close();
}

int gzz::TcpHost::Open(SOCKET sock)
{
	Sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (Sock == INVALID_SOCKET)
	{
		return static_cast<int>(NET_ERR::SOCKET_CREATE_FAIL);
	}

	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpHost::Bind(const char* ip, const short port)
{
	Addr.sin_family = AF_INET;
	Addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	Addr.sin_port = htons(port);

	int result = bind(Sock, (SOCKADDR*)&Addr, sizeof(Addr));
	if (result == SOCKET_ERROR)
	{
		return static_cast<int>(NET_ERR::BIND_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpHost::Close()
{
	closesocket(Sock);
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpHost::ForceClose()
{
	return Close();
}

int gzz::TcpHost::SetSocketOption()
{
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpHost::Listen()
{
	if (listen(Sock, SOMAXCONN) == SOCKET_ERROR)
	{
		return static_cast<int>(NET_ERR::LISTEN_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

SOCKET gzz::TcpHost::Accept()
{
	sockaddr_in addr;
	int			addrlen = sizeof(addr);
	SOCKET result;
	result = accept(Sock, (SOCKADDR*)&addr, &addrlen);
	return result;
}
