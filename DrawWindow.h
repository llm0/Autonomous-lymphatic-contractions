#if !defined(AFX_DRAWWINDOW_H__133B402F_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_)
#define AFX_DRAWWINDOW_H__133B402F_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DrawWindow window

class DrawWindow : public CWnd
{
// Construction
public:
	DrawWindow();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DrawWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DrawWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(DrawWindow)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWWINDOW_H__133B402F_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_)
