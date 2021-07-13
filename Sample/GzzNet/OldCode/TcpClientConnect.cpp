#include "TcpClientConnect.h"

int gzz::TcpClientConnect::Connect(const SOCKET sock, const SOCKADDR_IN addr)
{
	int result = WSAConnect(sock, (SOCKADDR*)(&addr), sizeof(addr), NULL, NULL, NULL, NULL);
	if (result == SOCKET_ERROR)
	{
		return static_cast<int>(NET_ERR::CONNECT_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpClientConnect::BindCompletionPort(SOCKET sock, HANDLE iocpHandle, ULONG_PTR key)
{
	HANDLE _handle = CreateIoCompletionPort((HANDLE)sock, iocpHandle, key, 0);
	if (_handle == INVALID_HANDLE_VALUE)
	{
		return static_cast<int>(NET_ERR::COMPLETIONPORT_BINDING_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}