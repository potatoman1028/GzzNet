#include "TcpListen.h"

gzz::TcpListen::TcpListen(unsigned short maxConnection, unsigned short port, HANDLE iocpHandle)
	:TcpSocketEX( 0), MaxConnect(maxConnection), Port(port)
{
	Open();
	SetSocketOption();
	Bind(this->Sock, iocpHandle);
	Listen();
}

gzz::TcpListen::~TcpListen()
{
	Release();
	ForceClose();
}

int gzz::TcpListen::SetSocketOption()
{
	BOOL _on = TRUE;

	// AcceptEx ������ Reuse �ϱ����� �ɼ��Դϴ�. 

	// Listen Socket �� ����ߴ� ��Ʈ�� �ٽ� ����� �� �ֵ��� ���ִ� �ɼ��Դϴ�.
	if (setsockopt(Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&_on, sizeof(_on)) == SOCKET_ERROR)
	{
		return static_cast<int>(NET_ERR::SET_SOCKET_RUSUEADDR_FAIL);
	}

	// AcceptEx�� �����Ǵ� ���Ϻ��� ���� ���� Ŀ�ؼ��� �����°��� �����մϴ�.
	if (setsockopt(Sock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&_on, sizeof(_on)) != 0)
	{
		return static_cast<int>(NET_ERR::SET_SOCKET_CONDITIONAL_ACCEPT_FAIL);
	}
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpListen::Listen()
{
	if (listen(Sock, MaxConnect) == SOCKET_ERROR)
	{
		return static_cast<int>(NET_ERR::LISTEN_FAIL);
	}
	
	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpListen::Bind(SOCKET sock, HANDLE iocpHandle)
{
	Addr.sin_family = AF_INET;
	Addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	Addr.sin_port = htons(Port);

	int result = bind(Sock, (SOCKADDR*)&Addr, sizeof(Addr));

	if (result == SOCKET_ERROR)
	{
		return static_cast<int>(NET_ERR::BIND_FAIL);
	}

	if (CreateIoCompletionPort((HANDLE)Sock, iocpHandle, 0, 0) == NULL)
	{
		return static_cast<int>(NET_ERR::COMPLETIONPORT_BINDING_FAIL);
	}

	return static_cast<int>(NET_ERR::SUCESS);
}

int gzz::TcpListen::Close(LPOVERLAPPED overlapped)
{
	return TcpSocketEX::ForceClose();
}

void gzz::TcpListen::Release()
{
}

