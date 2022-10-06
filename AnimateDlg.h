// AnimateDlg.h : header file
//

#if !defined(AFX_AnimateDLG_H__133B4026_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_)
#define AFX_AnimateDLG_H__133B4026_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MySliderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CAnimateDlg dialog

class CAnimateDlg : public CDialog
{
// Construction
public:
	CAnimateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAnimateDlg)
	enum { IDD = IDD_ANIMATE_DIALOG };
	MySliderCtrl	m_TimeCtrl;
	MySliderCtrl	m_SkipCtrl;
	CString	m_File;
	int		m_Time;
	int		m_Skip;
	BOOL	m_Cycle;
	//}}AFX_DATA
	static UINT m_Update_Msg;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAnimateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDrawButton();
	afx_msg void OnFileButton();
	afx_msg void OnReleasedcaptureTimeSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSkipSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCycleCheck();
	afx_msg void OnPauseButton();
	afx_msg LRESULT OnUpdateNow(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCutButton();
	BOOL m_ReverCount;
	afx_msg void OnBnClickedRevcouCheck();
	afx_msg void OnBnClickedCheckSaveca();
	BOOL m_SaveCa;
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AnimateDLG_H__133B4026_CF6E_11D5_A1BB_00D0094C43DF__INCLUDED_)
