#ifndef _PX_COMMON_DEF_H
#define _PX_COMMON_DEF_H

#include <queue>
#include <vector>
using namespace std;

#include "Windows.h"
#include "pxBufferPool.h"
#include "pxErrorDef.h"

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

extern int gettimeofday(struct timeval*, int*);

extern UINT64 GetCurrentTimestamp();

#define SAVE_BMP  0
#define SAVE_YUV  0
#define SAVE_H264 1
#define SAVE_H264_FROM_BUFFERLIST 0
#define TIME_ANALYZE_VIDEO 0

#define AV_RB24(x)  ((((uint8_t*)(x))[0] << 16) | \
	(((uint8_t*)(x))[1] <<  8) | \
	((uint8_t*)(x))[2])
#define AV_RB32(x)  ((((uint8_t*)(x))[0] << 24) | \
	(((uint8_t*)(x))[1] << 16) | \
	(((uint8_t*)(x))[2] <<  8) | \
	((uint8_t*)(x))[3])

#endif