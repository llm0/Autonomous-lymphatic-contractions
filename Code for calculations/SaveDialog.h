#if !defined(AFX_SAVEDIALOG_H__DC43B360_12A0_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_SAVEDIALOG_H__DC43B360_12A0_11D6_845F_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SaveDialog dialog

class SaveDialog : public CDialog
{
// Construction
public:
	SaveDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SaveDialog)
	enum { IDD = IDD_SAVE_DIALOG };
	long	m_CrtStep;
	long	m_SaveStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SaveDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SaveDialog)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnShowWindow(int bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEDIALOG_H__DC43B360_12A0_11D6_845F_00E04C392A24__INCLUDED_)
