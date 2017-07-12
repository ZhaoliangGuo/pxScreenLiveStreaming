#ifndef __pxRcordServerDef_H__
#define __pxRcordServerDef_H__
#include <vector>
using namespace std;

const int g_nMaxRecordTaskCount = 128;
const int g_nMaxDeviceClientCount = 8;
const int g_nMaxRecordCount = 8;
const int g_nMaxDeviceCount = 128;
const int g_nMaxBufferSize = 1*1024*1024;
const int g_nMaxVideoSize = 512*1024;

const int g_nMaxAdtsSamplingRates =	16;
const int g_nMetaData = 1024 * 320;
enum EPxRecordErrorType
{
	keRecordErrorType_Invliad = -1,
	keRecordErrorType_DeviceLost = 0,
	keRecordErrorType_DeviceTimeError = 1,
	keRecordErrorType_DeviceConnectError = 2,
	keRecordErrorType_DeviceHeadError = 3,
	keRecordErrorType_DeviceReadError = 4,	
	keRecordErrorType_Count
};

#define DEVICE_MPEG4	1
#define DEVICE_WMV9		2
#define DEVICE_MPEG2	3

#define VIEW_RECORD     1 //0预览 1录像（不创建解码器）


enum EPxRecordAudioVideoType
{
	keRecordAudioVideoType_Invalid = 0,
	keRecordAudioVideoType_AudioOnly = 1,
	keRecordAudioVideoType_VideoOnly = 2,
	keRecordAudioVideoType_AVBoth = 3,
	keRecordAudioVideoType_Count
};

enum EPxRecordDeviceType
{
	keRecordDeviceType_Invliad = -1,
	keRecordDeviceType_WMV = 0,//wmv
	keRecordDeviceType_MPEG264,//MPEG264
	keRecordDeviceType_HK,//HK
	keRecordDeviceType_Last
};

enum EPxRecordDeviceClientType
{
	keRecordDeviceClientType_Invliad = 0,
	keRecordDeviceClientType_WMV9 = 1,//wmv编码
	keRecordDeviceClientType_MPEG4_9601 = 2,//DVS小白盒
	keRecordDeviceClientType_MPEG4_ENC = 3,//VGA
	keRecordDeviceClientType_MPEG4_NEWD = 4,//中控原先wmv编码，先变成mpeg4编码
	keRecordDeviceClientType_H264 = 7,//H.264
	keRecordDeviceClientType_HKMPEG4 = 12,//海康的非正规MPEG的DVS
	keRecordDeviceClientType_HKH264 = 13,//海康非正规的264编码
	keRecordDeviceClientType_HKHIGH_264 = 14,//海康高清264编码器
	keRecordDeviceClientType_RTP_264 = 15,//海康的标准RTP方式，代替原来的DEV_HKHIGH_264类型
	keRecordDeviceClientType_HKHIGH_MPEG = 16,//海康高清Mepg4编码器 //lhj@2013.3.14 add
	keRecordDeviceClientType_Last
};

struct SPxRecordDeviceClientMPEG264
{
	HWND hDisplayWnd;
	char chConnetString[100];
	char chDeviceIP[20];
	int  nFormat;
};

struct SPxRecordDeviceClientWMV
{
	HWND hDisplayWnd;
	char chConnetString[100];
	char chDeviceIP[20];
	int  nFormat;
};

struct SPxRecordDeviceClientHK
{
	char chDeviceIP[20];                            //设备IP
	int  nChannelsNum;                              //设备
	int  nRecordTaskNum;                            //设备任务号
	char chDeviceLoginAcc[100];                     //设备登陆用户名	
	char chDeviceLoginPsd[100];                     //设备登陆密码
};

struct SPxRecordDeviceClient
{
	EPxRecordDeviceType eDeviceType;
	EPxRecordDeviceClientType eRecordDeviceClientType;          //设备类型
	bool bRecordVideo;
	bool bRecordAudio;
	union UPxDeviceClient
	{
		SPxRecordDeviceClientMPEG264 sClientMPEG264;
		SPxRecordDeviceClientWMV sClientWMV;
		SPxRecordDeviceClientHK sClientHK;
	}uDeviceClient;
};

enum EPxRecordFileType
{
	keRecordFileType_Invliad = -1,
	keRecordFileType_WMV = 0,
	keRecordFileType_ASF,
	keRecordFileType_FLV,
	keRecordFileType_MP4,
	keRecordFileType_M3U8,
	keRecordFileType_Count
};

enum EPxRecordVideoType
{
	keRecordVideoType_Invliad = -1,
	keRecordVideoType_WMV = 0,
	keRecordVideoType_MPEG4,
	keRecordVideoType_MPEG2,
	keRecordVideoType_H264,
	keRecordVideoType_MPEG2_HIGH,
	keRecordVideoType_H264_HIGH,
	keRecordVideoType_Count
};

enum EPxRecordAudioType
{
	keRecordAudioType_Invliad = -1,
	keRecordAudioType_AAC = 0,
	keRecordAudioType_MP2L2,
	keRecordAudioType_MP3,
	keRecordAudioType_Count
};

enum EPxRecordAudioSamples
{
	keRecordAudioSamples_Invliad = -1,//5500,16000,22050,32000,44100
	keRecordAudioSamples_5500 = 5500,
	keRecordAudioSamples_16000 = 16000,
	keRecordAudioSamples_22050 = 22050,
	keRecordAudioSamples_32000 = 32000,
	keRecordAudioSamples_44100 = 44100,
	keRecordAudioSamples_48000 = 48000,
	keRecordAudioSamples_Count
};

enum  EPxRecordAudioBits
{
	keRecordAudioBits_Invliad = -1,//16384，mp2l2为32768
	keRecordAudioBits_MP3 = 16384,
	keRecordAudioBits_MP2L2 = 32768,
	keRecordAudioBits_Count
};

struct SPxRecordDeviceClientPara
{
	int nWidth;
	int nHeight;
	int nBitrate;
	EPxRecordVideoType eVideoEncType;//for HK: 视频编码格式//视频编码类型 0 私有h264;1标准h264; 2标准mpeg4; 3-M-JPEG
	EPxRecordAudioType eAudioEncType;//for HK: 音频编码格式 音频编码类型 0-OggVorbis;1-G711_U;2-G711_A
	EPxRecordAudioSamples eAudioSampleRate;
	float fFrameRate;
	int nAudioChannels;
	int nAudioBitsPerSampleint;
	EPxRecordAudioBits eAvgBitsPerSec;
	char szWMVMediaType[72*8];

	SPxRecordDeviceClientPara()
	{
		nWidth = 0;
		nHeight = 0;
		nBitrate = 0;
		eVideoEncType = keRecordVideoType_Invliad;//for HK: 视频编码格式//视频编码类型 0 私有h264;1标准h264; 2标准mpeg4; 3-M-JPEG
		eAudioEncType = keRecordAudioType_Invliad;//for HK: 音频编码格式 音频编码类型 0-OggVorbis;1-G711_U;2-G711_A
		eAudioSampleRate = keRecordAudioSamples_Invliad;
		fFrameRate = 0.0f;
		nAudioChannels = 0;
		nAudioBitsPerSampleint = 0;
		eAvgBitsPerSec = keRecordAudioBits_Invliad;

	}

	~SPxRecordDeviceClientPara()
	{
		nWidth = 0;
		nHeight = 0;
		nBitrate = 0;
		eVideoEncType = keRecordVideoType_Invliad;//for HK: 视频编码格式//视频编码类型 0 私有h264;1标准h264; 2标准mpeg4; 3-M-JPEG
		eAudioEncType = keRecordAudioType_Invliad;//for HK: 音频编码格式 音频编码类型 0-OggVorbis;1-G711_U;2-G711_A
		eAudioSampleRate = keRecordAudioSamples_Invliad;
		fFrameRate = 0.0f;
		nAudioChannels = 0;
		nAudioBitsPerSampleint = 0;
		eAvgBitsPerSec = keRecordAudioBits_Invliad;
	}
};

struct SPxRecordFilePropertyASF
{
	size_t size;
};

struct SPxRecordFilePropertyFLV
{
	size_t size;
};

struct SPxRecordFilePropertyMP4
{
	size_t size;
};

struct SPxRecordStreamProperty
{
	bool bHasVideo;
	bool bHasAudio;

	EPxRecordVideoType eVideoType;//视频流编码类型(TYPE_H264)
	EPxRecordAudioType eAudioType;//音频流编码类型(TYPE_AAC)

	int nVideoWidth;//视频宽
	int nVideoHeight;//视频高
	int nVideoBitrate;//视频码率
	float fVideoFrameRate;//视频帧率

	int nAudioChannels;//音频声道数(1,2)
	EPxRecordAudioSamples eAudioSamplesPerSecond;//音频采样率(16000,22050,32000,44100)
	int nAudioBitsPerSampleint;//音频采样精度(8,16)
	//EPxRecordAudioBits eAvgBitsPerSec;//(asf文件，Mp3为16384，mp2l2为32768)

	//char szVideoIP[50];//视频IP
	//char szAudioIP[50];//音频IP
	//char szWMVMediaType[72*8];

	SPxRecordStreamProperty()
	{
		bHasVideo = false;
		bHasAudio = false;

		eVideoType      = keRecordVideoType_H264;
		eAudioType      = keRecordAudioType_AAC;
		nVideoWidth     = 1366;
		nVideoHeight    = 768;
		nVideoBitrate   = 1024*1024;
		fVideoFrameRate = 25;

		nAudioChannels         = 2;
		eAudioSamplesPerSecond = keRecordAudioSamples_32000;
		nAudioBitsPerSampleint = 16;
	}

	~SPxRecordStreamProperty()
	{
		bHasVideo = false;
		bHasAudio = false;

		eVideoType      = keRecordVideoType_H264;
		eAudioType      = keRecordAudioType_AAC;
		nVideoWidth     = 1366;
		nVideoHeight    = 768;
		nVideoBitrate   = 1024*1024;
		fVideoFrameRate = 25;

		nAudioChannels         = 2;
		eAudioSamplesPerSecond = keRecordAudioSamples_32000;
		nAudioBitsPerSampleint = 16;
	}
};

struct SPxRecordFileProperty
{
	EPxRecordFileType eFileType;

	vector<SPxRecordStreamProperty> aStreamProperty;

	//union UPxFileProperty
	//{
	//	SPxRecordFilePropertyASF sASFProperty;
	//	SPxRecordFilePropertyFLV sFLVProperty;
	//	SPxRecordFilePropertyMP4 sMP4Property;
	//}uFileProperty;
};

//#define TCP_UNI			1 //TCP单播
//#define UDP_UNI			2 //UDP单播
//#define UDP_MULTI		3 //UDP组播

enum EPxConnectType
{
	keConnectType_Invalid = 0,
	keConnectType_TCP_UNI = 1,
	keConnectType_UDP_UNI = 2,
	keConnectType_UDP_MULTI = 3,
	keConnectType_Last
};
#endif //__pxRcordServerDef_H__