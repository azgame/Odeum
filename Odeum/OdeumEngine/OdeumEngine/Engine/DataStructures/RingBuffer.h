#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "../../pch.h"
#include <queue>

template <typename T>
class RingBuffer
{
public:

	RingBuffer(size_t Size)
	{
		m_ringBuffer = std::vector<T>(Size);
		m_ringLimit = Size;
		m_head = m_tail = 0;
	}

	bool push_back(T Element)
	{
		bool result = false;
		std::lock_guard<std::mutex> lg(sm_mutex);

		if ((m_head + 1) % m_ringLimit != m_tail)
		{
			m_ringBuffer[m_head] = Element;
			m_head = (m_head + 1) % m_ringLimit;

			result = true;
		}

		return result;
	}

	bool pop_front(T& Element)
	{
		bool result = false;
		std::lock_guard<std::mutex> lg(sm_mutex);

		if (m_tail != m_head)
		{
			Element = m_ringBuffer[m_tail];
			m_head = (m_tail + 1) % m_ringLimit;

			result = true;
		}

		return result;
	}

	bool empty()
	{
		return m_tail == m_head;
	}

private:

	std::vector<T> m_ringBuffer;
	uint32_t m_head, m_tail;
	uint32_t m_ringLimit;
};



#endif