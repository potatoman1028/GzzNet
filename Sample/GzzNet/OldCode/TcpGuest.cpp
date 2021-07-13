#include "TcpGuest.h"

gzz::TcpGuest::TcpGuest(int id)
	:TcpSocket(id)
{
}

void gzz::TcpGuest::Release()
{
	AsyncTcpRecv::Release();
	AsyncTcpSend::Release();
}

int gzz::TcpGuest::SetSocketOption()
{
	return static_cast<int>(NET_ERR::SUCESS);
}
