// lymph.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "lymph.h"
#include "lymphDlg.h"
#include "Common.h"
#include "Evolute.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ClymphApp

BEGIN_MESSAGE_MAP(ClymphApp, CWinApp)
	//{{AFX_MSG_MAP(ClymphApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClymphApp construction

ClymphApp::ClymphApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only ClymphApp object

ClymphApp theApp;

/////////////////////////////////////////////////////////////////////////////
// ClymphApp initialization

int ClymphApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFileFind FileFind;
	LPSTR szCmdLine = GetCommandLine(); //获取命令行参数；
	argv = (char **)CommandLineToArgvW((LPCWSTR)szCmdLine, &argc); //拆分命令行参数字符串；

	GetModuleFileName(NULL,szFile,BUFFER_LENGTH);
	CurPath=szFile;
	CurPath=CurPath.Left(int(CurPath.GetLength())-(int(strlen(m_pszAppName))+5));
	if(!FileFind.FindFile((LPCTSTR)(CurPath+"\\dat")))
	{
		if(!CreateDirectory((LPCTSTR)(CurPath+"\\dat"),0))
		{
			MessageBox(NULL,"Error",(LPCTSTR)(CurPath+"\\DAT"),MB_OK);
		}
	}
	ClymphDlg dlg;
	int nResponse = int(dlg.DoModal());	
	if (nResponse == IDOK)
	{	
		pInfoDlg=new InfoDialog;
		m_pMainWnd=pInfoDlg;
		pInfoDlg->Create (IDD_INFO_DIALOG,NULL);
		pInfoDlg->ShowWindow (SW_SHOW);
		pParent=m_pMainWnd;	
		Update_Msg=RegisterWindowMessage("Update data now--lymph13");
		InfoDialog::m_Update_Msg=Update_Msg;
		InfoDialog::m_MapWndCls_Msg=RegisterWindowMessage("MapWndIsClosing--lymph13");
		MapWindow::m_MapWndCls_Msg=InfoDialog::m_MapWndCls_Msg;
		AfxBeginThread((AFX_THREADPROC)Start,pParent->m_hWnd,THREAD_PRIORITY_IDLE);
		return 1;
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return 0 so that we exit the
	//  application, rather than start the application's message pump.
	return 0;
}
