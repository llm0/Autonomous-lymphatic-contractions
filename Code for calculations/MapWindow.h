#if !defined(AFX_MAPWINDOW_H__AFAE7F81_BEE3_11D5_A1BB_00D0094C43DF__INCLUDED_)
#define AFX_MAPWINDOW_H__AFAE7F81_BEE3_11D5_A1BB_00D0094C43DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MapWindow window

#include "LymphaticVessle.h"

class MapWindow : public CWnd
{
// Construction
public:
	MapWindow();

// Attributes
public:

// Operations
public:
	void DrawMap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_iWidth;
	int m_iHeght;
	CPoint m_pp;
	 LymphaticVessle * m_pLymVessle;
	HBITMAP m_hBmp;
	CDC m_DrawDC;
	CWnd* m_pInfoWnd;
	CStatusBar m_StatusBar;
	static UINT indicators[];
	static UINT m_MapWndCls_Msg; 
	virtual ~MapWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(MapWindow)
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPWINDOW_H__AFAE7F81_BEE3_11D5_A1BB_00D0094C43DF__INCLUDED_)
