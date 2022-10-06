// MapWindow.cpp : implementation file
//
#include "stdafx.h"
#include "MapWindow.h"
#include "afxmt.h"
#include "Animate.h"
#include "math.h"
#include "Common.h"
#include "ColorMap.h"
#include <fstream>
using namespace std;


/////////////////////////////////////////////////////////////////////////////
// MapWindow

UINT MapWindow::m_MapWndCls_Msg;

UINT MapWindow::indicators[]=
{
	IDS_INDICATOR_INFO,
};

MapWindow::MapWindow()
{
	int i;
	for(i=0;i<TOTALTIME;i++)
	{
		dcCa[i]=-10;
		dcNO[i]=dcCa[i];
	}
	m_iCount = 10;
	lOldTime = -1;
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
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(AFX_ID_PAUSE, OnIdPause)
	ON_COMMAND(AFX_ID_OUTPUT, OnIdOutput)
	ON_COMMAND(ID_SAVENO, OnSaveno)
	ON_COMMAND(AFX_ID_SSRELAX, OnSsrelax)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_Menu, &MapWindow::OnRadiusSeries)
	ON_COMMAND(ID_32778, &MapWindow::OnVelocitySeries)
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

void MapWindow::DrawMap(int Unit)
{
	CRect rect;
	int i, idu;
	double k;
	char cc[100];
	CPen Pen, PenOrg, *pOldPen, pv, PenBlack;
	GetClientRect(&rect);
	rect.bottom -= 40;
	idu = Unit + iMove;
	if (rect.right - rect.left > (rect.bottom - rect.top) / double(HEIGHT)*WIDTH)
	{
		k = double(MapRect.right - MapRect.left) / HEIGHT;;
	}
	else
	{
		k = double(MapRect.right - MapRect.left) / WIDTH;;
	}
	Pen.CreatePen(PS_SOLID, 4, RGB(150, 0, 0));
	PenOrg.CreatePen(PS_SOLID, 4, RGB(00, 128, 200));
	PenBlack.CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	m_DrawDC.SelectObject(&PenBlack);
	if (iDraw3D == 1&&Unit==0)
	{
		DrawNO(k);
	}
	else if (iDraw3D == 2 && Unit == 0)
	{
		DrawP(k);
	}
	if (Unit == 0)
	{
		DrawPermeat(DrawRect, k);
	}
	if (iDrawV >= 0)
	{
		DrawV(DrawRect, k, Unit);
	}
	if (Unit == 0)
	{
		sprintf(cc, "Step = %d (%f s)", lNowStep, lNowStep*(2 * 0.75 - 1.0) / 6.0 / 0.01*(0.01 / 25.0)*(0.01 / 25.0));
		m_DrawDC.TextOut(DrawRect.right - 300, DrawRect.top + int((28)*k), cc);
		sprintf(cc, "Test same node = %f,Vy=%f", dTest, pAniData[0].m_Valve.m_p[0][2].m_fy);
		m_DrawDC.TextOut(DrawRect.right - 300, DrawRect.top + int((35)*k), cc);
	} 
	if (idu<iUnit)
	{
		DrawBoundary(k, Unit);
		ValveDual.Draw(&m_DrawDC, &DrawRect, k, Unit);//画瓣膜
	}
	DrawMem(k, Unit, 0);
	DrawMem(k, Unit, 1);
	pOldPen = m_DrawDC.SelectObject(&Pen);
	m_DrawDC.SelectObject(pOldPen);
	Pen.DeleteObject();
	pv.DeleteObject();
	PenOrg.DeleteObject();
	PenBlack.DeleteObject();
}

void MapWindow::DrawMem(double k, int Unit, int iUp)
{
	int i, idu;
	CPen Pen, PenOrg, *pOldPen, pv, PenBlack;
	Pen.CreatePen(1, 3, RGB(255, 0, 0));
	idu = Unit + iMove;
	if (idu >= iUnit)
	{
		return;
	}
	for (i = 0; i < SEGNUM - 1; i++)
	{
		/*if (pAniData[Unit].m_lymani.m_th[i])
		{
			pOldPen = m_DrawDC.SelectObject(&Pen);
		}*/
		if (i >= ((Unit == 0) ? LEFTVALVE : 0))
		{
			if ((iUnit - 1 - idu) % 2 == 0)
			{
				m_DrawDC.MoveTo(DrawRect.left + int((pAniData[Unit].m_lymani.m_p[iUp][i].m_fx - BEGINROW)*k),
					DrawRect.bottom - int((pAniData[Unit].m_lymani.m_p[iUp][i].m_fy)*k)
					- int((ih + SPACE)*(iUnit - 1 - idu)));
				m_DrawDC.LineTo(DrawRect.left + int((pAniData[Unit].m_lymani.m_p[iUp]
					[(i + 1) % SEGNUM].m_fx - BEGINROW)*k), DrawRect.bottom - int((pAniData[Unit]
					.m_lymani.m_p[iUp][(i + 1) % SEGNUM].m_fy)*k) - int((ih + SPACE)
					*(iUnit - 1 - idu)));
			}
			else
			{
				m_DrawDC.MoveTo(DrawRect.right - int((pAniData[Unit].m_lymani.m_p[iUp][i].m_fx - BEGINROW)*k),
					DrawRect.bottom - int((pAniData[Unit].m_lymani.m_p[iUp][i].m_fy)*k)
					- int((ih + SPACE)*(iUnit - 1 - idu)));
				m_DrawDC.LineTo(DrawRect.right - int((pAniData[Unit].m_lymani.m_p[iUp]
					[(i + 1) % SEGNUM].m_fx - BEGINROW)*k), DrawRect.bottom - int((pAniData[Unit]
					.m_lymani.m_p[iUp][(i + 1) % SEGNUM].m_fy)*k) - int((ih + SPACE)
					*(iUnit - 1 - idu)));
			}
		}
		/*if (pAniData[Unit].m_lymani.m_th[i])
		{
			m_DrawDC.SelectObject(pOldPen);
		}*/
	}
	for (i = 0; i < SEGNUM - 1; i++)
	{
		if (pAniData[Unit].m_lymani.m_Ca[0][i]>THRESHOLD)
		{
			pOldPen = m_DrawDC.SelectObject(&Pen);
		}
		if (i >= ((Unit == 0) ? LEFTVALVE : 0))
		{
			if ((iUnit - 1 - idu) % 2 == 0)
			{
				m_DrawDC.MoveTo(DrawRect.left + int((pAniData[Unit].m_lymani.m_p[iUp][i].m_fx - BEGINROW)*k),
					DrawRect.bottom - int(20 + (pAniData[Unit].m_lymani.m_Ca[0][i] / 200 * 10)*k)
					- int((ih + SPACE)*(iUnit - 1 - idu)));
				m_DrawDC.LineTo(DrawRect.left + int((pAniData[Unit].m_lymani.m_p[iUp]
					[(i + 1) % SEGNUM].m_fx - BEGINROW)*k), DrawRect.bottom
					- int(20 + (pAniData[Unit].m_lymani.m_Ca[0][(i + 1) % SEGNUM] / 200 * 10)*k) - int((ih + SPACE)
					*(iUnit - 1 - idu)));
			}
			else
			{
				m_DrawDC.MoveTo(DrawRect.right - int((pAniData[Unit].m_lymani.m_p[iUp][i].m_fx - BEGINROW)*k),
					DrawRect.bottom - int(20 + (pAniData[Unit].m_lymani.m_Ca[0][i] / 200 * 10)*k)
					- int((ih + SPACE)*(iUnit - 1 - idu)));
				m_DrawDC.LineTo(DrawRect.right - int((pAniData[Unit].m_lymani.m_p[iUp]
					[(i + 1) % SEGNUM].m_fx - BEGINROW)*k), DrawRect.bottom
					- int(20 + (pAniData[Unit].m_lymani.m_Ca[0][(i + 1) % SEGNUM] / 200 * 10)*k) - int((ih + SPACE)
					*(iUnit - 1 - idu)));
			}
		}
		if (pAniData[Unit].m_lymani.m_Ca[0][i]>THRESHOLD)
		{
			m_DrawDC.SelectObject(pOldPen);
		}
	}
	Pen.DeleteObject();
}

void MapWindow::OnClose() 
{	
	KillTimer(1001);
	FileIn.close();
	if(FileR.is_open())
	{
		FileR.close();
	}
	DeleteObject(&RedBsh);
	DeleteObject(&RedPn);
	DeleteObject(hBmp);
	DeleteObject(m_hBmp);
	if (fr.is_open())  fr.close();
	if (fo.is_open())  fo.close();
	hBmp=NULL;
	pDrawWnd->m_DrawDC.DeleteDC();
	pParent->SetDlgItemText(IDC_DRAW_BUTTON,"绘图");
	pParent->GetDlgItem(IDC_FILE_BUTTON)->EnableWindow(TRUE);
	pParent->GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
	pParent->SetDlgItemText(IDC_PAUSE_BUTTON,"暂停");
	ValveDual.~Valve();
	if(pText!=NULL)
	{
		delete []pText;
	}
	if(pPoly!=NULL)
	{
		delete []pPoly;
	}
	if(pPoint!=NULL)
	{
		delete []pPoint;
	}
	pText=NULL;
	pDrawWnd=NULL;
	pPoly=NULL;
	CWnd::OnClose();
	delete pDrawWnd;
	Menu.DestroyMenu();
	pDrawWnd=NULL;
	ld=0;
}

void MapWindow::OnSize(UINT nType, int cx, int cy) 
{	
	CRect rect;
	CWnd::OnSize(nType, cx, cy);	
	/*DeleteObject(m_hBmp);
	m_hBmp=CreateCompatibleBitmap(GetDC()->m_hDC,cx,cy);
	m_DrawDC.SelectObject(m_hBmp);*/	
	GetClientRect(&rect);
	m_StatusBar.SetWindowPos(&wndBottom,0,rect.bottom-25,rect.right,25,SWP_DRAWFRAME);
}

void MapWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	char cc[200];
	CRect rect;
	int ix,iy,ii=0, Unit;
	double k,eleven=0;
	if (m_pp != point)
	{
		GetClientRect(&rect);
		rect.bottom -= 40;
		point.y;
		if (rect.right - rect.left > (rect.bottom - rect.top) / double(HEIGHT)*WIDTH)
		{
			k = double(MapRect.right - MapRect.left) / HEIGHT;;
		}
		else
		{
			k = double(MapRect.right - MapRect.left) / WIDTH;
		}
		Unit = int((DrawRect.bottom - point.y) / (ih + SPACE));
		if ((iUnit - 1 - Unit - iMove) % 2 == 0)//反向
		{
			ix = int((point.x - rect.left) / k);
		}
		else
		{
			ix = WIDTH-1-int((point.x - rect.left) / k);
		}
		if (Unit < iUnit)
		{
			Unit = iUnit -1- Unit;
			iy = int((DrawRect.bottom - point.y - int((ih + SPACE)*(iUnit - 1 - Unit - iMove))) / k);
			ii = int((ix - pAniData[Unit].m_lymani.m_p[0][0].m_fx)*SEGNUM / (pAniData[Unit].m_lymani.m_p[0][SEGNUM - 1].m_fx
				- pAniData[Unit].m_lymani.m_p[0][0].m_fx));
			ii>0 ? ii : 0; ii<SEGNUM ? ii : SEGNUM - 1;
			if (ix>0 && ix<WIDTH - 1 && iy>0 && iy < HEIGHT - 1)
			{
				eleven = pow((pAniData[Unit].m_lymani.m_p[0][ii].m_fy -
					pAniData[Unit].m_lymani.m_p[1][ii].m_fy - 10) / (24 - 10), 11.0);
				sprintf(cc, "位置:(%d,%d),密度:%f,vx:%f,vy:%f,CaUp:%f,CaD:%f,NO:%f,Triger:%d,11term=%f", ix, iy,
					pAniData[Unit].m_Lattice[ix][iy].m_dDen, pAniData[Unit].m_Lattice[ix][iy].m_v.m_fx, pAniData[Unit].m_Lattice[ix][iy].m_v.m_fy,
					pAniData[Unit].m_lymani.m_Ca[0][ii], pAniData[Unit].m_lymani.m_Ca[1][ii],
					pAniData[Unit].m_Lattice[ix][iy].m_cNO, pAniData[Unit].m_lymani.m_th[0][ii], eleven);
				m_StatusBar.SetPaneText(0, cc);
			}
			m_pp = point;
		}
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
	Menu.LoadMenu(IDR_MENU_OUTPUT);
	SetMenu(&Menu);
	return 0;
}


void MapWindow::ReadForAnimate(fstream* ff)
{
	int iR,i;
	long *lb, l1; 
	for (i = 0; i < iUnit; i++)
	{
		ff->read((char*)&pAniData[i].m_bfl, sizeof(int)); 
		ff->read((char*)pAniData[i].m_bf, pAniData[i].m_bfl); 
		pAniData[i].m_dl=sizeof(pAniData[i]);
		iR=pAniData[i].unzip();
		if (iR != Z_OK) Show(iR, "解压错误2"); 
		if (i == iUnit/2)//输出的位置
		{
			pAniData[i].DistributeData();
		}
		SaveStress();
	}
	dTest = pAniData[iUnit / 2].m_Lattice[0][HEIGHT / 2].m_dDen -
		pAniData[iUnit / 2-1].m_Lattice[WIDTH-1][HEIGHT / 2].m_dDen;
	if(!ff->eof())
	{
		pAniDlg->m_Skip=ld*100.0/lSize;
		ld += iSkip;
		if (iSkip >= 2 && ld<lSize);
		{
			ff->seekg(lPosition[(ld-1>0)?ld-1:0], ios::beg);
		}
		pAniDlg->PostMessage(Update_Msg,0,0);
	}
	else
	{
	}
}

void MapWindow::InitData(fstream* ff)
{
	__int64 lp, *lb, l1;
	int iR,i,j;
	long st0=0;
	fstream fcut, fp, fpLumen, frMax;
	ld=0;
	fvalve.open((LPCTSTR)(FileName + "\\valve.txt"), ios::out);
	if (lCut != 0)
	{
		fcut.open((LPCTSTR)(FileName + "\\Cut.txt"), ios::out|ios::binary);
	}
	InitSave();
	for (i = 0; i < iUnit*WIDTH; i++)
	{
		pPress[i] = 0;
		pPressLumen[i] = 0;
		rMax[i] = 0;
	}
	lTime = 0;
	for(;;)
	{
		if(!FileIn.eof())
		{
			if (lCut != 0)
			{
				for (i = 0; i < iUnit; i++)
				{
					ff->read((char*)&pAniData[i].m_bfl, sizeof(pAniData[i].m_bfl)); 
					ff->read((char*)pAniData[i].m_bf, pAniData[i].m_bfl);
					pAniData[i].m_dl = sizeof(pAniData[i]);
					st0 = pAniData[i].m_lNowStep;
					iR = pAniData[i].unzip();
					if (sre.state == 1)
					{
						FindRelax(iUnit/2);
					}
					if (iR != Z_OK) Show(iR, "解压错误1");
					if (pAniData[i].m_lNowStep <= lCut)
					{
						fcut.write((char*)&pAniData[i].m_bfl, sizeof(pAniData[i].m_bfl));
						fcut.write((char*)pAniData[i].m_bf, pAniData[i].m_bfl);
					}
					else
					{
						fcut.close();
						lCut = 0;
					}
				}
			}
			else
			{
				for (i = 0; i < iUnit; i++)
				{
					ff->read((char*)&pAniData[i].m_bfl, sizeof(pAniData[i].m_bfl));
					if (!fr.is_open() )//|| ld % 10 != 0)
					{
						ff->seekg(pAniData[i].m_bfl, ios::cur);
					}
					else
					{
						ff->read((char*)pAniData[i].m_bf, pAniData[i].m_bfl);
						pAniData[i].m_dl = sizeof(pAniData[i]);
						iR = pAniData[i].unzip();
					}
				}
				if (fr.is_open() && !FileIn.eof() && bSaveCa&&pAniData[0].m_lNowStep>st0)//&& ld % 10 == 0)
				{
					SaveR();
					Flux(&fFlux);
				}
			}
			lp = *(__int64*)(((char*)&(ff->tellg())) + 8);//超过2G文件的定位
			if (lp < 0 || lp == ((ld - 1>0)?lPosition[ld - 1]:FALSE)) break;//超过2G文件结束不明，定位不变即结束
			lPosition[ld] = lp; 
		} 
		else
		{
			fp.open((LPCTSTR)(FileName + "\\p.txt"), ios::out);
			fpLumen.open((LPCTSTR)(FileName + "\\pLumen.txt"), ios::out);
			frMax.open((LPCTSTR)(FileName + "\\rMax.txt"), ios::out);
			for (i = 0; i < iUnit; i++)
			{
				for (j = 0; j < WIDTH-1; j++)
				{
					if (lTime != 0&&!(i==0&&j==0))
					{
						fp << i*(WIDTH-1) + j << " " << pPress[i*WIDTH + j]/lTime << endl;
						fpLumen << i*(WIDTH - 1) + j << " " << pPressLumen[i*WIDTH + j] / lTime << endl;
						if (rMax[i*WIDTH + j]>0)
						{
							frMax << i*(WIDTH - 1) + j << " " << rMax[i*WIDTH + j] << endl;
						}
					}
				}
			}
			fp.close();
			fpLumen.close();
			frMax.close();
			fvalve.close();
			break; 
		}
		ld++;
		if(ld>=lNumber)
		{
			lb = new __int64[lNumber];
			for(l1=0;l1<lNumber;l1++)
			{
				lb[l1]=lPosition[l1];
			}
			delete lPosition;
			lNumber*=2;
			lPosition = new __int64[lNumber];
			for(l1=0;l1<ld;l1++)
			{
				lPosition[l1]=lb[l1];
			}
			delete lb;
		}
	}
	lSize = ld; 
	ld=0;
	FileIn.close();
	fr.close();
	fo.close();
	FileIn.open((LPCTSTR)FileStr, ios::in | ios::binary);
}

void MapWindow::MoveTo(fstream* ff)
{
	long lp;
	lp = int(iposition*lSize / 100.0); //Show(lp,"aa"); 
	ff->seekg(lPosition[lp],ios::beg);
	ld=lp;
}

void MapWindow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect2,rect3;
  	int i,j;
	CClientDC dc(this);	
	char cc[50];
	int t0,t,t1,t2,t3,t4;
	//t0=20000;t1=100000;t2=500000;t3=1480000;t=1600000-t0;
	t0 = 552000; t1 = 562000; t2 = 568000; t3 = 5980000; t4 = 622000; t = 1600000 - t0;
	if (!bReverCount || m_iCount <= 0)
	{
		if (nIDEvent == 1001)
		{
			GetClientRect(&rect);
			if (FileIn.is_open())
			{
				if (!FileIn.eof())
				{
					ReadForAnimate(&FileIn);
					if (FileR.is_open())
					{
						FileR << lNowStep << " " << LymVessle.m_MemUp.m_segment[201 / 4].m_pst.m_dy
							- LymVessle.m_MemDown.m_segment[201 / 4].m_pst.m_dy
							<< " " << LymVessle.m_MemUp.m_segment[201 * 3 / 8].m_pst.m_dy
							- LymVessle.m_MemDown.m_segment[201 * 3 / 8].m_pst.m_dy
							<< " " << LymVessle.m_MemUp.m_segment[201 * 1 / 2].m_pst.m_dy
							- LymVessle.m_MemDown.m_segment[201 * 1 / 2].m_pst.m_dy << endl;
					}
					iStep++;
					VaryCaNO();
					Ca();
					if (FileIn.eof()||ld>=lSize)
					{
						FileR.close();
						if (iLine == 2)
						{
							//FileIn.seekp(fp,ios::beg);
							//FileIn.clear();
							if (fr.is_open())  fr.close();
							FileIn.close(); 
							FileIn.open((LPCTSTR)FileStr, ios::in | ios::binary);
							ld = 0;
							pAniDlg->m_Skip = 0;
							lOldTime = 0;
							//iSkip = 1;
							pAniDlg->PostMessage(Update_Msg, 1, 0);
						}
						else
						{
							FileIn.clear();
							FileIn.close();
							ReleaseDC(&dc); 
							ld = 0;
							if (fr.is_open())  fr.close();
							PostMessage(WM_CLOSE);
						}
						return;
					}
					if (lNowStep > lOldTime)
					{
						lOldTime = lNowStep;
						m_DrawDC.FillRect(&rect, &GrayBsh);
						for (i = 0; i < iUnit; i++)
						{
							DrawMap(i);
						}
						dc.BitBlt(rect.left, rect.top, rect.right - rect.left,
						  rect.bottom - rect.top - 40, &m_DrawDC, 0, 0, SRCCOPY);
					}
				}
				else
				{
				}
			}
			if ((lNowStep >= t0 && ((lNowStep - t0) % t == 0 || (lNowStep - t1) % t == 0 || (lNowStep - t2) % t == 0 || (lNowStep - t3) % t == 0) || (lNowStep - t4) % t == 0))//bSaveNO && 
			{
				//OnIdOutput();
			}
		}
	}
	else
	{
		if (m_iCount == 10)
		{
			KillTimer(1001);
			SetTimer(1001, 800, NULL);
		}
		else if (m_iCount == 1)
		{
			KillTimer(1001);
			SetTimer(1001, (100 - pAniDlg->m_Time) * 2 + 1, NULL);
		}
		sprintf(cc, "ReverCount：%d", m_iCount);
		dc.TextOut(100, 100, cc);
		m_iCount--;
	}
	ReleaseDC(&dc);
	CWnd::OnTimer(nIDEvent);
}

void MapWindow::DrawPermeat(CRect rect, double k)
{
	int i,j,ix1,ix2,iy1,iy2,ih;
	BOOL bFirstPoint=TRUE, bPositive;
	PerLine = pAniData[0].m_Permeat;
	ih = int(double(MapRect.right - MapRect.left) / WIDTH*HEIGHT);
	ix1=int(PerLine.m_LineV.m_p1.m_dx+0.5);//左侧向右画一点
	ix2=int(PerLine.m_LineUp.m_p2.m_dx);//右侧向左画一点
	iy1=int(PerLine.m_LineDown.m_p1.m_dy+0.5);//下侧向上画一点
	iy2=int(PerLine.m_LineUp.m_p1.m_dy);//上侧向下画一点
	if ((iUnit - 1-iMove) % 2 == 0)
	{
		bPositive = TRUE;
	}
	else
	{
		bPositive = FALSE;
	}
	for(i=ix2;i>=ix1;i--)//最右边点归瓣膜
	{
		if(PerLine.m_LineUp.IsHole(i,int(PerLine.m_LineUp.m_p1.m_dx)))
		{  
			if(!bFirstPoint)
			{
				if(i>ix1)
				{
					m_DrawDC.LineTo(Mapx(i - BEGINROW - 0.5,k,bPositive),
						rect.bottom - int(PerLine.m_LineUp.m_p1.m_dy*k) - int((ih + SPACE)*(iUnit-1-iMove)));
				}
				else
				{
					m_DrawDC.LineTo(Mapx(i - BEGINROW,k,bPositive),
						rect.bottom - int(PerLine.m_LineUp.m_p1.m_dy*k) - int((ih + SPACE)*(iUnit - 1 - iMove)));
				}
			}
			else
			{
				m_DrawDC.MoveTo(Mapx(i - BEGINROW + 0.5, k, bPositive),
					rect.bottom - int(PerLine.m_LineUp.m_p1.m_dy*k) - int((ih + SPACE)*(iUnit - 1 - iMove)));
				bFirstPoint=FALSE;
			}
		}
		else
		{
			bFirstPoint=TRUE;
		//	DrawArrow(Vector(i,PerLine.m_LineUp.m_p1.m_dy),pAniData[Unit].m_Lattice[i][int(PerLine.m_LineUp.m_p1.m_dy)].m_v,k);
		}
	}
	for(j=iy2;j>=iy1;j--)
	{	
		if(PerLine.m_LineV.IsHole(j,int(iy1)))
		{
			if(!bFirstPoint)
			{
				if(j>iy1)
				{
					m_DrawDC.LineTo(Mapx(ix1 - BEGINROW, k, bPositive),
						rect.bottom - int((j - 0.5)*k) - int((ih + SPACE)*(iUnit - 1 - iMove)));
				}
				else
				{
					m_DrawDC.LineTo(Mapx(ix1 - BEGINROW, k, bPositive),
						rect.bottom - int((j)*k) - int((ih + SPACE)*(iUnit - 1 - iMove)));
				}
			}
			else
			{
				m_DrawDC.MoveTo(Mapx(ix1 - BEGINROW, k, bPositive),
					rect.bottom - int((j + 0.5)*k) - int((ih + SPACE)*(iUnit - 1 - iMove)));
				bFirstPoint=FALSE;
			}
		}
		else
		{
			bFirstPoint=TRUE;
		}
	}
	for(i=ix1;i<=ix2;i++)//最右边点归瓣膜
	{
		if(PerLine.m_LineDown.IsHole(i,int(PerLine.m_LineDown.m_p1.m_dx)))
		{  
			if(!bFirstPoint)
			{
				m_DrawDC.LineTo(Mapx(i - BEGINROW + 0.5, k, bPositive),
					rect.bottom - int(PerLine.m_LineDown.m_p1.m_dy*k) - int((ih + SPACE)*(iUnit - 1 - iMove)));
			}
			else
			{
				m_DrawDC.MoveTo(Mapx(i - BEGINROW - 0.5, k, bPositive),
					rect.bottom - int(PerLine.m_LineDown.m_p1.m_dy*k) - int((ih + SPACE)*(iUnit - 1 - iMove)));
				bFirstPoint=FALSE;
			}
		}
		else
		{
			bFirstPoint=TRUE;
		}
	}
}
void MapWindow::DrawV(CRect rect, double k, int Unit)
{
	int i, j;
	for (i = 0; i <WIDTH; i++)
	{
		if (i % ix == 0)
		{
			for (j = 0; j < HEIGHT; j++)
			{
				if (j % iy == 0)
				{
					DrawArrow(Vector(i, j), pAniData[Unit].m_Lattice[i][j].m_v*iDrawV*1000,k,Unit);
				}
			}
		}
	}
}


void MapWindow::VaryCaNO()
{
	int i;
	if(iStep%TOTALTIME==0)
	{
		for(i=0;i<TOTALTIME;i++)
		{
			dcNO[i]=-10;
			dcCa[i]=-10;
		}
	}
	dcNO[iStep%TOTALTIME]=0;
	for(i=0;i<SEGNUM;i++)
	{
		dcNO[iStep%TOTALTIME]+=LymVessle.m_MemUp.m_segment[i].m_dcNO;
		dcNO[iStep%TOTALTIME]+=LymVessle.m_MemDown.m_segment[i].m_dcNO;
	}
	dcCa[iStep%TOTALTIME]=LymVessle.m_MemUp.m_segment[20].m_dcCa;
}


void MapWindow::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Menu.CreatePopupMenu();
	CWnd::OnRButtonUp(nFlags, point);
}

void MapWindow::OnIdPause() 
{
	// TODO: Add your command handler code here
	if(bDrawing)
	{
		KillTimer(1001);
		bDrawing=FALSE;
		Menu.CheckMenuItem(AFX_ID_PAUSE,MF_CHECKED);
	}
	else
	{
		SetTimer(1001,(100-pAniDlg->m_Time)*2+1,NULL);
		bDrawing=TRUE;
		Menu.CheckMenuItem(AFX_ID_PAUSE,MF_UNCHECKED);
	}
}

void MapWindow::OnIdOutput() 
{
	// TODO: Add your command handler code here
	int i,j,x1,y1,k;
	double x,y,p;
	char cc[50];
	fstream file1, file2, file3, file4,fCa;
	sprintf(cc,"%d",lNowStep);
	file1.open((LPCTSTR)(FileName+"\\"+cc+"Draw.txt"),ios::out);
	file1.precision(18);	
	LymVessle.SaveForMap(&file1);
	file1<<endl;
	ValveDual.SaveForMap(&file1);
	file1<<endl;
	PerLine.SaveForMap(&file1);
	file1.close();
	file2.open((LPCTSTR)(FileName+"\\"+cc+"DrawNO.txt"),ios::out);
	file3.open((LPCTSTR)(FileName+"\\"+cc+"DrawP.txt"),ios::out);
	file2.precision(18);file3.precision(18);
	for (j = MapRect.bottom; j >= MapRect.top; j--)
	{
		for (k = 0; k < iUnit; k++)
		{
			for (i = MapRect.left; i <= MapRect.right; i++)
			{
				file2 << interpolation(i, j, &MapRect, k) * 255/ NOSaturate << " ";
				x = double(i - MapRect.left) / (MapRect.right - MapRect.left)*(WIDTH - 1);
				y = double(j - MapRect.top) / (MapRect.bottom - MapRect.top)*(HEIGHT - 1);
				x1 = int(x + 0.5);
				y1 = int(y + 0.5);
				p = pAniData[k].m_Lattice[x1][y1].m_dDen;
				if (p > 50 || p<0 || p>pm)
				{
					if (p > 50)
					{
						p = pm;
					}
					else
					{
						p = pn + (pm - pn)*254/255;
					}
				
				}
				p = (p < pn) ? pn : p;
				file3 << (p - pn) / (pm - pn) * 255 << " ";
			}	
		}
		file2 << endl;
		file3 << endl;
	}
	file2.close();
	file3.close();
	file4.open((LPCTSTR)(FileName + "\\" + cc + "DrawV.txt"), ios::out); 
	for (k = 0; k < iUnit; k++)
	{
		for (i = 0; i < WIDTH; i++)
		{
			for (j = 0; j < HEIGHT; j++)
			{
				if (i % 20 == 0 && (j % 4 == 0 && j <= (HEIGHT - 1) / 2.0 || (HEIGHT - 1 - j) % 4 == 0 && j >= (HEIGHT - 1) / 2.0))
				{
					file4 << i+k*WIDTH << " " << j << " " <<//<<pAniData[Unit].m_Lattice[i][j].m_v.m_fx<<" "<<pAniData[Unit].m_Lattice[i][j].m_v.m_fy
						atan2(pAniData[k].m_Lattice[i][j].m_v.m_fy, pAniData[k].m_Lattice[i][j].m_v.m_fx) << " "
						<< sqrt(pAniData[k].m_Lattice[i][j].m_v.m_fx*pAniData[k].m_Lattice[i][j].m_v.m_fx
						+ pAniData[k].m_Lattice[i][j].m_v.m_fy*pAniData[k].m_Lattice[i][j].m_v.m_fy)
						<< endl;
				}
			}
		}
	}
	file4.close();
	fCa.open((LPCTSTR)(FileName + "\\" + cc + "Ca.txt"), ios::out);
	for (i = 0; i < iUnit; i++)
	{
		for (j = 0; j < SEGNUM - 1; j++)
		{
			if (j >= ((i == 0) ? LEFTVALVE : 0))
			{
				fCa << i*(WIDTH-1)+pAniData[i].m_lymani.m_p[0][j].m_fx << " " << pAniData[i].m_lymani.m_Ca[0][j] << endl;
			}
		}
	}
	fCa.close();
	/*file1.open((LPCTSTR)(FileName + "\\" + cc + "DrawVx.txt"), ios::out);
	file2.open((LPCTSTR)(FileName + "\\" + cc + "DrawVy.txt"), ios::out);
	for(j=HEIGHT-1;j>=0;j--)
	{
		for(i=0;i<WIDTH;i++)
		{
			file1<<pAniData[0].m_Lattice[i][j].m_v.m_fx<<" ";
			file2<<pAniData[0].m_Lattice[i][j].m_v.m_fy<<" ";
		}
		file1 << endl;
		file2 << endl;
	}
	file1.close(); file2.close();*/
//	SaveBMP(FileName + "\\DrawNO.bmp");
}

void MapWindow::DrawNO(double k)
{
	int i, i1, j, j1, Unit;
	int ix1, ix2, iy1, iy2;
	double No;
	ix1 = MapRect.left + 2; ix2 = ix1 + 13;
	iy1 = MapRect.top; iy2 = iy1 + 200;
	ColorMap cmap;
	CRect UnitRect;
	UnitRect = MapRect;
	for (Unit = 0; Unit < iUnit; Unit++)
	{
		UnitRect.top = MapRect.top ;
		UnitRect.bottom = MapRect.top + ih;
		for (j = UnitRect.bottom; j >= UnitRect.top; j--)
		{
			for (i = UnitRect.left; i <= UnitRect.right; i++)
			{
				No = interpolation(i, j, &UnitRect, Unit) / NOSaturate;
				cmap.Map(fmod(No * 255, 255.0), 'n');
				if ((iUnit - 1 - Unit - iMove) % 2 == 0)//反向
				{
					i1 = i;
				}
				else
				{
					i1 = (UnitRect.right + UnitRect.left) - i;
				}
				j1 = j - UnitRect.top + (iUnit - 1 - Unit-iMove)*(ih + SPACE);
				if (j1 >= MapRect.top && j1<MapRect.bottom)
				{
					pData[j1*nLnBytes + (i1 * 3 - UnitRect.left)] = cmap.m_B;
					pData[j1*nLnBytes + (i1 * 3 - UnitRect.left) + 1] = cmap.m_G;
					pData[j1*nLnBytes + (i1 * 3 - UnitRect.left) + 2] = cmap.m_R;
				}
			}
		}
	}
	SetDIBits(m_DrawDC.m_hDC, m_hBmp, 0, UnitRect.bottom - UnitRect.top + 1, pData, &bmpInfo, DIB_RGB_COLORS);
}

void MapWindow::DrawP(double k)
{
	int i, i1, j, j1, x1, y1, ix1, ix2, iy1, iy2, Unit;
	double x,y,p;
	ColorMap cmap;
	CRect UnitRect;
	UnitRect = MapRect;
	ix1 = MapRect.left + 2; ix2 = ix1 + 13;
	iy1 = MapRect.top; iy2 = iy1 + 200;
	for (Unit = 0; Unit < iUnit; Unit++)
	{
		UnitRect.top = MapRect.top;
		UnitRect.bottom = MapRect.top + ih;
		for (j = UnitRect.bottom; j >= UnitRect.top; j--)
		{
			for (i = UnitRect.left; i <= UnitRect.right; i++)
			{
				x = double(i - UnitRect.left) / (UnitRect.right - UnitRect.left)*(WIDTH);
				y = double(j - UnitRect.top) / (UnitRect.bottom - UnitRect.top)*(HEIGHT);
				x1 = int(x + 0.5);
				y1 = int(y + 0.5);
				p = pAniData[Unit].m_Lattice[x1][y1].m_dDen;
				if (p > 50 || p<0 || p>pm)
				{
					p = pm;
				}
				p = (p < pn) ? pn : p;
				//if (i < ix2&&i >= ix1&&j < iy2&&j >= iy1)
				{
					//cmap.Map(double(j - iy1) / (iy2 - iy1) * 255, 'n');
				}
				//else
				{
					cmap.Map((p - pn) / (pm - pn) * 255, 'n');
				}
				if ((iUnit - 1 - Unit - iMove) % 2 == 0)
				{
					i1 = i;
				}
				else
				{
					i1 =(UnitRect.right + UnitRect.left) - i;
				}
				j1 = j - UnitRect.top + (iUnit - 1 - Unit - iMove)*(ih + SPACE);
				if (j1 >= MapRect.top && j1 < MapRect.bottom)
				{
					pData[j1*nLnBytes + (i1 * 3 - UnitRect.left)] = cmap.m_B;
					pData[j1*nLnBytes + (i1 * 3 - UnitRect.left) + 1] = cmap.m_G;
					pData[j1*nLnBytes + (i1 * 3 - UnitRect.left) + 2] = cmap.m_R;
				}
			}
		}
	}
	SetDIBits(m_DrawDC.m_hDC,m_hBmp,0,MapRect.bottom-MapRect.top+1,pData,&bmpInfo,DIB_RGB_COLORS); 
}

void MapWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (int(nChar) == 88)
	{
		bx = TRUE; 
	}
	else if (int(nChar) == 89)
	{
		by = TRUE;
	}
	else if (int(nChar) == 78)
	{
		bn = TRUE;
	}
}

void MapWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//Show(int(nChar), "a");
	if(int(nChar)==69)
	{
		iDraw3D=0;
	}
	else if(int(nChar)==78)
	{
		iDraw3D=1;
	}
	else if(int(nChar)==80)
	{
		iDraw3D=2;
	}
	else if (int(nChar) == 86)
	{
		if (iDrawV < 0)
		{
			iDrawV = 1;
		}
		else
		{
			iDrawV = -1;
		}
	}
	else if (iDrawV>0 && int(nChar) == 187 && (0x8000 & GetKeyState(VK_LSHIFT)))
	{
		iDrawV += 5; 
	}
	else if (iDrawV > 1 && int(nChar) == 189 && (0x8000 & GetKeyState(VK_LSHIFT)))
	{
		iDrawV -= 5;
		if (iDrawV < 0)
		{
			iDrawV = 1;
		}
	}
	else if (int(nChar) == 88)
	{
		bx = FALSE;
	}
	else if (int(nChar) == 89)
	{
		by = FALSE;
	}
	else if (int(nChar) == 78)
	{
		bn = FALSE; 
	}
	else if (bx && int(nChar) == 187 )
	{
		ix += 1;
	}
	else if (ix>1&&bx && int(nChar) == 189)
	{
		ix -= 1;
		if (ix<1)
		{
			ix = 1;
		}
	}
	else if (by && int(nChar) == 187)
	{
		iy += 1; 
	}
	else if (iy>1 && by && int(nChar) == 189)
	{
		iy -= 1;
		if (iy<1)
		{
			iy = 1;
		}
	}
	else if (bn && int(nChar) == 187)
	{
		NOSaturate += 0.5; 
	}
	else if (NOSaturate>1 && bn && int(nChar) == 189)
	{
		NOSaturate -= 0.5;
		if (NOSaturate<1)
		{
			NOSaturate = 1;
		}
	}
	else if (iDrawV > 1 && int(nChar) == 189 && (0x8000 & GetKeyState(VK_LSHIFT)))
	{
		iDrawV -= 5;
		if (iDrawV < 0)
		{
			iDrawV = 1;
		}
	}
	else if (int(nChar)==38)
	{
		if (iMove < iUnit)
		{
			iMove++;
		}
	}
	else if (int(nChar) == 40)
	{
		if (iMove >0)
		{
			iMove--;
		}
	}
	//Show(int(nChar), "a");
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void MapWindow::DrawArrow(Vector p,VectorAnimate v,double k,int Unit)
{
	 CPoint p1,p2;
	double theta=20;
	double length=0.001*k;
	if ((iUnit - 1 - Unit-iMove) % 2 == 0)
	{
		p1.x = DrawRect.left + int(p.m_dx*k);
	}
	else
	{
		p1.x = DrawRect.right - int(p.m_dx*k);
	}
	p1.y = DrawRect.bottom - int(p.m_dy*k) - int((ih + SPACE)*(iUnit-1-Unit-iMove));
	if ((iUnit - 1 - Unit-iMove) % 2 == 0)
	{
		p2=p1+CPoint(int(v.m_fx*k),int(-v.m_fy*k));
	}
	else
	{
		p2 = p1 + CPoint(int(-v.m_fx*k), int(-v.m_fy*k));
	}
    theta=3.1415926*theta/180;//转换为弧度
    double Px,Py,P1x,P1y,P2x,P2y;
    //以P2为原点得到向量P2P1（P）
    Px=p1.x-p2.x;
    Py=p1.y-p2.y;
    //向量P旋转theta角得到向量P1
    P1x=Px*cos(theta)-Py*sin(theta);
    P1y=Px*sin(theta)+Py*cos(theta);
    //向量P旋转-theta角得到向量P2
    P2x=Px*cos(-theta)-Py*sin(-theta);
    P2y=Px*sin(-theta)+Py*cos(-theta);
    //伸缩向量至制定长度
    double x1,x2;
    x1=sqrt(P1x*P1x+P1y*P1y);
	P1x = P1x*0.4;//length/x1;
	P1y = P1y*0.4;// length / x1;
    x2=sqrt(P2x*P2x+P2y*P2y);
	P2x = P2x*0.4;// length / x2;
	P2y = P2y*0.4;// length / x2;
    //平移变量到直线的末端
	P1x = p2.x +P1x;
	P1y = p2.y +P1y;
    P2x=P2x+p2.x;
    P2y=P2y+p2.y;
     CPen pen,pen1,*oldpen;
    pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    oldpen=m_DrawDC.SelectObject(&pen);
    m_DrawDC.MoveTo(p1.x,p1.y);
    m_DrawDC.LineTo(p2.x,p2.y);
	m_DrawDC.LineTo(P1x, P1y);
    m_DrawDC.MoveTo(p2.x,p2.y);
    m_DrawDC.LineTo(P2x,P2y);
    m_DrawDC.SelectObject(oldpen);
}

void MapWindow::SaveBMP(CString ff) 
{
	int width,height,depth,i,j,ix1,iy1,ix2,iy2,nLn,xx;
	double x,y;
	CRect rect;
	fstream file,fr,fg,fb;
	BITMAPINFOHEADER infoHeader;
	BITMAPFILEHEADER fileHeader;
	ZeroMemory(&infoHeader,sizeof(BITMAPINFOHEADER));
	ZeroMemory(&fileHeader,sizeof(BITMAPFILEHEADER));
	//CSemaphore DataSemaphor(1,1,"数据");
	width  = DrawRect.right-DrawRect.left+1;
	height = DrawRect.bottom-DrawRect.top+1;	
	depth = 3;
	fileHeader.bfType = 0x4d42; 
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0 ;
	fileHeader.bfSize = nLn* height + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ;

	infoHeader.biBitCount = 24;
	infoHeader.biClrImportant = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biCompression = BI_RGB ;
	infoHeader.biHeight = height;
	infoHeader.biWidth =  width;
	infoHeader.biPlanes = 1;
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biSizeImage = width * height;
	infoHeader.biXPelsPerMeter = 3000;
	infoHeader.biYPelsPerMeter = 3000;
	file.open(ff,ios::out);
	if(file.is_open())
    {
        file.write((LPSTR)&fileHeader,sizeof(BITMAPFILEHEADER));
        file.write((LPSTR)&infoHeader,sizeof(BITMAPINFOHEADER));
        file.write((LPSTR)pData,nLnBytes*height);
        file.close();
    }
	fr.open(FileName+"\\fr.txt",ios::out);
	fb.open(FileName+"\\fb.txt",ios::out);
	fg.open(FileName+"\\fg.txt",ios::out);
	for(j=DrawRect.bottom;j>=DrawRect.top;j--)
	{
		for(i=DrawRect.left;i<=DrawRect.right;i++)
		{
			xx=pData[(j-MapRect.top)*nLnBytes+(i*3-MapRect.left)];
			fb<<xx<<" ";
			xx=pData[(j-MapRect.top)*nLnBytes+(i*3-MapRect.left)+1];
			fg<<xx<<" ";
			xx=pData[(j-MapRect.top)*nLnBytes+(i*3-MapRect.left)+2];
			fr<<xx<<" ";if(xx>255){	KillTimer(1001);
		bDrawing=FALSE;
		Menu.CheckMenuItem(AFX_ID_PAUSE,MF_CHECKED); Show(lNowStep,"step");Show(i,"i");Show(j,"j");Show(xx,"xx");}
         }
		fr<<endl;fb<<endl;fg<<endl;
	}
	fr.close();	fb.close();	fg.close();
}

void MapWindow::OnSaveno() 
{
	fstream ff;
	int i, j;
	ff.open((LPCTSTR)(FileName + "\\pr.txt"), ios::out);
	for (j = HEIGHT - 1; j--; j >= 0)
	{
		for (i = 0; i < WIDTH; i++)
		{
			if (pAniData[2].m_Lattice[i][j].m_dDen == 1000 && j != 0 && j != HEIGHT - 1)
			{
				ff << 1 << " ";
			}
			else
			{
				ff << 0 << " ";
			}
		}
		ff << endl;
	}
	ff.close();
	/*if(bSaveNO)
	{
		bSaveNO=FALSE;
		Menu.CheckMenuItem(ID_SAVENO,MF_UNCHECKED);
	}
	else
	{
		bSaveNO=TRUE;
		Menu.CheckMenuItem(ID_SAVENO,MF_CHECKED);
	}*/
}

void  MapWindow::Ca()
{
	fstream ff;
	ff.open((LPCTSTR)("Ca.txt"),ios::app);
	ff.precision(18);
	ff<<lNowStep<<" "<<LymVessle.m_MemUp.m_segment[201*3/8].m_dcCa<<endl;
	ff.close();
}

void MapWindow::DrawBoundary(double k, int Unit)
{
	CRect rect;
	CBrush brush;
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	char cc[100];
	brush.CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 0));
	CBrush *pOldBrush = m_DrawDC.SelectObject(&brush);
	CPen *pOldPen = m_DrawDC.SelectObject(&pen);
	rect.left = DrawRect.left+int(0*k);
	rect.right = DrawRect.left + int(WIDTH * k);
	rect.bottom = DrawRect.bottom - int((HEIGHT)* k) -int((ih + SPACE)*Unit);
	rect.top = DrawRect.bottom - int((HEIGHT - 2) * k) - int((ih + SPACE)*Unit);
	m_DrawDC.Rectangle(rect);//上水平线
	rect.bottom = DrawRect.bottom - int(2 * k) - int((ih + SPACE)*Unit);
	rect.top = DrawRect.bottom - int(0 * k) - int((ih + SPACE)*Unit);
	m_DrawDC.Rectangle(rect);//下水平线
	if (Unit == 0)
	{
		rect.bottom = DrawRect.bottom - int(HEIGHT * k) - int((ih + SPACE)*(iUnit - 1 - iMove));
		rect.top = DrawRect.bottom - int(0 * k) - int((ih + SPACE)*(iUnit - 1 - iMove));
		if ((iUnit - 1 - (Unit + iMove)) % 2 == 0)
		{
			rect.left = DrawRect.left + int(0 * k);
			rect.right = DrawRect.left + int(2 * k);	
		}
		else
		{
			rect.right = DrawRect.right - int(0 * k);
			rect.left = DrawRect.right - int(2 * k);
		}
		m_DrawDC.Rectangle(rect);//左竖线
	}
	else if (Unit == iUnit - 1)
	{
		rect.bottom = DrawRect.bottom - int(H1 * k);// +int((HEIGHT + 10)*Unit* k);
		rect.top = DrawRect.bottom - int(0 * k);// +int((HEIGHT + 10)*Unit* k);
		rect.left = DrawRect.left + int((WIDTH - 2) * k);
		rect.right = DrawRect.left + int(WIDTH * k);
		m_DrawDC.Rectangle(rect);//右竖线下
		rect.bottom = DrawRect.bottom - int(HEIGHT * k);// +int((HEIGHT + 10)*Unit* k);
		rect.top = DrawRect.bottom - int(H2 * k);// +int((HEIGHT + 10)*Unit* k);
		m_DrawDC.Rectangle(rect);//右竖线上
	}
	rect.bottom = DrawRect.top + int((HEIGHT+31) * k);
	rect.top = DrawRect.top + int((HEIGHT+39) * k);
	rect.left = DrawRect.left + int(8 * k);
	rect.right = DrawRect.left + int(16 * k);
	//m_DrawDC.Rectangle(rect);//标识1
	sprintf(cc, "Inlet, connect to tissue, pressure p1, equilibrium and zero velocity");
	//m_DrawDC.TextOut(DrawRect.left + int(18 * k), DrawRect.bottom - int((HEIGHT + 37.5) * k), cc);
	brush.DeleteObject();
	if (Unit == iUnit - 1)
	{
		brush.CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0));
		m_DrawDC.SelectObject(&brush);
		rect.bottom = DrawRect.bottom - int(H2 * k);// +int((HEIGHT + 10)*Unit* k);
		rect.top = DrawRect.bottom - int(H1 * k);// +int((HEIGHT + 10)*Unit* k);
		rect.left = DrawRect.left + int((WIDTH - 4) * k);
		rect.right = DrawRect.left + int(WIDTH * k);
		m_DrawDC.Rectangle(rect);//右竖线中
		rect.bottom = DrawRect.top - int((HEIGHT + 31) * k);
		rect.top = DrawRect.top - int((HEIGHT + 39) * k);
		rect.left = DrawRect.left + int(198 * k);
		rect.right = DrawRect.left + int(206 * k);
		//m_DrawDC.Rectangle(rect);//标识2
	}
	sprintf(cc, "Outlet, connect to next unit, pressure p2, press boundary condition");
	//m_DrawDC.TextOut(DrawRect.left + int(208 * k), DrawRect.bottom - int((HEIGHT + 37.5) * k), cc);
	sprintf(cc, "Solid line: impermeable (exept three vitical lines) , curved boundary condition");
	//m_DrawDC.TextOut(DrawRect.left + int(18 * k), DrawRect.bottom - int((HEIGHT + 28) * k), cc);
	m_DrawDC.SelectObject(pOldBrush);
	m_DrawDC.SelectObject(pOldPen);
	brush.DeleteObject();
	pen.DeleteObject();
}

void MapWindow::FindRelax(int Unit)
{
	double delta;
	if (pAniData[Unit].m_lNowStep>20000)
	{
		delta = pAniData[Unit].m_lymani.m_Ca[0][SEGNUM / 2] - sre.ca;
		sre.ca = pAniData[Unit].m_lymani.m_Ca[0][SEGNUM / 2];
		if (delta >= 0.00004&&sre.delta < 0.00004&&sre.found==1)
		{
			if (sre.ic == 2)
			{
				sre.ic++;
				sre.step = pAniData[Unit].m_lNowStep - 30000;
				Show(sre.step, "Contract starting");
				sre.found = 2;
				sre.lv = pAniData[Unit].m_Valve.m_p[1][0].m_fx;
				sre.rv = pAniData[Unit].m_Valve.m_p[3][0].m_fx;
				sre.length = pAniData[Unit].m_Valve.m_p[1][VALSEGNUM - 1].m_fx - pAniData[Unit].m_Valve.m_p[1][0].m_fx;
			}
			else
			{
				sre.ic++;
			}
		}
		sre.delta = delta;
	}
	else
	{
		sre.ca = pAniData[Unit].m_lymani.m_Ca[0][SEGNUM / 2];
	}
}

void MapWindow::OnSsrelax()
{
	if (sre.found == 0)
	{
		Menu.CheckMenuItem(AFX_ID_SSRELAX, MF_CHECKED); 
		sre.found = 1;
	}
	else if (sre.found == 1)
	{
		Menu.CheckMenuItem(AFX_ID_SSRELAX, MF_UNCHECKED);
		sre.found = 0;
	}
}

void MapWindow::CalStress()
{
	double dw,s1=0,s2=0,s3=0;
	int i,iv;
	for (i = 0; i < sre.length; i++)
	{
		iv = int((double)VALSEGNUM / sre.length*i);
		if (iv >= VALSEGNUM) iv = VALSEGNUM;
		dw = (pAniData[iUnit/2].m_Valve.m_p[0][iv].m_fy - pAniData[iUnit/2].m_Valve.m_p[1][iv].m_fy)/2;
		s1 += fabs(pAniData[iUnit/2].m_Lattice[sre.lv + i][HEIGHT / 2].m_v.m_fx / dw);
		dw = (pAniData[iUnit/2].m_Valve.m_p[2][iv].m_fy - pAniData[iUnit/2].m_Valve.m_p[3][iv].m_fy) / 2;
		s3 += fabs(pAniData[iUnit/2].m_Lattice[sre.rv + i][HEIGHT / 2].m_v.m_fx / dw);
		s2 += fabs(pAniData[iUnit/2].m_Lattice[int((sre.lv + sre.rv) / 2 - sre.length / 2) + i][HEIGHT / 2].m_v.m_fx / (HEIGHT - 1));
	}
	s1 /= sre.length; s2 /= sre.length;	s3 /= sre.length;
	sre.ls = s1; sre.ms = s2; sre.rs = s3;
}

void MapWindow::CalNO()
{
	double dw, s1 = 0, s2 = 0, s3 = 0;
	int i, j, iv, ivm, il = 0, im = 0, ir = 0;
	//for (i = 0; i < VALSEGNUM; i++)
	{
		//iv = valve2lymph(i,0);
		iv = 1;//
		for (j = int(pAniData[iUnit/2].m_lymani.m_p[1][iv].m_fy); j <= int(pAniData[iUnit/2].m_lymani.m_p[0][iv].m_fy + 0.5); j++)
		{
			s1 += pAniData[iUnit/2].m_Lattice[int(pAniData[iUnit/2].m_lymani.m_p[0][iv].m_fx)][j].m_cNO; il++;
		}
		//iv = valve2lymph(i, 2);
		iv = SEGNUM - 41;
		for (j = int(pAniData[iUnit/2].m_lymani.m_p[1][iv].m_fy); j <= int(pAniData[iUnit/2].m_lymani.m_p[0][iv].m_fy + 0.5); j++)
		{
			s3 += pAniData[iUnit/2].m_Lattice[int(pAniData[iUnit/2].m_lymani.m_p[0][iv].m_fx)][j].m_cNO; ir++;
		}
	}
	iv = valve2lymph(0, 2) / 2 - (valve2lymph(VALSEGNUM-1, 2)-valve2lymph(0, 2))/2;
	ivm = valve2lymph(0, 2) / 2 + (valve2lymph(VALSEGNUM - 1, 2) - valve2lymph(0, 2)) / 2;
	for (i = iv; i <= ivm; i++)
	{
		for (j = int(pAniData[iUnit/2].m_lymani.m_p[1][i].m_fy); j <= int(pAniData[iUnit/2].m_lymani.m_p[0][i].m_fy + 0.5); j++)
		{
			s2 += pAniData[iUnit/2].m_Lattice[int(pAniData[iUnit/2].m_lymani.m_p[0][i].m_fx)][j].m_cNO; im++;
		}
	}
	s1 /= il; s2 /= im;	s3 /= ir;
	sre.lno = s1; sre.mno = s2; sre.rno = s3;
}

void MapWindow::SaveStress()
{
	if (sre.interval == 0 && pAniData[iUnit/2].m_lNowStep != 0)
	{
		sre.interval = pAniData[iUnit/2].m_lNowStep;
	}
	if (sre.found == 1)
	{
		if (sre.interval == 0&&pAniData[iUnit/2].m_lNowStep!=0)
		{
			sre.interval = pAniData[iUnit/2].m_lNowStep;
		}
		FindRelax(iUnit/2); 
		if (sre.found == 2)
		{
			FileIn.seekg(lPosition[sre.step / sre.interval], ios::beg);
			iSkip = 100.0*sre.step / sre.interval / lSize;
			pAniDlg->m_Skip = iSkip;
			pAniDlg->PostMessage(Update_Msg, 1, 0);
		}
	}
	else if (sre.found == 2 )
	{
		CalStress();
		CalNO();
		fs.open((LPCTSTR)(FileName + "\\stress.txt"), ios::out);
		fs << sre.ls << " " << sre.ms << " " << sre.rs <<" " << sre.lno << " " << sre.mno << " " << sre.rno << endl;
		fs.close();
		Menu.CheckMenuItem(AFX_ID_SSRELAX, MF_UNCHECKED);
		sre.found = 0;
	}
}

int MapWindow::Mapx(double x, double k, BOOL bPos)
{
	if (bPos)
	{
		return DrawRect.left + int(x*k);
	}
	else
	{
		return DrawRect.right - int(x*k);
	}
}

void MapWindow::OnRadiusSeries()
{
	int i;
	if (fr.is_open())
	{
		fr.close();
	}
	fr.open((LPCTSTR)(FileName + "\\sr.txt"), ios::out);
	for (i = 0; i < iUnit*SEGNUM; i++)
	{
		fr << i *2.0*100.0 / 25.0 << " ";
	}
	fr << endl;
}

void MapWindow::InitSave()
{
	int i;
	if (fr.is_open())
	{
		fr.close();
		fo.close();
		fv.close();
	}
	if (fFlux.is_open())
	{
		fFlux.close();
	}
	if (bSaveCa)
	{
		fr.open((LPCTSTR)(FileName + "\\sr.txt"), ios::out);
		fo.open((LPCTSTR)(FileName + "\\open.txt"), ios::out);
		fFlux.open((LPCTSTR)(FileName + "\\flux.txt"), ios::out);
		fv.open((LPCTSTR)(FileName + "\\vr.txt"), ios::out);
	}
	/*for (i = 0; i < iUnit*SEGNUM; i++)
	{
		fr << i *2.0*100.0 / 25.0 << " ";
	}
	fr << endl;*/
}

void MapWindow::SaveR()
{
	int i,j,k;
	double y[2], open[2];
	double dr, dq;
	double v1, v2;
	for (i = 0; i < iUnit; i++)
	{
		v1 = 100;
		v2 = 100;
		for (j = 0; j < VALSEGNUM; j++)
		{
			if (pAniData[i].m_Valve.m_p[1][j].m_fy - pAniData[i].m_Valve.m_p[0][j].m_fy < v1)
			{
				v1 = pAniData[i].m_Valve.m_p[1][j].m_fy - pAniData[i].m_Valve.m_p[0][j].m_fy;
			}
			if (pAniData[i].m_Valve.m_p[3][j].m_fy - pAniData[i].m_Valve.m_p[2][j].m_fy< v2)
			{
				v2 = pAniData[i].m_Valve.m_p[3][j].m_fy - pAniData[i].m_Valve.m_p[2][j].m_fy;
			}
		}
		fvalve << v1 << " " << v2 << " ";
	}
	fvalve << endl;
	if (fr.is_open())
	{
		for (i = 0; i < iUnit; i++)
		{
			/*for (j = 0; j < SEGNUM; j++)
			{
			fv << (pAniData[i].m_lymani.m_p[0][j].m_fy - pAniData[i].m_lymani.m_p[1][j].m_fy)/25.0-ic*0.5<<" ";
			//fv << pAniData[i].m_lymani.m_Ca[0][j]<< " ";
			}*/
			fv << pAniData[i].m_Valve.m_p[1][VALSEGNUM - 1].m_fy - pAniData[i].m_Valve.m_p[0][VALSEGNUM - 1].m_fy << " "
				<< pAniData[i].m_Valve.m_p[3][VALSEGNUM - 1].m_fy - pAniData[i].m_Valve.m_p[2][VALSEGNUM - 1].m_fy << " ";
		}
		fv<< endl;
		for (i = 0; i < iUnit; i++)
		{
			for (j = 0; j < SEGNUM; j++)
			{
				dr += pAniData[i].m_lymani.m_p[0][j].m_fy - pAniData[i].m_lymani.m_p[1][j].m_fy;
			}
			for (j = 72; j < WIDTH; j++)
			{
				for (k = 0; k < HEIGHT; k++)
				{
					if (k < pAniData[i].m_lymani.m_p[0][int((j - 72) / 2)].m_fy&&k > pAniData[i].m_lymani.m_p[1][int((j - 72) / 2)].m_fy)
					{
						dq += pAniData[i].m_Lattice[j][k].m_v.m_fx;
					}
				}
			}
			dq /= (WIDTH-72);
		}
		dr /= 3;
		dr /= SEGNUM;
		fr << pAniData[0].m_lNowStep << " " << dr << " " << dq << endl;
	}
	if (fo.is_open())
	{
		fo << pAniData[0].m_lNowStep*(2 * 0.75 - 1) / 6.0 / 0.01*(0.01 / 25)*(0.01 / 25) << " ";
	}
	for (i = 0; i < iUnit; i++)
	{
		open[0] = 1000; open[1] = 1000;
		for (j = 1; j < VALSEGNUM; j++)
		{
			y[0] = fabs(pAniData[i].m_Valve.m_p[0][j].m_fy - pAniData[i].m_Valve.m_p[1][j].m_fy);
			y[1] = fabs(pAniData[i].m_Valve.m_p[2][j].m_fy - pAniData[i].m_Valve.m_p[3][j].m_fy);
			if (y[0] < open[0])
			{
				open[0] = y[0];
			}
			if (y[1] < open[1])
			{
				open[1] = y[1];
			}
		}
		fo << (open[0]*10+72+464*i) * 100.0 / 25 << " " << (open[1]*10+304+464*i) * 100.0 / 25 << " ";
	}
	for (i = 0; i < iUnit; i++)
	{
		fo << (0.5 * 10 + 72 + 464 * i) * 100.0 / 25 << " " << (0.5 * 10 + 304 + 464 * i) * 100.0 / 25 << " ";
	}
	fo << endl;
	ic++;
}

void MapWindow::OnVelocitySeries()
{
	// TODO:  在此添加命令处理程序代码
}
double MapWindow::Flux(fstream* ff)
{
	int i, j, k, iCount,iCountl;
	double Q1,Q2,Q3,Q1l,dr;
	Q1 = 0; Q2 = 0; Q3 = 0; Q1l = 0;
	LymVessle = pAniData[0].m_lymani;
	for (j = 0; j < HEIGHT; j++)
	{
		//for (i = 0; i < WIDTH; i++)
		{
		//	if (i >= int(pAniData[0].m_Valve.m_p[0][0].m_fx) && i <= int(pAniData[0].m_Valve.m_p[2][0].m_fx))
			{
				if (LymVessle.IsInner(int((pAniData[0].m_Valve.m_p[0][0].m_fx + pAniData[0].m_Valve.m_p[2][0].m_fx)/2), j) == 1)
				{
					Q1 += pAniData[0].m_Lattice[int((pAniData[0].m_Valve.m_p[0][0].m_fx + pAniData[0].m_Valve.m_p[2][0].m_fx) / 2)][j].m_v.m_fx;
				}
			}
		}
	}
	//Q1 /= (int(pAniData[0].m_Valve.m_p[2][0].m_fx) - int(pAniData[0].m_Valve.m_p[0][0].m_fx) + 1);

	LymVessle = pAniData[iUnit - 1].m_lymani;
	for (j = 0; j < HEIGHT; j++)
	{
		i = int(pAniData[iUnit - 1].m_Valve.m_p[2][0].m_fx);
	//	for (i = 0; i < WIDTH; i++)
		{
	//		if (i >= int(pAniData[iUnit - 1].m_Valve.m_p[0][0].m_fx) && i <= int(pAniData[iUnit - 1].m_Valve.m_p[2][0].m_fx))
			{
				if (LymVessle.IsInner(i, j) == 1)
				{
					Q2 += pAniData[iUnit - 1].m_Lattice[i][j].m_v.m_fx;
				}
				/*if (LymVessle.IsInner(int(pAniData[2].m_Valve.m_p[2][0].m_fx), j) == 1)
				{
					Q2 += pAniData[2].m_Lattice[int(pAniData[2].m_Valve.m_p[2][0].m_fx)][j].m_v.m_fx;
				}*/
			}
		}
	}
	//Q2 /= (int(pAniData[iUnit - 1].m_Valve.m_p[2][0].m_fx) - int(pAniData[iUnit - 1].m_Valve.m_p[0][0].m_fx) + 1);

	LymVessle = pAniData[iUnit-1].m_lymani;
	for (j = 0; j < HEIGHT; j++)
	{
		//for (i = 0; i < WIDTH; i++)
		{
			//if (i >= int(pAniData[iUnit - 1].m_Valve.m_p[0][0].m_fx) && i <= int(pAniData[iUnit - 1].m_Valve.m_p[2][0].m_fx))
			{
				//if (LymVessle.IsInner(i, j) == 1)
				if (j < H2 && j > H1)
				{
					Q3 += pAniData[iUnit - 1].m_Lattice[WIDTH-2][j].m_v.m_fx;
				}
			}
		}
	}
	//Q3 /= (int(pAniData[iUnit - 1].m_Valve.m_p[2][0].m_fx) - int(pAniData[iUnit - 1].m_Valve.m_p[0][0].m_fx)+1);
	ff->precision(18);
	*ff << pAniData[iUnit - 1].m_lNowStep << " " << Q1 << " " << Q2 << " " << Q3 << " " << pAniData[1].m_lymani.m_Ca[0][(RIGHTVALVE+LEFTVALVE)/2] << endl;
	if (pAniData[0].m_lNowStep > 200000)
	{
		for (i = 0; i < iUnit; i++)
		{
			for (j = 0; j < WIDTH; j++)
			{
				iCount = 0;
				Q1 = 0;
				iCountl = 0;
				Q1l = 0;
				for (k = 0; k < HEIGHT; k++)
				{
					if ((LymVessle.IsInner(j, k) == -1 || (i == 0 && j < pAniData[0].m_lymani.m_p[0][0].m_fx)) && pAniData[i].m_Lattice[j][k].m_dDen<2)
					{
						Q1 += pAniData[i].m_Lattice[j][k].m_dDen / 3;
						iCount++;
					}
					else if (LymVessle.IsInner(j, k) == 1 && pAniData[i].m_Lattice[j][k].m_dDen<2)
					{
						Q1l += pAniData[i].m_Lattice[j][k].m_dDen / 3;
						iCountl++;
					}
				}
				if (iCount != 0)
				{
					pPress[i*WIDTH + j] += Q1 / iCount;
				}
				if (iCountl != 0)
				{
					pPressLumen[i*WIDTH + j] += Q1l / iCountl;
				}
				for (k = 0; k < SEGNUM; k++)
				{
					if (pAniData[i].m_lymani.m_p[0][k].m_fx >= j&&pAniData[i].m_lymani.m_p[0][k].m_fx < j + 1)
					{
						dr = pAniData[i].m_lymani.m_p[0][k].m_fy - pAniData[i].m_lymani.m_p[1][k].m_fy;
						if (dr>rMax[i*WIDTH + j])
						{
							rMax[i*WIDTH + j] = dr;
						}
						break;
					}
				}
			}
		}
		lTime++;
	}
	return Q3;
}

