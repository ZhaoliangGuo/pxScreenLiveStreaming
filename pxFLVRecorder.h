#pragma once
#include "pxCommonDef.h"
#include "pxRecordDef.h"

//struct SPxBuffer;
class CPxFLVMuxer;

class CPxFLVRecorder
{
public:
	CPxFLVRecorder(void);
	~CPxFLVRecorder(void);

public:
	HRESULT SetFileName(char *in_pszFileName);

	HRESULT Begin();
	HRESULT ReceiveVideoData(int in_nStreamID, char *pchIP, SPxBuffer *in_pIMC);
	HRESULT ReceiveAudioData(int in_nStreamID, char *pchIP, SPxBuffer *in_pIMC);
	HRESULT End();

	HRESULT Close();

	HRESULT SetStreamProperty(SPxRecordStreamProperty *in_psFileStreamProperty);

private:
	CPxFLVMuxer        *m_oPxFLVMuxer;
	bool				m_bRecording;

private:
	int m_nLastVideoTimestamp;
	int m_nLastAudioTimestamp;
};

