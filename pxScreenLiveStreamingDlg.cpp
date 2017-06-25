
// pxScreenLiveStreamingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pxScreenLiveStreaming.h"
#include "pxScreenLiveStreamingDlg.h"
#include "afxdialogex.h"

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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CpxScreenLiveStreamingDlg �Ի���




CpxScreenLiveStreamingDlg::CpxScreenLiveStreamingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpxScreenLiveStreamingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pBitmapBuffer = NULL;
}

void CpxScreenLiveStreamingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpxScreenLiveStreamingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CpxScreenLiveStreamingDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CpxScreenLiveStreamingDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CpxScreenLiveStreamingDlg ��Ϣ��������

BOOL CpxScreenLiveStreamingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpxScreenLiveStreamingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpxScreenLiveStreamingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpxScreenLiveStreamingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool g_bStop = false;

void CpxScreenLiveStreamingDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	g_bStop = false;

	Init();

	DWORD dwThreadId;

	HANDLE hThread = CreateThread(NULL, NULL, ThreadStart, this, 0, &dwThreadId);
}


void CpxScreenLiveStreamingDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	g_bStop = true;


}

// TEST VIDEO
DWORD WINAPI ThreadStart(LPVOID lp)
{
	CpxScreenLiveStreamingDlg *pDlg = (CpxScreenLiveStreamingDlg *)lp;

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

		pDlg->memDC.BitBlt(0, 0, pDlg->m_nScreenWidth, pDlg->m_nScreenHeight, pDlg->pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

		GetDIBits(pDlg->memDC.m_hDC, 
			     (HBITMAP) pDlg->memBitmap.m_hObject, 
				 0, 
				 pDlg->m_nScreenHeight, 
				 pDlg->m_pBitmapBuffer, 
				(LPBITMAPINFO) &pDlg->m_BitmapInfoHeader, 
				DIB_RGB_COLORS);            // ��ȡλͼ����

		// test unit begin #2 : 
		// to check the capure result,
		// save it to a bmp file

		/*CString strCurTime = CTime::GetCurrentTime().Format("_%Y-%m-%d_%H-%M-%S");

		CString strBmpFileName = ".\\" + strCurTime + ".bmp";

		FILE *fp = fopen(strBmpFileName, "w+b");

		if (fp == NULL)
		{
			Sleep(1000);
			continue;
		}

		fwrite(&pDlg->m_BitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), fp); // д��λͼ�ļ�ͷ
		fwrite(&pDlg->m_BitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), fp); // д��λͼ��Ϣͷ

		fwrite(pDlg->m_pBitmapBuffer, 1, pDlg->m_nBitmapBufferLen, fp);   // д��λͼ����

		fclose(fp);*/

		// test unit end #2

		// S2: rgb to yuv420p
		rgb32_to_yv12_mmx((unsigned char *)pDlg->m_pYUVBuffer, 
			(unsigned char *)(pDlg->m_pYUVBuffer+pDlg->m_nUOffset), 
			(unsigned char *)(pDlg->m_pYUVBuffer+pDlg->m_nVOffset),
			(unsigned char *)pDlg->m_pBitmapBuffer, 
			pDlg->m_nScreenWidth, 
			pDlg->m_nScreenHeight, 
			pDlg->m_nScreenWidth);


		// test unit #3 begin
		// check the yuv

		FILE *fp = fopen("output.yuv", "a+");

		if (fp == NULL)
		{
			Sleep(1000);
			continue;
		}

		fwrite(pDlg->m_pYUVBuffer, 1, pDlg->m_nYUVBufferSize, fp);

		fclose(fp);
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

				FILE *fpH264File = fopen(szOutputFile, "ab+");

				fwrite(pkt.data, 1, pkt.size, fpH264File);

				if (fpH264File)
				{
					fclose(fpH264File);
					fpH264File = NULL;
				}

				av_free_packet(&pkt);
			}  
        }

		// S4: calculate the timestamp 
		// and add the video encoded buffer to video buffer list 

		Sleep(40);
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

bool CpxScreenLiveStreamingDlg::Init()
{
	/*int nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);*/

	int nFps = 25;
	m_nFps = nFps;
	m_fPeriod =  1000.0 / nFps;
	//m_nMode = nMode;
	QueryPerformanceFrequency(&m_liPerfFreq);

	// 
	//CDC *pDC = CDC::FromHandle(::GetDC(NULL));         // ��ȡ��ǰ������ĻDC
	//int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL); // �����ɫģʽ
	//int Width = pDC->GetDeviceCaps(HORZRES);
	//int Height = pDC->GetDeviceCaps(VERTRES);

	//CDC memDC;                                       // �ڴ�DC
	//memDC.CreateCompatibleDC(pDC);

	//CBitmap memBitmap, *oldmemBitmap;                // ��������Ļ���ݵ�bitmap
	//memBitmap.CreateCompatibleBitmap(pDC, nScreenWidth, nScreenHeight);

	//oldmemBitmap = memDC.SelectObject(&memBitmap);   // ��memBitmapѡ���ڴ�DC
	//memDC.BitBlt(0, 0, nScreenHeight, nScreenHeight, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

	/*m_hDesktopWnd = ::GetDesktopWindow();

	m_hScreenDC = ::GetDC(m_hDesktopWnd);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);

	m_hCompatibleBitmap = CreateCompatibleBitmap(m_hScreenDC, nScreenWidth, nScreenHeight); 
	SelectObject(m_hMemDC, m_hCompatibleBitmap);*/

	/*CDC **/
	pDC = CDC::FromHandle(::GetDC(NULL));         // ��ȡ��ǰ������ĻDC
	//int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL); // �����ɫģʽ
	int Width = pDC->GetDeviceCaps(HORZRES);
	int Height = pDC->GetDeviceCaps(VERTRES);

	m_nScreenWidth  = Width;
	m_nScreenHeight = Height;

	//CDC memDC;                                       // �ڴ�DC
	memDC.CreateCompatibleDC(pDC);

	//CBitmap memBitmap;                // ��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	/*oldmemBitmap = */
	memDC.SelectObject(&memBitmap);   // ��memBitmapѡ���ڴ�DC

	//memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

	// ����memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);        // ���λͼ��Ϣ

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