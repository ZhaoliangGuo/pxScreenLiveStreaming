#pragma once

#define DEFAULT_BUFFER_LIST_LEN (200)

typedef unsigned char * LPBYTE;
struct SPxBuffer
{
	LPBYTE lpBuffer;
	int    nDataLength;

	SPxBuffer()
	{
		lpBuffer    = NULL;
		nDataLength = 0;
	}

	~SPxBuffer()
	{
		lpBuffer    = NULL;
		nDataLength = 0;
	}
};

class SPxBufferPool
{
public:
	SPxBufferPool(void);
	~SPxBufferPool(void);

public:
	int InitBufferPool(int in_nBufferSize,  
		               int in_nBufferPoolLen = DEFAULT_BUFFER_LIST_LEN);

	void ReleaseBufferPool();

	int GetEmptyBufferPos();

public:
	int m_nBufferListLen;
	int m_nCurPos;// 指向下一个空闲的buffer的下标
};