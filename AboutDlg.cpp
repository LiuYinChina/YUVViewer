
#include "AboutDlg.h"

//////////////////////////////////////////////////////////////////////////

LPCSTR lpszText = "���Դ˹����׸�Ŭ�������ڶ�ý��һ���ϵĹ���ʦ��\r\rYUVViewer\r\r�ṩ�����¼��� YUV ���ݵ���ʾ��\rYV12,I420,YUY2|YUYV,YVYU,OnlyY,UYVY|UYNV|Y422,NV12,NV21\r\r����\r1.ѡ�� YV12 File �������ʾ����ʱ����ʾ������ YV12 ��ʽ׷����ѡ����ļ���\r\r2.Frame ���������༭��Ϊ Start,Finish ֡,���Բ��� ��0֡ -> ��N֡ ���� ��N֡ -> ��0֡��\r\r3.���޸� ImageSize.ini �����޸ĳ���ͼ���С��\r\r4.֧�������к���ק��\r\rYUVViewer ʹ�� DirectDraw ��ʾͼ��, ���ͼ��û����ʾ, ˵��������ʾϵͳ���ܲ�֧�� YV12 ��ʽ��\r\r                           ��ط 2013��10��15�� ���Ĵ�����";

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
