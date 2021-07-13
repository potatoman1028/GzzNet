#include "AsyncTcpRecv.h"

gzz::AsyncTcpRecv::AsyncTcpRecv()
	:RecvPosition(0)
{
	RecvBuf = new char[MAX_RECV_BUF_SIZE];
	ZeroMemory(RecvBuf, MAX_RECV_BUF_SIZE);

	Wsabuf.buf = nullptr;
	Wsabuf.len = 0;
}

gzz::AsyncTcpRecv::~AsyncTcpRecv()
{
	delete[] RecvBuf;
}

int gzz::AsyncTcpRecv::Receive(const SOCKET& sock, const LPOVERLAPPED& overraped)
{
	return RecvIO(sock, overraped);
}

void gzz::AsyncTcpRecv::Release()
{
	ZeroMemory(RecvBuf, sizeof(MAX_RECV_BUF_SIZE));
	RecvPosition = 0;
}

int gzz::AsyncTcpRecv::RecvIO(const SOCKET& sock, const LPOVERLAPPED& overraped)
{
	DWORD _bytes = 0;
	DWORD _flag = 0;

	ZeroMemory(overraped, sizeof(OVERLAPPED));
	Wsabuf.buf = RecvBuf;
	Wsabuf.len = MAX_RECV_BUF_SIZE;

	int _result = WSARecv(sock,
		&Wsabuf,
		1,
		&_bytes,
		&_flag,
		overraped,
		nullptr);

	if (_result == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		return static_cast<int>(NET_ERR::WSA_RECV_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

