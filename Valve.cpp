// Valve.cpp: implementation of the Valve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Animate.h"
#include "Valve.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Valve::Valve()
{

}

Valve::~Valve()
{
}

void Valve::Draw(CDC* pDC, CRect* pRect,double k, int Unit)
{
	int i,idu;
	idu = Unit + iMove;
	*this = pAniData[Unit].m_Valve;
	if ((iUnit - 1 - idu) % 2 == 0)
	{
		pDC->MoveTo(pRect->left + int((m_MemDL[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemDL[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//1
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->left + int((m_MemDL[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemDL[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}

		pDC->MoveTo(pRect->left + int((m_MemUL[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemUL[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//2
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->left + int((m_MemUL[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemUL[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}

		pDC->MoveTo(pRect->left + int((m_MemDR[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemDR[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//3
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->left + int((m_MemDR[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemDR[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}
		pDC->MoveTo(pRect->left + int((m_MemUR[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemUR[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//4
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->left + int((m_MemUR[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemUR[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}
	}
	else
	{
		pDC->MoveTo(pRect->right - int((m_MemDL[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemDL[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//1
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->right - int((m_MemDL[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemDL[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}

		pDC->MoveTo(pRect->right - int((m_MemUL[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemUL[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//2
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->right - int((m_MemUL[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemUL[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}

		pDC->MoveTo(pRect->right - int((m_MemDR[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemDR[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//3
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->right - int((m_MemDR[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemDR[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}
		pDC->MoveTo(pRect->right - int((m_MemUR[0].m_fx - BEGINROW)*k),
			pRect->bottom - int(m_MemUR[0].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));//4
		for (i = 1; i < VALSEGNUM; i++)
		{
			pDC->LineTo(pRect->right - int((m_MemUR[i].m_fx - BEGINROW)*k),
				pRect->bottom - int(m_MemUR[i].m_fy*k) - int((ih + SPACE)*(iUnit - 1 - idu)));
		}
	}
}

Valve& Valve::operator=(const ValveAnimate& v)
{
	int i;
	m_lActive = v.m_lActive;
	m_rActive = v.m_rActive;
	for(i=0;i<VALSEGNUM;i++)
	{
		m_MemDL[i] = v.m_p[0][i];
		m_MemUL[i] = v.m_p[1][i];
		m_MemDR[i] = v.m_p[2][i];
		m_MemUR[i] = v.m_p[3][i];
	}
	return *this;
}

void Valve::ReadForAnimate(fstream* ff)
{
	/*int i,j;
	*ff>>ValveDual.m_PetalDL>>ValveDual.m_PetalUL>>
		ValveDual.m_PetalDR>>ValveDual.m_PetalUR;
	if(iSegValve==-1)
	{
		for(i=0;i<WIDTH;i++)
		{
			for(j=0;j<HEIGHT;j++)
			{
				*ff>>lattice[i][j].m_cNO;
			}
		}
	}*/
}


void Valve::SaveForMap(fstream* ff)
{
	int i,j;
	for (j = 0; j < iUnit; j++)
	{
		if (pAniData[j].m_Valve.m_lActive)
		{
			for (i = 0; i < VALSEGNUM; i++)
			{
				*ff << pAniData[j].m_Valve.m_p[0][i].m_fx - BEGINROW+j*WIDTH << " " << pAniData[j].m_Valve.m_p[0][i].m_fy << endl;
			}
			*ff << endl;
			for (i = 0; i < VALSEGNUM; i++)
			{
				*ff << pAniData[j].m_Valve.m_p[1][i].m_fx - BEGINROW + j*WIDTH << " " << pAniData[j].m_Valve.m_p[1][i].m_fy << endl;
			}
			*ff << endl;
		}
		if (m_rActive)
		{
			for (i = 0; i < VALSEGNUM; i++)
			{
				*ff << pAniData[j].m_Valve.m_p[2][i].m_fx - BEGINROW + j*WIDTH << " " << pAniData[j].m_Valve.m_p[2][i].m_fy << endl;
			}
			*ff << endl;
			for (i = 0; i < VALSEGNUM; i++)
			{
				*ff << pAniData[j].m_Valve.m_p[3][i].m_fx - BEGINROW + j*WIDTH << " " << pAniData[j].m_Valve.m_p[3][i].m_fy << endl;
			}
			*ff << endl;
		}
	}
}