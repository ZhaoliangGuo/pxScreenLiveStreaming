#include "StdAfx.h"
#include "pxFLVRecorder.h"
#include "pxErrorDef.h"
#include "CPxFLVMuxer.h"

CPxFLVRecorder::CPxFLVRecorder(void)
{
	m_nLastVideoTimestamp = 0;
	m_nLastAudioTimestamp = 0;
}

CPxFLVRecorder::~CPxFLVRecorder(void)
{
	if(m_oPxFLVMuxer)
	{
		delete m_oPxFLVMuxer;
		m_oPxFLVMuxer=NULL;
	}
}

HRESULT CPxFLVRecorder::SetFileName( char *in_pszFileName )
{
	HRESULT hr = NS_NOERROR;

	//NsLogNotifyA_Add_file(0,0,"CPxFLVRecorder::SetFileName, BEGIN! ");

	/*if( in_pwszFileName == NULL )
	{	
		hr = NS_E_INVALID_PARAMETER;
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::SetFileName: NS_E_INVALID_PARAMETER,in_pwszFileName is NULL", true);
	}*/

	m_oPxFLVMuxer = new CPxFLVMuxer();
	if (m_oPxFLVMuxer == NULL)
	{
		hr = NS_E_OUT_OF_MEMORY;
		/*NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::SetFileName: NS_E_OUT_OF_MEMORY,new CPxFLVMuxer(", true);*/
	}

	hr = m_oPxFLVMuxer->AddFile(in_pszFileName);
	/*NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		"CPxFLVRecorder::SetFileName: PX_E_PXFLVRECORDER_ADDFILE_ERROR", true);*/

	/*NsLogNotifyA_Add_file(0,0,"CPxFLVRecorder::SetFileName, SUCCEEDED! ");*/

	return hr;
}

//HRESULT __stdcall CPxFLVRecorder::BeginSetFileProperty()
//{
//	HRESULT hr = NS_NOERROR;
//	NsLogNotifyA_Add_file(0,0,"CPxFLVRecorder::BeginSetFileProperty! ");
//	m_sFileProperty.eFileType = keRecordFileType_Invliad;
//	m_sFileProperty.aStreamProperty.empty();
//	return hr;
//}
//
//HRESULT __stdcall CPxFLVRecorder::SetFileProperty_FileType(EPxRecordFileType in_eFileType)
//{
//	HRESULT hr = NS_NOERROR;
//	NsLogNotifyA_Add_file(0,0,"CPxFLVRecorder::SetFileProperty_FileType! ");
//	m_sFileProperty.eFileType = in_eFileType;
//	return hr;
//}
//
HRESULT CPxFLVRecorder::SetStreamProperty(SPxRecordStreamProperty *in_psFileStreamProperty)
{
	HRESULT hr = NS_NOERROR;
	if (in_psFileStreamProperty == NULL)
	{
		hr = NS_E_INVALID_PARAMETER;
		/*NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::AddFileProperty_StreamProperty: NS_E_INVALID_PARAMETER,in_psFileStreamProperty is NULL", true);*/
	}

	/*NsLogNotifyA_Add_file(0,0, 
		"CPxFLVRecorder::AddFileProperty_StreamProperty! bHasVideo: %d, bHasAudio:%d, fVideoFrameRate:%d,\n \
		nVideoHeight:%d, nVideoWidth:%d, eAudioSamplesPerSecond:%d, nAudioChannels:%d, nAudioBitsPerSampleint:%d.\n",
		in_psFileStreamProperty->bHasVideo ? 1 : 0,
		in_psFileStreamProperty->bHasAudio ? 1 : 0,
		in_psFileStreamProperty->fVideoFrameRate,
		in_psFileStreamProperty->nVideoHeight,
		in_psFileStreamProperty->nVideoWidth,
		in_psFileStreamProperty->eAudioSamplesPerSecond,
		in_psFileStreamProperty->nAudioChannels,
		in_psFileStreamProperty->nAudioBitsPerSampleint);*/

	m_oPxFLVMuxer->SetStreamProperty(in_psFileStreamProperty);

	return hr;
}

HRESULT CPxFLVRecorder::Begin()
{
	HRESULT hr = NS_NOERROR;

	hr = m_oPxFLVMuxer->BeginWriteFile();
	/*NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		"CPxFLVRecorder::Begin: PX_E_PXFLVRECORDER_BEGINWRITRFILR_ERROR", true);*/

	return hr;
}

HRESULT CPxFLVRecorder::ReceiveVideoData(
	int in_nStreamID,
	char *pchIP,
	SPxBuffer *in_pIMC
	)
{
	HRESULT hr = NS_NOERROR;

	/*if(in_nStreamID < 0)
	{
		hr = NS_E_INVALID_PARAMETER;	
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::ReceiveVideoData: NS_E_INVALID_PARAMETER, in_nStreamID < 0", true);
	}

	if(pchIP == NULL)
	{
		hr = NS_E_INVALID_PARAMETER;	
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::ReceiveVideoData: NS_E_INVALID_PARAMETER, pchIP == NULL", true);
	}	

	if(in_pIMC == NULL)
	{
		hr = NS_E_INVALID_PARAMETER;	
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::ReceiveVideoData: NS_E_INVALID_PARAMETER, in_pIMC == NULL", true);
	}	*/

	int nTimeStamp = static_cast <int> (in_pIMC->ui64Timestamp);

	if (0 == m_nLastVideoTimestamp)
	{
		m_nLastVideoTimestamp = nTimeStamp;
	} 

	int nDeltaTs = nTimeStamp - m_nLastVideoTimestamp;

	char szMsgBuffer[1024] = {0};
	sprintf_s(szMsgBuffer, 1024, "CPxFLVRecorder::ReceiveVideoData timestamp: %d, VideoTimestamp:%d, nDeltaTs: %d ms\n", nTimeStamp, m_nLastVideoTimestamp, nDeltaTs);
	OutputDebugStringA(szMsgBuffer);

	hr = m_oPxFLVMuxer->WriteVideoSample(in_nStreamID, 
	in_pIMC->lpBuffer,
	in_pIMC->nDataLength,
	in_pIMC->bVideoKeyFrame, 
	nTimeStamp);

	m_nLastVideoTimestamp = nTimeStamp;

	return hr;
}

HRESULT CPxFLVRecorder::ReceiveAudioData(
	int in_nStreamID,
	char *pchIP,
	SPxBuffer *in_pIMC
	)
{
	HRESULT hr = NS_NOERROR;

	/*if( in_nStreamID < 0 )
	{
		hr = NS_E_INVALID_PARAMETER;	
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::ReceiveAudioData: NS_E_INVALID_PARAMETER, in_nStreamID < 0", true);
	}

	if( pchIP == NULL )
	{
		hr = NS_E_INVALID_PARAMETER;	
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::ReceiveAudioData: NS_E_INVALID_PARAMETER, pchIP == NULL", true);
	}

	if( in_pIMC == NULL )
	{
		hr = NS_E_INVALID_PARAMETER;	
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
			"CPxFLVRecorder::ReceiveAudioData: NS_E_INVALID_PARAMETER, in_pIMC == NULL", true);
	}*/

	int nTimeStamp = static_cast <int> (in_pIMC->ui64Timestamp);

	if (0 == m_nLastAudioTimestamp)
	{
		m_nLastAudioTimestamp = nTimeStamp;
	}

	int nDeltaTs = nTimeStamp - m_nLastAudioTimestamp;

	char szMsgBuffer[1024] = {0};
	sprintf_s(szMsgBuffer, 1024, "CPxFLVRecorder::ReceiveAudioData timestamp: %d, nDeltaTs: %d ms\n", nTimeStamp, nDeltaTs);
	//OutputDebugStringA(szMsgBuffer);

	hr = m_oPxFLVMuxer->WriteAudioSample(in_nStreamID, 
		                                 in_pIMC->lpBuffer, 
		                                 in_pIMC->nDataLength, 
		                                 nTimeStamp);

	m_nLastAudioTimestamp = nTimeStamp;

	return hr;
}

HRESULT CPxFLVRecorder::End()
{
	HRESULT hr = NS_NOERROR;
	hr = m_oPxFLVMuxer->EndWriteFile();
	/*NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		"CPxFLVRecorder::End: PX_E_PXFLVRECORDER_ENDWRITEFILE_ERROR", true);*/

	return hr;
}

HRESULT CPxFLVRecorder::Close()
{
	HRESULT hr = NS_NOERROR;
	hr = m_oPxFLVMuxer->CloseFile();
	/*NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgDirectShowFLVRecoder, keLogPkgDirectShowFLVRecoderFuncGeneral, hr,
		"CPxFLVRecorder::Close: PX_E_PXFLVRECORDER_DELFILE_ERROR", true);*/

	return hr;
}
