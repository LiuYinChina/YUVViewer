
#ifndef MainDialog_H
#define MainDialog_H

//////////////////////////////////////////////////////////////////////////

#include <AfxWin.h>
#include <AfxCmn.h>

#include <AtlFile.h>

#include "Resource.h"

#include "EasySize.h"

#include "WndYUVImage.h"

//////////////////////////////////////////////////////////////////////////

enum
{
	STATE_STOP, 
	STATE_PLAY, 
};

//////////////////////////////////////////////////////////////////////////

class CMainDialog : public CDialog
{
	DECLARE_EASYSIZE;

public:
	CMainDialog(CWnd *pParent = NULL);

	void SetSrcFilePath(LPCTSTR lpszSrcFilePath);

	//{{AFX_DATA(CMainDialog)
	enum { IDD = IDD_YUVViewer_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMainDialog)
protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CMainDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnFourCCSelChange();
	afx_msg void OnImageSizeSelChange();
	afx_msg void OnDisplayComponetsSelChange();
	afx_msg void OnBtnBrowseSrc();
	afx_msg void OnBtnBrowseDst();
	afx_msg void OnBtnFrameJump();
	afx_msg void OnBtnForward();
	afx_msg void OnBtnBack();
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnStop();
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBtnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitControl();
	void Reset();
	DWORD Display();
	DWORD Update(BOOL bSave);
	void EnlargeBuffer(DWORD dwSize);
	void GetImageSize(DWORD &dwWidth, DWORD &dwHeight);

private:
	CAtlFile m_fileSrc;
	CAtlFile m_fileDst;
	CWndYUVImage m_wndYUVImage;

	INT m_nCurSelFourCC;
	INT m_nDisplayComponet;
	DWORD m_dwWidth;
	DWORD m_dwHeight;

	DWORD m_dwState;
	LPBYTE m_pBufferSrc;
	LPBYTE m_pBufferDst;
	DWORD m_dwImageLength;
	DWORD m_dwBufferCapability;

	DWORD m_dwFrameStart;
	DWORD m_dwFrameFinish;

	DWORD m_dwFrameIndex;
	DWORD m_dwFrameCount;
	ULONGLONG m_dwFileLength;
	CRect m_rtMinSize;

	DWORD m_dwLastFileOffset;
	DWORD m_dwLastImageLength;

	CHAR m_szSrcFilePath[MAX_PATH];
};

//////////////////////////////////////////////////////////////////////////

#endif
