#pragma once
#include <Windows.h>
#include <mutex>

// OVERLAPPED 를 상속받는 클래스들. Pool로 관리하기 편하다고 생각해서 만듦.

namespace gzz
{
	class ICompletionItem : public OVERLAPPED
	{
	private:
		int Id;
	public:
		ICompletionItem() { Id = -1; };
		virtual ~ICompletionItem() = default;

	public:
		int GetId() { return Id; }
		void SetId(const int id) { Id = id; }
		virtual int Process(const int completionBytes) abstract;
		virtual bool Switching() abstract;
	};

	class ICompletionItemEX : public OVERLAPPED
	{
	protected:
		int Id;
		int SockId = -1;

	public:
		ICompletionItemEX() = delete;
		ICompletionItemEX(int id) { Id = id; }
		virtual ~ICompletionItemEX() = default;

	public:
		int GetId() { return Id; }
		int GetSockId() { return SockId; }
		void SetId(const int id) { Id = id; }
		virtual int Process(const int completionBytes, LPOVERLAPPED other) abstract;
	};
}
