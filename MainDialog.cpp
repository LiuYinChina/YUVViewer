
#include "MainDialog.h"

#include "Utility.h"

#include "AboutDlg.h"

#include <WindowsX.h>
#include <AfxDlgs.h>
#include <Shlwapi.h>

//////////////////////////////////////////////////////////////////////////

#include <functional>
#include <algorithm>

#pragma comment(lib, "WinMM")
#pragma comment(lib, "Shlwapi")
#pragma comment(lib, "DirectDraw")

using std::bind1st;
using std::binary_function;
using std::for_each;

//////////////////////////////////////////////////////////////////////////

extern "C"
{
	#include "ColorSpace.h"
}

//////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "ColorSpace")

//////////////////////////////////////////////////////////////////////////

typedef VOID (WINAPI *ConvertFunction)(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight);

//////////////////////////////////////////////////////////////////////////

template <BOOL bEnable>
struct EnableWnd : public binary_function<CWnd *, INT, INT>
{
public:
	INT operator()(CWnd *pWnd, INT nChildWndID) const
	{
		CWnd *pChildWnd = pWnd->GetDlgItem(nChildWndID);
		if (pChildWnd) {
			pChildWnd->EnableWindow(bEnable);
		}
		return 0;
	}
};

//////////////////////////////////////////////////////////////////////////

typedef struct tagFourCCConfig
{
	LPCTSTR lpszFourCC;
	LPCTSTR lpszComponentOrder;
	FLOAT fImageSizeRate;
	FLOAT fYStrideRate;
	FLOAT fUVStrideRate;
	ConvertFunction pConvertFunction;
} FourCCConfig;

typedef struct tagDisplayComponet
{
	LPCTSTR lpDisplayComponet;
	DWORD dwDisplayFlag;
} DisplayComponet;

//////////////////////////////////////////////////////////////////////////

enum
{
	YComponet = 0x01 << 0, 
	UComponet = 0x01 << 1, 
	VComponet = 0x01 << 2, 
};

DisplayComponet gDisplayComponets[] = 
{
	{ TEXT("YUV all Componets"),YComponet|UComponet|VComponet, }, 
	{ TEXT("Y Componet only"),	YComponet, }, 
	{ TEXT("U Componet only"),	UComponet, }, 
	{ TEXT("V Componet only"),	VComponet, }, 
	{ TEXT("UV Componet"),		UComponet|VComponet, }, 
};

//////////////////////////////////////////////////////////////////////////

extern "C" packedFunc rgb_to_yv12_mmx;
extern "C" packedFunc bgr_to_yv12_mmx;
extern "C" packedFunc rgba_to_yv12_mmx;
extern "C" packedFunc bgra_to_yv12_mmx;
extern "C" packedFunc yuyv_to_yv12_mmx;
extern "C" packedFunc uyvy_to_yv12_mmx;
extern "C" packedFunc yuyv_to_yv12_mmx;

//////////////////////////////////////////////////////////////////////////

VOID WINAPI RGB555ToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	rgb555_to_yv12_c(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI RGB565ToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	rgb565_to_yv12_c(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI RGBToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	rgb_to_yv12_mmx(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI BGRToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	bgr_to_yv12_mmx(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI BGRAToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	bgra_to_yv12_mmx(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI ABGRToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	abgr_to_yv12_c(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI RGBAToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	rgba_to_yv12_mmx(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI ARGBToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	argb_to_yv12_c(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI YUYVToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	yuyv_to_yv12_mmx(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI UYVYToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	uyvy_to_yv12_mmx(pImage, dwStride, pY, pV, pU, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI YVYUToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	yuyv_to_yv12_mmx(pImage, dwStride, pY, pU, pV, dwYStride, dwUVStride, dwWidth, dwHeight, 0);
}
VOID WINAPI YV12ToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	LPBYTE lpCurrent = pImage;
	CopyMemory(pY, lpCurrent, dwStride * dwHeight);
	lpCurrent += dwStride * dwHeight;
	CopyMemory(pU, lpCurrent, (dwUVStride * dwHeight >> 1));
	lpCurrent += (dwUVStride * dwHeight >> 1);
	CopyMemory(pV, lpCurrent, (dwUVStride * dwHeight >> 1));
}
VOID WINAPI I420ToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	LPBYTE lpCurrent = pImage;
	CopyMemory(pY, lpCurrent, dwStride * dwHeight);
	lpCurrent += dwStride * dwHeight;
	CopyMemory(pV, lpCurrent, (dwUVStride * dwHeight >> 1));
	lpCurrent += (dwUVStride * dwHeight >> 1);
	CopyMemory(pU, lpCurrent, (dwUVStride * dwHeight >> 1));
}

VOID WINAPI YToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	LPBYTE lpCurrent = pImage;
	CopyMemory(pY, lpCurrent, dwStride * dwHeight);
	lpCurrent += dwStride * dwHeight;

	FillMemory(pU, (dwUVStride * dwHeight >> 1), 0x7F);
	FillMemory(pV, (dwUVStride * dwHeight >> 1), 0x7F);
}

VOID CopyMemoryEx(LPVOID lpBufferDst, DWORD dwDstStep, LPVOID lpBufferSrc, DWORD dwSrcStep, DWORD dwCopyItems)
{
	LPBYTE lpSrc = (LPBYTE) lpBufferSrc;
	LPBYTE lpDst = (LPBYTE) lpBufferDst;

	for (DWORD i=0; i<dwCopyItems; ++i) {
		*lpDst = *lpSrc;
		lpSrc += dwSrcStep;
		lpDst += dwDstStep;
	}
}

VOID WINAPI NV12ToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	LPBYTE lpCurrent = pImage;
	CopyMemory(pY, lpCurrent, dwStride * dwHeight);
	lpCurrent += dwStride * dwHeight;

	LPBYTE lpUBase = lpCurrent + 0;
	LPBYTE lpVBase = lpCurrent + 1;
	CopyMemoryEx(pU, 1, lpUBase, 2, dwUVStride * dwHeight >> 1);
	CopyMemoryEx(pV, 1, lpVBase, 2, dwUVStride * dwHeight >> 1);
}

VOID WINAPI NV21ToYV12(LPBYTE pImage, DWORD dwStride, LPBYTE &pY, LPBYTE &pU, LPBYTE &pV, DWORD dwYStride, DWORD dwUVStride, DWORD dwWidth, DWORD dwHeight)
{
	LPBYTE lpCurrent = pImage;
	CopyMemory(pY, lpCurrent, dwStride * dwHeight);
	lpCurrent += dwStride * dwHeight;

	LPBYTE lpVBase = lpCurrent + 0;
	LPBYTE lpUBase = lpCurrent + 1;
	CopyMemoryEx(pU, 1, lpUBase, 2, dwUVStride * dwHeight >> 1);
	CopyMemoryEx(pV, 1, lpVBase, 2, dwUVStride * dwHeight >> 1);
}

//////////////////////////////////////////////////////////////////////////

FourCCConfig gFourCCConfig[] = 
{
	{ TEXT("YV12"),				TEXT("[Y][V][U]"),	1.5,	1.0,	1.0,	YV12ToYV12, }, 
	{ TEXT("I420"),				TEXT("[Y][U][V]"),	1.5,	1.0,	1.0,	I420ToYV12, }, 
	{ TEXT("YUY2|YUYV"),		TEXT("[YUYV]"),		2.0,	2.0,	1.0,	YUYVToYV12, }, 
	{ TEXT("YVYU"),				TEXT("[YVYU]"),		2.0,	2.0,	1.0,	YVYUToYV12, }, 
	{ TEXT("Only Y"),			TEXT("[YYYY]"),		1.0,	1.0,	1.0,	YToYV12, }, 
	{ TEXT("UYVY|UYNV|Y422"),	TEXT("[UYVY]"),		2.0,	2.0,	1.0,	UYVYToYV12, }, 
	{ TEXT("NV12"),				TEXT("[Y][UV]"),	1.5,	1.0,	1.0,	NV12ToYV12, }, 
	{ TEXT("NV21"),				TEXT("[Y][VU]"),	1.5,	1.0,	1.0,	NV21ToYV12, }, 
};

LPCTSTR gImageSizeConfig[] = 
{
	{ TEXT(" 176 X 120    \r\n"),	}, 
	{ TEXT(" 176 X 144    \r\n"),	}, 
	{ TEXT(" 352 X 240    \r\n"),	}, 
	{ TEXT(" 352 X 288    \r\n"),	}, 
	{ TEXT(" 704 X 480    \r\n"),	}, 
	{ TEXT(" 704 X 576    D1\r\n"),	}, 
	{ TEXT(" 720 X 480    D1\r\n"),	}, 
	{ TEXT(" 720 X 576    D1\r\n"),	}, 
	{ TEXT(" 640 X 480    \r\n"),	}, 
	{ TEXT(" 800 X 600    \r\n"),	}, 
	{ TEXT("1024 X 768    \r\n"),	}, 
	{ TEXT("1280 X 1024   \r\n"),	}, 
	{ TEXT("1600 X 1200   \r\n"),	}, 
	{ TEXT("1280 X 720    720P\r\n"),	}, 
	{ TEXT("1440 X 1080   \r\n"),	}, 
	{ TEXT("1920 X 1080   1080P\r\n"),	}, 
};

//////////////////////////////////////////////////////////////////////////

static ConvertFunction GetConvertor(DWORD dwFourCCIndex)
{
	return (dwFourCCIndex < ItemCount(gFourCCConfig)) ? gFourCCConfig[dwFourCCIndex].pConvertFunction : NULL;
}
static FLOAT GetImageSizeRate(DWORD dwFourCCIndex)
{
	return (dwFourCCIndex < ItemCount(gFourCCConfig)) ? gFourCCConfig[dwFourCCIndex].fImageSizeRate : 0.0f;
}
static FLOAT GetYStrideRate(DWORD dwFourCCIndex)
{
	return (dwFourCCIndex < ItemCount(gFourCCConfig)) ? gFourCCConfig[dwFourCCIndex].fYStrideRate : 0.0f;
}
static FLOAT GetUVStrideRate(DWORD dwFourCCIndex)
{
	return (dwFourCCIndex < ItemCount(gFourCCConfig)) ? gFourCCConfig[dwFourCCIndex].fUVStrideRate : 0.0f;
}
static LPCTSTR GetComponentOrder(DWORD dwFourCCIndex)
{
	return (dwFourCCIndex < ItemCount(gFourCCConfig)) ? gFourCCConfig[dwFourCCIndex].lpszComponentOrder : NULL;
}

//////////////////////////////////////////////////////////////////////////

INT nIDGroupPlay[] = 
{
	IDC_CMB_FOURCC, 
	IDC_CMB_IMAGESIZE, 

	IDC_EDT_FRAME_START, 
	IDC_EDT_FRAME_FINISH, 
	IDC_BTN_FRAME_JUMP, 
	IDC_BTN_FORWARD, 
	IDC_BTN_BACK, 
	IDC_BTN_PLAY, 
	IDC_BTN_STOP, 
	IDC_CMB_COMPONETS, 

	IDC_EDT_WIDTH, 
	IDC_EDT_HEIGHT, 
};

//////////////////////////////////////////////////////////////////////////

CMainDialog::CMainDialog(CWnd *pParent)
	:	CDialog(CMainDialog::IDD, pParent), 
		m_dwState(STATE_STOP), 
		m_dwWidth(0), 
		m_dwHeight(0), 
		m_pBufferSrc(NULL), 
		m_pBufferDst(NULL), 
		m_dwImageLength(0), 
		m_dwBufferCapability(0), 
		m_dwFrameIndex(0), 
		m_dwFrameCount(0), 
		m_dwFrameStart(0), 
		m_dwFrameFinish(0), 
		m_dwFileLength(0), 
		m_nCurSelFourCC(0), 
		m_nDisplayComponet(0), 
		m_dwLastFileOffset(-1), 
		m_dwLastImageLength(-1)
{
	//{{AFX_DATA_INIT(CMainDialog)
	//}}AFX_DATA_INIT

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//////////////////////////////////////////////////////////////////////////

	ZeroMemory(m_szSrcFilePath, sizeof(m_szSrcFilePath));
}

//////////////////////////////////////////////////////////////////////////

void CMainDialog::SetSrcFilePath(LPCTSTR lpszSrcFilePath)
{
	if (lpszSrcFilePath != NULL) {
		lstrcpy(m_szSrcFilePath, lpszSrcFilePath);
	}
}

//////////////////////////////////////////////////////////////////////////

void CMainDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDialog)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDialog)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

	ON_CBN_SELCHANGE(IDC_CMB_FOURCC, OnFourCCSelChange)
	ON_CBN_SELCHANGE(IDC_CMB_IMAGESIZE, OnImageSizeSelChange)
	ON_CBN_SELCHANGE(IDC_CMB_COMPONETS, OnDisplayComponetsSelChange)
	ON_BN_CLICKED(IDC_BTN_SRC_BROWSE, OnBtnBrowseSrc)
	ON_BN_CLICKED(IDC_BTN_DST_BROWSE, OnBtnBrowseDst)
	ON_BN_CLICKED(IDC_BTN_FRAME_JUMP, OnBtnFrameJump)
	ON_BN_CLICKED(IDC_BTN_FORWARD, OnBtnForward)
	ON_BN_CLICKED(IDC_BTN_BACK, OnBtnBack)
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

BEGIN_EASYSIZE_MAP(CMainDialog)
	EASYSIZE(IDC_STATIC_SRC_YUVFILE,	ES_BORDER,					ES_BORDER,				ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_SRC_PATH,			IDC_STATIC_SRC_YUVFILE,		ES_BORDER,				ES_BORDER,	ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_SRC_BROWSE,		IDC_EDT_SRC_PATH,			ES_BORDER,				ES_KEEPSIZE,ES_KEEPSIZE,0)

	EASYSIZE(IDC_STATIC_DST_YUVFILE,	ES_BORDER,					IDC_STATIC_SRC_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_DST_PATH,			IDC_STATIC_DST_YUVFILE,		IDC_STATIC_SRC_YUVFILE,	ES_BORDER,	ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_DST_BROWSE,		IDC_EDT_DST_PATH,			IDC_STATIC_SRC_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)

	EASYSIZE(IDC_STATIC_FOURCC,			ES_BORDER,					IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_CMB_FOURCC,			IDC_STATIC_FOURCC,			IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_STATIC_ORDER,			IDC_CMB_FOURCC,				IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_ORDER,				IDC_STATIC_ORDER,			IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_STATIC_IMAGESIZE,		IDC_EDT_ORDER,				IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_CMB_IMAGESIZE,			IDC_STATIC_IMAGESIZE,		IDC_STATIC_DST_YUVFILE,	ES_BORDER,	ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_WIDTH,				IDC_CMB_IMAGESIZE,			IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_STATIC_X,				IDC_EDT_WIDTH,				IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_HEIGHT,			IDC_STATIC_X,				IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_ABOUT,				IDC_EDT_HEIGHT,				IDC_STATIC_DST_YUVFILE,	ES_KEEPSIZE,ES_KEEPSIZE,0)

	EASYSIZE(IDC_STATIC_FRAME,			ES_BORDER,					IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_FRAME_START,		IDC_STATIC_FRAME,			IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_FRAME_FINISH,		IDC_EDT_FRAME_START,		IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_STATIC_FPS,			IDC_EDT_FRAME_FINISH,		IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_FPS,				IDC_STATIC_FPS,				IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_EDT_FRAME_INFO,		IDC_EDT_FPS,				IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_FRAME_JUMP,		IDC_EDT_FRAME_INFO,			IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_FORWARD,			IDC_BTN_FRAME_JUMP,			IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_BACK,				IDC_BTN_FORWARD,			IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_PLAY,				IDC_BTN_BACK,				IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_STOP,				IDC_BTN_PLAY,				IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_STATIC_DISPLAY,		IDC_BTN_STOP,				IDC_STATIC_FOURCC,		ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_CMB_COMPONETS,			IDC_STATIC_DISPLAY,			IDC_STATIC_FOURCC,		ES_BORDER,	ES_KEEPSIZE,0)

	EASYSIZE(IDC_STATIC_IMAGE,			ES_BORDER,					IDC_STATIC_FRAME,		ES_BORDER,	ES_BORDER,0)
END_EASYSIZE_MAP

/////////////////////////////////////////////////////////////////////////////

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//////////////////////////////////////////////////////////////////////////

	GetWindowRect(&m_rtMinSize);
	INIT_EASYSIZE;

	InitControl();
	GetImageSize(m_dwWidth, m_dwHeight);

	m_dwFrameStart = 0;
	m_dwFrameFinish= 0;
	for_each(nIDGroupPlay, nIDGroupPlay + ItemCount(nIDGroupPlay), bind1st(EnableWnd<FALSE>(), this));

	//////////////////////////////////////////////////////////////////////////

	if (PathFileExists(m_szSrcFilePath)) {
		SetTimer(0, 1000 / GetDlgItemInt(IDC_EDT_FPS, NULL, FALSE), NULL);
		SetDlgItemText(IDC_EDT_SRC_PATH, m_szSrcFilePath);
		for_each(nIDGroupPlay, nIDGroupPlay + ItemCount(nIDGroupPlay), bind1st(EnableWnd<TRUE>(), this));

		m_fileSrc.Create(m_szSrcFilePath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
		m_fileSrc.GetSize(m_dwFileLength);
		Update(TRUE);
	}
	Update(FALSE);
	return TRUE;
}

void CMainDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CMainDialog::OnPaint() 
{
	if (IsIconic()) {
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint();
	}
}

HCURSOR CMainDialog::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMainDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
}

void CMainDialog::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(m_rtMinSize.Width(), m_rtMinSize.Height(), fwSide, pRect);
}

//////////////////////////////////////////////////////////////////////////

void CMainDialog::OnDestroy()
{
	KillTimer(0);

	m_fileSrc.Close();

	delete []m_pBufferSrc;
	delete []m_pBufferDst;
	m_pBufferSrc = NULL;
	m_pBufferDst = NULL;

	//////////////////////////////////////////////////////////////////////////

	CDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////

void CMainDialog::GetImageSize(DWORD &dwWidth, DWORD &dwHeight)
{
	HWND hWndImageSize	= ::GetDlgItem(GetSafeHwnd(), IDC_CMB_IMAGESIZE);
	INT nCurSelImageSize = ComboBox_GetCurSel(hWndImageSize);

	DWORD dwData= ComboBox_GetItemData(hWndImageSize, nCurSelImageSize);
	dwWidth	= dwData >> 16;
	dwHeight= dwData & 0xFFFF;
}

void CMainDialog::OnTimer(UINT_PTR nIDEvent)
{
	Display();
	if (m_dwFrameIndex == m_dwFrameFinish) {
		m_dwState = STATE_STOP;
	}
	if (m_dwState == STATE_STOP) {
		KillTimer(nIDEvent);
	}

	if (m_dwState == STATE_PLAY) {
		m_fileDst.Write(m_pBufferDst, m_dwWidth * m_dwHeight * 3 >> 1);
		m_dwFrameIndex += (m_dwFrameFinish < m_dwFrameStart) ? 0 : +1;
		m_dwFrameIndex += (m_dwFrameFinish > m_dwFrameStart) ? 0 : -1;

		CHAR szText[128] = { 0 };
		wsprintf(szText, "%u/%u", m_dwFrameIndex, m_dwFrameCount);
		SetDlgItemText(IDC_EDT_FRAME_INFO, szText);
	}
}

void CMainDialog::InitControl()
{
	::DragAcceptFiles(GetSafeHwnd(), TRUE);
	m_wndYUVImage.SubclassDlgItem(IDC_STATIC_IMAGE, this);

	HWND hWndFourCC		= ::GetDlgItem(GetSafeHwnd(), IDC_CMB_FOURCC);
	HWND hWndComponets	= ::GetDlgItem(GetSafeHwnd(), IDC_CMB_COMPONETS);
	HWND hWndImageSize	= ::GetDlgItem(GetSafeHwnd(), IDC_CMB_IMAGESIZE);

	for (DWORD i=0; i<ItemCount(gFourCCConfig); ++i) {
		ComboBox_AddString(hWndFourCC, gFourCCConfig[i].lpszFourCC);
	}
	for (DWORD i=0; i<ItemCount(gDisplayComponets); ++i) {
		ComboBox_AddString(hWndComponets, gDisplayComponets[i].lpDisplayComponet);
	}

	//////////////////////////////////////////////////////////////////////////

	CHAR szConfigPath[MAX_PATH+1] = { 0 };
	::GetModuleFileName(NULL, szConfigPath, MAX_PATH);
	::PathRemoveFileSpec(szConfigPath);
	::StrCat(szConfigPath, "\\ImageSize.ini");

	FILE *pStream = fopen(szConfigPath, "r");
	if (pStream == NULL) {
		pStream = fopen(szConfigPath, "w");
		for (DWORD i=0; i<ItemCount(gImageSizeConfig); ++i) {
			fputs(gImageSizeConfig[i], pStream);
		}
		fclose(pStream);
		pStream = NULL;

		pStream = fopen(szConfigPath, "r");
	}

	INT nIndex	= 0;
	WORD wWidth	= 0;
	WORD wHeight= 0;
	CHAR szImageSize[1024] = { 0 };
	while (1) {
		wWidth	= 0;
		wHeight= 0;
		if (fgets(szImageSize, sizeof(szImageSize)-1, pStream) == NULL) {
			break;
		}

		sscanf(szImageSize, "%d X %d", &wWidth, &wHeight);
		if (wWidth == 0 || wHeight == 0) {
			continue;
		}
		nIndex = ComboBox_AddString(hWndImageSize, szImageSize);
		ComboBox_SetItemData(hWndImageSize, nIndex, (wWidth << 16) | wHeight);
	}
	fclose(pStream);
	pStream = NULL;

	//////////////////////////////////////////////////////////////////////////

	ComboBox_SetCurSel(hWndFourCC, 0);
	ComboBox_SetCurSel(hWndImageSize, 0);
	ComboBox_SetCurSel(hWndComponets, 0);

	SetDlgItemInt(IDC_EDT_FPS, 25);

	DWORD dwData= ComboBox_GetItemData(hWndImageSize, 0);
	m_dwWidth = dwData >> 16;
	m_dwHeight= dwData & 0xFFFF;
	SetDlgItemInt(IDC_EDT_WIDTH, m_dwWidth, FALSE);
	SetDlgItemInt(IDC_EDT_HEIGHT, m_dwHeight, FALSE);
}
void CMainDialog::OnFourCCSelChange()
{
	Update(TRUE);
	Update(FALSE);
	Display();
}
void CMainDialog::OnImageSizeSelChange()
{
	HWND hWndImageSize	= ::GetDlgItem(GetSafeHwnd(), IDC_CMB_IMAGESIZE);
	INT nCurSelImageSize = ComboBox_GetCurSel(hWndImageSize);

	DWORD dwData= ComboBox_GetItemData(hWndImageSize, nCurSelImageSize);
	m_dwWidth = dwData >> 16;
	m_dwHeight= dwData & 0xFFFF;
	SetDlgItemInt(IDC_EDT_WIDTH, m_dwWidth, FALSE);
	SetDlgItemInt(IDC_EDT_HEIGHT, m_dwHeight, FALSE);

	Update(TRUE);
	m_dwFrameFinish = m_dwFrameCount - 1;
	Update(FALSE);
	Display();
}
void CMainDialog::OnDisplayComponetsSelChange()
{
	Update(TRUE);
	Update(FALSE);
	Display();
}

void CMainDialog::Reset()
{
	m_dwFrameIndex = 0;
	m_dwFrameCount = 0;
	m_dwFrameStart = 0;
	m_dwFrameFinish= 0;
	m_dwFileLength = 0;

	m_dwLastFileOffset	= -1;
	m_dwLastImageLength	= -1;

	Update(FALSE);
	m_wndYUVImage.Clear();
}

void CMainDialog::OnBtnBrowseSrc()
{
	SetDlgItemText(IDC_EDT_SRC_PATH, (LPCTSTR) "");

	m_fileSrc.Close();
	Reset();

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, TEXT("All Files (*.*)|*.*||"), this);
	fileDlg.m_ofn.lpstrTitle = "Select a yuv file";

	for_each(nIDGroupPlay, nIDGroupPlay + ItemCount(nIDGroupPlay), bind1st(EnableWnd<FALSE>(), this));
	if (fileDlg.DoModal() != IDOK) {
		return;
	}
	if (!PathFileExists((LPCTSTR) fileDlg.GetPathName())) {
		return;
	}
	for_each(nIDGroupPlay, nIDGroupPlay + ItemCount(nIDGroupPlay), bind1st(EnableWnd<TRUE>(), this));

	SetDlgItemText(IDC_EDT_SRC_PATH, (LPCTSTR) fileDlg.GetPathName());
	m_fileSrc.Create(fileDlg.GetPathName(), GENERIC_READ, 0, OPEN_EXISTING);
	m_fileSrc.GetSize(m_dwFileLength);

	Update(TRUE);
	m_dwFrameFinish = m_dwFrameCount - 1;
	Update(FALSE);
	Display();
}

void CMainDialog::OnBtnBrowseDst()
{
	SetDlgItemText(IDC_EDT_DST_PATH, (LPCTSTR) "");
	m_fileDst.Close();

	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, TEXT("All Files (*.*)|*.*||"), this);
	fileDlg.m_ofn.lpstrTitle = "Select a yuv file";
	if (fileDlg.DoModal() != IDOK) {
		return;
	}

	SetDlgItemText(IDC_EDT_DST_PATH, (LPCTSTR) fileDlg.GetPathName());
	if (PathFileExists(fileDlg.GetPathName())) {
		m_fileDst.Create(fileDlg.GetPathName(), GENERIC_WRITE, 0, OPEN_EXISTING);
		m_fileDst.Seek(0, FILE_END);
	}
	else {
		m_fileDst.Create(fileDlg.GetPathName(), GENERIC_WRITE, 0, CREATE_ALWAYS);
	}
}

DWORD CMainDialog::Display()
{
	DWORD dwOffset = m_dwFrameIndex * m_dwImageLength;
	if (dwOffset >= m_dwFileLength) {
		return ERROR_SEEK;
	}

	if (m_dwLastFileOffset != dwOffset || m_dwLastImageLength != m_dwImageLength) {
		m_fileSrc.Seek(dwOffset, FILE_BEGIN);
		m_fileSrc.Read(m_pBufferSrc, m_dwImageLength);

		m_dwLastFileOffset	= dwOffset;
		m_dwLastImageLength	= m_dwImageLength;
	}

	LPBYTE pY = m_pBufferDst;
	LPBYTE pV = pY + (m_dwWidth * m_dwHeight);
	LPBYTE pU = pV + (m_dwWidth * m_dwHeight >> 2);

	ConvertFunction convertor = GetConvertor(m_nCurSelFourCC);
	if (convertor != NULL) {
		convertor(m_pBufferSrc, 
			m_dwWidth * GetYStrideRate(m_nCurSelFourCC), 
			pY, 
			pU, 
			pV, 
			m_dwWidth, 
			m_dwWidth >> 1, 
			m_dwWidth, 
			m_dwHeight);

		if (!(gDisplayComponets[m_nDisplayComponet].dwDisplayFlag & YComponet)) {
			FillMemory(pY, (m_dwWidth * m_dwHeight), 0x7F);
		}
		if (!(gDisplayComponets[m_nDisplayComponet].dwDisplayFlag & UComponet)) {
			FillMemory(pU, (m_dwWidth * m_dwHeight >> 2), 0x7F);
		}
		if (!(gDisplayComponets[m_nDisplayComponet].dwDisplayFlag & VComponet)) {
			FillMemory(pV, (m_dwWidth * m_dwHeight >> 2), 0x7F);
		}
	}

	m_wndYUVImage.Display(m_dwBufferCapability, m_dwWidth, m_dwHeight, IMAGE_FORMAT_YV12, pY, pU, pV);
	return ERROR_SUCCESS;
}

DWORD CMainDialog::Update(BOOL bSave)
{
	HWND hWndFourCC	= ::GetDlgItem(GetSafeHwnd(), IDC_CMB_FOURCC);
	HWND hDisplayComponets = ::GetDlgItem(GetSafeHwnd(), IDC_CMB_COMPONETS);

	if (bSave) {
		m_nCurSelFourCC	= ComboBox_GetCurSel(hWndFourCC);

		m_dwWidth	= GetDlgItemInt(IDC_EDT_WIDTH);
		m_dwHeight	= GetDlgItemInt(IDC_EDT_HEIGHT);
		m_dwImageLength = m_dwWidth * m_dwHeight * GetImageSizeRate(m_nCurSelFourCC);
		EnlargeBuffer(m_dwWidth * m_dwHeight << 2);
		m_dwFrameCount	= (m_dwFileLength / m_dwImageLength) + ((m_dwFileLength % m_dwImageLength) != 0);

		m_dwFrameStart	= min(GetDlgItemInt(IDC_EDT_FRAME_START), m_dwFrameCount-1);
		m_dwFrameFinish	= min(GetDlgItemInt(IDC_EDT_FRAME_FINISH), m_dwFrameCount-1);

		m_dwFrameIndex	= min(m_dwFrameIndex, m_dwFrameCount-1);

		m_nDisplayComponet = ComboBox_GetCurSel(hDisplayComponets);
	}
	else {
		ComboBox_SetCurSel(hWndFourCC, m_nCurSelFourCC);

		SetDlgItemInt(IDC_EDT_WIDTH, m_dwWidth, FALSE);
		SetDlgItemInt(IDC_EDT_HEIGHT, m_dwHeight, FALSE);

		SetDlgItemInt(IDC_EDT_FRAME_START, m_dwFrameStart, FALSE);
		SetDlgItemInt(IDC_EDT_FRAME_FINISH, m_dwFrameFinish, FALSE);

		ComboBox_SetCurSel(hDisplayComponets, m_nDisplayComponet);
		SetDlgItemText(IDC_EDT_ORDER, gFourCCConfig[m_nCurSelFourCC].lpszComponentOrder);

		CHAR szText[128] = { 0 };
		wsprintf(szText, "%u/%u", m_dwFrameIndex, m_dwFrameCount);
		SetDlgItemText(IDC_EDT_FRAME_INFO, szText);
	}
	return ERROR_SUCCESS;
}

void CMainDialog::EnlargeBuffer(DWORD dwSize)
{
	if (m_dwBufferCapability < dwSize) {
		m_dwBufferCapability = dwSize;

		delete []m_pBufferSrc;
		delete []m_pBufferDst;
		m_pBufferSrc = new BYTE[m_dwBufferCapability];
		m_pBufferDst = new BYTE[m_dwBufferCapability];
		FillMemory(m_pBufferSrc, m_dwBufferCapability, 0x80);
		FillMemory(m_pBufferDst, m_dwBufferCapability, 0x80);
	}
}

void CMainDialog::OnBtnFrameJump()
{
	DWORD dwFrameStart = GetDlgItemInt(IDC_EDT_FRAME_START);
	m_dwFrameStart	= (dwFrameStart > (m_dwFrameCount-1))  ? (m_dwFrameCount-1) : dwFrameStart;
	m_dwFrameIndex	= m_dwFrameStart;
	Display();
	Update(FALSE);
}

void CMainDialog::OnBtnForward()
{
	if (m_dwFrameIndex == (m_dwFrameCount-1)) {
		return;
	}

	++m_dwFrameIndex;
	Display();
	Update(TRUE);
	Update(FALSE);
}

void CMainDialog::OnBtnBack()
{
	if (m_dwFrameIndex == 0) {
		return;
	}

	--m_dwFrameIndex;
	Display();
	Update(TRUE);
	Update(FALSE);
}

void CMainDialog::OnBtnPlay()
{
	Update(TRUE);
	Update(FALSE);

	if (STATE_PLAY != m_dwState) {
		m_dwFrameIndex = m_dwFrameStart;
	}
	m_dwState = STATE_PLAY;

	DWORD dwFPS = GetDlgItemInt(IDC_EDT_FPS, NULL, FALSE);
	dwFPS = ((dwFPS == 0) || (dwFPS > 100)) ? 1 : dwFPS;
	SetDlgItemInt(IDC_EDT_FPS, dwFPS);

	SetTimer(0, 1000 / dwFPS, NULL);
}
void CMainDialog::OnBtnStop()
{
	m_dwState = STATE_STOP;
}

void CMainDialog::OnDropFiles(HDROP hDropInfo)
{
	if (m_dwState != STATE_PLAY) {
		return;
	}

	CHAR szFilePath[MAX_PATH+1] = { 0 };

	UINT nFileCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (nFileCount != 1) {
		::DragFinish(hDropInfo);
		return;
	}

	UINT nChars = ::DragQueryFile(hDropInfo, 0, szFilePath, MAX_PATH);
	::DragFinish(hDropInfo);

	if (PathFileExists(szFilePath)) {
		for_each(nIDGroupPlay, nIDGroupPlay + ItemCount(nIDGroupPlay), bind1st(EnableWnd<TRUE>(), this));

		SetDlgItemText(IDC_EDT_SRC_PATH, szFilePath);
		m_fileSrc.Close();
		m_fileSrc.Create(szFilePath, GENERIC_READ, 0, OPEN_EXISTING);
		m_fileSrc.GetSize(m_dwFileLength);

		Update(TRUE);
		m_dwFrameFinish = m_dwFrameCount - 1;
		Update(FALSE);
		Display();
	}
}

void CMainDialog::OnBtnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
