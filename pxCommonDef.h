#ifndef _PX_COMMON_DEF_H
#define _PX_COMMON_DEF_H

#include <queue>
#include <vector>
using namespace std;

#include "Windows.h"
#include "pxBufferPool.h"
#include "pxErrorDef.h"

// 音频采集
#include <mmsystem.h>

// 定义内存池
extern struct SPxBuffer;

// YUV
extern vector <SPxBuffer> g_vYUVBufferPool; // YUV内存池
extern queue  <SPxBuffer> g_qYUVBufferList; // 存储采集的YUV数据
extern SPxBufferPool      g_oYUVBufferPool;

// H.264/AAC
extern vector <SPxBuffer> g_vCodedBufferPool; // 存储编码后数据的内存池(H.264/AAC)
extern SPxBufferPool      g_oCodedBufferPool;
extern queue <SPxBuffer>  g_qCodedBufferList; // 存储编码后数据的队列
extern CRITICAL_SECTION   g_csBufferPool;

extern int gettimeofday(struct timeval*, int*);

extern UINT64 GetCurrentTimestamp();

#define VIDEO_SAVE_BMP  0
#define VIDEO_SAVE_YUV  0
#define VIDEO_SAVE_H264 0
#define VIDEO_SAVE_H264_FROM_BUFFERLIST 0
#define VIDEO_TIME_ANALYZE 0

#define DEBUG_CAPTURE_PCM 1
#define AUDIO_SAVE_PCM 1
#define AUDIO_SAVE_AAC 0
#define AUDIO_SAVE_AAC_FROM_BUFFERLIST 0

#define AV_RB24(x)  ((((uint8_t*)(x))[0] << 16) | \
	(((uint8_t*)(x))[1] <<  8) | \
	((uint8_t*)(x))[2])
#define AV_RB32(x)  ((((uint8_t*)(x))[0] << 24) | \
	(((uint8_t*)(x))[1] << 16) | \
	(((uint8_t*)(x))[2] <<  8) | \
	((uint8_t*)(x))[3])

#endif