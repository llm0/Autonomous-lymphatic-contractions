// SaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "lymph.h"
#include "SaveDialog.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SaveDialog dialog


SaveDialog::SaveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SaveDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(SaveDialog)
	m_CrtStep = lNowStep;
	m_SaveStep = lNowStep + 20;
	//}}AFX_DATA_INIT
}


void SaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SaveDialog)
	DDX_Text(pDX, IDC_CRT_STEP_EDIT, m_CrtStep);
	DDX_Text(pDX, IDC_START_EDIT, m_SaveStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SaveDialog, CDialog)
	//{{AFX_MSG_MAP(SaveDialog)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SaveDialog message handlers

void SaveDialog::OnOk() 
{
	UpdateData(1);
	if(m_SaveStep<m_CrtStep)
	{
		return;
	}
	lSave = m_SaveStep;
	CDialog::OnOK();		
}

void SaveDialog::OnCancel() 
{
	CDialog::OnCancel();	
}

void SaveDialog::OnShowWindow(int bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}
