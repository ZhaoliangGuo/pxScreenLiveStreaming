#ifndef _PX_FLV_GLOBAL_H_
#define _PX_FLV_GLOBAL_H_ 

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#include "assert.h"

#define ASSERT assert

#include <WinSock2.h>
#include "pxRecordDef.h"
#include "stdint.h"

#pragma pack(1)

struct SPxFLVRecorderFileHeader
{
	unsigned char uchSignature1;
	unsigned char uchSignature2;
	unsigned char uchSignature3;
	unsigned char uchVersion;
	unsigned char uchTypeFlagsVideo : 1;
	unsigned char : 1;
	unsigned char uchTypeFlagsAudio : 1;
	unsigned char : 5;
	unsigned int  unDataOffset;
};


struct SPxFLVRecorderTagHeader
{
	unsigned char uchTagType;
	unsigned char uchDataSize[3];
	unsigned char uchTimestamp[3];
	unsigned char uchTimestampExtended;
	unsigned char uchStreamID[3];
};

struct sPxFLVRecorderAudioData
{
	unsigned char uchSoundType : 1;
	unsigned char uchSoundSize : 1;
	unsigned char uchSoundRate : 2;
	unsigned char uchSoundFormat : 4;
};

struct sPxFLVRecorderAACAudioData
{
	unsigned char uchAACPacketType;
};

struct sPxFLVRecorderVideoData
{
	unsigned char uchCodecID : 4;
	unsigned char uchFrameType : 4;
};

struct sPxFLVRecorderAVCVideoData
{
	unsigned char uchAVCPacketType;
	char          chCompositionTime[3];
};

enum ePxAVCNalType
{
	H264NT_NAL = 0,
	H264NT_SLICE,
	H264NT_SLICE_DPA,
	H264NT_SLICE_DPB,
	H264NT_SLICE_DPC,
	H264NT_SLICE_IDR,
	H264NT_SEI,
	H264NT_SPS,
	H264NT_PPS,
};


//写FLV头信息的一些辅助函数和信息
enum ePxAMFDataType
{ 
	AMF_NUMBER = 0, AMF_BOOLEAN, AMF_STRING, AMF_OBJECT,
	AMF_MOVIECLIP,		/* reserved, not used */
	AMF_NULL, AMF_UNDEFINED, AMF_REFERENCE, AMF_ECMA_ARRAY, AMF_OBJECT_END,
	AMF_STRICT_ARRAY, AMF_DATE, AMF_LONG_STRING, AMF_UNSUPPORTED,
	AMF_RECORDSET,		/* reserved, not used */
	AMF_XML_DOC, AMF_TYPED_OBJECT,
	AMF_AVMPLUS,		/* switch to AMF3 */
	AMF_INVALID = 0xff
};
struct sPxAACAdtsHeader
{
	char chNullBuf[7];
};


char inline *put_byte( char *out_chPut, unsigned char in_uchVal )
{
	out_chPut[0] = in_uchVal;
	return out_chPut+1;
}
char inline *put_be16(char *out_chPut, unsigned short in_usVal )
{
	out_chPut[1] = in_usVal & 0xff;
	out_chPut[0] = in_usVal >> 8;
	return out_chPut+2;
}

char inline *put_be24(char *out_chPut,unsigned int in_unVal )
{
	out_chPut[2] = in_unVal & 0xff;
	out_chPut[1] = in_unVal >> 8;
	out_chPut[0] = in_unVal >> 16;
	return out_chPut+3;
}

char inline *put_be32(char *out_chPut, unsigned int in_unVal )
{
	out_chPut[3] = in_unVal & 0xff;
	out_chPut[2] = in_unVal >> 8;
	out_chPut[1] = in_unVal >> 16;
	out_chPut[0] = in_unVal >> 24;
	return out_chPut+4;
}

char inline *put_be64( char *out_chPut, uint64_t in_unVal )
{
	out_chPut=put_be32( out_chPut, in_unVal >> 32 );
	out_chPut=put_be32( out_chPut, in_unVal );
	return out_chPut;
}

char inline *put_amf_string( char *out_chPut, const char *in_chBuf )
{
	uint16_t len = (uint16_t)strlen( in_chBuf );
	out_chPut=put_be16( out_chPut, len );
	memcpy(out_chPut,in_chBuf,len);
	return out_chPut+len;
}

char inline *put_amf_double( char *out_chPut, double in_dBuf )
{
	*out_chPut++ = AMF_NUMBER;	// type: Number
	{
		unsigned char *ci, *co;
		ci = (unsigned char *)&in_dBuf;
		co = (unsigned char *)out_chPut;
		co[0] = ci[7];
		co[1] = ci[6];
		co[2] = ci[5];
		co[3] = ci[4];
		co[4] = ci[3];
		co[5] = ci[2];
		co[6] = ci[1];
		co[7] = ci[0];
	}
	return out_chPut+8;
}

// convert unicode to TCHAR
BOOL inline ConvertFromWCHARToTCHAR(LPWSTR pszWCHAR, LPTSTR pszTCHAR, int nMax)
{
#ifdef _UNICODE
	wcsncpy(pszTCHAR, pszWCHAR, nMax);
	pszTCHAR[nMax - 1] = L'\0';
	return TRUE;
#else
	int nRet = WideCharToMultiByte(CP_THREAD_ACP, 0, pszWCHAR, -1,	(LPSTR)pszTCHAR, nMax, NULL, NULL);
	return (nRet>0);
#endif
}

// convert TCHAR to CHAR
BOOL inline ConvertFromTCHARToCHAR(LPCTSTR pszTCHAR, LPSTR pszCHAR, int nMax)
{
#ifdef _UNICODE
	int nRet = WideCharToMultiByte(CP_THREAD_ACP, 0, pszTCHAR, -1, pszCHAR, nMax, NULL, NULL);
	return (nRet>0);
#else
	strncpy(pszCHAR, pszTCHAR, nMax);
	pszCHAR[nMax - 1] = '\0';
	return TRUE;
#endif
}

// convert TCHAR to unicode
BOOL inline ConvertFromTCHARToWCHAR(LPCTSTR pszTCHAR, LPWSTR pszWCHAR, int nMax)
{
#ifdef _UNICODE
	wcsncpy(pszWCHAR, pszTCHAR, nMax);
	pszWCHAR[nMax - 1] = L'\0';
	return TRUE;
#else
	int nRet = MultiByteToWideChar(CP_THREAD_ACP, 0, (LPCSTR)pszTCHAR, -1,	pszWCHAR, nMax);
	return (nRet>0);
#endif
}

#endif