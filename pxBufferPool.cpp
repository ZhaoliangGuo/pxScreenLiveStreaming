#include "StdAfx.h"
#include "pxBufferPool.h"
#include <vector>
using namespace std;

extern vector <SPxBuffer> g_vCodedBufferPool;

SPxBufferPool::SPxBufferPool(void)
{
	::InitializeCriticalSection(&g_csBufferPool); 
	m_nCurPos = 0;
}


SPxBufferPool::~SPxBufferPool(void)
{
	::DeleteCriticalSection(&g_csBufferPool);    //释放里临界区
	m_nCurPos = 0;
}

int SPxBufferPool::InitBufferPool(int in_nBufferSize, int in_nBufferPoolLen)
{
	::EnterCriticalSection(&g_csBufferPool); 

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

		g_vCodedBufferPool.push_back(sPxBuffer);
	}

	m_nBufferListLen = g_vCodedBufferPool.size();

	::LeaveCriticalSection(&g_csBufferPool);  

	return g_vCodedBufferPool.size();
}

void SPxBufferPool::ReleaseBufferPool()
{
	::EnterCriticalSection(&g_csBufferPool);
	SPxBuffer sPxBuffer;

	while (!g_vCodedBufferPool.empty())
	{
		sPxBuffer = g_vCodedBufferPool.back();
		VirtualFree(sPxBuffer.lpBuffer, 0, MEM_RELEASE);
		sPxBuffer.lpBuffer = NULL;

		g_vCodedBufferPool.pop_back();
	}

	::LeaveCriticalSection(&g_csBufferPool);  
}

int SPxBufferPool::GetEmptyBufferPos()
{
	::EnterCriticalSection(&g_csBufferPool);

	int nPos = 0;

	// 环形内存
	if (DEFAULT_BUFFER_LIST_LEN == m_nCurPos)
	{
		m_nCurPos = 0;

		nPos = m_nCurPos;

		return nPos;
	}

	nPos = m_nCurPos;
	m_nCurPos++;

	::LeaveCriticalSection(&g_csBufferPool); 

	return nPos;
}

void SPxBufferPool::SetBufferAt(int in_nPos, 
								SPxBuffer *in_psBuffer )
{
	::EnterCriticalSection(&g_csBufferPool);

	g_vCodedBufferPool[in_nPos].eMediaType     = in_psBuffer->eMediaType;
	memcpy(g_vCodedBufferPool[in_nPos].lpBuffer, in_psBuffer->lpBuffer, in_psBuffer->nDataLength);
	g_vCodedBufferPool[in_nPos].nDataLength    = in_psBuffer->nDataLength;
	g_vCodedBufferPool[in_nPos].tvTimestamp    = in_psBuffer->tvTimestamp;
	g_vCodedBufferPool[in_nPos].bVideoKeyFrame = in_psBuffer->bVideoKeyFrame;

	::LeaveCriticalSection(&g_csBufferPool); 
}

//void SPxBufferPool::SetBufferAt(int in_nPos, 
//	EPxMediaType in_keMediaType, 
//	uint8_t *in_ui8Data, 
//	int in_nDataLength, 
//	timeval in_tvTimestamp )
//{
//	::EnterCriticalSection(&g_csBufferPool);
//
//	g_vCodedBufferPool[in_nPos].eMediaType  = in_keMediaType;
//	memcpy(g_vCodedBufferPool[in_nPos].lpBuffer, in_ui8Data, in_nDataLength);
//	g_vCodedBufferPool[in_nPos].nDataLength = in_nDataLength;
//	g_vCodedBufferPool[in_nPos].tvTimestamp = in_tvTimestamp;
//
//	::LeaveCriticalSection(&g_csBufferPool); 
//}
