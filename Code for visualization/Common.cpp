#include "stdafx.h"
#include "MapWindow.h"
#include <fstream>
#include "Common.h"


MapWindow* pDrawWnd;
CAnimateDlg* pAniDlg;
CWnd* pParent;
CString FileStr,FileName;
double dTest;
double NOSaturate = 1;
int iMove = 0;
int iTextNum;
double dWidth,dHeight;
int iCaptionW;
int iParStyle;
BOOL bDrawing=TRUE;
BOOL bReverCount = FALSE;
Text* pText=NULL;
CPoint* pPoly=NULL;
DWORD fp;
CRect rect;
int iCoordinateX,iCoordinateY;
CString Filter("data file(*.dat)|*.dat|All File(*.*)|*.*||");
char szFile [BUFFER_LENGTH];
char szFileTitle [BUFFER_LENGTH];
char strDir[70];
char strTime[128];
CString CurPath;
int kk=0;
int iSkip=1;
int iposition = 0;
int iLine=2;
int icc=-1;
int iStep = 0;
int iUnit = 1;
int ih;
BOOL bStart=FALSE;
double dr0;
double ds;
double dx0,dy0;
fstream FileIn, FileR, fs, fFlux;
CBrush GrayBsh(RGB(200,200,200));
CBrush RedBsh(RGB(255,0,0));
CPen RedPn(PS_SOLID,1,RGB(255,0,0));
CRect DrawRect;
HBITMAP hBmp;
char chr[50];
Vector* pPoint=NULL;
int iSegValve;
long lCut = 0;
long lOldTime=-1;
LymphaticVessle LymVessle;
PermeatLine PerLine;
Valve ValveDual;
double CaThreshold=0.1;//
double Velocity1[HEIGHT];
double Velocity2[HEIGHT];
double Stress[HEIGHT];
long lNowStep;
double dcCa[TOTALTIME];
double dcNO[TOTALTIME];
//LatticeAnimate lattice[WIDTH][HEIGHT];
CMenu Menu;
float Color[21][3] = { {0,0,0},{0,0,0.25},{0,0,0.5},{0,0,0.75},{0,0,1},
      {0,0.25,1},{0,0.5,1},{0,0.75,1},{0,1,1},{0,1,0.75},
      {0,1,0.5},{0,1,0.25},{0,1,0},{0.25,1,0},{0.5,1,0},
      {0.75,1,0},{1,1,0},{1,0.5,0},{1,0,0},{1,0,0.5},{1,0,1}
      };

AnimateData* pAniData =NULL;
int iDraw3D=1;
int iDrawV = -1;
int ix = 4;
int iy = 8;
BOOL bx = FALSE;
BOOL by = FALSE;
BOOL bn = FALSE;
CBitmap bp;
BYTE *pData=NULL;
long nLnBytes;
CRect MapRect;
BITMAPINFO bmpInfo; 
BOOL bSaveNO=FALSE;
fstream fr, fv, fo, fvalve;
int ic = 0;
double * pPress=NULL;
double * pPressLumen = NULL;
double * rMax = NULL;
long lTime = 0;

__int64 * lPosition;
long  lNumber=100000;
long ld=0;
long lSize=0;
long lRead=0;
UINT Update_Msg;
int iTime;
double pm = 1.01;
double pn = 0.9982;
ssre sre = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
BOOL bSaveCa;

int valve2lymph(int iOr, int iIndex)
{
	double dx, dDeltax;
	int ir;
	dDeltax = (pAniData[iUnit/2].m_lymani.m_p[0][SEGNUM - 1].m_fx - pAniData[iUnit/2].m_lymani.m_p[0][0].m_fx ) / (SEGNUM - 1);
	if (iIndex == 0 || iIndex == 1)
	{
		ir = int((pAniData[iUnit / 2].m_Valve.m_p[0][iOr].m_fx - pAniData[iUnit / 2].m_lymani.m_p[0][0].m_fx) / dDeltax + 0.1);
	}
	else
	{
		ir = int((pAniData[iUnit / 2].m_Valve.m_p[2][iOr].m_fx - pAniData[iUnit / 2].m_lymani.m_p[0][0].m_fx) / dDeltax + 0.1);
	}
	if (ir < 0) ir = 0;
	return  ir;
}

void WINAPI Stop(HWND hWnd)
{
	KillTimer(hWnd,1001);
}

void Show(int Par,char*ch)
{
	char cc[50];
	sprintf(cc,"%d",Par);
	MessageBox(NULL,ch,cc,MB_OK);
}

void Show(double Par,char*ch)
{
	char cc[50];
	sprintf(cc,"%20.18e",Par);
	MessageBox(NULL,ch,cc,MB_OK);
}

void Show(char Par,char*ch)
{
	char cc[50];
	sprintf(cc,"%c",Par);
	MessageBox(NULL,ch,cc,MB_OK);
}

void intertroplation(double y[3],double x)
{
	int ix;
	ix=int(x*21);
	y[0]=Color[ix][0]+(Color[ix+1][0]-Color[ix][0])*(x*21-ix);
	y[1]=Color[ix][1]+(Color[ix+1][1]-Color[ix][1])*(x*21-ix);
	y[2]=Color[ix][2]+(Color[ix+1][2]-Color[ix][2])*(x*21-ix);
}

void interpolation(Vector* v1,Vector* v2, Vector* v)
{
	v->m_dy=v1->m_dy+(v2->m_dy-v1->m_dy)/(v2->m_dx-v1->m_dx)*(v->m_dx-v1->m_dx);
}

double interpolation(int i,int j,CRect* rect,int Unit)
{
	int x1,y1,x2,y2;
	double x,y;
	Vector v1,v2,v11,v22,v;
	x = double(i-rect->left)/(rect->right-rect->left)*(WIDTH-1);
	y = double(j-rect->top)/(rect->bottom-rect->top)*(HEIGHT-1);
	x1=int(x);x2=x1+1;
	y1=int(y);y2=y1+1;
	if(y2<HEIGHT)
	{
		v1.m_dx=y1;
		v1.m_dy = pAniData[Unit].m_Lattice[x1][y1].m_cNO;
		v2.m_dy = pAniData[Unit].m_Lattice[x1][y2].m_cNO;
		v2.m_dx=y2;
		v11.m_dx=y;
		interpolation(&v1,&v2,&v11);
		if(x2<WIDTH)
		{
			v1.m_dy = pAniData[Unit].m_Lattice[x2][y1].m_cNO; v2.m_dy = pAniData[Unit].m_Lattice[x2][y2].m_cNO;
			v22.m_dx=y;
			interpolation(&v1,&v2,&v22);
		}
		else
		{
			v22.m_dy = pAniData[Unit].m_Lattice[x1][y1].m_cNO;
		}
	}
	else
	{
		v11.m_dy = pAniData[Unit].m_Lattice[x1][y1].m_cNO;
	}
	if(x2<WIDTH)
	{
		v.m_dx=x;v11.m_dx=x1;v22.m_dx=x2;
		interpolation(&v11,&v22,&v);
	}
	else
	{
		v.m_dy=v11.m_dy;
	}
	return v.m_dy;
}