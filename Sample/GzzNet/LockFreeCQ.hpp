#pragma once
#include "CircularQueue.h"

template<typename Data>
inline LockFreeCQ<Data>::LockFreeCQ(const int bufsize) : Tail(0), Head(0), MaxSize(bufsize), DataBuffer(nullptr)
{
	DataBuffer = new Data[bufsize];
	memset(DataBuffer, 0, bufsize);
}

template<typename Data>
inline LockFreeCQ<Data>::~LockFreeCQ()
{
	if (DataBuffer)
	{
		delete[] DataBuffer;
	}
}

template<typename Data>
inline bool LockFreeCQ<Data>::Enqueue(Data& d)
{
	if (IsFull()) return false;

	DataBuffer[Tail] = d;
	Tail = NextIndex(Tail);
	return true;
}

template<typename Data>
inline bool LockFreeCQ<Data>::Dequeue(Data& d)
{
	if (IsEmpty()) return false;

	d = DataBuffer[Head];
	Head = NextIndex(Head);
	return true;
}

template<typename Data>
inline int LockFreeCQ<Data>::NextIndex(const int current_index)
{
	return (current_index + 1) % MaxSize;
}

template<typename Data>
inline bool LockFreeCQ<Data>::IsFull()
{
	return (NextIndex(Tail) == Head);
}

template<typename Data>
inline bool LockFreeCQ<Data>::IsEmpty()
{
	return (Head == Tail);
}

template<typename Data>
inline const int LockFreeCQ<Data>::GetTail()
{
	return Tail;
}

template<typename Data>
inline const int LockFreeCQ<Data>::GetHead()
{
	return Head;
}

template<typename Data>
inline const int LockFreeCQ<Data>::GetCurrentCount()
{
	return (Tail - Head + MaxSize) % MaxSize;
}

template<typename Data>
inline const int LockFreeCQ<Data>::GetMaxSize()
{
	return MaxSize;
}

template<typename Data>
inline const Data* LockFreeCQ<Data>::GetData()
{
	return DataBuffer;
}
