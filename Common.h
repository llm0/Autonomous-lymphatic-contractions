#include "MapWindow.h"
#include <fstream>
#include "Text.h"
#include "Vector.h"
#include "lymph_h.h"
#include "LymphaticVessle.h"
#include "PermeatLine.h"
#include "resource.h"
#include "AnimateDlg.h"
#include "Line.h"
#include "Valve.h"
#include "AnimateData.h"
#include "LatticeAnimate.h"

#define BUFFER_LENGTH 256

extern MapWindow* pDrawWnd;
extern CAnimateDlg* pAniDlg;
extern CWnd* pParent;
extern CString FileStr;
extern CString CurPath,FileName;

extern int iMove;
extern double dTest;
extern double NOSaturate;
extern int iTextNum;
extern double dWidth,dHeight;
extern int iCaptionW;
extern int iParStyle;
extern BOOL bDrawing;
extern BOOL bReverCount;
extern Text* pText;
extern DWORD fp;
extern CRect rect;
extern int iCoordinateX,iCoordinateY;
extern int iLine;
extern CString Filter;
extern char szFile [BUFFER_LENGTH];
extern char szFileTitle [BUFFER_LENGTH];
extern char strDir[70];
extern char strTime[128];
extern CPoint* pPoly;
extern fstream FileIn,FileR,fs;
extern CBrush GrayBsh;
extern CBrush RedBsh;
//extern CPen WhitePn;
extern CPen RedPn;
extern int kk;
extern CRect DrawRect;
extern HBITMAP hBmp;
extern char chr[];
extern int iSkip;
extern int iposition;
extern int icc;
extern int iStep;
extern int iUnit;
extern int ih;
extern BOOL bStart;
extern double dr0;
extern double ds;
extern double dx0,dy0;
extern Vector* pPoint;
extern int iSegValve;

extern long lCut;
extern long lOldTime;

extern LymphaticVessle LymVessle;
extern PermeatLine PerLine;
extern Valve ValveDual;
extern double CaThreshold;
extern double Velocity1[HEIGHT];
extern double Velocity2[HEIGHT];
extern double Stress[HEIGHT];
extern long lNowStep;
extern double dcCa[TOTALTIME];
extern double dcNO[TOTALTIME];
extern LatticeAnimate lattice[WIDTH][HEIGHT];
extern CMenu Menu;
extern float Color[21][3];
extern AnimateData* pAniData;
extern int iDraw3D;
extern int iDrawV;
extern int ix;
extern int iy;
extern BOOL bx;
extern BOOL by;
extern BOOL bn;
extern CBitmap bp;
extern BYTE *pData;
extern long nLnBytes;
extern	BITMAPINFO bmpInfo; 
void CALLBACK lpTimerProc(HWND hWnd, UINT uMsg,UINT idEvent,DWORD dwTime);
void WINAPI Stop(HWND hWnd);
void WINAPI SetTimerElapse(HWND hWnd,int iElapse);
void Show(int Par,char*ch);
void Show(double Par,char*ch);
void Show(char Par,char*ch);
void interpolation(double y[3],double x);
void interpolation(Vector* v1,Vector* v2, Vector* v);
double interpolation(int i,int j,CRect* rect,int Unit);
int valve2lymph(int iOr, int iIndex);

extern CRect MapRect;

extern BOOL bSaveNO;
extern __int64 * lPosition;
extern long  lNumber;
extern long ld;
extern long lSize;
extern UINT Update_Msg;
extern int iTime;
extern long lRead;
extern double pm, pn;
extern fstream fr, fv, fo, fFlux, fvalve;
extern int ic;
extern BOOL bSaveCa;
extern double * pPress;
extern double * pPressLumen;
extern double * rMax;

extern long lTime;

struct ssre
{
	int state;
	double delta;
	double ca;
	int found;
	int ic;
	long step;
	double ls;
	double ms;
	double rs;
	double lno;
	double mno;
	double rno;
	int lv;
	int rv;
	int length;
	int interval;
};

extern ssre sre;
