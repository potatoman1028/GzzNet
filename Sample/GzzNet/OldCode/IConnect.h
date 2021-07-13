#pragma once
#include "NetDefine.h"

namespace gzz
{
	class IConnect abstract
	{
	public:
		IConnect() = default;
		virtual ~IConnect() = default;

	public:
		virtual int Connect(const SOCKET sock, const SOCKADDR_IN addr) abstract;
		virtual int BindCompletionPort(SOCKET sock, HANDLE iocpHandle, ULONG_PTR key) abstract;
	};
}