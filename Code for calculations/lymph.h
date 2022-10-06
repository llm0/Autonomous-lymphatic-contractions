// lymph.h : main header file for the lymph application
//

#if !defined(AFX_lymph_H__01398C44_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_lymph_H__01398C44_0E8B_11D6_845F_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// ClymphApp:
// See lymph.cpp for the implementation of this class
//

class ClymphApp : public CWinApp
{
public:
	ClymphApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClymphApp)
	public:
	virtual int InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(ClymphApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_lymph_H__01398C44_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
