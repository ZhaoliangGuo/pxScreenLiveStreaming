#pragma once
#include "pxFLVGlobal.h"


class CPxFLVMuxer
{
public:
	CPxFLVMuxer();
	virtual ~CPxFLVMuxer();

public:
	HRESULT AddFile(char *in_pwszFileName);
	HRESULT CloseFile(void);

	HRESULT BeginWriteFile(void);
	HRESULT EndWriteFile(void);

	HRESULT SetFileProperty(SPxRecordFileProperty *in_psFileProperty);

	HRESULT AddAudioStream(int in_nStreamNum, int in_nType, int in_nChannels, int in_nSamplesPerSecond, int in_nBitsPerSample);
	HRESULT AddVideoStream(int in_nStreamNum, int in_nType, int in_nWidth, int in_nHeight, int in_nBitrate, int in_nFrameRate);

	HRESULT WriteAudioSample(int in_nStreamNum, unsigned char  *in_pBuffer, int in_nBufferLen, int in_nTimeStamp);
	HRESULT WriteVideoSample(int in_nStreamNum, unsigned char  *in_pBuffer, int in_nBufferLen, bool in_bIsKeyFrame, int in_nTimeStamp);

	HRESULT FlushFile(void);
	HRESULT Write(char *in_pBuffer, int in_nBufferLen);

	HRESULT GetAVCNalType(LPVOID in_pBSBuf, const LONG in_nBSLen, ePxAVCNalType *out_eAVCNalType, int *out_nAVCNalLen);
	HRESULT GetBufferFrameType(unsigned char *in_pBuffer, int in_nBufferLen, ePxAVCNalType *out_eBufferFrameType, int *out_nBufferFrameHeadLen, int *out_nBufferFrameLen);
private:
	SPxFLVRecorderFileHeader    m_sFlvFileHeader;
	SPxFLVRecorderTagHeader     m_sFlvFileTagHeader;
	unsigned int                m_nLastTagSize;
	sPxFLVRecorderAudioData     m_sFlvFileAudioData;
	sPxFLVRecorderAACAudioData  m_sFlvFileAACAudioData;
	sPxFLVRecorderVideoData     m_sFlvFileVideoData;
	sPxFLVRecorderAVCVideoData  m_sFlvFileAVCVideoData;

	bool                        m_bAVCInited;
	bool                        m_bAACInited;
	bool                        m_bFirstIFrame;

	DWORD                       m_dwDurationPosition;			//FLV文件头信息中对应录制文件总时长的偏移量
	DWORD                       m_dwFileSizePosition;			//FLV文件头信息中对应录制文件总大小的偏移量

	FILE                        *m_fpFLVFile;			         //生成文件的指针，主要用于FLV文件
	double                      m_dFLVFileDuration;		         //生成文件的总时长
	//SPxRecordFileProperty       m_sPxRecordFileProperty;

	int                         m_nVideoStreamNum;					//视频流编号
	char                        m_szVideoFileName[_MAX_PATH];
	//CNsCriticalSection          m_oCritDevice;

	long                        m_lVideoTimeSample;
	long                        m_lAudioTimeSample;


	LARGE_INTEGER               m_lnSysBeginVideoTimeSample;
	LARGE_INTEGER               m_lnSysBeginAudioTimeSample;

	long                        m_lSysDValueVideoTimeSample;
	long                        m_lSysDValueAudioTimeSample;

	SPxRecordStreamProperty     m_sRecordStreamProperty;
};