// Petal.cpp: implementation of the Petal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Animate.h"
#include "Petal.h"
#include "Common.h"

double Petal::m_Length;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Petal::Petal()
{
	m_NO = NULL;	
}

Petal::~Petal()
{
	if(m_NO!=NULL)
	{
		delete m_NO;
	}
	m_NO=NULL;
}

Vector Petal::Vertex()
{
	m_Apex=m_p0+Vector(m_Length*cos(m_dTheta),m_Length*sin(m_dTheta));
	return m_Apex;
}

void Petal::Draw(CDC* pDC, CRect* pRect,double k)
{
	int i;
	double y[3];
	Vector vv,v1;
	CPen pv,*pOldPen;
	pDC->MoveTo(pRect->left+int(m_p0.m_dx*k),
		 pRect->bottom-int(m_p0.m_dy*k));
	if(iSegValve>=LIMITSEG)
	{
		vv=(m_Apex-m_p0)/iSegValve;
		for(i=0;i<iSegValve;i++)
		{
			v1=m_p0+vv*(i+1);
//			intertroplation(y,m_NO[i]);
			pv.CreatePen(PS_SOLID,0,RGB(y[0]*255,y[1]*255,y[2]*255));
			pOldPen =pDC->SelectObject(&pv);
			pDC->LineTo(pRect->left+int(v1.m_dx*k),
				pRect->bottom-int(v1.m_dy*k));
			pDC->SelectObject(pOldPen);	
			pv.DeleteObject();
		}
	}
	else
	{
		pDC->LineTo(pRect->left+int(m_Apex.m_dx*k),
			pRect->bottom-int(m_Apex.m_dy*k));
	}
}

Petal& Petal::operator=(const PetalAnimate& v)
{
	m_p0 = v.m_p0;
	m_dTheta = v.m_Theta;
	return *this;
}

fstream& operator>>(fstream& fs,Petal& v)
{
	int i;
	fs>>v.m_p0.m_dx>>v.m_p0.m_dy;
	fs>>v.m_dTheta;
	if(iSegValve>=LIMITSEG)
	{
		if(v.m_NO==NULL)
		{
			v.m_NO=new double[iSegValve];
		}
		for(i=0;i<iSegValve;i++)
		{
			fs>>v.m_NO[i];
		}
	}
	return fs;
}
