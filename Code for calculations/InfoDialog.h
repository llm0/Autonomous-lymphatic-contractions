#if !defined(AFX_INFODIALOG_H__C7DB2546_0EAC_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_INFODIALOG_H__C7DB2546_0EAC_11D6_845F_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// InfoDialog dialog

#include "MapWindow.h"
#include "Resource.h"
#include <fstream>
#include "Lattice.h"
#include "LymphaticVessle.h"

class InfoDialog : public CDialog
{
// Construction
public:
	InfoDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(InfoDialog)
	enum { IDD = IDD_INFO_DIALOG };
	long	m_Step;
	double	m_TotalDensity;
	float	m_Velocity;
	//}}AFX_DATA

public:
	int m_iWidth;
	int m_iHeight;
	MapWindow* m_pMapWnd;
	LymphaticVessle* m_pLymVessle;
	Lattice** m_Lattice;
	static UINT m_Update_Msg;
	static UINT m_MapWndCls_Msg;
public:
	afx_msg LRESULT OnUpdateNow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMapWndCls(WPARAM wParam, LPARAM lParam);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InfoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(InfoDialog)
	afx_msg void OnDrawButton();
	afx_msg void OnExitButton();
	afx_msg void OnClose();
	afx_msg void OnSaveButton();
	afx_msg void OnShowWindow(int bShow, UINT nStatus);
	afx_msg void OnVelocityMenuitem();
	afx_msg void OnShearMenuitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFODIALOG_H__C7DB2546_0EAC_11D6_845F_00E04C392A24__INCLUDED_)
