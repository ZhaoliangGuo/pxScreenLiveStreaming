
// pxScreenLiveStreamingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pxScreenLiveStreaming.h"
#include "pxScreenLiveStreamingDlg.h"
#include "afxdialogex.h"
#include "pxCommonDef.h"

extern "C"
{
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ThreadStart(LPVOID lp);
DWORD WINAPI ThreadVideoCaptureTest(LPVOID lp);
DWORD WINAPI ThreadWriteTest(LPVOID lp);

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

bool g_bStop = false;

void CPxScreenLiveStreamingDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bStop = false;

	DWORD dwThreadId;

	HANDLE hThread = CreateThread(NULL, NULL, ThreadStart, this, 0, &dwThreadId);
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
	while(true)
	{
		while (!g_qCodedBufferList.empty())
		{
			SPxBuffer sPxBuffer = g_qCodedBufferList.front();

			FILE *fpH264File = fopen("output_v2.h264", "ab+");

			fwrite(sPxBuffer.lpBuffer, 1, sPxBuffer.nDataLength, fpH264File);

			if (fpH264File)
			{
				fclose(fpH264File);
				fpH264File = NULL;
			}

			g_qCodedBufferList.pop();
		}

		Sleep(2);
	}

	return 0;
}

// TEST VIDEO
DWORD WINAPI ThreadStart(LPVOID lp)
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

#if DEBUG_DURATION
		UINT64 ui64Start = GetCurrentTimestamp();
#endif 

		pDlg->memDC.BitBlt(0, 0, pDlg->m_nScreenWidth, pDlg->m_nScreenHeight, pDlg->pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

		GetDIBits(pDlg->memDC.m_hDC, 
			     (HBITMAP) pDlg->memBitmap.m_hObject, 
				 0, 
				 pDlg->m_nScreenHeight, 
				 pDlg->m_pBitmapBuffer, 
				(LPBITMAPINFO) &pDlg->m_BitmapInfoHeader, 
				DIB_RGB_COLORS);            // 获取位图数据

#if DEBUG_DURATION
		UINT64 ui64BMPDone = GetCurrentTimestamp();

		ZeroMemory(szMsgBuffer, 1024);
		sprintf_s(szMsgBuffer, 1024, "BMP: %I64u ms", ui64BMPDone - ui64Start);
		OutputDebugStringA(szMsgBuffer);
#endif
		// test unit begin #2 : 
		// to check the capure result,
		// save it to a bmp file
#if SAVE_BMP
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

#if DEBUG_DURATION
		UINT64 ui64YUVDone = GetCurrentTimestamp();

		ZeroMemory(szMsgBuffer, 1024);
		sprintf_s(szMsgBuffer, 1024, "YUV: %I64u ms", ui64YUVDone - ui64BMPDone);
		OutputDebugStringA(szMsgBuffer);
#endif 

		// test unit #3 begin
		// check the yuv
#if SAVE_YUV
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

		//pFrame->pts = nVideoFrameCnt;

		pFrame->pts= nVideoFrameCnt * (pCodecCtx->time_base.num * 1000)/(pCodecCtx->time_base.den);

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
#if SAVE_H264
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

#if DEBUG_DURATION
				UINT64 ui64H264Done = GetCurrentTimestamp();
				ZeroMemory(szMsgBuffer, 1024);
				sprintf_s(szMsgBuffer, 1024, "264: %I64u ms", ui64H264Done - ui64YUVDone);
				OutputDebugStringA(szMsgBuffer);
#endif
				
				//unsigned int uiTimestamp = 0;
				timeval tvTimestamp;
				gettimeofday(&tvTimestamp, NULL);

				UINT64 ui64TimeStamp = ((UINT64)tvTimestamp.tv_sec) * 1000 + tvTimestamp.tv_usec / 1000;
	
#if DEBUG_DURATION
				ZeroMemory(szMsgBuffer, 1024);
				sprintf_s(szMsgBuffer, 1024, "Video timestamp: %I64u, Delta:%I64u", ui64TimeStamp, ui64TimeStamp - ui64LastVideTimeStamp);
				OutputDebugStringA(szMsgBuffer);
#endif 

				ui64LastVideTimeStamp = ui64TimeStamp;

				/*tvTimestamp.tv_sec = 0;
				tvTimestamp.tv_usec = 0;*/
				int nPos = g_oCodedBufferPool.GetEmptyBufferPos();
				g_oCodedBufferPool.SetBufferAt(nPos, kePxMediaType_Video, pkt.data, pkt.size, tvTimestamp);
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

			FILE *fpH264File = fopen(szOutputFile, "ab+");

			fwrite(pkt.data, 1, pkt.size, fpH264File);

			if (fpH264File)
			{
				fclose(fpH264File);
				fpH264File = NULL;
			}

			/*int nPos = g_oPxBufferPool.GetEmptyBufferPos();
			memcpy(g_vlpBufferPool[nPos].lpBuffer, pkt.data, pkt.size);
			g_vlpBufferPool[nPos].nDataLength = pkt.size;

			g_qCodedBufferList.push(g_vlpBufferPool[nPos]);*/

			timeval tvTimestamp;
			gettimeofday(&tvTimestamp, NULL);

			UINT64 ui64TimeStamp = ((UINT64)tvTimestamp.tv_sec) * 1000 + tvTimestamp.tv_usec / 1000;

#if DEBUG_DURATION
			ZeroMemory(szMsgBuffer, 1024);
			sprintf_s(szMsgBuffer, 1024, "Video timestamp: %I64u, Delta:%I64u", ui64TimeStamp, ui64TimeStamp - ui64LastVideTimeStamp);
			OutputDebugStringA(szMsgBuffer);
#endif

			ui64LastVideTimeStamp = ui64TimeStamp;

			/*tvTimestamp.tv_sec = 0;
			tvTimestamp.tv_usec = 0;*/
			int nPos = g_oCodedBufferPool.GetEmptyBufferPos();
			g_oCodedBufferPool.SetBufferAt(nPos, kePxMediaType_Video, pkt.data, pkt.size, tvTimestamp);
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