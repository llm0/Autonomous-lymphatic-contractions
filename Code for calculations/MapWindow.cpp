// MapWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MapWindow.h"
#include "afxmt.h"
#include "math.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MapWindow

UINT MapWindow::m_MapWndCls_Msg;

UINT MapWindow::indicators[]=
{
	IDS_INDICATOR_READY,
	IDS_INDICATOR_POSITION
};

MapWindow::MapWindow()
{
}

MapWindow::~MapWindow()
{
}


BEGIN_MESSAGE_MAP(MapWindow, CWnd)
	//{{AFX_MSG_MAP(MapWindow)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MapWindow message handlers

void MapWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(rect.left,rect.top,rect.right-rect.left,
		rect.bottom-rect.top
		,&m_DrawDC,0,0,SRCCOPY);
	// Do not call CWnd::OnPaint() for painting messages
}

void MapWindow::DrawMap()
{
	CRect rect;
    int i;
	double k;
	char cc[100];
	CPen Pen,PenOrg,*pOldPen;
	GetClientRect(&rect);
	rect.bottom-=40;
	k=double(rect.bottom-rect.top)/HEIGHT;
	if (WIDTH*k > rect.right - rect.left)
	{
		k = double(rect.right - rect.left) / WIDTH;
	}
	Pen.CreatePen(PS_SOLID,0,RGB(255,0,0));
	PenOrg.CreatePen(PS_SOLID,0,RGB(00,128,200));
	BoundaryLattice Outer,Inner;
	sprintf(cc, "ff+fCa:%f,fk+fb:%f", SolValve.m_MemUL.m_Seg[VALSEGNUM-2]->m_f.m_dy
		, SolValve.m_MemUL.m_Seg[VALSEGNUM - 2]->m_ka.m_dy);//*(2*0.75-1.0)/6.0/0.01*(0.01/25.0)*(0.01/25.0),"s");
	m_DrawDC.TextOut(100, 50, cc);
	for(i=0;i<SEGNUM-1;i++)//»­ÉÏ±ßÄ¤
	{
		if(LymVessle.m_MemUp.m_segment[i].m_bPassTh)
		{
			pOldPen =m_DrawDC.SelectObject(&Pen);
		}
		m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemUp.m_segment[i].m_pst.m_dx*k),
			rect.bottom-int(LymVessle.m_MemUp.m_segment[i].m_pst.m_dy*k));
		m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemUp.m_segment[(i+1)%SEGNUM].m_pst.m_dx*k),
			rect.bottom-int(LymVessle.m_MemUp.m_segment[(i+1)%SEGNUM].m_pst.m_dy*k));
		if(LymVessle.m_MemUp.m_segment[i].m_bPassTh)
		{
			m_DrawDC.SelectObject(pOldPen);
		}
	}
	for(i=0;i<SEGNUM-1;i++)//»­ÏÂ±ßÄ¤
	{
		if(LymVessle.m_MemDown.m_segment[i].m_bPassTh)
		{
			pOldPen =m_DrawDC.SelectObject(&Pen);
		}
		m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemDown.m_segment[i].m_pst.m_dx*k),
			rect.bottom-int(LymVessle.m_MemDown.m_segment[i].m_pst.m_dy*k));
		m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemDown.m_segment[(i+1)%SEGNUM].m_pst.m_dx*k),
			rect.bottom-int(LymVessle.m_MemDown.m_segment[(i+1)%SEGNUM].m_pst.m_dy*k));
		if(LymVessle.m_MemDown.m_segment[i].m_bPassTh)
		{
			m_DrawDC.SelectObject(pOldPen);
		}
	}
	/*for (i = 0; i<SolValve.m_il; i++)//»­°êÄ¤Link
	{
		m_DrawDC.MoveTo(rect.left + int(SolValve.m_link[i].m_iFlux*k),
			rect.bottom - int(SolValve.m_link[i].m_iFluy*k));
		m_DrawDC.LineTo(rect.left + int(SolValve.m_link[i].m_iSolx*k),
			rect.bottom - int(SolValve.m_link[i].m_iSoly*k));
		m_DrawDC.Ellipse(rect.left + int((SolValve.m_link[i].m_bf.m_dx - 0.3)*k),
			rect.bottom - int((SolValve.m_link[i].m_bf.m_dy - 0.3)*k),
			rect.left + int((SolValve.m_link[i].m_bf.m_dx + 0.3)*k),
			rect.bottom - int((SolValve.m_link[i].m_bf.m_dy + 0.3)*k));
	}*/
	for(i=0;i<PermeatLine::m_iBouNum;i++)//»­¿ÉÍ¨Í¸Çø
	{
	//	for(j=0;j<PermeatLine::m_pBoud[i].m_iLinNum;j++)
		{
	/*		m_DrawDC.MoveTo(rect.left+int(PermeatLine::m_pBoud[i].m_link[j].m_iFlux*k),
				rect.bottom-int(PermeatLine::m_pBoud[i].m_link[j].m_iFluy*k));
			m_DrawDC.LineTo(rect.left+int(PermeatLine::m_pBoud[i].m_link[j].m_iSolx*k),
				rect.bottom-int(PermeatLine::m_pBoud[i].m_link[j].m_iSoly*k));*/
			m_DrawDC.Ellipse(rect.left+int((PermeatLine::m_pBoud[i].m_link[0].m_iSolx-0.3)*k),
			rect.bottom-int((PermeatLine::m_pBoud[i].m_link[0].m_iSoly-0.3)*k),
			rect.left+int((PermeatLine::m_pBoud[i].m_link[0].m_iSolx+0.3)*k),
			rect.bottom-int((PermeatLine::m_pBoud[i].m_link[0].m_iSoly+0.3)*k));
		}
	}
	SolValve.Draw(&m_DrawDC, &rect, k);
	//for(i=0;i<ValveDual.m_il;i++)//°êÄ¤µÄLink
	{
	/*	m_DrawDC.MoveTo(rect.left+int(ValveDual.m_link[i].m_iFlux*k),
			rect.bottom-int(ValveDual.m_link[i].m_iFluy*k));
		m_DrawDC.LineTo(rect.left+int(ValveDual.m_link[i].m_iSolx*k),
			rect.bottom-int(ValveDual.m_link[i].m_iSoly*k));
		m_DrawDC.Ellipse(rect.left+int((ValveDual.m_link[i].m_iSolx-0.1)*k),
			rect.bottom-int((ValveDual.m_link[i].m_iSoly-0.1)*k),
			rect.left+int((ValveDual.m_link[i].m_iSolx+0.1)*k),
			rect.bottom-int((ValveDual.m_link[i].m_iSoly+0.1)*k));*/
	}

	m_DrawDC.MoveTo(rect.left+int(WIDTH/2*k),
			rect.bottom-int(0));
	for(i=0;i<HEIGHT;i++)//»­vx
	{
		m_DrawDC.LineTo(rect.left+int((WIDTH/2+lattice[WIDTH/2][i].m_v.m_dx*500)*k),
			rect.bottom-int(i*k));
	}

	m_DrawDC.MoveTo(rect.left+int(WIDTH*9/10*k),
			rect.bottom-int((HEIGHT-1)*k));
	m_DrawDC.LineTo(rect.left+int(WIDTH*9/10*k),
			rect.bottom-int(0));
	for(i=0;i<HEIGHT;i++)//»­vx
	{
		m_DrawDC.LineTo(rect.left+int((WIDTH*9/10+lattice[int(WIDTH*9.0/10)][i].m_v.m_dx*400)*k),
			rect.bottom-int(i*k));
	}
	pOldPen =m_DrawDC.SelectObject(&Pen);
	m_DrawDC.MoveTo(rect.left+int(WIDTH/2*k),
			rect.bottom-int(0));
	for(i=0;i<HEIGHT;i++)//»­stress
	{
		m_DrawDC.LineTo(rect.left+int((WIDTH/2+lattice[WIDTH/2][i].m_Stress*4000)*k),
			rect.bottom-int(i*k));
	}
	m_DrawDC.MoveTo(rect.left+int(10*k),
			rect.bottom-int(50+dcNO[0]*0.1));
	for(i=1;i<100;i++)//»­NOÔö³¤
	{
		if(dcNO[i]>=0)
		{
			m_DrawDC.LineTo(rect.left+int((10+i)*k),
			rect.bottom-int(50+dcNO[i]*0.1));
		}
	}
	m_DrawDC.SelectObject(pOldPen);
	m_DrawDC.MoveTo(rect.left+int(10*k),
			rect.bottom-int(50+dcCa[0]*50));
	for(i=1;i<100;i++)//»­CaÔö³¤
	{
		if(dcCa[i]>=0)
		{
			m_DrawDC.LineTo(rect.left+int((10+i)*k),
			rect.bottom-int(50+dcCa[i]*50));
		}
	}
	m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemUp.m_segment[0].m_pst.m_dx*k),
			rect.bottom-int((20+LymVessle.m_MemUp.m_segment[0].m_pst.m_dy+CaThreshold*10)*k));
	m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemUp.m_segment[SEGNUM-1].m_pst.m_dx*k),
			rect.bottom-int((20+LymVessle.m_MemUp.m_segment[0].m_pst.m_dy+CaThreshold*10)*k));
	m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemUp.m_segment[0].m_pst.m_dx*k),
			rect.bottom-int((20+LymVessle.m_MemUp.m_segment[0].m_pst.m_dy+CaThreshold*10)*k));
	for(i=0;i<SEGNUM;i++)//»­ÉÏÄ¤Ca
	{
		if(LymVessle.m_MemUp.m_segment[i].m_dcCa>=CaThreshold)
		{
			pOldPen =m_DrawDC.SelectObject(&PenOrg);
		}
		m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemDown.m_segment[i].m_pst.m_dx*k),
			rect.bottom-int((20+LymVessle.m_MemUp.m_segment[0].m_pst.m_dy
			+LymVessle.m_MemUp.m_segment[i].m_dcCa*10)*k));
		m_DrawDC.SelectObject(pOldPen);
	}
	m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemDown.m_segment[0].m_pst.m_dx*k),
			rect.bottom-int((-20+LymVessle.m_MemDown.m_segment[0].m_pst.m_dy+CaThreshold*10)*k));
	m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemDown.m_segment[SEGNUM-1].m_pst.m_dx*k),
			rect.bottom-int((-20+LymVessle.m_MemDown.m_segment[0].m_pst.m_dy+CaThreshold*10)*k));
	m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemDown.m_segment[0].m_pst.m_dx*k),
			rect.bottom-int((-20+LymVessle.m_MemDown.m_segment[0].m_pst.m_dy+CaThreshold*10)*k));
	for(i=0;i<SEGNUM;i++)//»­ÏÂÄ¤Ca
	{
		if(LymVessle.m_MemDown.m_segment[i].m_dcCa>=CaThreshold)
		{
			pOldPen =m_DrawDC.SelectObject(&PenOrg);
		}
		m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemDown.m_segment[i].m_pst.m_dx*k),
			rect.bottom-int((-20+LymVessle.m_MemDown.m_segment[0].m_pst.m_dy
			+LymVessle.m_MemDown.m_segment[i].m_dcCa*10)*k));
		m_DrawDC.SelectObject(pOldPen);
	}

	pOldPen =m_DrawDC.SelectObject(&Pen);
	m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemUp.m_segment[0].m_pst.m_dx*k),
			rect.bottom-int((20+LymVessle.m_MemUp.m_segment[0].m_pst.m_dy+10)*k));
	m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemUp.m_segment[SEGNUM-1].m_pst.m_dx*k),
			rect.bottom-int((20+LymVessle.m_MemUp.m_segment[0].m_pst.m_dy+10)*k));
	for(i=0;i<SEGNUM;i++)//»­ÉÏÄ¤NO
	{
		Outer.m_dx=LymVessle.m_MemUp.m_segment[i].m_pst.m_dx;
		Outer.m_dy=LymVessle.m_MemUp.m_segment[i].m_pst.m_dy;
		LymVessle.m_MemUp.m_segment[i].Nearest(&Outer,&Inner);
		if (i == 0)
		{
			m_DrawDC.MoveTo(rect.left + int(LymVessle.m_MemUp.m_segment[i].m_pst.m_dx*k),
				rect.bottom - int((20 + LymVessle.m_MemUp.m_segment[0].m_pst.m_dy
				+ lattice[Inner.m_ix][Inner.m_iy].m_dcNO * 10)*k));
		}
		else
		{
			m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemUp.m_segment[i].m_pst.m_dx*k),
				rect.bottom-int((20+LymVessle.m_MemUp.m_segment[0].m_pst.m_dy
				+lattice[Inner.m_ix][Inner.m_iy].m_dcNO*10)*k));
		}
	}
	m_DrawDC.MoveTo(rect.left+int(LymVessle.m_MemDown.m_segment[0].m_pst.m_dx*k),
			rect.bottom-int((-20+LymVessle.m_MemDown.m_segment[0].m_pst.m_dy+10)*k));
	m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemDown.m_segment[SEGNUM-1].m_pst.m_dx*k),
			rect.bottom-int((-20+LymVessle.m_MemDown.m_segment[0].m_pst.m_dy+10)*k));
	for(i=0;i<SEGNUM;i++)//»­ÏÂÄ¤NO
	{
		Outer.m_dx=LymVessle.m_MemDown.m_segment[i].m_pst.m_dx;
		Outer.m_dy=LymVessle.m_MemDown.m_segment[i].m_pst.m_dy;
		LymVessle.m_MemDown.m_segment[i].Nearest(&Outer,&Inner);
		if (i == 0)
		{
			m_DrawDC.MoveTo(rect.left + int(LymVessle.m_MemDown.m_segment[i].m_pst.m_dx*k),
				rect.bottom - int((-20 + LymVessle.m_MemDown.m_segment[0].m_pst.m_dy
				+ lattice[Inner.m_ix][Inner.m_iy].m_dcNO * 10)*k));
		}
		else
		{
			m_DrawDC.LineTo(rect.left+int(LymVessle.m_MemDown.m_segment[i].m_pst.m_dx*k),
			rect.bottom-int((-20+LymVessle.m_MemDown.m_segment[0].m_pst.m_dy
			+lattice[Inner.m_ix][Inner.m_iy].m_dcNO*10)*k));
		}
	}
	m_DrawDC.SelectObject(pOldPen);
	m_DrawDC.MoveTo(int(rect.left+m_iWidth*k),0);
	m_DrawDC.LineTo(int(rect.left+m_iWidth*k),rect.bottom);
	m_DrawDC.Ellipse(rect.left+int((LymVessle.m_MemUp.m_segment[0].m_pst.m_dx-0.3)*k),
			rect.bottom-int((LymVessle.m_MemUp.m_segment[0].m_pst.m_dy-0.3)*k),
			rect.left+int((LymVessle.m_MemUp.m_segment[0].m_pst.m_dx+0.3)*k),
			rect.bottom-int((LymVessle.m_MemUp.m_segment[0].m_pst.m_dy+0.3)*k));
	m_DrawDC.Ellipse(rect.left+int((LymVessle.m_MemDown.m_segment[0].m_pst.m_dx-0.3)*k),
			rect.bottom-int((LymVessle.m_MemDown.m_segment[0].m_pst.m_dy-0.3)*k),
			rect.left+int((LymVessle.m_MemDown.m_segment[0].m_pst.m_dx+0.3)*k),
			rect.bottom-int((LymVessle.m_MemDown.m_segment[0].m_pst.m_dy+0.3)*k));
}

void MapWindow::OnClose() 
{
	DeleteObject(m_hBmp);
	m_DrawDC.DeleteDC();
	m_pInfoWnd->PostMessage(m_MapWndCls_Msg);
	CWnd::OnClose();
}

void MapWindow::OnSize(UINT nType, int cx, int cy) 
{	
	CRect rect;
	CWnd::OnSize(nType, cx, cy);	
	DeleteObject(m_hBmp);
	m_hBmp=CreateCompatibleBitmap(GetDC()->m_hDC,cx,cy);
	m_DrawDC.SelectObject(m_hBmp);	
	GetClientRect(&rect);
	m_StatusBar.SetWindowPos(&wndBottom,0,rect.bottom-25,rect.right,25,SWP_DRAWFRAME);
}

void MapWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	char cc[200];
	CRect rect;
	int ix,iy,ii;
	double k;
	if(m_pp!=point)
	{
		GetClientRect(&rect);
		rect.bottom-=40;
		k = double(rect.bottom - rect.top) / HEIGHT;
		if (WIDTH*k > rect.right - rect.left)
		{
			k = double(rect.right - rect.left) / WIDTH;
		}
		ix = int((point.x - rect.left) / k);//*HEIGHT/(rect.bottom-rect.top));
		iy = int((rect.bottom - point.y) / k);// *HEIGHT / (rect.bottom - rect.top));
		ii=int((ix-LymVessle.m_MemUp.m_dx1)*SEGNUM/(LymVessle.m_MemUp.m_dx2-LymVessle.m_MemUp.m_dx1));
		ii>0?ii:0;ii<SEGNUM?ii:SEGNUM-1;
		if(ix>0&&ix<WIDTH-1&&iy>0&&iy<HEIGHT-1)
		{
			sprintf_s(cc, "Î»ÖÃ:(%d,%d),ÃÜ¶È:%f,vx:%f,vy:%f,±êÖ¾:%d,CaUp:%f,CaDown:%f,NO:%f,Triger:%d", ix, iy, lattice[ix][iy].m_dDen,
				float(lattice[ix][iy].m_v.m_dx),float(lattice[ix][iy].m_v.m_dy),lattice[ix][iy].m_iSol,
				float(LymVessle.m_MemUp.m_segment[ii].m_dcCa),float(LymVessle.m_MemDown.m_segment[ii].m_dcCa),
				float(lattice[ix][iy].m_dcNO),LymVessle.m_MemUp.m_segment[ii].m_bPassTh);
			m_StatusBar.SetPaneText(1,cc);
		//	m_StatusBar.SetPaneText(0,cc);
		}
		m_pp=point;
	}
	CWnd::OnMouseMove(nFlags, point);
}

int MapWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rect;
	GetClientRect(&rect);
	m_StatusBar.CreateEx(this);
	m_StatusBar.SetWindowPos(&wndBottom,0,rect.bottom-25,rect.right,25,SWP_DRAWFRAME);
	m_StatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
	return 0;
}
