#include "StdAfx.h"
#include "pxBufferPool.h"
#include <vector>
using namespace std;

extern vector <SPxBuffer> g_vlpBufferPool;

SPxBufferPool::SPxBufferPool(void)
{
	m_nCurPos = 0;
}


SPxBufferPool::~SPxBufferPool(void)
{
	m_nCurPos = 0;
}

int SPxBufferPool::InitBufferPool(int in_nBufferSize, int in_nBufferPoolLen)
{
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

	return g_vlpBufferPool.size();
}

void SPxBufferPool::ReleaseBufferPool()
{
	SPxBuffer sPxBuffer;

	while (!g_vlpBufferPool.empty())
	{
		sPxBuffer = g_vlpBufferPool.back();
		VirtualFree(sPxBuffer.lpBuffer, 0, MEM_RELEASE);
		sPxBuffer.lpBuffer = NULL;

		g_vlpBufferPool.pop_back();
	}
}

int SPxBufferPool::GetEmptyBufferPos()
{
	int nPos = 0;

	// »·ÐÎÄÚ´æ
	if (DEFAULT_BUFFER_LIST_LEN == m_nCurPos)
	{
		m_nCurPos = 0;

		nPos = m_nCurPos;

		return nPos;
	}

	nPos = m_nCurPos;
	m_nCurPos++;

	return nPos;
}
