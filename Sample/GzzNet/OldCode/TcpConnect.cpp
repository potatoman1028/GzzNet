#include "TcpConnect.h"

int gzz::TcpConnect::Connect(const SOCKET sock, const SOCKADDR_IN addr)
{
	/// <summary>
	/// [2020-11-03 ��ȿ��]
	/// TODO : ���߿� AcceptEX or ConnectEX �� �� �κп� �߰������ش�.
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
