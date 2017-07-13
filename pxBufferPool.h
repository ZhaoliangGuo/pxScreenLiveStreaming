#pragma once

#define DEFAULT_BUFFER_LIST_LEN (200)
#define DEFAULT_BUFFER_SIZE  (1024*1024)

typedef enum EPxMediaType
{
	kePxMediaType_Invalid = -1,
	kePxMediaType_Video,
	kePxMediaType_Audio,
	kePxMediaType_Cnt
}EPxMediaType;

typedef unsigned char * LPBYTE;
typedef unsigned char uint8_t;

struct SPxBuffer
{
	EPxMediaType eMediaType;
	LPBYTE       lpBuffer;
	int          nDataLength;
	timeval      tvTimestamp;
	bool         bVideoKeyFrame;

	SPxBuffer()
	{
		eMediaType          = kePxMediaType_Invalid;
		lpBuffer            = NULL;
		nDataLength         = 0;
		tvTimestamp.tv_sec  = 0;
		tvTimestamp.tv_usec = 0;
		bVideoKeyFrame      = false;
	}

	~SPxBuffer()
	{
		eMediaType          = kePxMediaType_Invalid;
		lpBuffer            = NULL;
		nDataLength         = 0;
		tvTimestamp.tv_sec  = 0;
		tvTimestamp.tv_usec = 0;
		bVideoKeyFrame      = false;
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
	/*void SetBufferAt(int in_nPos, 
		             EPxMediaType in_keMediaType, 
					 uint8_t *in_ui8Data, 
					 int in_nDataLength, 
					 timeval in_tvTimestamp);*/
	void SetBufferAt(int in_nPos, SPxBuffer *in_psBuffer );
public:
	int m_nBufferListLen;
	int m_nCurPos;// 指向下一个空闲的buffer的下标
};