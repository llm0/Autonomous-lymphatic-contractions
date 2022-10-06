#if !defined(AFX_MYSLIDERCTRL_H__239F9B56_2C11_4183_B8C4_D090B85B788C__INCLUDED_)
#define AFX_MYSLIDERCTRL_H__239F9B56_2C11_4183_B8C4_D090B85B788C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySliderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MySliderCtrl window

class MySliderCtrl : public CSliderCtrl
{
// Construction
public:
	MySliderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MySliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MySliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(MySliderCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSLIDERCTRL_H__239F9B56_2C11_4183_B8C4_D090B85B788C__INCLUDED_)
