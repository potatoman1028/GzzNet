#include "NetDefine.h"
#include "InterfacesHead.h"

namespace gzz
{
	class TcpClientConnect : public IConnect
	{
	public:
		TcpClientConnect() = default;
		virtual ~TcpClientConnect() = default;

	public:
		virtual int Connect(const SOCKET sock, const SOCKADDR_IN addr);
		virtual int BindCompletionPort(const SOCKET sock, const HANDLE iocpHandle, ULONG_PTR key);
	};
}