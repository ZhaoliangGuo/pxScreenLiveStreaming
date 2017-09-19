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

	//SPxBuffer sPxBuffer = m_qBufferList.front();

	::LeaveCriticalSection(&m_csBufferList);

	return &m_qBufferList.front();
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
	::EnterCriticalSection(&m_csBufferList);

	int nSize = m_qBufferList.size();

	::LeaveCriticalSection(&m_csBufferList);

	return nSize;
}

bool CPxQueueBuffer::IsEmpty()
{
	::EnterCriticalSection(&m_csBufferList);

	bool bIsEmpty = m_qBufferList.empty();

	::LeaveCriticalSection(&m_csBufferList);

	return bIsEmpty;
}
