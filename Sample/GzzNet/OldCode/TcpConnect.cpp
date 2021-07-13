#include "TcpConnect.h"

int gzz::TcpConnect::Connect(const SOCKET sock, const SOCKADDR_IN addr)
{
	/// <summary>
	/// [2020-11-03 김효곤]
	/// TODO : 나중에 AcceptEX or ConnectEX 를 이 부분에 추가시켜준다.
	/// </summary>
	/// <returns></returns>
	/// 
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpConnect::BindCompletionPort(SOCKET sock, HANDLE iocpHandle, ULONG_PTR key)
{
	HANDLE _handle = CreateIoCompletionPort((HANDLE)sock, iocpHandle, key, 0);
	if (_handle == INVALID_HANDLE_VALUE)
	{
		return static_cast<int>(NET_ERR::COMPLETIONPORT_BINDING_FAIL);
	}	
	return static_cast<int>(NET_ERR::SUCESS);
}
