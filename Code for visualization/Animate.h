// Animate.h : main header file for the Animate application
//

#if !defined(AFX_Animate_H__133B4024_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_)
#define AFX_Animate_H__133B4024_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_
#include <fstream>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAnimateApp:
// See Animate.cpp for the implementation of this class
//

class CAnimateApp : public CWinApp
{
public:
	CAnimateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAnimateApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Animate_H__133B4024_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_)
