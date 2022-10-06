// PermeatLine.cpp: implementation of the PermeatLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PermeatLine.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PermeatLine::PermeatLine()
{
}

PermeatLine::~PermeatLine()
{

}

void PermeatLine::init()
{
	int x1, x2;
	x1 = int(WIDTH / 30.0) + 4; //4:2int(WIDTH / 30.0) + 3;//?????????
	x2 = int(WIDTH / 5.0);
	m_LineUp.m_p1 = Vector(x1, H2);
	m_LineUp.m_p2 = Vector(x2, H2);
	m_LineDown.m_p1 = Vector(x1, H1);
	m_LineDown.m_p2 = Vector(x2, H1);
	m_LineV.m_p1 = Vector(x1, H1);
	m_LineV.m_p2 = Vector(x1, H2);
	m_LineUp.m_iHole=2;m_LineDown.m_iHole=2;m_LineV.m_iHole=2;
	m_LineUp.m_iType=2;m_LineDown.m_iType=1;m_LineV.m_iType=-1;
	m_LineUp.init();m_LineDown.init();m_LineV.init();
}

PermeatLine& PermeatLine::operator=(const PermeatAnimate& v)
{
	m_LineUp.m_iHole =  v.m_iHole;
	m_LineDown.m_iHole = v.m_iHole;
	m_LineV.m_iHole = v.m_iHole;
	m_LineUp.m_p1 = v.m_p[0][0];
	m_LineUp.m_p2 = v.m_p[0][1];
	m_LineDown.m_p1 = v.m_p[1][0];
	m_LineDown.m_p2 = v.m_p[1][1];
	m_LineV.m_p1 = v.m_p[2][0];
	m_LineV.m_p2 = v.m_p[2][1];
	return *this;
}

void PermeatLine::ReadForAnimate(fstream* ff)
{
	*ff>>m_LineUp.m_iHole;m_LineDown.m_iHole=m_LineUp.m_iHole;m_LineV.m_iHole=m_LineUp.m_iHole;
	*ff>>m_LineUp.m_p1.m_dx>>m_LineUp.m_p1.m_dy>>m_LineUp.m_p2.m_dx>>m_LineUp.m_p2.m_dy;
	*ff>>m_LineDown.m_p1.m_dx>>m_LineDown.m_p1.m_dy>>m_LineDown.m_p2.m_dx>>m_LineDown.m_p2.m_dy;
	*ff>>m_LineV.m_p1.m_dx>>m_LineV.m_p1.m_dy>>m_LineV.m_p2.m_dx>>m_LineV.m_p2.m_dy;
}

void PermeatLine::SaveForMap(fstream* ff)
{
	int i,j,ix1,ix2,iy1,iy2;
	ix1=int(m_LineV.m_p1.m_dx+0.5);//左侧向右画一点
	ix2=int(m_LineUp.m_p2.m_dx);//右侧向左画一点
	iy1=int(m_LineDown.m_p1.m_dy+0.5);//下侧向上画一点
	iy2=int(m_LineUp.m_p1.m_dy);//上侧向下画一点
	for(i=ix2;i>=ix1;i--)//最右边点归瓣膜
	{
		if(m_LineUp.IsHole(i,int(m_LineUp.m_p1.m_dx)))
		{  
			*ff << i - BEGINROW << " " << m_LineUp.m_p1.m_dy << endl;
		}
		else
		{
			*ff<<endl;
		}
	}
	*ff<<endl;
	for(i=ix2;i>=ix1;i--)//最右边点归瓣膜
	{
		if(PerLine.m_LineDown.IsHole(i,int(PerLine.m_LineDown.m_p1.m_dx)))
		{  
			*ff << i - BEGINROW << " " << m_LineDown.m_p1.m_dy << endl;
		}
		else
		{
			*ff<<endl;
		}
	}
	*ff<<endl;
	for(j=iy1;j<=iy2;j++)	
	{	
		if(PerLine.m_LineV.IsHole(j,int(PerLine.m_LineV.m_p1.m_dy)))
		{
			*ff << ix1 - BEGINROW << " " << j << endl;
		}
		else
		{
			*ff<<endl;
		}
	}
}