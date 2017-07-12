#pragma once
#include "pxCommonDef.h"

//struct SPxBuffer;
class CPxFLVMuxer;

class CPxFLVRecorder
{
public:
	CPxFLVRecorder(void);
	~CPxFLVRecorder(void);

public:
	HRESULT SetFileName(char *in_pszFileName);
	//virtual HRESULT __stdcall SetFileProperty(SPxRecordFileProperty *in_psFileProperty);
	/*virtual HRESULT __stdcall BeginSetFileProperty();
	virtual HRESULT __stdcall SetFileProperty_FileType(EPxRecordFileType in_eFileType);
	virtual HRESULT __stdcall AddFileProperty_StreamProperty(SPxRecordStreamProperty *in_psFileStreamProperty);
	virtual HRESULT __stdcall EndSetFileProperty();
	virtual HRESULT __stdcall SetErrorNotifyInterface(IPxRecordErrorNotify *in_pIErrorNotify);*/
	HRESULT Begin();
	HRESULT ReceiveVideoData(
		int in_nStreamID,
		char *pchIP,
		SPxBuffer *in_pIMC
		);
	HRESULT ReceiveAudioData(
		int in_nStreamID,
		char *pchIP,
		SPxBuffer *in_pIMC
		);
	//virtual HRESULT __stdcall Switch(uint64_t in_ui64SwitchAt);
	HRESULT End();

	HRESULT Close();

	/*HRESULT GetRecordErrorNotify(IPxRecordErrorNotify **out_pIIErrorNotify);

	virtual HRESULT __stdcall SetRecordParam(SPxLiveStreamRecordParam *in_psRecordParam);
	virtual HRESULT __stdcall GetRecordParam(SPxLiveStreamRecordParam &out_psRecordParam);*/

private:
	/*TNsSmartPtr<IPxRecordErrorNotify> m_pJRecordErrorNotify;
	SPxRecordFileProperty m_sFileProperty;*/
	CPxFLVMuxer       *m_oPxFLVMuxer;
	/*SPxLiveStreamRecordParam m_sRecordParam;*/
	bool				m_bRecording;
};

