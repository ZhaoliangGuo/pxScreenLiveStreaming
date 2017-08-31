#include "StdAfx.h"
#include "pxBufferPool.h"
#include <vector>
using namespace std;

CPxBufferPool::CPxBufferPool(void)
{
	::InitializeCriticalSection(&m_csRingBuffer); 
	m_nCurPos = 0;
}

CPxBufferPool::~CPxBufferPool(void)
{
	::DeleteCriticalSection(&m_csRingBuffer);    //释放里临界区
	m_nCurPos = 0;
}

int CPxBufferPool::Init(int in_nBufferSize, int in_nBufferPoolLen)
{
	::EnterCriticalSection(&m_csRingBuffer); 

	SPxBuffer sPxBuffer;
	
	for (int i = 0; i < in_nBufferPoolLen; i++)
	{
		sPxBuffer.lpBuffer = (LPBYTE)VirtualAlloc( NULL ,
			in_nBufferSize, 
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE );

		if(NULL == sPxBuffer.lpBuffer)
		{
			/*DWORD dwError = GetLastError() ;
			HRESULT_FROM_WIN32( dwError ) ;*/

			break;
		}

		m_vRingBuffer.push_back(sPxBuffer);
	}

	m_nBufferListLen = m_vRingBuffer.size();

	::LeaveCriticalSection(&m_csRingBuffer);  

	return m_vRingBuffer.size();
}

void CPxBufferPool::Release()
{
	::EnterCriticalSection(&m_csRingBuffer);
	SPxBuffer sPxBuffer;

	while (!m_vRingBuffer.empty())
	{
		sPxBuffer = m_vRingBuffer.back();
		VirtualFree(sPxBuffer.lpBuffer, 0, MEM_RELEASE);
		sPxBuffer.lpBuffer = NULL;

		m_vRingBuffer.pop_back();
	}

	::LeaveCriticalSection(&m_csRingBuffer);  
}

int CPxBufferPool::GetEmptyBufferPos()
{
	::EnterCriticalSection(&m_csRingBuffer);

	int nPos = 0;

	// 环形内存
	if ((m_nBufferListLen - 1) == m_nCurPos)
	{
		m_nCurPos = 0;

		nPos = m_nCurPos;

		::LeaveCriticalSection(&m_csRingBuffer); 

		return nPos;
	}

	nPos = m_nCurPos;

	m_nCurPos++;

	::LeaveCriticalSection(&m_csRingBuffer); 

	return nPos;
}

void CPxBufferPool::SetBufferAt(int in_nPos, SPxBuffer *in_psBuffer )
{
	::EnterCriticalSection(&m_csRingBuffer);

	m_vRingBuffer[in_nPos].eMediaType     = in_psBuffer->eMediaType;
	memcpy(m_vRingBuffer[in_nPos].lpBuffer, in_psBuffer->lpBuffer, in_psBuffer->nDataLength);
	m_vRingBuffer[in_nPos].nDataLength    = in_psBuffer->nDataLength;
	m_vRingBuffer[in_nPos].tvTimestamp    = in_psBuffer->tvTimestamp;
	m_vRingBuffer[in_nPos].bVideoKeyFrame = in_psBuffer->bVideoKeyFrame;

	::LeaveCriticalSection(&m_csRingBuffer); 
}

SPxBuffer *CPxBufferPool::GetBufferAt(int in_nPos)
{
	::EnterCriticalSection(&m_csRingBuffer);

	if (kePxMediaType_Invalid == m_vRingBuffer[in_nPos].bVideoKeyFrame)
	{
		::LeaveCriticalSection(&m_csRingBuffer);

		return NULL;
	}

	SPxBuffer *psPxBuffer = &m_vRingBuffer[in_nPos];

	::LeaveCriticalSection(&m_csRingBuffer);

	return psPxBuffer;
}