#pragma once
#include "pxBufferDef.h"
#include <queue>
using namespace std;

class CPxQueueBuffer
{
public:
	CPxQueueBuffer(void);
	~CPxQueueBuffer(void);

public:
	void      Push(SPxBuffer *in_psBuffer);
	SPxBuffer Front();
	void      Pop();
	int       Size();
	bool      IsEmpty();

public:
	queue <SPxBuffer> m_qBufferList;
	CRITICAL_SECTION  m_csBufferList;
};

