
#ifndef AboutDlg_H
#define AboutDlg_H

//////////////////////////////////////////////////////////////////////////

#include <AfxWin.h>

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd *pParent = NULL);
	virtual ~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ABOUT };

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange *pDX);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
};

//////////////////////////////////////////////////////////////////////////

#endif
