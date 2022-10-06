// MySliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "animate.h"
#include "MySliderCtrl.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MySliderCtrl

MySliderCtrl::MySliderCtrl()
{
}

MySliderCtrl::~MySliderCtrl()
{
}


BEGIN_MESSAGE_MAP(MySliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(MySliderCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MySliderCtrl message handlers

void MySliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(pDrawWnd!=NULL)
	{
		if(bDrawing)
		{
			pDrawWnd->KillTimer(1001);
		}
	}	
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void MySliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//GetParent()->UpdateData(TRUE);
	if(pDrawWnd!=NULL)
	{
		if(bDrawing)
		{
			pDrawWnd->SetTimer(1001, (100 - pAniDlg->m_Time) * 2 + 1, NULL);
		}
	}
	CSliderCtrl::OnLButtonUp(nFlags, point);
}
