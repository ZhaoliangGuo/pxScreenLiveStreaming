#ifndef _PX_BUFFER_DEF_H
#define _PX_BUFFER_DEF_H

typedef enum EPxMediaType
{
	kePxMediaType_Invalid = -1,
	kePxMediaType_Video,
	kePxMediaType_Audio,
	kePxMediaType_Cnt
}EPxMediaType;

typedef unsigned char *LPBYTE;
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
		Flush();
	}

	~SPxBuffer()
	{
		Flush();
	}

	void Flush()
	{
		eMediaType          = kePxMediaType_Invalid;
		lpBuffer            = NULL;
		nDataLength         = 0;
		tvTimestamp.tv_sec  = 0;
		tvTimestamp.tv_usec = 0;
		bVideoKeyFrame      = false;
	}
};

#endif