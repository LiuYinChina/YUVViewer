
#ifndef WndYUVImage_H
#define WndYUVImage_H

//////////////////////////////////////////////////////////////////////////

#include <AfxWin.h>
#include <AfxExt.h>

#include "DirectDraw.h"

//////////////////////////////////////////////////////////////////////////

template <typename tUnique>
class WndYUVImageT	:	public CWnd
{
public:
	WndYUVImageT()
		:	m_pYUVImage(NULL), 
			m_hDirectDraw(NULL), 
			m_dwImageCapability(0)
	{
		DD_Initialize(&m_hDirectDraw);
	}

	void EnlargeBuffer(DWORD dwSize)
	{
		if (m_dwImageCapability < dwSize) {
			m_dwImageCapability = dwSize;

			delete []m_pYUVImage;
			m_pYUVImage = new BYTE[m_dwImageCapability];
		}
	}

	void Clear()
	{
		if (m_pYUVImage != NULL) {
			FillMemory(m_pYUVImage, m_dwImageCapability, 0x80);
			DD_Draw(m_hDirectDraw, GetSafeHwnd(), &m_imageProperties);
		}
	}
	void Display(DWORD dwBufferCapability, DWORD dwWidth, DWORD dwHeight, DWORD dwImageFormat, LPBYTE pY, LPBYTE pU, LPBYTE pV)
	{
		EnlargeBuffer(dwBufferCapability);

		CopyMemory(m_pYUVImage, pY, dwBufferCapability);

		m_imageProperties.dwWidth = dwWidth;
		m_imageProperties.dwHeight= dwHeight;
		m_imageProperties.dwImageFormat = dwImageFormat;
		m_imageProperties.lpY = m_pYUVImage;
		m_imageProperties.lpU = m_pYUVImage + (pU - pY);
		m_imageProperties.lpV = m_pYUVImage + (pV - pY);
		DD_Draw(m_hDirectDraw, GetSafeHwnd(), &m_imageProperties);
	}

protected:
	//{{AFX_MSG(WndYUVImageT)
	afx_msg void OnDestroy()
	{
		DD_Uninitialize(m_hDirectDraw);
		m_hDirectDraw = NULL;

		delete []m_pYUVImage;
		m_pYUVImage = NULL;
	}
	afx_msg void OnPaint()
	{
		CPaintDC dc(this);
	}
	afx_msg BOOL OnEraseBkgnd(CDC *pDC)
	{
		BOOL bFlag = TRUE;
		if (DD_Draw(m_hDirectDraw, GetSafeHwnd(), &m_imageProperties) != ERROR_SUCCESS) {
			bFlag = CWnd::OnEraseBkgnd(pDC);
		}
		return bFlag;
	}
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_hDirectDraw;
	ImageProperties m_imageProperties;

	LPBYTE m_pYUVImage;
	DWORD m_dwImageCapability;
};

//////////////////////////////////////////////////////////////////////////

BEGIN_TEMPLATE_MESSAGE_MAP(WndYUVImageT, tUnique, CWnd)
	//{{AFX_MSG_MAP(WndYUVImageT)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

typedef WndYUVImageT<DWORD> CWndYUVImage;

//////////////////////////////////////////////////////////////////////////

#endif
