#include "StdAfx.h"
#include "pxBufferPool.h"
#include <vector>
using namespace std;

extern vector <SPxBuffer> g_vlpBufferPool;

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

		g_vlpBufferPool.push_back(sPxBuffer);
	}

	m_nBufferListLen = g_vlpBufferPool.size();

	::LeaveCriticalSection(&g_csBufferPool);  

	return g_vlpBufferPool.size();
}

void SPxBufferPool::ReleaseBufferPool()
{
	::EnterCriticalSection(&g_csBufferPool);
	SPxBuffer sPxBuffer;

	while (!g_vlpBufferPool.empty())
	{
		sPxBuffer = g_vlpBufferPool.back();
		VirtualFree(sPxBuffer.lpBuffer, 0, MEM_RELEASE);
		sPxBuffer.lpBuffer = NULL;

		g_vlpBufferPool.pop_back();
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
								EPxMediaType in_keMediaType, 
								uint8_t *in_ui8Data, 
								int in_nDataLength, 
								unsigned int uiTimestamp )
{
	::EnterCriticalSection(&g_csBufferPool);

	g_vlpBufferPool[in_nPos].eMediaType  = in_keMediaType;
	memcpy(g_vlpBufferPool[in_nPos].lpBuffer, in_ui8Data, in_nDataLength);
	g_vlpBufferPool[in_nPos].nDataLength = in_nDataLength;
	g_vlpBufferPool[in_nPos].uiTimestamp = uiTimestamp;

	::LeaveCriticalSection(&g_csBufferPool); 
}
