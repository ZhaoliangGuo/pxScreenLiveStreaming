#include "CPxFLVMuxer.h"
#include "pxErrorDef.h"
#include "pxRecordDef.h"
#include <io.h>
#include <iostream>
using namespace std;

//const int g_nMaxAdtsSamplingRates =	16;
//const int g_nMetaData = 1024 * 320;

unsigned int unPxAdtsSamplingRates[g_nMaxAdtsSamplingRates] = 
{
	96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 
	16000, 12000, 11025, 8000, 7350, 0, 0, 0
};


CPxFLVMuxer::CPxFLVMuxer()
{
	memset(&m_sFlvFileHeader,0,sizeof(m_sFlvFileHeader));
	memset(&m_sFlvFileTagHeader,0,sizeof(m_sFlvFileTagHeader));
	m_nLastTagSize = 0;
	memset(&m_sFlvFileAudioData,0,sizeof(m_sFlvFileAudioData));
	memset(&m_sFlvFileAACAudioData,0,sizeof(m_sFlvFileAACAudioData));
	memset(&m_sFlvFileVideoData,0,sizeof(m_sFlvFileVideoData));
	memset(&m_sFlvFileAVCVideoData,0,sizeof(m_sFlvFileAVCVideoData));
	m_bAVCInited = false;
	m_bAACInited = false;
	m_bFirstIFrame = false;
	m_fpFLVFile = NULL;			 
	m_dwDurationPosition = 0;		 
	m_dwFileSizePosition = 0;
	m_dFLVFileDuration = 0;
	/*m_sPxRecordFileProperty.eFileType = keRecordFileType_FLV;
	m_sPxRecordFileProperty.aStreamProperty.empty();*/
	//m_sPxRecordFileProperty.uFileProperty.sFLVProperty.size = 0;
	m_nVideoStreamNum = 0;				 
	memset(&m_szVideoFileName,0,sizeof(m_szVideoFileName));

	m_lVideoTimeSample = 0;
	m_lAudioTimeSample = 0;

	m_lSysDValueVideoTimeSample = 0;
	m_lSysDValueAudioTimeSample = 0;

	m_bVideoSampleArrived = false;
	m_bAudioSampleArrived = false;
}

CPxFLVMuxer::~CPxFLVMuxer()
{
	if (m_fpFLVFile)
	{
		fclose(m_fpFLVFile);
		m_fpFLVFile = NULL;		
	}
}

HRESULT CPxFLVMuxer::AddFile(char *in_pwszFileName)
{
	HRESULT hr = NS_NOERROR;


	if( in_pwszFileName == NULL )
	{
		hr = NS_E_INVALID_PARAMETER;
		/*//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVMuxer::AddFile: NS_E_INVALID_PARAMETER,in_pwszFileName is NULL", true);*/
	}
	
	//if (!ConvertFromWCHARToTCHAR(in_pwszFileName, m_szVideoFileName, _MAX_PATH))
	//{
	//	hr = NS_E_INVALID_FILE;
	//	/*//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
	//		"CPxASFRecordTask::CreateOneFileRecorder: NS_E_INVALID_FILE!", true);*/
	//}
	
	/*//CNsAutoLock lock(&m_oCritDevice);*/
	if (m_fpFLVFile == NULL)
	{
		m_fpFLVFile = fopen(in_pwszFileName, "wb");
		if (m_fpFLVFile == NULL)
		{
			hr = PX_E_PXFLVRECORDER_CREATEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::AddFile: PX_E_PXFLVRECORDER_CREATEFILE_ERROR, File create fail,File Name is %s",m_szVideoFileName);
			/*//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			chError, true);*/
		}
	}
	else
	{
		hr = PX_E_PXFLVRECORDER_CREATEFILE_ERROR;
	    char chError[200];
	    sprintf( chError,"CPxFLVMuxer::AddFile: PX_E_PXFLVRECORDER_CREATEFILE_ERROR, File is exist, File Name is %s",m_szVideoFileName);
		/* //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		chError, true);*/
	}

	m_sFlvFileHeader.uchSignature1 = 'F';	
	m_sFlvFileHeader.uchSignature2 = 'L';
	m_sFlvFileHeader.uchSignature3 = 'V';
	m_sFlvFileHeader.uchVersion = 0x01;	
	m_sFlvFileHeader.unDataOffset = htonl(0x09);	

	////NsLogNotifyA_Add_file(0,0,"CPxFLVMuxer::AddFile, SUCCEEDED, %s! ",in_pwszFileName);

	return hr;
}

HRESULT CPxFLVMuxer::CloseFile(void)
{
	HRESULT hr = NS_NOERROR;

	////CNsAutoLock lock(&m_oCritDevice);
	if (m_fpFLVFile != NULL)
	{
		int nResult = fclose(m_fpFLVFile);
		m_fpFLVFile = NULL;	
		if(nResult == 0)
		{	
			////NsLogNotifyA_Add_file(0,0,"CPxFLVMuxer::DelFile,file delete success,filename is %s",m_szVideoFileName);
		}
		else
		{
			hr = PX_E_PXFLVRECORDER_DELFILE_ERROR;
		    char chError[200];
		    sprintf( chError,"CPxFLVMuxer::DelFile: PX_E_PXFLVRECORDER_DELFILE_ERROR, delete file is fail, File Name is %s",m_szVideoFileName);
		    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			   // chError, true);
		}
	}
	else 
	{
		hr = PX_E_PXFLVRECORDER_DELFILE_ERROR;
	    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
	 	    //"CPxFLVMuxer::DelFile: PX_E_PXFLVRECORDER_DELFILE_ERROR, file name is NULL", true);
    }

	return hr;
}

//HRESULT CPxFLVMuxer::SetFileProperty(SPxRecordFileProperty *in_psFileProperty)
//{
//	HRESULT hr = NS_NOERROR;
//
//	//NsLogNotifyA_Add_file(0,0,"CPxFLVMuxer::SetFileProperty, BEGIN! ");
//
//	if(in_psFileProperty == NULL)
//	{
//		hr = NS_E_INVALID_PARAMETER;
//		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
//			"CPxFLVMuxer::SetFileProperty: NS_E_INVALID_PARAMETER,in_psFileProperty is NULL", true);
//	}
//
//	m_sPxRecordFileProperty.eFileType = in_psFileProperty->eFileType;
////	m_sPxRecordFileProperty.uFileProperty = in_psFileProperty->uFileProperty;
//	int nCountVdieoStream = in_psFileProperty->aStreamProperty.size();
//	if(nCountVdieoStream > 1 || nCountVdieoStream==0)
//	{
//		hr = NS_E_INVALID_PARAMETER;
//	    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
//		    "CPxFLVMuxer::SetFileProperty: NS_E_INVALID_PARAMETER,nCountVdieoStream>1 || nCountVdieoStream==0 is invalid", true);
//	}
//
//	m_nVideoStreamNum = 0;
//
//	for(int nIdx=0 ; nIdx < nCountVdieoStream ; nIdx++)
//	{
//		//in_psFileProperty->aStreamProperty[nIdx].fVideoFrameRate = 30;
//		m_sPxRecordFileProperty.aStreamProperty.push_back(in_psFileProperty->aStreamProperty[nIdx]);
//	}	
//
//	if(m_sRecordStreamProperty.bHasAudio == true)
//	{
//		hr = AddAudioStream(m_nVideoStreamNum,
//			    m_sRecordStreamProperty.eAudioType,
//			    m_sRecordStreamProperty.nAudioChannels,
//			    m_sRecordStreamProperty.eAudioSamplesPerSecond,
//			    m_sRecordStreamProperty.nAudioBitsPerSampleint);
//		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
//			"CPxFLVMuxer::SetFileProperty: AddAudioStream error", true);
//	}
//
//	if(m_sRecordStreamProperty.bHasVideo == true)
//	{
//		 hr = AddVideoStream(m_nVideoStreamNum,
//			    m_sRecordStreamProperty.eVideoType,
//			    m_sRecordStreamProperty.nVideoWidth,
//			    m_sRecordStreamProperty.nVideoHeight,
//			    m_sRecordStreamProperty.nVideoBitrate,
//			    m_sRecordStreamProperty.fVideoFrameRate);
//		 //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
//			 "CPxFLVMuxer::SetFileProperty: AddVideoStream error", true);
//	}
//
//	//NsLogNotifyA_Add_file(0,0,"CPxFLVMuxer::SetFileProperty, SUCCEEDED! ");
//
//	return hr;
//}

HRESULT CPxFLVMuxer::BeginWriteFile(void)
{
	HRESULT hr = NS_NOERROR;

	//NsLogNotifyA_Add_file(0,0,"CPxFLVMuxer::BeginWriteFile, BEGIN! ");

	////CNsAutoLock lock(&m_oCritDevice);
	
	hr = Write((char *)&m_sFlvFileHeader, sizeof(SPxFLVRecorderFileHeader));	//写入9字节文件头
	if (FAILED(hr))
	{
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::BeginWriteFile: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, File header write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}

	//char chMetaData[g_nMetaData];
	char *chMetaData = new char[g_nMetaData];
	if (chMetaData == NULL)
	{
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::BeginWriteFile: OUT OF MEMORY, File header write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}

	char *chMetaDataTemp = (char *)chMetaData;


	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_STRING);
	chMetaDataTemp = put_amf_string(chMetaDataTemp, "onMetaData");

	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_ECMA_ARRAY);
	chMetaDataTemp = put_be32(chMetaDataTemp, 21);

	chMetaDataTemp = put_amf_string(chMetaDataTemp, "MetaDataCreator");
	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_STRING);
	chMetaDataTemp = put_amf_string(chMetaDataTemp, "created by ZL Guo @2017");

	chMetaDataTemp = put_amf_string(chMetaDataTemp, "hasKeyFrames");
	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_BOOLEAN);
	chMetaDataTemp = put_byte(chMetaDataTemp, 0);

	chMetaDataTemp = put_amf_string(chMetaDataTemp, "hasVideo");
	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_BOOLEAN);
	chMetaDataTemp = put_byte(chMetaDataTemp, 1);

	chMetaDataTemp = put_amf_string(chMetaDataTemp, "hasAudio");
	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_BOOLEAN);
	chMetaDataTemp = put_byte(chMetaDataTemp, 1);

	chMetaDataTemp = put_amf_string(chMetaDataTemp, "hasMatadata");
	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_BOOLEAN);
	chMetaDataTemp = put_byte(chMetaDataTemp, 1);

	chMetaDataTemp = put_amf_string(chMetaDataTemp, "canSeekToEnd");
	chMetaDataTemp = put_byte(chMetaDataTemp, AMF_BOOLEAN);
	chMetaDataTemp = put_byte(chMetaDataTemp, 0);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "duration");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)0.0);

	m_dwDurationPosition = (DWORD)(chMetaDataTemp - chMetaData + sizeof(SPxFLVRecorderFileHeader) + sizeof(SPxFLVRecorderTagHeader) + 4 - 8);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "width");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nVideoWidth);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "height");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nVideoHeight);

	// add by gzl begin
	/*chMetaDataTemp = put_amf_string( chMetaDataTemp, "displayWidth");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nVideoWidth);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "displayHeight");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nVideoHeight);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "frameWidth");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nVideoWidth);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "frameHeight");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nVideoHeight);*/
	// add by gzl end

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "videodatarate");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nVideoBitrate);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "framerate");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.fVideoFrameRate);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "videocodecid");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)7.0);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "audiosamplerate");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.eAudioSamplesPerSecond);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "audiosamplesize");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)m_sRecordStreamProperty.nAudioBitsPerSampleint);

	/*if(1 == m_sRecordStreamProperty.nAudioChannels)
	{
		chMetaDataTemp = put_amf_string(chMetaDataTemp, "Mono");
		chMetaDataTemp = put_byte(chMetaDataTemp, AMF_BOOLEAN);
		chMetaDataTemp = put_byte(chMetaDataTemp, 0);
	}
	else if(2 == m_sRecordStreamProperty.nAudioChannels)*/
	{
		chMetaDataTemp = put_amf_string(chMetaDataTemp, "stereo");
		chMetaDataTemp = put_byte(chMetaDataTemp, AMF_BOOLEAN);
		chMetaDataTemp = put_byte(chMetaDataTemp, 1);
	}

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "audiocodecid");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)10.0);

	chMetaDataTemp = put_amf_string( chMetaDataTemp, "filesize");
	chMetaDataTemp = put_amf_double( chMetaDataTemp, (double)0.0);

	m_dwFileSizePosition = (DWORD)(chMetaDataTemp -chMetaData + sizeof(SPxFLVRecorderFileHeader) + sizeof(SPxFLVRecorderTagHeader) + 4 - 8);

	chMetaDataTemp = put_be24( chMetaDataTemp, 9);

	unsigned int unTagSize = (unsigned int)(chMetaDataTemp - chMetaData);

	m_sFlvFileTagHeader.uchTagType = 0x12;
	m_sFlvFileTagHeader.uchDataSize[0] = (unTagSize >> 16) & 0xff;
	m_sFlvFileTagHeader.uchDataSize[1] = (unTagSize >>  8) & 0xff;
	m_sFlvFileTagHeader.uchDataSize[2] = (unTagSize >>  0) & 0xff;

	m_nLastTagSize = htonl(m_nLastTagSize);
	hr = Write((char *)&m_nLastTagSize, sizeof(unsigned int));
	if (FAILED(hr))
	{
		delete []chMetaData;
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::BeginWriteFile: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, File tag size write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}
	m_nLastTagSize = ntohl(m_nLastTagSize);

	int nFLVRecorderFileTag = sizeof(SPxFLVRecorderTagHeader);
	hr = Write((char *)&m_sFlvFileTagHeader, nFLVRecorderFileTag);
	if (FAILED(hr))
	{
		delete []chMetaData;
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::BeginWriteFile: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, SPxFLVRecorderFileTag write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}
	hr = Write(chMetaData, unTagSize);
	if (FAILED(hr))
	{
		delete []chMetaData;
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::BeginWriteFile: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, chMetaData write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}

	m_nLastTagSize = unTagSize + sizeof(SPxFLVRecorderTagHeader);
	
	delete []chMetaData;

	//NsLogNotifyA_Add_file(0,0,"CPxFLVMuxer::BeginWriteFile, SUCCEEDED! ");

	return hr;
}

HRESULT CPxFLVMuxer::EndWriteFile(void)
{
	HRESULT hr = NS_NOERROR;

	//CNsAutoLock lock(&m_oCritDevice);

	m_nLastTagSize = htonl(m_nLastTagSize);
	hr = Write((char *)&m_nLastTagSize, sizeof(unsigned int));		
	if (FAILED(hr))
	{
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::EndWriteFile: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, the last tagsize write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}
	m_nLastTagSize = ntohl(m_nLastTagSize);

	//FILE* file = fopen(m_chVideoFileName, "r");
	//if (file)
	//{
	//	m_dFLVFileDuration = filelength(fileno(file));
	//	fclose(file);
	//	file = NULL;
	//}
	//else
	//{
	//	//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgPxDeviceLiveSource, keLogPkgPxDeviceLiveSourceFuncGeneral, NS_E_FILE_NOT_FOUND,
	//		"CPxRecorderManager::StopRecord Get FLV file len:fopen FAIL", true);
	//}
	
	fseek(m_fpFLVFile, m_dwDurationPosition, SEEK_SET);
	unsigned char *uchIn, uchOut[8];
	m_dFLVFileDuration /= 1000;
	uchIn = (unsigned char *)&m_dFLVFileDuration;
	uchOut[0] = uchIn[7];
	uchOut[1] = uchIn[6];
	uchOut[2] = uchIn[5];
	uchOut[3] = uchIn[4];
	uchOut[4] = uchIn[3];
	uchOut[5] = uchIn[2];
	uchOut[6] = uchIn[1];
	uchOut[7] = uchIn[0];
	hr = Write((char *)uchOut, sizeof(uchOut));
	if (FAILED(hr))
	{
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::EndWriteFile: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, file Duration write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}

	fseek(m_fpFLVFile, 0, SEEK_END);   
	double dFLVFileAllSize = ftell(m_fpFLVFile);
	fseek(m_fpFLVFile, m_dwFileSizePosition, SEEK_SET);

	uchIn = (unsigned char *)&dFLVFileAllSize;
	uchOut[0] = uchIn[7];
	uchOut[1] = uchIn[6];
	uchOut[2] = uchIn[5];
	uchOut[3] = uchIn[4];
	uchOut[4] = uchIn[3];
	uchOut[5] = uchIn[2];
	uchOut[6] = uchIn[1];
	uchOut[7] = uchIn[0];
	hr = Write((char *)uchOut, sizeof(uchOut));
	if (FAILED(hr))
	{
		hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::EndWriteFile: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, file all size write fail,File Name is %s",m_szVideoFileName);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//chError, true);
	}

	if (m_fpFLVFile)
	{
		fclose(m_fpFLVFile);
		m_fpFLVFile = NULL;	
	}

	return hr;
}

HRESULT CPxFLVMuxer::AddAudioStream(int in_nStreamNum, int in_nType, int in_nChannels, int in_nSamplesPerSecond, int in_nBitsPerSample)
{
	HRESULT hr = NS_NOERROR;

	//CNsAutoLock lock(&m_oCritDevice);
	if(in_nStreamNum < 0 || in_nType < 0  || in_nChannels < 0 || in_nSamplesPerSecond < 0 || in_nBitsPerSample< 0 )
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::AddAudioStream: NS_E_INVALID_PARAMETER,in_nStreamNum<0 or in_nType<0 or in_nChannels<0 or in_nSamplesPerSecond<0 or in_nBitsPerSample<0", true);
	}

	/*if (m_sRecordStreamProperty.eAudioType == keRecordAudioType_AAC)
	{*/
		m_sFlvFileHeader.uchTypeFlagsAudio = 0x01;
		
	//}
	//else
	//{
	//	hr = NS_E_INVALID_PARAMETER;
	//	//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
	//		//"CPxFLVMuxer::AddAudioStream: NS_E_INVALID_PARAMETER,audio type is not AAC", true);
 //   }

	return hr;
}

HRESULT CPxFLVMuxer::AddVideoStream(int in_nStreamNum, int in_nType, int in_nWidth, int in_nHeight, int in_nBitrate, int in_nFrameRate)
{
	HRESULT hr = NS_NOERROR;
	
	//CNsAutoLock lock(&m_oCritDevice);
	if(in_nStreamNum < 0 || in_nType < 0  || in_nWidth < 0 || in_nHeight < 0 || in_nBitrate < 0 || in_nFrameRate < 0 )
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::AddVideoStream: NS_E_INVALID_PARAMETER,in_nStreamNum<0 or in_nType<0 or in_nWidth<0 or in_nHeight<0 or in_nBitrate<0 or in_nFrameRate<0", true);
	}

	/*if (m_sRecordStreamProperty.eVideoType == keRecordVideoType_H264
		|| m_sRecordStreamProperty.eVideoType == keRecordVideoType_H264_HIGH)
	{*/
		m_sFlvFileHeader.uchTypeFlagsVideo = 0x01;
	//}
	//else
	//{
	//	hr = NS_E_INVALID_PARAMETER;
	//	//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
	//		"CPxFLVMuxer::AddVideoStream: NS_E_INVALID_PARAMETER,video type is not H264", true);
	//}

	return hr;
}

#define FLV_MSG_BUFFER_SIZE (2048)
char szMsgBuffer[FLV_MSG_BUFFER_SIZE] = {0};

HRESULT CPxFLVMuxer::WriteAudioSample(int in_nStreamNum, unsigned char  *in_pBuffer, int in_nBufferLen, int in_lTimeStamp)
{
	HRESULT hr = NS_NOERROR;

	ZeroMemory(szMsgBuffer, FLV_MSG_BUFFER_SIZE);

	if(in_nStreamNum < 0)
	{
		hr = NS_E_INVALID_PARAMETER;
	    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		    //"CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER,in_nStreamNum < 0", true);
	}

	if(in_pBuffer == NULL )
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER,in_pBuffer == NULL", true);
	}

	if(in_nBufferLen < 0)
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER,in_nBufferLen < 0", true);
	}

	unsigned long ulTimeStamp = static_cast <unsigned long> (in_lTimeStamp);

	//long ulTimeStamp = in_lTimeStamp;

	//if(ulTimeStamp < 0 )
	//{
	//	hr = NS_E_INVALID_PARAMETER;

	//	sprintf_s(szMsgBuffer, 
	//			  FLV_MSG_BUFFER_SIZE, 
	//			  "CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER, ulTimeStamp(%lu) < 0, File Name : %s",
	//			  ulTimeStamp,
	//			  m_szVideoFileName);

	//	//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
	//		//szMsgBuffer, true);
	//}

	if(m_bFirstIFrame == false && m_sRecordStreamProperty.bHasVideo == true)
	{		
		//char chError[200];
		//sprintf( chError,"CCPxFLVMuxer::WriteAudioSample: m_bFirstIFrame is false");
		////NsLogNotifyA_Add_file(0,0,chError);
		return hr;
	}

	char szMsgBuffer[FLV_MSG_BUFFER_SIZE] = {0};

	//CNsAutoLock lock(&m_oCritDevice);
	
	if(	m_lAudioTimeSample == 0 && !m_bAudioSampleArrived)
	{
		m_bAudioSampleArrived = true;
		m_lAudioTimeSample = ulTimeStamp;
		ulTimeStamp = ulTimeStamp - m_lAudioTimeSample;
		QueryPerformanceCounter(&m_lnSysBeginAudioTimeSample);
	}
	else
	{
		if( (ulTimeStamp + m_lSysDValueAudioTimeSample) <= m_lAudioTimeSample )
		{
			LARGE_INTEGER  lnNowTimeStamp;
			QueryPerformanceCounter(&lnNowTimeStamp);
			m_lSysDValueAudioTimeSample = (lnNowTimeStamp.QuadPart - m_lnSysBeginAudioTimeSample.QuadPart )/1000 + m_lAudioTimeSample - ulTimeStamp;
		}
		ulTimeStamp = (ulTimeStamp + m_lSysDValueAudioTimeSample - m_lAudioTimeSample);
	}
	
	if (m_bAACInited == false) // 参数尚未初始化 进行初始化
	{
		unsigned char uchAudioSpecificConfig[1024] = {0};
		memset(uchAudioSpecificConfig,0,sizeof(uchAudioSpecificConfig));
		int nPos = 2;
		unsigned char uchAudioObjectType = 2; 
	    unsigned char uchSamplingFrequencyIndex = 0;	
		while(uchSamplingFrequencyIndex < g_nMaxAdtsSamplingRates)
	    {
		    if (unPxAdtsSamplingRates[uchSamplingFrequencyIndex] == (unsigned int)m_sRecordStreamProperty.eAudioSamplesPerSecond)
		    {
		    	break;
		    }

		    uchSamplingFrequencyIndex++;
	    }

		if(uchSamplingFrequencyIndex == g_nMaxAdtsSamplingRates)
		{
			hr = NS_E_INVALID_PARAMETER;

			uchSamplingFrequencyIndex = 5;  // add by gzl ; set the default index: 5 for 32000

		   /* NSD_SAFE_REPORT_ERROR(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			    "CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER,unPxAdtsSamplingRates can find valid value", true);*/

		}

	    unsigned char uchChannelConfiguration = (unsigned char)m_sRecordStreamProperty.nAudioChannels; 
	    unsigned char uchFrameLengthFlag = 0;
	    unsigned char uchDependsOnCoreCoder = 0;
	    unsigned char uchExtensionFlag = 0;

	    uchAudioSpecificConfig[0] = (unsigned char)(uchAudioObjectType<<3)  +  (unsigned char)((uchSamplingFrequencyIndex & 0x0E)>>1);
	    uchAudioSpecificConfig[1] = (unsigned char)((uchSamplingFrequencyIndex & 0x01)<<7) + (unsigned char)((uchChannelConfiguration & 0x0F) << 3)
											    + (unsigned char)(uchFrameLengthFlag & 0x01) + (unsigned char)(uchDependsOnCoreCoder & 0x01) + (unsigned char)(uchExtensionFlag & 0x01);
		m_sFlvFileTagHeader.uchTagType = 0x08;
		unsigned int unTagSize = nPos + sizeof(sPxFLVRecorderAudioData) + sizeof(sPxFLVRecorderAACAudioData);
		m_sFlvFileTagHeader.uchDataSize[0] = (unTagSize >> 16) & 0xff;
		m_sFlvFileTagHeader.uchDataSize[1] = (unTagSize >>  8) & 0xff;
		m_sFlvFileTagHeader.uchDataSize[2] = (unTagSize >>  0) & 0xff;

		m_sFlvFileTagHeader.uchTimestamp[0] = (BYTE)((ulTimeStamp >> 16) & 0xff);	
		m_sFlvFileTagHeader.uchTimestamp[1] = (BYTE)((ulTimeStamp >>  8) & 0xff);
		m_sFlvFileTagHeader.uchTimestamp[2] = (BYTE)((ulTimeStamp >>  0) & 0xff);
		m_sFlvFileTagHeader.uchTimestampExtended = (BYTE)((ulTimeStamp >> 24) & 0xff);	

		if(m_sRecordStreamProperty.eAudioType == keRecordAudioType_AAC)
		{
		    m_sFlvFileAudioData.uchSoundFormat = 0x0A;
		}
		else
		{
			hr = NS_E_INVALID_PARAMETER;
		    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			   // "CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER,audio type is not aac", true);
		}

		/*
			Sampling rate: 2bit

			The following values are defined:
			0 = 5.5 kHz
			1 = 11 kHz
			2 = 22 kHz
			3 = 44 kHz
		*/

		switch(m_sRecordStreamProperty.eAudioSamplesPerSecond)
		{
		case keRecordAudioSamples_5500:
			m_sFlvFileAudioData.uchSoundRate = 0x00;
			break;
		case keRecordAudioSamples_16000:
			m_sFlvFileAudioData.uchSoundRate = 0x01;
			break;
		case keRecordAudioSamples_22050:
			m_sFlvFileAudioData.uchSoundRate = 0x02;
			break;
		case keRecordAudioSamples_32000:
		case keRecordAudioSamples_44100:
			m_sFlvFileAudioData.uchSoundRate = 0x03;
			break;

		default:
			m_sFlvFileAudioData.uchSoundRate = 0x03; // add by gzl 
			hr = NS_E_INVALID_PARAMETER;
			sprintf_s(szMsgBuffer, 
					  FLV_MSG_BUFFER_SIZE, 
					  "CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER, audio sound rate (eAudioSamplesPerSecond:%d) is invalid.\n FileName: %s ", 					  
					  m_sRecordStreamProperty.eAudioSamplesPerSecond,
					  m_szVideoFileName);

			//NSD_SAFE_REPORT_ERROR(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr, szMsgBuffer, true);

			break;
		}

		switch(m_sRecordStreamProperty.nAudioBitsPerSampleint)
		{
		case 8:
			m_sFlvFileAudioData.uchSoundSize = 0x00;
			break;
		case 16:
			m_sFlvFileAudioData.uchSoundSize = 0x01;
			break;
		default:
			m_sFlvFileAudioData.uchSoundSize = 0x01;

			hr = NS_E_INVALID_PARAMETER;

			/*sprintf_s(szMsgBuffer, 
				FLV_MSG_BUFFER_SIZE, 
				"CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER, audio sound size is invalid, nAudioBitsPerSampleint:%d.\n FileName:%s", 
				m_sRecordStreamProperty.nAudioBitsPerSampleint,
				m_szVideoFileName);*/

			/*NSD_SAFE_REPORT_ERROR(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			szMsgBuffer, true);*/
			break;
		}

		switch(m_sRecordStreamProperty.nAudioChannels)
		{
		case 1:
			m_sFlvFileAudioData.uchSoundType = 0x00;
			break;

		case 2:
			m_sFlvFileAudioData.uchSoundType = 0x01;
			break;

		default:
			m_sFlvFileAudioData.uchSoundType = 0x01;

			hr = NS_E_INVALID_PARAMETER;
			sprintf_s(szMsgBuffer, 
				FLV_MSG_BUFFER_SIZE, 
				"CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER, audio sound size is invalid, nAudioChannels:%d.\n FileName:%s", 
				m_sRecordStreamProperty.nAudioChannels,
				m_szVideoFileName);

			/*NSD_SAFE_REPORT_ERROR(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			szMsgBuffer, true);*/

			break;
		}

		m_sFlvFileAACAudioData.uchAACPacketType = 0; 

		m_nLastTagSize = htonl(m_nLastTagSize);
		hr = Write((char *)&m_nLastTagSize, sizeof(unsigned int));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			sprintf_s(szMsgBuffer, 
				      FLV_MSG_BUFFER_SIZE, 
				      "CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, m_nLastTagSize write fail,File Name is %s", 
				      m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//szMsgBuffer, true);
		}

		m_nLastTagSize = ntohl(m_nLastTagSize);

		hr = Write((char *)&m_sFlvFileTagHeader, sizeof(SPxFLVRecorderTagHeader));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, SPxFLVRecorderFileTag write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}	
		hr = Write((char *)&m_sFlvFileAudioData, sizeof(sPxFLVRecorderAudioData));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, sPxFLVRecorderAudioData write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}	
		hr = Write((char *)&m_sFlvFileAACAudioData, sizeof(sPxFLVRecorderAACAudioData));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, sPxFLVRecorderAACAudioData write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}	
		hr = Write((char *)uchAudioSpecificConfig, nPos);
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, uchAudioSpecificConfig write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}	

		m_nLastTagSize = nPos + sizeof(SPxFLVRecorderTagHeader) + sizeof(sPxFLVRecorderAudioData) + sizeof(sPxFLVRecorderAACAudioData);
		m_bAACInited = true;
	}
	else if (m_bAACInited == true && in_nBufferLen > 7)
	{	
		in_pBuffer += sizeof(sPxAACAdtsHeader);
		in_nBufferLen -= sizeof(sPxAACAdtsHeader);

		m_sFlvFileTagHeader.uchTagType = 0x08;
		unsigned int unTagSize = in_nBufferLen + sizeof(sPxFLVRecorderAudioData) + sizeof(sPxFLVRecorderAACAudioData);
		m_sFlvFileTagHeader.uchDataSize[0] = (unTagSize >> 16) & 0xff;
		m_sFlvFileTagHeader.uchDataSize[1] = (unTagSize >>  8) & 0xff;
		m_sFlvFileTagHeader.uchDataSize[2] = (unTagSize >>  0) & 0xff;
		m_sFlvFileTagHeader.uchTimestamp[0] = (BYTE)((ulTimeStamp >> 16) & 0xff);		
		m_sFlvFileTagHeader.uchTimestamp[1] = (BYTE)((ulTimeStamp >>  8) & 0xff);
		m_sFlvFileTagHeader.uchTimestamp[2] = (BYTE)((ulTimeStamp >>  0) & 0xff);
		m_sFlvFileTagHeader.uchTimestampExtended = (BYTE)((ulTimeStamp >> 24) & 0xff);	
		if(m_sRecordStreamProperty.eAudioType == keRecordAudioType_AAC)
		{
			m_sFlvFileAudioData.uchSoundFormat = 0x0A;
		}

		m_sFlvFileAACAudioData.uchAACPacketType = 1; 

		m_nLastTagSize = htonl(m_nLastTagSize);
		hr = Write((char *)&m_nLastTagSize, sizeof(unsigned int));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, AACRaw m_nLastTagSize write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}

		m_nLastTagSize = ntohl(m_nLastTagSize);

		hr = Write((char *)&m_sFlvFileTagHeader, sizeof(SPxFLVRecorderTagHeader));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, AACRaw SPxFLVRecorderFileTag write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		hr = Write((char *)&m_sFlvFileAudioData, sizeof(sPxFLVRecorderAudioData));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, AACRaw sPxFLVRecorderAudioData write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		hr = Write((char *)&m_sFlvFileAACAudioData, sizeof(sPxFLVRecorderAACAudioData));
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, AACRaw sPxFLVRecorderAACAudioData write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		hr = Write((char *)in_pBuffer, in_nBufferLen);
		if(FAILED(hr))
		{
			hr = PX_E_PXFLVRECORDER_WRITEFILE_ERROR;
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteAudioSample: PX_E_PXFLVRECORDER_WRITEFILE_ERROR, AACRaw in_pBuffer write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		m_nLastTagSize = in_nBufferLen + sizeof(SPxFLVRecorderTagHeader) + sizeof(sPxFLVRecorderAudioData) + sizeof(sPxFLVRecorderAACAudioData);
		m_dFLVFileDuration = ulTimeStamp;
	}
	else
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::WriteAudioSample: NS_E_INVALID_PARAMETER,in_nBufferLen<=7", true);
	}
	return hr;
}

HRESULT CPxFLVMuxer::WriteVideoSample(int in_nStreamNum, unsigned char  *in_pBuffer, int in_nBufferLen, bool in_bIsKeyFrame, int in_lTimeStamp)
{
	HRESULT hr = NS_NOERROR;

	if(in_nStreamNum < 0 )
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::WriteVideoSample: NS_E_INVALID_PARAMETER,in_nStreamNum < 0", true);
	}

	if(in_pBuffer == NULL )
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::WriteVideoSample: NS_E_INVALID_PARAMETER,in_pBuffer == NULL", true);
	}

	if(in_nBufferLen < 0)
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::WriteVideoSample: NS_E_INVALID_PARAMETER, in_nBufferLen < 0", true);
	}

	unsigned long ulTimeStamp = static_cast <unsigned long> (in_lTimeStamp);

	if(m_bFirstIFrame == false)
	{
		if(in_bIsKeyFrame == true)
		{
			m_bFirstIFrame = true;
		}
		else
		{
			/*char chError[200];
			sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: in_bIsKeyFrame is false");
			//NsLogNotifyA_Add_file(0,0,chError);*/
			return hr;
		}
	}

	//CNsAutoLock lock(&m_oCritDevice);
	int nAVCNalLen = 0;
	ePxAVCNalType eAVCNalType = H264NT_NAL;
	int nSpsHeadLen = 0;
	int nPpsHeadLen = 0;
	int nSeiHeadLen = 0;
	int nIFrameHeadLen = 0;
	int nPFrameHeadLen = 0;
	bool bSpsFlag = false;

	if(	m_lVideoTimeSample == 0 && !m_bVideoSampleArrived)
	{
		m_bVideoSampleArrived = true;

		m_lVideoTimeSample = ulTimeStamp;
		ulTimeStamp        = ulTimeStamp - m_lVideoTimeSample;
		QueryPerformanceCounter(&m_lnSysBeginVideoTimeSample);
	}
	else
	{
		if( (ulTimeStamp + m_lSysDValueVideoTimeSample) <= (m_lVideoTimeSample) )
		{
			LARGE_INTEGER  lnNowTimeStamp;
			QueryPerformanceCounter(&lnNowTimeStamp);
			m_lSysDValueVideoTimeSample = (lnNowTimeStamp.QuadPart - m_lnSysBeginVideoTimeSample.QuadPart )/1000 
				                          + m_lVideoTimeSample - ulTimeStamp;
		}

		ulTimeStamp = (ulTimeStamp + m_lSysDValueVideoTimeSample - m_lVideoTimeSample);
	}

	// AVCDecoderConfigurationRecord
	if (m_bAVCInited == false)
	{	
		hr = GetAVCNalType(in_pBuffer, in_nBufferLen, &eAVCNalType, &nAVCNalLen);
		if(FAILED(hr))
		{
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteVideoSample: GetAVCNalType error,eAVCNalType is %d,nAVCNalLen is %d",eAVCNalType,nAVCNalLen);
		    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr, chError, true);
		}

		if (eAVCNalType == H264NT_SPS)
		{
			nSpsHeadLen = nAVCNalLen;
			m_sFlvFileVideoData.uchFrameType = 0x01;
			m_sFlvFileVideoData.uchCodecID = 0x07;
			m_sFlvFileAVCVideoData.uchAVCPacketType = 0x00;
			m_sFlvFileAVCVideoData.chCompositionTime[0] = 0x00;
			m_sFlvFileAVCVideoData.chCompositionTime[1] = 0x00;
			m_sFlvFileAVCVideoData.chCompositionTime[2] = 0x00;

			// 01 4d 40 1f ff e1 20 14  
			unsigned char uchAVCDecoderConfigurationRecord[1024];
			uchAVCDecoderConfigurationRecord[0] = 0x01;
			uchAVCDecoderConfigurationRecord[1] = 0x42;
			uchAVCDecoderConfigurationRecord[2] = 0xC0;
			uchAVCDecoderConfigurationRecord[3] = 0x15;
			uchAVCDecoderConfigurationRecord[4] = 0x03;
			uchAVCDecoderConfigurationRecord[5] = 0x01;

			/*uchAVCDecoderConfigurationRecord[0] = 0x01;
			uchAVCDecoderConfigurationRecord[1] = 0x4d;
			uchAVCDecoderConfigurationRecord[2] = 0x40;
			uchAVCDecoderConfigurationRecord[3] = 0x1F;
			uchAVCDecoderConfigurationRecord[4] = 0xFF;
			uchAVCDecoderConfigurationRecord[5] = 0xE1;*/

			unsigned short usSpsLen = 0;
			int nIBuffer = nSpsHeadLen;
			for (; nIBuffer<in_nBufferLen - nSpsHeadLen; nIBuffer++)
			{
				if(in_pBuffer[nIBuffer] == 0x00 && in_pBuffer[nIBuffer + 1] == 0x00 && in_pBuffer[nIBuffer + 2] == 0x00 && in_pBuffer[nIBuffer + 3] == 0x01)
				{
					nPpsHeadLen = 4;
					break;
				}
				if(in_pBuffer[nIBuffer] == 0x00 && in_pBuffer[nIBuffer + 1] == 0x00 && in_pBuffer[nIBuffer + 2] == 0x01)
				{
					nPpsHeadLen = 3;
					break;
				}
			}
			if (nIBuffer != in_nBufferLen - nSpsHeadLen)
			{
				usSpsLen = nIBuffer - nSpsHeadLen;
			}

			int nBufferPos = 6;
			usSpsLen = htons(usSpsLen);
			memcpy(uchAVCDecoderConfigurationRecord + nBufferPos, &usSpsLen, sizeof(unsigned short));
			usSpsLen = ntohs(usSpsLen);
			nBufferPos += sizeof(unsigned short);

			memcpy(uchAVCDecoderConfigurationRecord + nBufferPos, in_pBuffer + nSpsHeadLen, usSpsLen);
			nBufferPos += usSpsLen;

			uchAVCDecoderConfigurationRecord[nBufferPos] = 0x01;
			nBufferPos++;

			unsigned short usPpsLen = 0;
			for (nIBuffer= nSpsHeadLen + usSpsLen + nPpsHeadLen; nIBuffer<in_nBufferLen - nSpsHeadLen; nIBuffer++)
			{
				if (   in_pBuffer[nIBuffer]     == 0x00 
					&& in_pBuffer[nIBuffer + 1] == 0x00 
					&& in_pBuffer[nIBuffer + 2] == 0x00 
					&& in_pBuffer[nIBuffer + 3] == 0x01)
				{
					break;
				}

				if (   in_pBuffer[nIBuffer]     == 0x00 
					&& in_pBuffer[nIBuffer + 1] == 0x00 
					&&in_pBuffer[nIBuffer + 2]  == 0x01)
				{
					break;
				}
			}
			if (nIBuffer != in_nBufferLen - nSpsHeadLen)
			{
				usPpsLen = nIBuffer - (nSpsHeadLen + usSpsLen + nPpsHeadLen);
			}

			ePxAVCNalType out_eBufferFrameType = H264NT_NAL;
			int out_nBufferFrameHeadLen = 0;
			int out_nBufferFrameLen = 0;
			int nBufferNextFrameLen = 0;
			while(out_eBufferFrameType != H264NT_SLICE_IDR && out_eBufferFrameType != H264NT_SLICE)
			{
				if( (nSpsHeadLen + usSpsLen + nPpsHeadLen +  nBufferNextFrameLen) < in_nBufferLen)
				{
					hr = GetBufferFrameType(in_pBuffer+nSpsHeadLen + usSpsLen + nPpsHeadLen  + nBufferNextFrameLen,
						                   (in_nBufferLen-(nSpsHeadLen + usSpsLen + nPpsHeadLen + nBufferNextFrameLen)), 
										   &out_eBufferFrameType, &out_nBufferFrameHeadLen, &out_nBufferFrameLen);
					if(FAILED(hr))
					{
						char chError[200];
						sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: GetBufferFrameType error");
						//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr, chError, true);
					}

					switch(out_eBufferFrameType)
					{
					case H264NT_SPS:
						nSpsHeadLen = out_nBufferFrameHeadLen;
						break;
					case H264NT_PPS:
						nPpsHeadLen = out_nBufferFrameHeadLen;
						break;
					case H264NT_SEI:
						nSeiHeadLen = out_nBufferFrameHeadLen;
						break;
					case H264NT_SLICE_IDR:
						nIFrameHeadLen = out_nBufferFrameHeadLen;
						break;
					default:
						nPFrameHeadLen = out_nBufferFrameHeadLen;
						break;
					}

					nBufferNextFrameLen = nBufferNextFrameLen + out_nBufferFrameHeadLen + out_nBufferFrameLen;
				}
				else
				{
					char chError[200];
					sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: (nSpsHeadLen+nBufferNextFrameLen) >= in_nBufferLen");
					//NsLogNotifyA_Add_file(0,0,chError);
					break;
				}
			}	
			
			usPpsLen = htons(usPpsLen);
			memcpy(uchAVCDecoderConfigurationRecord + nBufferPos, &usPpsLen, sizeof(unsigned short));
			usPpsLen = ntohs(usPpsLen);
			nBufferPos += sizeof(unsigned short);
		
			memcpy(uchAVCDecoderConfigurationRecord + nBufferPos, in_pBuffer + nSpsHeadLen + usSpsLen + nPpsHeadLen, usPpsLen);
			nBufferPos += usPpsLen;
			
			
			m_nLastTagSize = htonl(m_nLastTagSize);
			hr = Write((char *)&m_nLastTagSize, sizeof(unsigned int));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, m_nLastTagSize write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			m_nLastTagSize = ntohl(m_nLastTagSize);
			
			m_sFlvFileTagHeader.uchTagType = 0x09;
			unsigned int unTagSize = nBufferPos + sizeof(sPxFLVRecorderVideoData) + sizeof(sPxFLVRecorderAVCVideoData);
			m_sFlvFileTagHeader.uchDataSize[0]  = (unTagSize >> 16) & 0xff;
			m_sFlvFileTagHeader.uchDataSize[1]  = (unTagSize >>  8) & 0xff;
			m_sFlvFileTagHeader.uchDataSize[2]  = (unTagSize >>  0) & 0xff;

			m_sFlvFileTagHeader.uchTimestamp[0] = (BYTE)((ulTimeStamp >> 16) & 0xff);
			m_sFlvFileTagHeader.uchTimestamp[1] = (BYTE)((ulTimeStamp >>  8) & 0xff);
			m_sFlvFileTagHeader.uchTimestamp[2] = (BYTE)((ulTimeStamp >>  0) & 0xff);
			m_sFlvFileTagHeader.uchTimestampExtended = (BYTE)((ulTimeStamp >> 24) & 0xff);

			m_sFlvFileTagHeader.uchStreamID[0]  = 0x00;
			m_sFlvFileTagHeader.uchStreamID[1]  = 0x00;
			m_sFlvFileTagHeader.uchStreamID[2]  = 0x00;
			
			hr = Write((char *)&m_sFlvFileTagHeader, sizeof(SPxFLVRecorderTagHeader));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, SPxFLVRecorderFileTag write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			hr = Write((char *)&m_sFlvFileVideoData, sizeof(sPxFLVRecorderVideoData));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, sPxFLVRecorderVideoData write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			hr = Write((char *)&m_sFlvFileAVCVideoData, sizeof(sPxFLVRecorderAVCVideoData));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, sPxFLVRecorderAVCVideoData write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			hr = Write((char *)uchAVCDecoderConfigurationRecord, nBufferPos);
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, uchAVCDecoderConfigurationRecord write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			m_nLastTagSize = unTagSize + sizeof(SPxFLVRecorderTagHeader);
			in_pBuffer = in_pBuffer + (nSpsHeadLen + usSpsLen + nPpsHeadLen + nBufferNextFrameLen - out_nBufferFrameHeadLen);
			in_nBufferLen = in_nBufferLen - (nSpsHeadLen + usSpsLen + nPpsHeadLen + nBufferNextFrameLen - out_nBufferFrameHeadLen);
			m_bAVCInited = true;
		}
	} // end of 'AVC Sequence header': AVCDecoderConfigurationRecord

	if (m_bAVCInited == true && in_nBufferLen > 4)
	{
		nSpsHeadLen = 0;
		nPpsHeadLen = 0;
		nSeiHeadLen = 0;
		nIFrameHeadLen = 0;
		nPFrameHeadLen = 0;

		hr = GetAVCNalType(in_pBuffer, in_nBufferLen, &eAVCNalType, &nAVCNalLen);
		if(FAILED(hr))
		{
			char chError[200];
			sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: GetAVCNalType error,m_bAVCInited is true,eAVCNalType is %d,nAVCNalLen is %d",eAVCNalType,nAVCNalLen);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr, chError, true);
		}

		if (H264NT_SPS == eAVCNalType || H264NT_SEI == eAVCNalType)
		{
			nSpsHeadLen = nAVCNalLen;

			ePxAVCNalType out_eBufferFrameType = H264NT_NAL;
			int out_nBufferFrameHeadLen = 0;
			int out_nBufferFrameLen = 0;
			int nBufferNextFrameLen = 0;

			while(out_eBufferFrameType != H264NT_SLICE_IDR && out_eBufferFrameType != H264NT_SLICE)
			{
				if( (nSpsHeadLen + nBufferNextFrameLen) < in_nBufferLen)
				{
					hr = GetBufferFrameType(in_pBuffer + nSpsHeadLen + nBufferNextFrameLen,
						                   (in_nBufferLen - nSpsHeadLen - nBufferNextFrameLen), 
										   &out_eBufferFrameType, &out_nBufferFrameHeadLen, &out_nBufferFrameLen);
					if(FAILED(hr))
					{
						char chError[200];
						sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: GetBufferFrameType error");
						//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr, chError, true);
					}
					switch(out_eBufferFrameType)
					{
					case H264NT_SPS:
						nSpsHeadLen = out_nBufferFrameHeadLen;
						break;
					case H264NT_PPS:
						nPpsHeadLen = out_nBufferFrameHeadLen;
						break;
					case H264NT_SEI:
						nSeiHeadLen = out_nBufferFrameHeadLen;
						break;
					case H264NT_SLICE_IDR:
						nIFrameHeadLen = out_nBufferFrameHeadLen;
						break;
					default:
						nPFrameHeadLen = out_nBufferFrameHeadLen;
						break;
					}

					nBufferNextFrameLen = nBufferNextFrameLen + out_nBufferFrameHeadLen + out_nBufferFrameLen;
				}
				else
				{
					char chError[200];
					sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: (nSpsHeadLen+nBufferNextFrameLen) >= in_nBufferLen");
					//NsLogNotifyA_Add_file(0,0,chError);
					break;
				}
			}

			char chBufferHeader[4];
			unsigned int unTagSize = 0;
			if(out_eBufferFrameType == H264NT_SLICE_IDR || out_eBufferFrameType == H264NT_SLICE)
			{
				int nDataSize = (in_nBufferLen - nSpsHeadLen - nBufferNextFrameLen);
				chBufferHeader[0] = (nDataSize >> 24) & 0xff;
				chBufferHeader[1] = (nDataSize >> 16) & 0xff;
				chBufferHeader[2] = (nDataSize >>  8) & 0xff;
				chBufferHeader[3] = (nDataSize >>  0) & 0xff;
				unTagSize = nDataSize + 4 + sizeof(sPxFLVRecorderVideoData) + sizeof(sPxFLVRecorderAVCVideoData);
				//数据总长度-开头NAL-数据长度（4）-video-acv
			}
			else
			{
				hr = NS_E_INVALID_PARAMETER;
				char chError[200];
				sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: NS_E_INVALID_PARAMETER,out_eBufferFrameType=%d", out_eBufferFrameType);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowDeviceHK, keLogPkgDirectShowDeviceHKFuncGeneral, hr, chError, true);
			}
			m_sFlvFileTagHeader.uchTagType = 0x09;
			m_sFlvFileTagHeader.uchDataSize[0] = (unTagSize >> 16) & 0xff;
			m_sFlvFileTagHeader.uchDataSize[1] = (unTagSize >>  8) & 0xff;
			m_sFlvFileTagHeader.uchDataSize[2] = (unTagSize >>  0) & 0xff;

			m_sFlvFileTagHeader.uchTimestamp[0] = (BYTE)((ulTimeStamp >> 16) & 0xff);
			m_sFlvFileTagHeader.uchTimestamp[1] = (BYTE)((ulTimeStamp >>  8) & 0xff);
			m_sFlvFileTagHeader.uchTimestamp[2] = (BYTE)((ulTimeStamp >>  0) & 0xff);
			m_sFlvFileTagHeader.uchTimestampExtended = (BYTE)((ulTimeStamp >> 24) & 0xff);

			// test by gzl 20171019
			unsigned int uiTimeStamp = 0;
			uiTimeStamp =   (m_sFlvFileTagHeader.uchTimestampExtended << 24)
				+ (m_sFlvFileTagHeader.uchTimestamp[0]      << 16)
				+ (m_sFlvFileTagHeader.uchTimestamp[1]      << 8) 
				+ (m_sFlvFileTagHeader.uchTimestamp[2]      << 0);
			char szMsgBuffer[1024] = {0};
			sprintf_s(szMsgBuffer, 1024, "CPxFLVMuxer::WriteVideoSample SPS &&& Video uiTimeStamp:%u, FrameType:SPS", 
				      uiTimeStamp);
			OutputDebugStringA(szMsgBuffer);
			// test by gzl end

			if(out_eBufferFrameType == H264NT_SLICE_IDR)
			{
				m_sFlvFileVideoData.uchFrameType = 0x01;
			}
			else if(out_eBufferFrameType == H264NT_SLICE)
			{
				m_sFlvFileVideoData.uchFrameType = 0x02;
			}
			m_sFlvFileVideoData.uchCodecID = 0x07;
			m_sFlvFileAVCVideoData.uchAVCPacketType = 0x01;
			
			m_nLastTagSize = htonl(m_nLastTagSize);
			hr = Write((char *)&m_nLastTagSize, sizeof(unsigned int));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, m_nLastTagSize write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			m_nLastTagSize = ntohl(m_nLastTagSize);


			hr = Write((char *)&m_sFlvFileTagHeader, sizeof(SPxFLVRecorderTagHeader));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, SPxFLVRecorderFileTag write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			hr = Write((char *)&m_sFlvFileVideoData, sizeof(sPxFLVRecorderVideoData));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, sPxFLVRecorderVideoData write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			hr = Write((char *)&m_sFlvFileAVCVideoData, sizeof(sPxFLVRecorderAVCVideoData));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, sPxFLVRecorderAVCVideoData write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			hr = Write(chBufferHeader, 4);
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, chBufferHeader write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
			hr = Write((char *)(in_pBuffer + nSpsHeadLen + nBufferNextFrameLen), (in_nBufferLen - (nSpsHeadLen + nBufferNextFrameLen)));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, i frame or p frame buffer write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}

			m_nLastTagSize = unTagSize + sizeof(SPxFLVRecorderTagHeader);
			m_dFLVFileDuration = ulTimeStamp;
	}
	else if ( eAVCNalType == H264NT_SLICE_IDR ||  eAVCNalType == H264NT_SLICE)
	{
		nSpsHeadLen = nAVCNalLen;

		ePxAVCNalType out_eBufferFrameType = eAVCNalType;
		int nIFrameHeadLen = 0;
		int nPFrameHeadLen = 0;
		
		if (in_pBuffer[0] == 0x00 && in_pBuffer[1] == 0x00 &&	in_pBuffer[2] == 0x00 && in_pBuffer[3] == 0x01)
		{
			if(out_eBufferFrameType == H264NT_SLICE_IDR)
			{
				nIFrameHeadLen = 4;
			}
			else if(out_eBufferFrameType == H264NT_SLICE)
			{
				nPFrameHeadLen = 4;
			}
		}
		else if (in_pBuffer[0] == 0x00 && in_pBuffer[1] == 0x00 &&	in_pBuffer[2] == 0x01)
		{
			if(out_eBufferFrameType == H264NT_SLICE_IDR)
			{
				nIFrameHeadLen = 3;
			}
			else if(out_eBufferFrameType == H264NT_SLICE)
			{
				nPFrameHeadLen = 3;
			}
		}
		else
		{
			hr = NS_E_INVALID_PARAMETER;
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowDeviceHK, keLogPkgDirectShowDeviceHKFuncGeneral, hr, 
				//"CCPxFLVMuxer::WriteVideoSample: NS_E_INVALID_PARAMETER,in_pBuffer value is invalid", true);
		}
	
		char chBufferHeader[4];
		unsigned int unTagSize = 0;
		if(out_eBufferFrameType == H264NT_SLICE_IDR )
		{
			chBufferHeader[0] = ((in_nBufferLen - nIFrameHeadLen ) >> 24) & 0xff;
			chBufferHeader[1] = ((in_nBufferLen - nIFrameHeadLen)  >> 16) & 0xff;
			chBufferHeader[2] = ((in_nBufferLen - nIFrameHeadLen)  >>  8) & 0xff;
			chBufferHeader[3] = ((in_nBufferLen - nIFrameHeadLen)  >>  0) & 0xff;
			unTagSize = in_nBufferLen - nIFrameHeadLen + 4 + sizeof(sPxFLVRecorderVideoData) + sizeof(sPxFLVRecorderAVCVideoData);//数据总长度-开头NAL-数据长度（4）-video-acv

		}
		else if( out_eBufferFrameType == H264NT_SLICE )
		{

			chBufferHeader[0] = ((in_nBufferLen - nPFrameHeadLen) >> 24) & 0xff;
			chBufferHeader[1] = ((in_nBufferLen - nPFrameHeadLen) >> 16) & 0xff;
			chBufferHeader[2] = ((in_nBufferLen - nPFrameHeadLen) >>  8) & 0xff;
			chBufferHeader[3] = ((in_nBufferLen - nPFrameHeadLen) >>  0) & 0xff;
			unTagSize = in_nBufferLen - nPFrameHeadLen + 4 + sizeof(sPxFLVRecorderVideoData) + sizeof(sPxFLVRecorderAVCVideoData);//数据总长度-开头NAL-数据长度（4）-video-acv

		}
		else
		{
			hr = NS_E_INVALID_PARAMETER;
			char chError[200];
			sprintf( chError,"CCPxFLVMuxer::WriteVideoSample: NS_E_INVALID_PARAMETER,out_eBufferFrameType=%d", out_eBufferFrameType);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowDeviceHK, keLogPkgDirectShowDeviceHKFuncGeneral, hr, chError, true);
		}

		m_sFlvFileTagHeader.uchTagType = 0x09;
		m_sFlvFileTagHeader.uchDataSize[0] = (unTagSize >> 16) & 0xff;
		m_sFlvFileTagHeader.uchDataSize[1] = (unTagSize >>  8) & 0xff;
		m_sFlvFileTagHeader.uchDataSize[2] = (unTagSize >>  0) & 0xff;
		m_sFlvFileTagHeader.uchTimestamp[0] = (BYTE)((ulTimeStamp >> 16) & 0xff);
		m_sFlvFileTagHeader.uchTimestamp[1] = (BYTE)((ulTimeStamp >>  8) & 0xff);
		m_sFlvFileTagHeader.uchTimestamp[2] = (BYTE)((ulTimeStamp >>  0) & 0xff);
		m_sFlvFileTagHeader.uchTimestampExtended = (BYTE)((ulTimeStamp >> 24) & 0xff);

		// test by gzl 20171019
		unsigned int uiTimeStamp = 0;
		uiTimeStamp =   (m_sFlvFileTagHeader.uchTimestampExtended << 24)
			+ (m_sFlvFileTagHeader.uchTimestamp[0]      << 16)
			+ (m_sFlvFileTagHeader.uchTimestamp[1]      << 8) 
			+ (m_sFlvFileTagHeader.uchTimestamp[2]      << 0);
		char szMsgBuffer[1024] = {0};
		sprintf_s(szMsgBuffer, 1024, "CPxFLVMuxer::WriteVideoSample &&& Video uiTimeStamp:%u, FrameType:%s", uiTimeStamp,
			out_eBufferFrameType == H264NT_SLICE_IDR ? "KeyFrame" : "P or B Frame");
		OutputDebugStringA(szMsgBuffer);
		// test by gzl end

		if(out_eBufferFrameType == H264NT_SLICE_IDR)
		{
			m_sFlvFileVideoData.uchFrameType = 0x01;
		}
		else if(out_eBufferFrameType == H264NT_SLICE)
		{
			m_sFlvFileVideoData.uchFrameType = 0x02;
		}
		m_sFlvFileVideoData.uchCodecID = 0x07;
		m_sFlvFileAVCVideoData.uchAVCPacketType = 0x01;

		m_nLastTagSize = htonl(m_nLastTagSize);
		hr = Write((char *)&m_nLastTagSize, sizeof(unsigned int));
		if(FAILED(hr))
		{
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, m_nLastTagSize write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		m_nLastTagSize = ntohl(m_nLastTagSize);

		hr = Write((char *)&m_sFlvFileTagHeader, sizeof(SPxFLVRecorderTagHeader));
		if(FAILED(hr))
		{
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, SPxFLVRecorderFileTag write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		hr = Write((char *)&m_sFlvFileVideoData, sizeof(sPxFLVRecorderVideoData));
		if(FAILED(hr))
		{
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, sPxFLVRecorderVideoData write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		hr = Write((char *)&m_sFlvFileAVCVideoData, sizeof(sPxFLVRecorderAVCVideoData));
		if(FAILED(hr))
		{
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, sPxFLVRecorderAVCVideoData write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		hr = Write(chBufferHeader, 4);
		if(FAILED(hr))
		{
			char chError[200];
			sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, chBufferHeader write fail,File Name is %s",m_szVideoFileName);
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//chError, true);
		}
		if(out_eBufferFrameType == H264NT_SLICE_IDR)
		{
			hr = Write((char *)(in_pBuffer + nIFrameHeadLen), (in_nBufferLen - nIFrameHeadLen));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, i frame  buffer write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
		}
		else if(out_eBufferFrameType == H264NT_SLICE)
		{
			hr = Write((char *)(in_pBuffer + nPFrameHeadLen), (in_nBufferLen - nPFrameHeadLen));
			if(FAILED(hr))
			{
				char chError[200];
				sprintf( chError,"CPxFLVMuxer::WriteVideoSample: Write error, p frame buffer write fail,File Name is %s",m_szVideoFileName);
				//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
					//chError, true);
			}
		}
		
		m_nLastTagSize = unTagSize + sizeof(SPxFLVRecorderTagHeader);
		//m_dwDurationPosition = in_nTimeStamp;
		m_dFLVFileDuration = ulTimeStamp;
	   
	    }
	}
	else
	{
		hr = NS_E_INVALID_PARAMETER;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::WriteVideoSample: NS_E_INVALID_PARAMETER,in_nBufferLen<4 in_nBufferLen = %d ",in_nBufferLen);
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr, chError, true);
	}

	return hr;
}

HRESULT CPxFLVMuxer::FlushFile(void)
{
	HRESULT hr = NS_NOERROR;
	//CNsAutoLock lock(&m_oCritDevice);
	if (m_fpFLVFile)
	{
		fflush(m_fpFLVFile);
		m_fpFLVFile = NULL;
	}

	return hr;
}

HRESULT CPxFLVMuxer::Write(char *in_pBuffer, int in_nBufferLen)
{
	HRESULT hr = NS_NOERROR;

	if( in_pBuffer == NULL || in_nBufferLen < 0 )
	{
		hr = NS_E_INVALID_PARAMETER;
		char chError[200];
		sprintf( chError,"CPxFLVMuxer::Write: NS_E_INVALID_PARAMETER,in_pBuffer is NULL or in_nBufferLen<0 ");
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr, chError, true);
	}

	size_t stWritten = 0;
	while(in_nBufferLen)
	{
		int nRet = fwrite(in_pBuffer + stWritten, 1, in_nBufferLen, m_fpFLVFile);
		if (nRet < in_nBufferLen)
		{
			//NsLogNotifyA_Add_file(0,0,"m_Ret < in_nBufferLen,m_Ret=%d, in_nBufferLen =%d",nRet,in_nBufferLen);
		}
		stWritten += nRet;
		in_nBufferLen -= nRet;
	}

	return hr;
}


HRESULT CPxFLVMuxer::GetAVCNalType(LPVOID in_pBSBuf, const LONG in_nBSLen, ePxAVCNalType *out_eAVCNalType, int *out_nAVCNalLen)
{
	HRESULT hr = NS_NOERROR;
	if( in_pBSBuf  == NULL || in_nBSLen < 5 || out_eAVCNalType== NULL || out_nAVCNalLen ==NULL )
	{
		hr = NS_E_INVALID_PARAMETER;
	    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		    //"CPxFLVMuxer::GetAVCNalType: NS_E_INVALID_PARAMETER,in_pBSBuf is NULL or in_nBSLen <5 or out_nAVCNalType== NULL or out_nAVCNalLen ==NULL", true);
	}

	int nAVCNalType = -1;
	unsigned char* pBSBufTemp = (unsigned char*)in_pBSBuf;
	unsigned long ulNalFlag = pBSBufTemp[2] & 0x1F; 
	if(ulNalFlag == 1)
	{
		*out_nAVCNalLen = 3;
		unsigned long ulAVCNalType = pBSBufTemp[3] & 0x1F;  
		if ( ulAVCNalType <= H264NT_PPS )
		{
			nAVCNalType = ulAVCNalType;
			//*out_nAVCNalType = ulAVCNalType;
		}
		else
		{
			hr = NS_E_INVALID_PARAMETER;
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//"CPxFLVMuxer::GetAVCNalType: NS_E_INVALID_PARAMETER,ulAVCNalType is invalid", true);
		}
	}
	else if(ulNalFlag == 0)
	{
		*out_nAVCNalLen = 4;
		unsigned long ulAVCNalType = pBSBufTemp[4] & 0x1F; 
		if ( ulAVCNalType <= H264NT_PPS )
		{
			nAVCNalType = ulAVCNalType;
			//*out_nAVCNalType = ulAVCNalType;
		}
		else
		{
			hr = NS_E_INVALID_PARAMETER;
			//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
				//"CPxFLVMuxer::GetAVCNalType: NS_E_INVALID_PARAMETER,ulAVCNalType is invalid", true);
		}
	}
	else
	{
		hr = NS_E_INVALID_PARAMETER;
	    //NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		    //"CPxFLVMuxer::GetAVCNalType: NS_E_INVALID_PARAMETER,in_pBSBuf is invalid", true);
	}
	switch(nAVCNalType)
	{
	case 0:
		*out_eAVCNalType = H264NT_NAL;
		break;
	case 1:
		*out_eAVCNalType = H264NT_SLICE;
		break;
	case 2:
		*out_eAVCNalType = H264NT_SLICE_DPA;
		break;
	case 3:
		*out_eAVCNalType = H264NT_SLICE_DPB;
		break;
	case 4:
		*out_eAVCNalType = H264NT_SLICE_DPC;
		break;
	case 5:
		*out_eAVCNalType = H264NT_SLICE_IDR;
		break;
	case 6:
		*out_eAVCNalType = H264NT_SEI;
		break;
	case 7:
		*out_eAVCNalType = H264NT_SPS;
		break;
	case 8:
		*out_eAVCNalType = H264NT_PPS;
		break;
	default:
		*out_eAVCNalType = H264NT_NAL;
		break;
	}
	return hr;

}

HRESULT CPxFLVMuxer::GetBufferFrameType(unsigned char *in_pBuffer, int in_nBufferLen, ePxAVCNalType *out_eBufferFrameType, int *out_nBufferFrameHeadLen, int *out_nBufferFrameLen)
{
	HRESULT hr = NS_NOERROR;

	if( in_pBuffer  == NULL || in_nBufferLen < 5)
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::GetBufferFrameType: NS_E_INVALID_PARAMETER,in_pBSBuf is NULL or in_nBSLen <5", true);
	}

	int nIBuffer = 0;
	for( ; nIBuffer < in_nBufferLen - 4 ; nIBuffer++)
	{
		if ((in_pBuffer[nIBuffer] == 0x00 && in_pBuffer[nIBuffer + 1] == 0x00 && in_pBuffer[nIBuffer + 2] == 0x00 && in_pBuffer[nIBuffer + 3] == 0x01))
		{
			*out_nBufferFrameHeadLen = 4;
			switch(in_pBuffer[nIBuffer + 4])
			{
			case 0x67:
				*out_eBufferFrameType = H264NT_SPS;
				break;
			case 0x68:
				*out_eBufferFrameType = H264NT_PPS;
				break;
			case 0x65:
				*out_eBufferFrameType = H264NT_SLICE_IDR;
				break;
			case 0x66:
				*out_eBufferFrameType = H264NT_SEI;
				break;
			default:
				*out_eBufferFrameType = H264NT_SLICE;
				break;		
			}

			break;
		}

		if ((in_pBuffer[nIBuffer] == 0x00 && in_pBuffer[nIBuffer + 1] == 0x00 && in_pBuffer[nIBuffer + 2] == 0x01))
		{
			*out_nBufferFrameHeadLen = 3;
			switch(in_pBuffer[nIBuffer + 3])
			{
			case 0x67:
				*out_eBufferFrameType = H264NT_SPS;
				break;
			case 0x68:
				*out_eBufferFrameType = H264NT_PPS;
				break;
			case 0x65:
				*out_eBufferFrameType = H264NT_SLICE_IDR;
				break;
			case 0x66:
				*out_eBufferFrameType = H264NT_SEI;
				break;
			default:
				*out_eBufferFrameType = H264NT_SLICE;
				break;		
			}

			break;
		}
	}

	if(nIBuffer == (in_nBufferLen - 4))
	{
		hr = NS_E_INVALID_PARAMETER;
		//NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			//"CPxFLVMuxer::GetBufferFrameType: NS_E_INVALID_PARAMETER,nIBuffer == (in_nBufferLen - 4)", true);
	}
	else
	{
		*out_nBufferFrameLen = nIBuffer ;
	}

	return hr;
}

HRESULT CPxFLVMuxer::SetStreamProperty(SPxRecordStreamProperty *in_psFileStreamProperty)
{
	HRESULT hr = NS_NOERROR;

	if (NULL == in_psFileStreamProperty)
	{
		return NS_E_FAIL;
	}

	m_sRecordStreamProperty = *in_psFileStreamProperty;

	return hr;
}
