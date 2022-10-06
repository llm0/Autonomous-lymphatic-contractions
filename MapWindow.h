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
#include <fstream>
using namespace std;

class MapWindow : public CWnd
{
// Construction
public:
	MapWindow();

// Attributes
public:

// Operations
public:
	void DrawMap(int Unit);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawPermeat(CRect rect, double k);
	int m_iWidth;
	int m_iHeght;
	int m_iCount;
	CPoint m_pp;
	LymphaticVessle * m_pLymVessle;
	HBITMAP m_hBmp;
	CDC m_DrawDC;
	CWnd* m_pInfoWnd;
	CStatusBar m_StatusBar;
	void ReadForAnimate(fstream* ff);
	static UINT indicators[];
	static UINT m_MapWndCls_Msg; 
	virtual ~MapWindow();
	void VaryCaNO();
	void DrawNO(double k);
	void DrawP(double k);
	void DrawArrow(Vector p,VectorAnimate v,double k,int Unit);
	void SaveBMP(CString);
	void Ca();
	void InitData(fstream* ff);
	void MoveTo(fstream* ff);
	void DrawV(CRect rect, double k, int Unit);
	void DrawMem(double k, int Unit,int iUp);
	int Mapx(double x, double k, BOOL bPos);
	void DrawBoundary(double k, int Unit);
	void FindRelax(int Unit);
	void CalStress();
	void CalNO();
	void SaveStress();
	void SaveR();
	void InitSave();
	double Flux(fstream *ff);
	// Generated message map functions
protected:
	//{{AFX_MSG(MapWindow)
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnIdPause();
	afx_msg void OnIdOutput();
	afx_msg void OnSaveno();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSsrelax();
	afx_msg void OnRadiusSeries();
	afx_msg void OnVelocitySeries();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPWINDOW_H__AFAE7F81_BEE3_11D5_A1BB_00D0094C43DF__INCLUDED_)
