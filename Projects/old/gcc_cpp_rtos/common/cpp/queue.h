#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <common/commonHeader.h>
#include <common/cpp/nonCopyable.h>

namespace cpp_ns
{

template <typename ElementType, uint08 queueSize, ElementType emptyElem>
class Queue
	: private NonCopyable
{
public:
	Queue();
	~Queue();

public:
	inline bool isEmpty() const;
	inline bool isFull() const;

	inline void setTask(const ElementType& elem);
	inline ElementType getTask();

private:
	volatile ElementType m_queueBuffer[queueSize];
	uint08 m_begin;
	uint08 m_end;
	uint08 m_size;
};

} // namespace cpp_ns

// //////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////

template <typename ElementType, uint08 queueSize, ElementType emptyElem>
cpp_ns::Queue<ElementType, queueSize, emptyElem>::Queue()
	: m_begin(0)
	, m_end(0)
	, m_size(0)
{}

template <typename ElementType, uint08 queueSize, ElementType emptyElem>
cpp_ns::Queue<ElementType, queueSize, emptyElem>::~Queue() {}

template <typename ElementType, uint08 queueSize, ElementType emptyElem>
bool cpp_ns::Queue<ElementType, queueSize, emptyElem>::isEmpty() const
{
	return !m_size;
}

template <typename ElementType, uint08 queueSize, ElementType emptyElem>
bool cpp_ns::Queue<ElementType, queueSize, emptyElem>::isFull() const
{
	return (queueSize == m_size);
}

template <typename ElementType, uint08 queueSize, ElementType emptyElem>
void cpp_ns::Queue<ElementType, queueSize, emptyElem>::setTask(const ElementType& elem)
{
	if (isFull())
	{
		return;
	}

	++m_size;
	++m_begin;
	m_begin %= queueSize;
	m_queueBuffer[m_begin] = elem;
}

template <typename ElementType, uint08 queueSize, ElementType emptyElem>
ElementType cpp_ns::Queue<ElementType, queueSize, emptyElem>::getTask()
{
	if (isEmpty())
	{
		return emptyElem;
	}

	const ElementType retElem = m_queueBuffer[m_end++];
	m_end %= queueSize;
	--m_size;
	return retElem;
}

#endif // _QUEUE_H_
