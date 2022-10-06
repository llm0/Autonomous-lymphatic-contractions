// InfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "InfoDialog.h"
#include "afxmt.h"
#include "SaveDialog.h"
#include "Common.h"
#include "CUDA_Transfer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InfoDialog dialog

UINT InfoDialog::m_Update_Msg;
UINT InfoDialog::m_MapWndCls_Msg;
extern void DataBak(int bAction);

InfoDialog::InfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(InfoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(InfoDialog)
	m_Step = 0;
	m_TotalDensity = 0.0;
	m_Velocity = 0.0f;
	m_pMapWnd=NULL;
	m_pLymVessle=NULL;
	//}}AFX_DATA_INIT
}


void InfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InfoDialog)
	DDX_Text(pDX, IDC_STEP_EDIT, m_Step);
	DDX_Text(pDX, IDC_TOTAL_DENSITY_EDIT, m_TotalDensity);
	DDX_Text(pDX, IDC_VELOCITY_EDIT, m_Velocity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(InfoDialog, CDialog)
	//{{AFX_MSG_MAP(InfoDialog)
	ON_BN_CLICKED(IDC_DRAW_BUTTON, OnDrawButton)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SAVE_BUTTON, OnSaveButton)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_PULSE_MENUITEM, OnVelocityMenuitem)
	ON_REGISTERED_MESSAGE(m_Update_Msg,OnUpdateNow)
	ON_REGISTERED_MESSAGE(m_MapWndCls_Msg,OnMapWndCls)
	ON_COMMAND(ID_SHEAR_MENUITEM, OnShearMenuitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InfoDialog message handlers

void InfoDialog::OnDrawButton() 
{
	CRect rect;
	rect.left=10;
	rect.top=10;
	rect.right=1500;
	rect.bottom=480;
	CBrush RedBsh(RGB(255,0,0)),GreBsh(RGB(200,200,200));
	CSemaphore DataSemaphor(1,1,"膜数据同步_并行13");
	DataSemaphor.Lock();
	if(m_pMapWnd==NULL)
	{
		m_pMapWnd=new MapWindow;
		m_pMapWnd->m_pLymVessle=m_pLymVessle;
		m_pMapWnd->m_iHeght=m_iHeight;
		m_pMapWnd->m_iWidth=m_iWidth;
		m_pMapWnd->m_pInfoWnd=(CWnd*)this;
		m_pMapWnd->CreateEx(0,AfxRegisterWndClass(
			CS_HREDRAW|CS_VREDRAW,0,(HBRUSH)GreBsh.m_hObject),
			"画图窗口",WS_OVERLAPPEDWINDOW,
			rect,NULL,0);
		m_pMapWnd->ShowWindow(SW_SHOW);

		m_pMapWnd->m_DrawDC.CreateCompatibleDC(m_pMapWnd->GetDC());
		m_pMapWnd->m_hBmp=CreateCompatibleBitmap(m_pMapWnd->GetDC()->m_hDC,rect.right
		-rect.left,rect.bottom-rect.top);
		m_pMapWnd->m_DrawDC.SelectObject(m_pMapWnd->m_hBmp);
		m_pMapWnd->m_DrawDC.SelectObject(&RedBsh);
		m_pMapWnd->m_DrawDC.FillRect(&rect,&GreBsh);
		SetDlgItemText(IDC_DRAW_BUTTON,"关闭绘图");
	}
	else
	{
		m_pMapWnd->SendMessage(WM_CLOSE);
	}
	DataSemaphor.Unlock();	
}

void InfoDialog::OnExitButton() 
{
	SendMessage(WM_CLOSE);	
}

void InfoDialog::OnClose() 
{
	CSemaphore DataSemaphor(1,1,"膜数据同步_并行13");
	DataSemaphor.Lock();
	if(m_pMapWnd!=NULL)
	{
		m_pMapWnd->SendMessage(WM_CLOSE);
		delete m_pMapWnd;
	}
	freeMem();
	CDialog::OnClose();
	DestroyWindow();
	delete this;		
}

LRESULT InfoDialog::OnUpdateNow(WPARAM wParam, LPARAM lParam)
{
	if (bCanCuda)
	{
		SetWindowText("动态信息(可以并行计算）");
	}
	else
	{
		SetWindowText("动态信息(不能并行计算）");
	}
	return UpdateData(0);
}

LRESULT InfoDialog::OnMapWndCls(WPARAM wParam, LPARAM lParam)
{
	SetDlgItemText(IDC_DRAW_BUTTON,"绘图");
	delete m_pMapWnd;
	m_pMapWnd=NULL;
	return 1;
}

void InfoDialog::OnSaveButton() 
{
	CSemaphore DataSemaphor(1,1,"膜数据同步_并行13");
	if(m_pMapWnd!=NULL)
	{
		m_pMapWnd->ShowWindow(SW_HIDE);
	}
	DataSemaphor.Lock();
	SaveDialog SaveDlg;
	if(SaveDlg.DoModal()==IDCANCEL)
	{
		DataSemaphor.Unlock();
		if (m_pMapWnd != NULL)
		{
			m_pMapWnd->ShowWindow(SW_SHOW);
		}
		if(MessageBox("你需要保存数据以求速度场吗？","注意!",MB_OKCANCEL)==IDOK)
		{
			DataBak(0);
		}
		return;
	}
	if(m_pMapWnd!=NULL)
	{
		m_pMapWnd->ShowWindow(SW_SHOW);
	}
	SetDlgItemText(IDC_SAVE_BUTTON,"保存");	
	DataSemaphor.Unlock();	
}

void InfoDialog::OnShowWindow(int bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void InfoDialog::OnVelocityMenuitem() 
{
	CMenu* pMenu;
	pMenu = GetMenu();
	if (!bPulse)
	{
		pMenu->CheckMenuItem(ID_PULSE_MENUITEM, MF_CHECKED);
		bPulse = 1;
	}
	else
	{
		pMenu->CheckMenuItem(ID_PULSE_MENUITEM, MF_UNCHECKED);
		bPulse = 0;
	}
}

void InfoDialog::OnShearMenuitem() 
{
	// TODO: Add your command handler code here
	
}
