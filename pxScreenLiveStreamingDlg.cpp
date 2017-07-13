
// pxScreenLiveStreamingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pxScreenLiveStreaming.h"
#include "pxScreenLiveStreamingDlg.h"
#include "afxdialogex.h"
#include "pxCommonDef.h"
#include "pxFLVRecorder.h"
//#include "pxFLVCommonfDef.h"

char szAVCSequenceHeader[AVC_SEQUENCE_HEADER_LEN];
char szAACSequenceHeader[AAC_SEQUENCE_HEADER_LEN];

static WAVEFORMATEX g_sWaveFormat;

extern "C"
{
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ThreadVideoEncoder(LPVOID lp);
DWORD WINAPI ThreadVideoCaptureTest(LPVOID lp);
DWORD WINAPI ThreadWriteTest(LPVOID lp);
DWORD WINAPI ThreadAudioEncoder(LPVOID lp);

void CALLBACK waveInProc( 
	HWAVEIN hwi,    //声音输入设备句柄
	UINT uMsg,      //产生的消息，由系统给出
	DWORD dwInstance,//在waveinopen中给出要传递给该函数的数据
	DWORD dwParam1, //附加数据１
	DWORD dwParam2  //附加数据２
	);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpxScreenLiveStreamingDlg 对话框




CPxScreenLiveStreamingDlg::CPxScreenLiveStreamingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPxScreenLiveStreamingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pBitmapBuffer = NULL;
}

CPxScreenLiveStreamingDlg::~CPxScreenLiveStreamingDlg()
{
	g_oCodedBufferPool.ReleaseBufferPool();
}


void CPxScreenLiveStreamingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPxScreenLiveStreamingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CPxScreenLiveStreamingDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPxScreenLiveStreamingDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CpxScreenLiveStreamingDlg 消息处理程序

BOOL CPxScreenLiveStreamingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	Init();

	// TODO: 在此添加额外的初始化代码

	/*int nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);*/
	int nYUVBufferSize = m_nScreenWidth * m_nScreenHeight * 1.5;

	g_oYUVBufferPool.InitBufferPool(nYUVBufferSize, 100);
	g_oCodedBufferPool.InitBufferPool(1024*1024);

	DWORD dwVideoCaptureThreadId;

	HANDLE hVideoCaptureThread = CreateThread(NULL, NULL, ThreadVideoCaptureTest, this, 0, &dwVideoCaptureThreadId);

	DWORD dwThreadId;

	HANDLE hThread = CreateThread(NULL, NULL, ThreadWriteTest, this, 0, &dwThreadId);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPxScreenLiveStreamingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPxScreenLiveStreamingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPxScreenLiveStreamingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool g_bStop         = false;
bool g_bVideoDataFinished = false;

void CPxScreenLiveStreamingDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bStop         = false;
	g_bVideoDataFinished = false;

	DWORD dwVideoThreadId;

	HANDLE hThread = CreateThread(NULL, NULL, ThreadVideoEncoder, this, 0, &dwVideoThreadId);

	DWORD dwAudioThreadId;
	HANDLE hAudioEncodeThread = CreateThread(NULL, NULL, ThreadAudioEncoder, this, 0, &dwAudioThreadId);
}


void CPxScreenLiveStreamingDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bStop = true;
}


DWORD WINAPI ThreadVideoCaptureTest(LPVOID lp)
{

	return 0;
}

DWORD WINAPI ThreadWriteTest(LPVOID lp)
{
	CPxFLVRecorder oFLVRecorder;
	char *szFLVFileName = "output.flv";
	oFLVRecorder.SetFileName(szFLVFileName);
	oFLVRecorder.Begin();

	while(true)
	{
		while (!g_qCodedBufferList.empty())
		{
			SPxBuffer sPxBuffer = g_qCodedBufferList.front();

#if VIDEO_SAVE_H264_FROM_BUFFERLIST
			FILE *fpH264File = fopen("output_v2.h264", "ab+");

			fwrite(sPxBuffer.lpBuffer, 1, sPxBuffer.nDataLength, fpH264File);

			if (fpH264File)
			{
				fclose(fpH264File);
				fpH264File = NULL;
			}
#endif
			oFLVRecorder.ReceiveVideoData(0, "127.0.0.1", &sPxBuffer);

			g_qCodedBufferList.pop();
		}

		Sleep(2);

		if (g_bVideoDataFinished)
		{
			break;
		}
	}

	oFLVRecorder.End();
	oFLVRecorder.Close();

	return 0;
}

// TEST VIDEO
DWORD WINAPI ThreadVideoEncoder(LPVOID lp)
{
	CPxScreenLiveStreamingDlg *pDlg = (CPxScreenLiveStreamingDlg *)lp;

	AVCodec        *pCodec    = NULL;
	AVCodecContext *pCodecCtx = NULL;

	int nRet;

	AVFrame *pFrame = NULL;
	AVPacket pkt;


	AVCodecID codec_id = AV_CODEC_ID_H264;

	avcodec_register_all();

	pCodec = avcodec_find_encoder(codec_id);
	if (!pCodec) 
	{
		printf("Codec not found\n");
		return -1;
	}

	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (!pCodecCtx) 
	{
		printf("Could not allocate video codec context\n");
		return -1;
	}

	pCodecCtx->bit_rate       = 1024*1024;
	/*pCodecCtx->rc_max_rate    = 1024*1024;
	pCodecCtx->rc_min_rate    = 1024*1024;
	pCodecCtx->rc_buffer_size = 1024*1024;*/

	pCodecCtx->width         = pDlg->m_nScreenWidth;
	pCodecCtx->height        = pDlg->m_nScreenHeight;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;
	pCodecCtx->gop_size      = 25;
	pCodecCtx->max_b_frames  = 1;
	pCodecCtx->pix_fmt       = AV_PIX_FMT_YUV420P;

	if (codec_id == AV_CODEC_ID_H264)
	{
		av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);
	}

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) 
	{
		printf("Could not open codec\n");

		return -1;
	}

	pFrame = av_frame_alloc();
	if (!pFrame) 
	{
		printf("Could not allocate video frame\n");
		return -1;
	}

	int nPictureSize = avpicture_get_size(pCodecCtx->pix_fmt, 
							          pCodecCtx->width, 
									  pCodecCtx->height);

	uint8_t* picture_buf = (uint8_t *)av_malloc(nPictureSize);
	avpicture_fill((AVPicture *)pFrame, 
		           picture_buf, 
				   pCodecCtx->pix_fmt, 
		           pCodecCtx->width, 
				   pCodecCtx->height);

	int nVideoFrameCnt = 0;

	int got_packet = 0;

	int nH264BytesCount = 0;

	char *szOutputFile = "output.h264";
	/*FILE *fpH264File = fopen(szOutputFile, "wb+");
	if (!fpH264File) 
	{
		printf("Could not open %s\n", szOutputFile);

		return -1;
	}*/

	UINT64 ui64LastVideTimeStamp = 0; 

	char szMsgBuffer[1024] = {0};

	SPxBuffer sTempBuffer;

	while (true)
	{
		if (g_bStop)
		{
			break;
		}

		// test unit #1 begin
		/*CString strCurTime = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
		OutputDebugStringA(strCurTime);
		Sleep(1000);*/
		// test unit #1 end

		// S1: capture screen in rgb format
		/*BitBlt(pDlg->m_hMemDC, 0, 0, pDlg->m_nScreenWidth, pDlg->m_nScreenHeight, 
		pDlg->m_hScreenDC, 0, 0, SRCCOPY);

		GetDIBits(pDlg->m_hMemDC, pDlg->m_hCompatibleBitmap, 0, pDlg->m_nScreenHeight, 
		pDlg->m_pBitmapBuffer, &pDlg->m_BitmapInfo, DIB_RGB_COLORS);*/

#if VIDEO_TIME_ANALYZE
		UINT64 ui64Start = GetCurrentTimestamp();
#endif 

		timeval tvTimestamp;
		gettimeofday(&tvTimestamp, NULL);

		pDlg->memDC.BitBlt(0, 0, pDlg->m_nScreenWidth, pDlg->m_nScreenHeight, pDlg->pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

		GetDIBits(pDlg->memDC.m_hDC, 
			     (HBITMAP) pDlg->memBitmap.m_hObject, 
				 0, 
				 pDlg->m_nScreenHeight, 
				 pDlg->m_pBitmapBuffer, 
				(LPBITMAPINFO) &pDlg->m_BitmapInfoHeader, 
				DIB_RGB_COLORS);            // 获取位图数据

#if VIDEO_TIME_ANALYZE
		UINT64 ui64BMPDone = GetCurrentTimestamp();

		ZeroMemory(szMsgBuffer, 1024);
		sprintf_s(szMsgBuffer, 1024, "BMP: %I64u ms", ui64BMPDone - ui64Start);
		OutputDebugStringA(szMsgBuffer);
#endif
		// test unit begin #2 : 
		// to check the capure result,
		// save it to a bmp file
#if VIDEO_SAVE_BMP
		CString strCurTime = CTime::GetCurrentTime().Format("_%Y-%m-%d_%H-%M-%S");

		CString strBmpFileName = ".\\" + strCurTime + ".bmp";

		FILE *fp = fopen(strBmpFileName, "w+b");

		if (fp == NULL)
		{
			Sleep(1000);
			continue;
		}

		fwrite(&pDlg->m_BitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), fp); // 写入位图文件头
		fwrite(&pDlg->m_BitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), fp); // 写入位图信息头

		fwrite(pDlg->m_pBitmapBuffer, 1, pDlg->m_nBitmapBufferLen, fp);   // 写入位图数据

		fclose(fp);
#endif
		// test unit end #2

		// S2: rgb to yuv420p
		rgb32_to_yv12_mmx((unsigned char *)pDlg->m_pYUVBuffer, 
			(unsigned char *)(pDlg->m_pYUVBuffer+pDlg->m_nUOffset), 
			(unsigned char *)(pDlg->m_pYUVBuffer+pDlg->m_nVOffset),
			(unsigned char *)pDlg->m_pBitmapBuffer, 
			pDlg->m_nScreenWidth, 
			pDlg->m_nScreenHeight, 
			pDlg->m_nScreenWidth);

#if VIDEO_TIME_ANALYZE
		UINT64 ui64YUVDone = GetCurrentTimestamp();

		ZeroMemory(szMsgBuffer, 1024);
		sprintf_s(szMsgBuffer, 1024, "YUV: %I64u ms", ui64YUVDone - ui64BMPDone);
		OutputDebugStringA(szMsgBuffer);
#endif 

		// test unit #3 begin
		// check the yuv
#if VIDEO_SAVE_YUV
		FILE *fp = fopen("output.yuv", "a+");

		if (fp == NULL)
		{
		Sleep(1000);
		continue;
		}

		fwrite(pDlg->m_pYUVBuffer, 1, pDlg->m_nYUVBufferSize, fp);

		fclose(fp);
#endif
		// test unit #3 end

		// S3: encode yuv420p
		pFrame->data[0] = pDlg->m_pYUVBuffer;                     // Y
		pFrame->data[1] = pDlg->m_pYUVBuffer + pDlg->m_nUOffset;  // U 
		pFrame->data[2] = pDlg->m_pYUVBuffer + pDlg->m_nVOffset;  // V

		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;

		pFrame->pts= ((UINT64)tvTimestamp.tv_sec) * 1000 + tvTimestamp.tv_usec / 1000;;

        /* encode the image */
        nRet = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_packet);
        if (nRet < 0) 
		{
            printf("Error encoding frame\n");
            return -1;
        }

        if (got_packet) 
		{
			//if (pkt.size > 0)
			{
				printf("Succeed to encode frame: %5d\tsize:%5d\n", nVideoFrameCnt, pkt.size);

				nH264BytesCount += pkt.size;

				nVideoFrameCnt++;
#if VIDEO_SAVE_H264
				FILE *fpH264File = fopen(szOutputFile, "ab+");

				fwrite(pkt.data, 1, pkt.size, fpH264File);

				if (fpH264File)
				{
					fclose(fpH264File);
					fpH264File = NULL;
				}
#endif

				/*int nPos = g_oPxBufferPool.GetEmptyBufferPos();
				memcpy(g_vlpBufferPool[nPos].lpBuffer, pkt.data, pkt.size);
				g_vlpBufferPool[nPos].nDataLength = pkt.size;

				g_qCodedBufferList.push(g_vlpBufferPool[nPos]);*/

#if VIDEO_TIME_ANALYZE
				UINT64 ui64H264Done = GetCurrentTimestamp();
				ZeroMemory(szMsgBuffer, 1024);
				sprintf_s(szMsgBuffer, 1024, "264: %I64u ms", ui64H264Done - ui64YUVDone);
				OutputDebugStringA(szMsgBuffer);
#endif
				
				//unsigned int uiTimestamp = 0;
				

#if VIDEO_TIME_ANALYZE
				UINT64 ui64TimeStamp = ((UINT64)tvTimestamp.tv_sec) * 1000 + tvTimestamp.tv_usec / 1000;

				ZeroMemory(szMsgBuffer, 1024);
				sprintf_s(szMsgBuffer, 1024, "Video timestamp: %I64u, Delta:%I64u", ui64TimeStamp, ui64TimeStamp - ui64LastVideTimeStamp);
				OutputDebugStringA(szMsgBuffer);
				ui64LastVideTimeStamp = ui64TimeStamp;
#endif 

				int nNALType = 0;

				if (pkt.size > 5)
				{
					if (AV_RB32(pkt.data) == 0x00000001)	
					{
						nNALType = pkt.data[4] & 0x1F;
					}
					else if (AV_RB24(pkt.data) == 0x000001)
					{
						nNALType = pkt.data[3] & 0x1F;
					}

					if (5 == nNALType
						||
						7 == nNALType
						||
						8 == nNALType)
					{
						sTempBuffer.bVideoKeyFrame = true;
					}
					else
					{
						sTempBuffer.bVideoKeyFrame = false;
					}
				}

				sTempBuffer.lpBuffer    = pkt.data;
				sTempBuffer.nDataLength = pkt.size;
				sTempBuffer.eMediaType  = kePxMediaType_Video;
				sTempBuffer.tvTimestamp = tvTimestamp;

				int nPos = g_oCodedBufferPool.GetEmptyBufferPos();
				g_oCodedBufferPool.SetBufferAt(nPos, &sTempBuffer);
				g_qCodedBufferList.push(g_vCodedBufferPool[nPos]);

				av_free_packet(&pkt);
			}  
        }

		// S4: calculate the timestamp 
		// and add the video encoded buffer to video buffer list 

		//Sleep(40);
	}

	//Flush Encoder

	got_packet = 1;
	while (got_packet)
	{
		nRet = avcodec_encode_video2(pCodecCtx, &pkt, NULL, &got_packet);
		if (nRet < 0) 
		{
			printf("Error encoding frame\n");
			return -1;
		}

		if (got_packet) 
		{
			printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", pkt.size);

#if VIDEO_SAVE_H264
			FILE *fpH264File = fopen(szOutputFile, "ab+");

			fwrite(pkt.data, 1, pkt.size, fpH264File);

			if (fpH264File)
			{
				fclose(fpH264File);
				fpH264File = NULL;
			}
#endif
			/*int nPos = g_oPxBufferPool.GetEmptyBufferPos();
			memcpy(g_vlpBufferPool[nPos].lpBuffer, pkt.data, pkt.size);
			g_vlpBufferPool[nPos].nDataLength = pkt.size;

			g_qCodedBufferList.push(g_vlpBufferPool[nPos]);*/

			timeval tvTimestamp;
			gettimeofday(&tvTimestamp, NULL);


#if VIDEO_TIME_ANALYZE
			UINT64 ui64TimeStamp = ((UINT64)tvTimestamp.tv_sec) * 1000 + tvTimestamp.tv_usec / 1000;

			ZeroMemory(szMsgBuffer, 1024);
			sprintf_s(szMsgBuffer, 1024, "Video timestamp: %I64u, Delta:%I64u", ui64TimeStamp, ui64TimeStamp - ui64LastVideTimeStamp);
			OutputDebugStringA(szMsgBuffer);
			ui64LastVideTimeStamp = ui64TimeStamp;
#endif

			/*tvTimestamp.tv_sec = 0;
			tvTimestamp.tv_usec = 0;*/
			/*int nPos = g_oCodedBufferPool.GetEmptyBufferPos();
			g_oCodedBufferPool.SetBufferAt(nPos, kePxMediaType_Video, pkt.data, pkt.size, tvTimestamp);
			g_qCodedBufferList.push(g_vCodedBufferPool[nPos]);*/

			int nNALType = 0;

			if (pkt.size > 5)
			{
				if (AV_RB32(pkt.data) == 0x00000001)	
				{
					nNALType = pkt.data[4] & 0x1F;
				}
				else if (AV_RB24(pkt.data) == 0x000001)
				{
					nNALType = pkt.data[3] & 0x1F;
				}

				if (5 == nNALType
					||
					7 == nNALType
					||
					8 == nNALType)
				{
					sTempBuffer.bVideoKeyFrame = true;
				}
				else
				{
					sTempBuffer.bVideoKeyFrame = false;
				}
			}

			sTempBuffer.lpBuffer    = pkt.data;
			sTempBuffer.nDataLength = pkt.size;
			sTempBuffer.eMediaType  = kePxMediaType_Video;
			sTempBuffer.tvTimestamp = tvTimestamp;

			int nPos = g_oCodedBufferPool.GetEmptyBufferPos();
			g_oCodedBufferPool.SetBufferAt(nPos, &sTempBuffer);
			g_qCodedBufferList.push(g_vCodedBufferPool[nPos]);

			av_free_packet(&pkt);	

			nVideoFrameCnt++;
		}
	}

	/*if (fpH264File)
	{
		fclose(fpH264File);
		fpH264File = NULL;
	}*/
	
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);
	//av_freep(&pFrame->data[0]);
	av_frame_free(&pFrame);

	int nH264KB = nH264BytesCount / 1024.0;
	int nH264MB = nH264KB         / 1024.0;

	g_bVideoDataFinished = true;

	return 0;
}

bool CPxScreenLiveStreamingDlg::Init()
{
	/*int nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);*/

	int nFps = 25;
	m_nFps = nFps;
	m_fPeriod =  1000.0 / nFps;
	//m_nMode = nMode;
	QueryPerformanceFrequency(&m_liPerfFreq);

	// 
	//CDC *pDC = CDC::FromHandle(::GetDC(NULL));         // 获取当前整个屏幕DC
	//int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL); // 获得颜色模式
	//int Width = pDC->GetDeviceCaps(HORZRES);
	//int Height = pDC->GetDeviceCaps(VERTRES);

	//CDC memDC;                                       // 内存DC
	//memDC.CreateCompatibleDC(pDC);

	//CBitmap memBitmap, *oldmemBitmap;                // 建立和屏幕兼容的bitmap
	//memBitmap.CreateCompatibleBitmap(pDC, nScreenWidth, nScreenHeight);

	//oldmemBitmap = memDC.SelectObject(&memBitmap);   // 将memBitmap选入内存DC
	//memDC.BitBlt(0, 0, nScreenHeight, nScreenHeight, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

	/*m_hDesktopWnd = ::GetDesktopWindow();

	m_hScreenDC = ::GetDC(m_hDesktopWnd);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);

	m_hCompatibleBitmap = CreateCompatibleBitmap(m_hScreenDC, nScreenWidth, nScreenHeight); 
	SelectObject(m_hMemDC, m_hCompatibleBitmap);*/

	/*CDC **/
	pDC = CDC::FromHandle(::GetDC(NULL));         // 获取当前整个屏幕DC
	//int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL); // 获得颜色模式
	int Width = pDC->GetDeviceCaps(HORZRES);
	int Height = pDC->GetDeviceCaps(VERTRES);

	m_nScreenWidth  = Width;
	m_nScreenHeight = Height;

	//CDC memDC;                                       // 内存DC
	memDC.CreateCompatibleDC(pDC);

	//CBitmap memBitmap;                // 建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	/*oldmemBitmap = */
	memDC.SelectObject(&memBitmap);   // 将memBitmap选入内存DC

	//memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

	// 保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);        // 获得位图信息

	m_BitmapInfoHeader.biBitCount = bmp.bmBitsPixel;
	m_BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BitmapInfoHeader.biCompression = BI_RGB;
	m_BitmapInfoHeader.biPlanes = 1;
	m_BitmapInfoHeader.biWidth     = bmp.bmWidth;
	m_BitmapInfoHeader.biHeight    = bmp.bmHeight;
	m_BitmapInfoHeader.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight; // must be DWORD aligned
	m_BitmapInfoHeader.biXPelsPerMeter = 0;
	m_BitmapInfoHeader.biYPelsPerMeter = 0;
	m_BitmapInfoHeader.biClrUsed = 0;
	m_BitmapInfoHeader.biClrImportant = 0;

	m_BitmapFileHeader.bfType = 0x4D42;
	m_BitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_BitmapInfoHeader.biSizeImage;
	m_BitmapFileHeader.bfReserved1 = 0;
	m_BitmapFileHeader.bfReserved2 = 0;
	m_BitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	if(m_pBitmapBuffer == NULL)
	{
		m_pBitmapBuffer = new BYTE [m_BitmapInfoHeader.biSizeImage];
		if(NULL == m_pBitmapBuffer)
		{
			return false;
		}
	}
	m_nBitmapBufferLen = m_BitmapInfoHeader.biSizeImage;

	m_nYUVBufferSize = m_nScreenWidth*m_nScreenHeight*3/2;

	m_pYUVBuffer = (BYTE *)new BYTE[m_nYUVBufferSize];
	if(NULL == m_pYUVBuffer)
	{
		printf("new yuv buffer failed \n");
		return false;
	}

	m_nUOffset = m_nScreenWidth*m_nScreenHeight;
	m_nVOffset = m_nUOffset+m_nUOffset/4;

	return true;
}

// 回调函数
/*static */DWORD CALLBACK MicrophoneCallback(  
	HWAVEIN hWaveIn,  // 声音输入设备句柄  
	UINT  uMsg,       // 产生的消息，由系统给出
	DWORD dwInstance, // 在waveinopen中给出要传递给该函数的数据  
	DWORD dwParam1,   
	DWORD dwParam2);  

bool g_bAudioCapture = false;
HWAVEIN  hWaveIn  = NULL;

DWORD WINAPI ThreadAudioEncoder(LPVOID lp)
{
	CPxScreenLiveStreamingDlg *pDlg = (CPxScreenLiveStreamingDlg *)lp;

	if (g_bAudioCapture)
	{
		return 0;
	}

	g_bAudioCapture = true;

	memset(&g_sWaveFormat,0,sizeof(WAVEFORMATEX));   

	g_sWaveFormat.wFormatTag      = WAVE_FORMAT_PCM;  
	g_sWaveFormat.nChannels       = 1;  
	g_sWaveFormat.wBitsPerSample  = 16;  
	g_sWaveFormat.nSamplesPerSec  = 8000L;  
	g_sWaveFormat.nBlockAlign     = 2; 
	g_sWaveFormat.nAvgBytesPerSec = 16000L;  
	g_sWaveFormat.cbSize          = 0;

	int nAudioSampleCount  = 0;
	char szMsgBuffer[1024] = {0};

	MMRESULT mmReturn   = 0;

	//DWORD    m_ThreadID = ::GetCurrentThreadId();

	BYTE *pBuffer1;//采集音频时的数据缓存
	WAVEHDR wHdr1; //采集音频时包含数据缓存的结构体

	//使用waveInOpen函数开启音频采集
	/*HANDLE wait = CreateEvent(NULL, 0, 0, NULL);
	mmReturn = ::waveInOpen(&hWaveIn, WAVE_MAPPER, &g_sWaveFormat, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);*/

	// WAVE_MAPPER，系统则会自动寻找合适设备

	//mmReturn = ::waveInOpen(&hWaveIn, WAVE_MAPPER, &g_sWaveFormat, NULL, 0L, CALLBACK_NULL);

	// open wavein device
	//mmReturn = ::waveInOpen( &hWaveIn, WAVE_MAPPER, &g_sWaveFormat, m_ThreadID, NULL, CALLBACK_THREAD);

	mmReturn = waveInOpen(&hWaveIn, 
						  WAVE_MAPPER, 
						  &g_sWaveFormat,  
		                  (DWORD)(MicrophoneCallback), 
						  NULL, 
						  CALLBACK_FUNCTION); 

	if(mmReturn)
	{
	//	//waveInErrorMsg(mmReturn, "in Start()");

		OutputDebugStringA("waveInOpen Already Start()");

		return 0;
	}

	//// start recording
	//mmReturn = ::waveInStart(hWaveIn);

	while (true)
	{
		if (g_bStop)
		{
			break;
		}

		// test sample count begin
		sprintf_s(szMsgBuffer, 1024, "ThreadAudioEncoder nAudioSampleCount:%d", nAudioSampleCount);
		OutputDebugStringA(szMsgBuffer);
		//Sleep(1000);
		nAudioSampleCount++;
		// test sample count end

		// 建立两个数组（这里可以建立多个数组）用来缓冲音频数据
		DWORD bufsize = 1024*100;//每次开辟10k的缓存存储录音数据

		//while (i--)//录制20左右秒声音，结合音频解码和网络传输可以修改为实时录音播放的机制以实现对讲功能

		pBuffer1              = new BYTE[bufsize];
		wHdr1.lpData          = (LPSTR)pBuffer1;
		wHdr1.dwBufferLength  = bufsize;
		wHdr1.dwBytesRecorded = 0;
		wHdr1.dwUser          = 0;
		wHdr1.dwFlags         = 0;
		wHdr1.dwLoops         = 1;

		waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//准备一个波形数据块头用于录音
		waveInAddBuffer(hWaveIn,     &wHdr1, sizeof (WAVEHDR));//指定波形数据块为录音输入缓存

		waveInStart(hWaveIn);//开始录音

		Sleep(1000);//等待声音录制1s

		waveInReset(hWaveIn);//停止录音

#if AUDIO_SAVE_PCM
		FILE *pf = NULL;

		//fopen_s(&pf, "output.pcm", "wb");
		fopen_s(&pf, "output.pcm", "ab");

		fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);

		if (pf)
		{
			fclose(pf);
			pf = NULL;
		}
#endif 
		if (pBuffer1)
		{
			delete pBuffer1;   
			pBuffer1 = NULL;
		}

		// make several input buffers and add them to the input queue

		// S1: Capture the audio of outside (PCM Format)

		// S2: Encode the PCM data to AAC data

		// S3: Calculate the timestamp and add the buffer to BufferList

	}

	::waveInClose(hWaveIn);

	return 0;
}

DWORD CALLBACK MicrophoneCallback(HWAVEIN hwavein, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)  
{  
	//这个CIMAADPCMDlg就是你的音频采集类  
	CPxScreenLiveStreamingDlg *pWnd = (CPxScreenLiveStreamingDlg*)dwInstance;

	switch(uMsg)   
	{  
	case WIM_OPEN:
#if DEBUG_CAPTURE_PCM
		OutputDebugStringA("WIM_OPEN\n"); 
#endif 
		break;  

	case WIM_DATA: 
#if DEBUG_CAPTURE_PCM
		OutputDebugStringA("WIM_DATA\n"); 
#endif
		//这里就是对采集到的数据做处理的地方，我是做了发送处理  
		//((PWAVEHDR)dwParam1)->lpData这就是采集到的数据指针  
		//((PWAVEHDR)dwParam1)->dwBytesRecorded这就是采集到的数据长度  
		//re = send(pWnd->sends,((PWAVEHDR)dwParam1)->lpData,((PWAVEHDR)dwParam1)->dwBytesRecorded,0);  
		//处理完了之后还要再把这个缓冲数组加回去  
		//pWnd->win代表是否继续采集，因为当停止采集的时候，只有当所有的  
		//缓冲数组都腾出来之后才能close，所以需要停止采集时就不要再waveInAddBuffer了  
		/*if(pWnd->win)  
			waveInAddBuffer (hwavein, (PWAVEHDR) dwParam1, sizeof (WAVEHDR)) ;  
		TRACE("%d\n",re); */

		break;  

	case WIM_CLOSE: 
#if DEBUG_CAPTURE_PCM
		OutputDebugStringA("WIM_CLOSE\n");  
#endif

		break;  

	default:  
		break;  
	}  

	return 0;  
}  