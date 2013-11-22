
#include "AboutDlg.h"

//////////////////////////////////////////////////////////////////////////

LPCSTR lpszText = "谨以此工具献给努力工作在多媒体一线上的工程师。\r\rYUVViewer\r\r提供了以下几种 YUV 数据的显示，\rYV12,I420,YUY2|YUYV,YVYU,OnlyY,UYVY|UYNV|Y422,NV12,NV21\r\r功能\r1.选择 YV12 File 则会在显示数据时将显示数据以 YV12 格式追加至选择的文件。\r\r2.Frame 后面两个编辑框为 Start,Finish 帧,可以播放 第0帧 -> 第N帧 或者 第N帧 -> 第0帧。\r\r3.可修改 ImageSize.ini 增加修改常用图像大小。\r\r4.支持命令行和拖拽。\r\rYUVViewer 使用 DirectDraw 显示图像, 如果图像没有显示, 说明您的显示系统可能不支持 YV12 格式。\r\r                           刘胤 2013年10月15日 于四川长虹";

//////////////////////////////////////////////////////////////////////////

CAboutDlg::CAboutDlg(CWnd *pParent /*=NULL*/)
	:	CDialog(CAboutDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//////////////////////////////////////////////////////////////////////////

	SetDlgItemText(IDC_STATIC_ABOUT, lpszText);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnDestroy()
{
	//	...

	CDialog::OnDestroy();
}
