#include "StdAfx.h"
#include "pxQueueBuffer.h"

CPxQueueBuffer::CPxQueueBuffer(void)
{
	::InitializeCriticalSection(&m_csBufferList);
}

CPxQueueBuffer::~CPxQueueBuffer(void)
{
	::DeleteCriticalSection(&m_csBufferList);
}

void CPxQueueBuffer::Push(SPxBuffer *in_psBuffer)
{
	::EnterCriticalSection(&m_csBufferList);
	if (NULL == in_psBuffer)
	{
		::LeaveCriticalSection(&m_csBufferList);

		return ;
	}

	m_qBufferList.push(*in_psBuffer);

	::LeaveCriticalSection(&m_csBufferList);
}

SPxBuffer *CPxQueueBuffer::Front()
{
	::EnterCriticalSection(&m_csBufferList);

	if (m_qBufferList.size() == 0)
	{
		::LeaveCriticalSection(&m_csBufferList);

		return NULL;
	}

	SPxBuffer *psPxBuffer = &m_qBufferList.front();

	::LeaveCriticalSection(&m_csBufferList);

	return psPxBuffer;
}

void CPxQueueBuffer::Pop()
{
	::EnterCriticalSection(&m_csBufferList);
	if (0 == m_qBufferList.size())
	{
		::LeaveCriticalSection(&m_csBufferList);

		return ;
	}

	m_qBufferList.pop();

	::LeaveCriticalSection(&m_csBufferList);
}

int CPxQueueBuffer::Size()
{
	return m_qBufferList.size();
}

bool CPxQueueBuffer::IsEmpty()
{
	return m_qBufferList.empty();
}
