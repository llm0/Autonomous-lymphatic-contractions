// PermeatLine.cpp: implementation of the PermeatLine class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "lymph.h"
#include "PermeatLine.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
Lattice (*(PermeatLine::m_lattice))[HEIGHT];
int* PermeatLine::m_iPrjx;
int* PermeatLine::m_iPrjy;
int (*(PermeatLine::m_iPrjxy))[9];
int* PermeatLine::m_iRev;
int PermeatLine::m_iBouNum=0;
Boundary* PermeatLine::m_pBoud=NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PermeatLine::PermeatLine()
{
	m_iBouNum=0;
}

PermeatLine::~PermeatLine()
{

}

void PermeatLine::init(int bf)
{
	if (!bf)
	{
		//x1 = int(WIDTH / 30.0) + 4; //4:2int(WIDTH / 30.0) + 3;//与膜相接处刚好为洞
		//x2 = int(WIDTH / 5.0);
		m_LineUp.m_p1 = Vector(PX1, H2);
		m_LineUp.m_p2 = Vector(PX2, H2);
		m_LineDown.m_p1 = Vector(PX1, H1);
		m_LineDown.m_p2 = Vector(PX2, H1);
		m_LineV.m_p1 = Vector(PX1, H1);
		m_LineV.m_p2 = Vector(PX1, H2);
		m_LineUp.m_iHole = 2; m_LineDown.m_iHole = 2; m_LineV.m_iHole = 2;
		m_LineUp.m_iCycle = 4; m_LineDown.m_iCycle = 4; m_LineV.m_iCycle = 4;
		m_LineUp.m_iType = 2; m_LineDown.m_iType = 1; m_LineV.m_iType = -1;
	}
	m_LineUp.init(); m_LineDown.init(); m_LineV.init();
	Petal::m_Length = (m_LineUp.m_p1.m_dy - m_LineDown.m_p1.m_dy)*0.5;//,dR0;
}

void PermeatLine::DrawPermeatArea()
{
	int i, j, ix1, ix2, iy1, iy2;
	ix1 = int(m_LineV.m_p1.m_dx + 0.5);//左侧向右画一点
	ix2 = int(m_LineUp.m_p2.m_dx);//右侧向左画一点
	iy1 = int(m_LineDown.m_p1.m_dy + 0.5);//下侧向上画一点
	iy2 = int(m_LineUp.m_p1.m_dy);//上侧向下画一点
	for (i = ix1; i < ix2; i++)//最右边点归瓣膜
	{
		for (j = iy1; j <= iy2; j++)
		{
			if (j == iy1 || j == iy2)//水平方向
			{
				//m=int(i-m_LineUp.m_p1.m_dx)%(m_LineUp.m_iHole*2);
				//if(m>=0&&m<=1)
				if (m_LineUp.IsHole(i, int(m_LineUp.m_p1.m_dx)))
				{
					m_lattice[i][j].m_iSol = 0;
					continue;
				}
			}
			if (i == ix1)//竖直方向
			{
				if (m_LineV.IsHole(j, int(m_LineV.m_p1.m_dy)))
				{
					m_lattice[i][j].m_iSol = 0;
					continue;
				}
			}
			m_lattice[i][j].m_iSol = -1;
		}
	}
}

void PermeatLine::FindLink()
{
	m_iBouNum = 0;
	m_LineUp.FindLink(&m_iBouNum, m_pBoud);
	m_LineDown.FindLink(&m_iBouNum, m_pBoud);
	m_LineV.FindLink(&m_iBouNum, m_pBoud);
}

void PermeatLine::Collide()
{
	int n, i, k, j;
	int fx, fy, sx, sy;
	double dDelta;
	double dLinBuff[2];
	Link link[2];
	for (i = 0; i < m_iBouNum; i++)
	{
		for (k = 0; k < m_pBoud[i].m_iLinNum; k++)
		{
			for (j = 0; j < 2; j++)//0外面是流体，里面是固体，1反过来//2
			{
				if (j == 0)
				{
					link[0] = m_pBoud[i].m_link[k];
				}
				else
				{
					link[1] = -m_pBoud[i].m_link[k];
				}
				fx = link[j].m_iFlux;
				fy = link[j].m_iFluy;
				sx = link[j].m_iSolx;
				sy = link[j].m_iSoly;
				n = link[j].m_iDir;
				dDelta = link[j].m_dDlf;
				dLinBuff[j] = Curvebound(sx, sy, n, dDelta);
			}//j
			for (j = 0; j < 2; j++)//0外面是流体，里面是固体，1反过来//2
			{
				if (lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol < 0)
				{
					lattice[link[j].m_iSolx][link[j].m_iSoly].m_df[iRev[link[j].m_iDir]]
						= dLinBuff[j];
				}
			}
		}
	}
}


double PermeatLine::Curvebound(int x,int y,int Order,double delta)
{
	double dfStar=0;
	double dDotMetF,dDotMetBF;
	double dVelx;
	double dVely;
	double dVbfx,dVbfy,dKappa;
	int iFx,iffx;
	int iFy,iffy;
	iFx = x - iPrjx[Order];
	iFy = y - iPrjy[Order];
	dVelx = lattice[iFx][iFy].m_v.m_dx;//f点
	dVely = lattice[iFx][iFy].m_v.m_dy;
	if (delta >= 0.5)
	{
		dVbfx = dVelx*(delta - 1) / delta;//虚拟点速度
		dVbfy = dVely*(delta - 1) / delta;
		dKappa = (2 * delta - 1) / dTau;
	}
	else
	{
		iffx = iFx - iPrjx[Order];
		iffy = iFy - iPrjy[Order];
		if (lattice[iffx][iffy].m_iSol < 0&&
			!(lattice[iffx][iffy].m_iSol==-2&& lattice[iffx][iffy].m_ipr == 2))
		{
			dVbfx = lattice[iffx][iffy].m_v.m_dx;
			dVbfy = lattice[iffx][iffy].m_v.m_dy;
			dKappa = (2 * delta - 1) / (dTau - 2);
		}
		else
		{
			return lattice[iFx][iFy].m_df[Order];
		}
	}
	dDotMetF = iPrjx[Order] * dVelx + iPrjy[Order] * dVely;
	dDotMetBF = iPrjx[Order] * dVbfx + iPrjy[Order] * dVbfy;
	dfStar = lattice[iFx][iFy].m_dDen*dCoe[Order] * (1 + 3 * dDotMetBF + 4.5*dDotMetF*
		dDotMetF - 1.5*(dVelx*dVelx + dVely*dVely));
	dfStar = (1 - dKappa)*lattice[iFx][iFy].m_df[Order] + dfStar*dKappa;
	return dfStar;
}

void PermeatLine::Oneway()
{
	m_LineUp.Oneway();
	m_LineDown.Oneway();
	m_LineV.Oneway();
}

void PermeatLine::Pack(char* buffer, int  size, int* position)
{
	m_LineUp.Pack(buffer, size, position);
	m_LineDown.Pack(buffer, size, position);
	m_LineV.Pack(buffer, size, position);
}

void PermeatLine::Unpack(char* buffer, int  size, int* position)
{
	m_LineUp.Unpack(buffer, size, position);
	m_LineDown.Unpack(buffer, size, position);
	m_LineV.Unpack(buffer, size, position);
}

void PermeatLine::SaveForAnimate(fstream *ff)
{
	(*ff) << m_LineUp.m_iHole << endl;
	(*ff) << m_LineUp.m_p1.m_dx << " " << m_LineUp.m_p1.m_dy << " " << m_LineUp.m_p2.m_dx << " " << m_LineUp.m_p2.m_dy << endl;
	(*ff) << m_LineDown.m_p1.m_dx << " " << m_LineDown.m_p1.m_dy << " " << m_LineDown.m_p2.m_dx << " " << m_LineDown.m_p2.m_dy << endl;
	(*ff) << m_LineV.m_p1.m_dx << " " << m_LineV.m_p1.m_dy << " " << m_LineV.m_p2.m_dx << " " << m_LineV.m_p2.m_dy << endl;
}

MPI_File& operator<<(MPI_File& fs, PermeatLine& v)//3*
{
	fs << v.m_LineUp;
	fs << v.m_LineDown; 
	fs << v.m_LineV;
	return fs;
}

MPI_File& operator>>(MPI_File& fs, PermeatLine& v)
{
	fs >> v.m_LineUp;
	fs >> v.m_LineDown;
	fs >> v.m_LineV;
	return fs;
}
