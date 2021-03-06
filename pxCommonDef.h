#ifndef _PX_COMMON_DEF_H
#define _PX_COMMON_DEF_H

#include <queue>
#include <vector>
using namespace std;

#include <assert.h> 

#include "Windows.h"
#include "pxBufferPool.h"
#include "pxQueueBuffer.h"
#include "pxErrorDef.h"
#include "pxBufferDef.h"


// 音频采集
#include <mmsystem.h>

// 定义内存池

// YUV
extern CPxBufferPool      g_oYUVBufferPool;      // YUV内存池
extern CPxQueueBuffer     g_oYUVQueueBuffer; // 存储采集的YUV数据

// PCM
extern CPxBufferPool      g_oPCMBufferPool;      // PCM内存池
extern CPxQueueBuffer     g_oPCMQueueBuffer; // 存储采集的PCM数据

// H.264/AAC
extern CPxBufferPool      g_oCodedBufferPool;    // 存储编码后数据的内存池(H.264/AAC)
extern CPxQueueBuffer     g_oCodedQueueBuffer;   // 存储编码后数据的队列

extern int gettimeofday(struct timeval*, int*);

extern UINT64 GetCurrentTimestamp();

#define VIDEO_SAVE_BMP  0
#define VIDEO_SAVE_YUV  0
#define VIDEO_SAVE_YUV_FROM_BUFFERLIST 0
#define VIDEO_SAVE_H264 0
#define VIDEO_SAVE_H264_FROM_BUFFERLIST 0
#define DEBUG_VIDEO_TIME_ANALYZE 0

#define DEBUG_AUDIO_CAPTURE_PCM 0
#define AUDIO_SAVE_PCM 0
#define AUDIO_SAVE_PCM_FROM_BUFFERLIST 0
#define AUDIO_SAVE_AAC 0
#define AUDIO_SAVE_AAC_FROM_BUFFERLIST 0

#define AV_RB24(x)  ((((uint8_t*)(x))[0] << 16) | \
	(((uint8_t*)(x))[1] <<  8) | \
	((uint8_t*)(x))[2])
#define AV_RB32(x)  ((((uint8_t*)(x))[0] << 24) | \
	(((uint8_t*)(x))[1] << 16) | \
	(((uint8_t*)(x))[2] <<  8) | \
	((uint8_t*)(x))[3])

#define DEFAULT_VIDEO_FRAMERATE (25)
#define AUDIO_SAMPLE_PER_SECOND (44100)

// 判断当前帧是否为关键帧
// 如果是SPS PPS I帧 SEI为关键帧
extern bool g_IsKeyFrame(const uint8_t *in_kui8Data, const int in_knSize);

extern bool g_WriteFile(const char *in_kpszFileName, const uint8_t *in_kui8Data, const int in_knSize, char *in_kszMode = "ab+");

#endif