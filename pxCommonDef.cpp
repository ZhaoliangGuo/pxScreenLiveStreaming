#include "pxCommonDef.h"
#include <time.h>
#include <stdio.h>

// YUV
CPxBufferPool      g_oYUVBufferPool;      // YUV内存池
CPxQueueBuffer     g_oYUVQueueBuffer; // 存储采集的YUV数据

// PCM
CPxBufferPool      g_oPCMBufferPool;      // PCM内存池
CPxQueueBuffer     g_oPCMQueueBuffer;     // 存储采集的PCM数据

// H.264/AAC
CPxBufferPool      g_oCodedBufferPool;    // 存储编码后数据的内存池(H.264/AAC)
CPxQueueBuffer     g_oCodedQueueBuffer;    // 存储编码后数据的队列

// used to make sure that static variables in gettimeofday() aren't initialized simultaneously by multiple threads
#define _WIN32_WCE (1)
static LONG initializeLock_gettimeofday = 0; 

int gettimeofday(struct timeval* tp, int* /*tz*/) 
{
	static LARGE_INTEGER tickFrequency, epochOffset;

	static bool isInitialized = false;

	LARGE_INTEGER tickNow;

#if !defined(_WIN32_WCE)
	QueryPerformanceCounter(&tickNow);
#else
	tickNow.QuadPart = GetTickCount();
#endif

	if (!isInitialized) 
	{
		if(1 == InterlockedIncrement(&initializeLock_gettimeofday)) 
		{
#if !defined(_WIN32_WCE)
			// For our first call, use "ftime()", so that we get a time with a proper epoch.
			// For subsequent calls, use "QueryPerformanceCount()", because it's more fine-grain.
			struct timeb tb;
			ftime(&tb);
			tp->tv_sec = tb.time;
			tp->tv_usec = 1000*tb.millitm;

			// Also get our counter frequency:
			QueryPerformanceFrequency(&tickFrequency);
#else
			/* FILETIME of Jan 1 1970 00:00:00. */
			const LONGLONG epoch = 116444736000000000LL;
			FILETIME fileTime;
			LARGE_INTEGER time;
			GetSystemTimeAsFileTime(&fileTime);

			time.HighPart = fileTime.dwHighDateTime;
			time.LowPart  = fileTime.dwLowDateTime;

			// convert to from 100ns time to unix timestamp in seconds, 1000*1000*10
			tp->tv_sec = (long)((time.QuadPart - epoch) / 10000000L);

			/*
			GetSystemTimeAsFileTime has just a seconds resolution,
			thats why wince-version of gettimeofday is not 100% accurate, usec accuracy would be calculated like this:
			// convert 100 nanoseconds to usec
			tp->tv_usec= (long)((time.QuadPart - epoch)%10000000L) / 10L;
			*/
			tp->tv_usec = 0;

			// resolution of GetTickCounter() is always milliseconds
			tickFrequency.QuadPart = 1000;
#endif     
			// compute an offset to add to subsequent counter times, so we get a proper epoch:
			epochOffset.QuadPart = tp->tv_sec * tickFrequency.QuadPart 
				+ (tp->tv_usec * tickFrequency.QuadPart) / 1000000L 
				- tickNow.QuadPart;

			// next caller can use ticks for time calculation
			isInitialized = true; 
			return 0;
		} 
		else 
		{
			InterlockedDecrement(&initializeLock_gettimeofday);
			// wait until first caller has initialized static values
			while(!isInitialized)
			{
				Sleep(1);
			}
		}
	}

	// adjust our tick count so that we get a proper epoch:
	tickNow.QuadPart += epochOffset.QuadPart;

	tp->tv_sec  =  (long)(tickNow.QuadPart / tickFrequency.QuadPart);
	tp->tv_usec = (long)(((tickNow.QuadPart % tickFrequency.QuadPart) * 1000000L) / tickFrequency.QuadPart);

	return 0;
}

UINT64 GetCurrentTimestamp()
{
	timeval tvTimestamp;
	gettimeofday(&tvTimestamp, NULL);

	UINT64 ui64TimeStamp = ((UINT64)tvTimestamp.tv_sec) * 1000 + tvTimestamp.tv_usec / 1000;

	return ui64TimeStamp;
}
