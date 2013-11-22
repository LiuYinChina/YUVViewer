
#include "MainApplication.h"
#include "MainDialog.h"

#include <AfxDisp.h>

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMainApplication, CWinApp)
	//{{AFX_MSG_MAP(CMainApplication)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CMainApplication::CMainApplication()	{}

/////////////////////////////////////////////////////////////////////////////

CMainApplication theApp;

/////////////////////////////////////////////////////////////////////////////

BOOL CMainApplication::InitInstance()
{
	AfxEnableControlContainer();

	CMainDialog dlg;

	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		dlg.SetSrcFilePath(cmdInfo.m_strFileName);
	}
	dlg.DoModal();
	return FALSE;
}
