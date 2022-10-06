#include "stdafx.h"
#include "Line.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Line::Line(Vector p1,Vector p2)
{
	m_p1=p1;
	m_p2=p2;
	if(fabs(m_p2.m_dx-m_p1.m_dx)>1e-16)
	{
		m_dk=(m_p2.m_dy-m_p1.m_dy)/(m_p2.m_dx-m_p1.m_dx);
		if(m_dk>100)
		{
			m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
		}
	}
	else if(fabs(m_p2.m_dy-m_p1.m_dy)>1e-16)
	{
		m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
	}
	else
	{
		m_dk=0;
		m_dk_re=0;
	}
}

void Line::init()
{
	if(fabs(m_p2.m_dx-m_p1.m_dx)>1e-16)
	{
		m_dk=(m_p2.m_dy-m_p1.m_dy)/(m_p2.m_dx-m_p1.m_dx);
		m_dk_re=1e17;
		if(m_dk>100)
		{
			m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
		}
	}
	else if(fabs(m_p2.m_dy-m_p1.m_dy)>1e-16)//接近竖直线
	{
		m_dk_re=(m_p2.m_dx-m_p1.m_dx)/(m_p2.m_dy-m_p1.m_dy);
		m_dk=2e17;
	}
	else
	{
		m_dk=0;
		m_dk_re=0;
	}
}

Line::~Line()
{

}

Line& Line::operator=(const Line &v)
{
	m_p1 = v.m_p1;
	m_p2 = v.m_p2;
	m_dk = v.m_dk;
	return *this;
}


BOOL Line::IsHole(int ix,int ix0)
{
	int m = 0;
	if (m_iHole != 0)
	{
		m=int(ix-ix0)%(m_iHole*2);
	}
	if(m>=0&&m<=1)//0,1,2是，3,4,5,不是
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
