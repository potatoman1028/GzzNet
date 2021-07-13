#include "TcpSocket.h"

gzz::TcpSocket::~TcpSocket()
{
}

int gzz::TcpSocket::Create()
{
	Sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (Sock == INVALID_SOCKET)
	{
		return static_cast<int>(NET_ERR::SOCKET_CREATE_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpSocket::Open(SOCKET sock)
{
	Sock = sock;
	if (Sock != INVALID_SOCKET)
	{
		char Ip[32];

		int _len = sizeof(Addr);
		getpeername(Sock, (SOCKADDR*)&Addr, &_len);
		inet_ntop(AF_INET, &Addr.sin_addr, Ip, sizeof(Ip));

		SetIP(Ip);
		SetPort(ntohs(Addr.sin_port));
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpSocket::Bind(const char* ip, const short port)
{
	SetIP(ip);
	SetPort(port);

	Addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &Addr.sin_addr);
	Addr.sin_port = htons(port);
	int result = bind(Sock, (SOCKADDR*)&Addr, sizeof(Addr));

	if (result == SOCKET_ERROR)
	{
		return static_cast<int>(NET_ERR::BIND_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpSocket::SetSocketOption()
{
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpSocket::Close()
{
	Release();
	shutdown(Sock, SD_BOTH);
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpSocket::ForceClose()
{
	Release();

	// https://chfhrqnfrhc.tistory.com/entry/%EC%86%8C%EC%BC%93-%EC%A2%85%EB%A3%8C 테스트 해볼 것.
	LINGER  ling = { 0, };
	ling.l_onoff = 1;
	ling.l_linger = 0;

	setsockopt(Sock, SOL_SOCKET, SO_LINGER, (CHAR*)&ling, sizeof(ling));
	closesocket(Sock);
	Sock = -1;
	return static_cast<int>(NET_ERR::SUCESS);
}
