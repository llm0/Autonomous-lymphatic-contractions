// Petal.cpp: implementation of the Petal class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#//include "lymph.h"
#include "Petal.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double Petal::m_Length;
double Petal::m_dI;
int Petal::m_ixn;
int Petal::m_iyn;
int Petal::m_ixm;
int Petal::m_iym;
Lattice(*Petal::m_lattice)[HEIGHT];
int* Petal::m_iPrjx;
int* Petal::m_iPrjy;
int(*Petal::m_iPrjxy)[9];
int* Petal::m_iRev;
Link* Petal::m_link;
double Petal::m_ddT;
double Petal::m_ss;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Petal::Petal()
{
	m_dOmigar = 0;
	m_dBelta = 0;
	m_dTheta = 0;
}

Petal::~Petal()
{

}

Vector Petal::Vertex()
{
	m_Apex = *m_p0 + Vector(m_Length*cos(m_dTheta), m_Length*sin(m_dTheta));
	if (m_dTheta<dhPI || m_dTheta>dhPI)
	{
		if (m_dTheta<-dhPI || m_dTheta>-dhPI)
		{
			m_dk = tan(m_dTheta);
			m_dk_re = 2e17;
		}
		else
		{
			m_dk = -2e17;
			m_dk_re = cos(m_dTheta) / sin(m_dTheta);
		}
	}
	else
	{
		m_dk = 2e17;
		m_dk_re = cos(m_dTheta) / sin(m_dTheta);
	}
	return m_Apex;
}

void Petal::TurnRound()
{
	m_dOmigar += (m_dBelta + m_dElasticMoment)*m_ddT - m_dOmigar*0.000008;// 0.00008;
	m_dTheta += m_dOmigar*m_ddT;
	if ((m_Apex.m_dx - m_p0->m_dx) > 1e-16)
	{
		m_dk = (m_Apex.m_dy - m_p0->m_dy) / (m_Apex.m_dx - m_p0->m_dx);
	}
	else
	{
		m_dk = 2e16;
		m_dk_re = (m_Apex.m_dx - m_p0->m_dx) / (m_Apex.m_dy - m_p0->m_dy);
	}
	Vertex();
}

void Petal::ElasticForce()
{
	double dLimit, dkk;
	double dh;
	dh = dhPI - dDelta;
	dLimit = PI / 10 - dDelta;
	dkk = 0.0000000001;
	if ((m_dTheta >= dLimit &&m_dTheta <= dh)
		|| (m_dTheta <= -(dLimit) && m_dTheta >= -dh))
	{
		m_dElasticMoment = -dkk*(m_dTheta - m_dTheta0);//0.0000004
	}
	else
	{
		if (m_dTheta<-dh)
		{
			m_dElasticMoment = -dkk*(m_dTheta - m_dTheta0)*pow(dDelta
				/ (m_dTheta - (m_dTheta0 - dDelta)), 11);
		}
		else if (m_dTheta>dh)
		{
			m_dElasticMoment = -dkk*(m_dTheta - m_dTheta0)*pow(dDelta
				/ ((m_dTheta0 + dDelta) - m_dTheta), 11);
		}
		else if (m_dTheta<dLimit &&m_iType<0)
		{
			m_dElasticMoment = -dkk*(m_dTheta - m_dTheta0)*pow(dDelta
				/ (m_dTheta - (dLimit - dDelta)), 11);
		}
		else if (m_dTheta>-(dLimit) && m_iType>0)
		{
			m_dElasticMoment = -dkk*(m_dTheta - m_dTheta0)*pow(dDelta
				/ (dDelta - (m_dTheta + dLimit)), 11);
		}
	}
}

void Petal::Force(int iSection)
{
	int i, k, m, n;
	double Press[2][2], Delta[2][2] = { 1, 0, 0, 1 }, FluidVel[2], WallVel[2], ds[2], ds_rv[2], dPi;
	double df[2];
	Vector v1, v2, v, vc, vv, vd, fout;
	BoundaryLattice OuterB, InnerB;
	v = (m_Apex - *m_p0) / iSection;//分为is段,is+1个节点
	vd = v;
	if (m_iType > 0)
	{
		vd.Roll(-PI / 2);//上边瓣膜顺时针转
	}
	else
	{
		vd.Roll(PI / 2);//下边瓣膜逆时针转
	}
	vd = vd / vd.Mod();
	ds[0] = m_ss*vd.m_dx; ds[1] = m_ss*vd.m_dy;
	ds_rv[0] = -ds[0]; ds_rv[1] = -ds[1];
	for (i = 0; i < iSection; i++)
	{
		OuterB.Clear();
		InnerB.Clear();
		v1 = (*m_p0) + (v*i); v2 = (*m_p0) + (v*(i + 1));//
		vc = (v1 + v2) / 2.0;
		OuterB.m_dx = vc.m_dx;//位置
		OuterB.m_dy = vc.m_dy;
		InnerB.m_dx = OuterB.m_dx;
		InnerB.m_dy = OuterB.m_dy;
		Nearest(&OuterB, &InnerB, i);//外点，内点
		vv = vc - (*m_p0);
		vv = vv.Roll(PI / 2)*m_dOmigar;
		WallVel[0] = vv.m_dx;
		WallVel[1] = vv.m_dy;
		df[0] = 0; df[1] = 0;
		m_dBelta = 0;
		FluidVel[0] = OuterB.m_v.m_dx; FluidVel[1] = OuterB.m_v.m_dy;
		for (m = 0; m < 2; m++)//外面
		{
			for (n = 0; n < 2; n++)
			{
				dPi = 0;
				for (k = 0; k < 9; k++)
				{
					dPi += (m_iPrjxy[m][k] - FluidVel[m])*(m_iPrjxy[n][k] - FluidVel[n])*OuterB.m_df[k];
				}
				Press[m][n] = -1.0 / 6.0 / dTau*OuterB.m_dDen*Delta[m][n] - (1 - 0.5 / dTau)*dPi;
			}
		}
		for (m = 0; m < 2; m++)
		{
			for (n = 0; n < 2; n++)
			{
				df[m] += (-Press[m][n] + OuterB.m_dDen*FluidVel[m] *
					(FluidVel[n] - WallVel[n]))*ds[n];
			}
		}
		m_fo[i] = (df[0] * v.m_dx + df[1] * v.m_dy) / m_ss;//v没归一,算出沿面积的分力
		fout.m_dx = df[0]; fout.m_dy = df[1];
		FluidVel[0] = InnerB.m_v.m_dx; FluidVel[1] = InnerB.m_v.m_dy;
		for (m = 0; m < 2; m++)//里面
		{
			for (n = 0; n < 2; n++)
			{
				dPi = 0;
				for (k = 0; k < 9; k++)
				{
					dPi += (m_iPrjxy[m][k] - FluidVel[m])*(m_iPrjxy[n][k] - FluidVel[n])*InnerB.m_df[k];
				}
				Press[m][n] = -1.0 / 6.0 / dTau*InnerB.m_dDen*Delta[m][n] - (1 - 0.5 / dTau)*dPi;
			}
		}
		for (m = 0; m < 2; m++)
		{
			for (n = 0; n < 2; n++)
			{
				df[m] += (-Press[m][n] + InnerB.m_dDen*FluidVel[m] *
					(FluidVel[n] - WallVel[n]))*ds_rv[n];
			}
		}
		vv = vc - (*m_p0);
		m_dBelta += vv.m_dx*df[1] - vv.m_dy*df[0];
		m_fi[i] = ((df[0] - fout.m_dx) * v.m_dx + (df[1] - fout.m_dy) * v.m_dy) / m_ss;//v没归一，,算出沿面积的分力
	}
	m_dBelta = m_dBelta / m_dI;
}

void Petal::Nearest(BoundaryLattice *Outer, BoundaryLattice *Inner, int i = 0)
{
	int m, n, ilr, iPx, iPy, nn;
	double dx, dy, kki0, kko0, kk;
	dx = Outer->m_dx;
	dy = Outer->m_dy;
	nn = 1;
	for (;;)
	{
		Outer->m_iSol = -4;
		Inner->m_iSol = -4;
		kki0 = 10;
		kko0 = 10;
		for (m = -nn; m <= nn; m++)
		{
			for (n = -nn; n <= nn; n++)
			{
				ilr = 0;
				iPx = int(dx + 0.5) + m;//Minimium,第一点
				iPy = int(dy + 0.5) + n;
				if (InRegionX(iPx) && iPy > 0 && iPy<HEIGHT - 1)
				{
					if (lattice[iPx][iPy].m_iSol == -1 || lattice[iPx][iPy].m_iSol == -3)
					{
						kk = sqrt((iPx - dx)*(iPx - dx) + (iPy - dy)*(iPy - dy));//距离
						if (m_iType == -2 || m_iType == 2 || m_iType == -3 || m_iType == 3)
						{
							if (OnLeftSide(iPx, iPy))
							{
								if (Inner->m_iSol == -4 || kki0>kk)
								{
									kki0 = kk;
									*Inner = lattice[iPx][iPy];//iox=iPx;ioy=iPy;
									Inner->m_ix = iPx; Inner->m_iy = iPy;
									//	iix=iPx;iiy=iPy;
								}
							}
							else
							{
								if (Outer->m_iSol == -4 || kko0 > kk)
								{
									kko0 = kk;
									*Outer = lattice[iPx][iPy];
									Outer->m_ix = iPx; Outer->m_iy = iPy;
									//	iox=iPx;ioy=iPy;
								}
							}
						}
					}
				}
			}
		}
		if (Outer->m_iSol != -4 && Inner->m_iSol != -4)
		{
			break;
		}
		nn++;
		if (nn > 4)
		{
			printf("Petal NearestErrory");
		}
	}
}

void Petal::FindLinkInArea(int *iLink)
{
	int i, j, m;
	int iFlux, iFluy;
	Vector p;
	m_ixn = int(m_p0->m_dx); m_ixm = int(m_Apex.m_dx);
	if (m_iType > 0)
	{
		m_iyn = int(m_Apex.m_dy); m_iym = int(m_p0->m_dy);
	}
	else
	{
		m_iyn = int(m_p0->m_dy); m_iym = int(m_Apex.m_dy);
	}
	m_ixn--; m_ixm = (m_ixm + 1) < iEnd_Row ? m_ixm + 1 : iEnd_Row - 1; m_iyn--; m_iym++;//区域扩大
	for (i = m_ixn; i < m_ixm; i++)//扫描区域
	{
		for (j = m_iyn; j < m_iym; j++)//扫描区域
		{
			for (m = 1; m < 9; m++)//m的方向由固体指向流体
			{
				iFlux = i + m_iPrjx[m];  //fluid node -1~n
				iFluy = j + m_iPrjy[m];  //iFluy -1~n
				if (i < m_p0->m_dx&&iFlux < m_p0->m_dx)
				{
					continue;
				}
				if (InRegionX(iFlux) && iFluy >= 0 && iFluy < HEIGHT)
				{
					if (m_lattice[i][j].m_iSol == -3 && m_lattice[iFlux][iFluy].m_iSol == -1 ||
						m_lattice[i][j].m_iSol == -3 && m_lattice[iFlux][iFluy].m_iSol == 0 ||
						m_lattice[i][j].m_iSol == 0 && m_lattice[iFlux][iFluy].m_iSol == -1) //由管内指向管外
					{
						m_link[*iLink].m_iSolx = i;
						m_link[*iLink].m_iSoly = j;
						m_link[*iLink].m_iFlux = iFlux;
						m_link[*iLink].m_iFluy = iFluy;
						m_link[*iLink].m_iDir = m_iRev[m];//调整为从流体指向固体
						if (IsCross(Vector(iFlux, iFluy), Vector(i, j), &p))
						{
							m_link[*iLink].m_bf = p;
							m_link[*iLink].m_iSegf = 0;
							m_link[*iLink].m_iType = m_iType;
							if (m_link[*iLink].m_iDir != 3 && m_link[*iLink].m_iDir != 4)//非竖直方向
							{
								m_link[*iLink].m_dDlf = fabs(m_link[*iLink].m_bf.m_dx - m_link[*iLink].m_iFlux);
							}
							else
							{
								m_link[*iLink].m_dDlf = fabs(m_link[*iLink].m_bf.m_dy - m_link[*iLink].m_iFluy);
							}
							(*iLink)++;
							if (*iLink > LINKNUM)
							{
								printf("细胞链数超过了设定值");
							}
						}
					}
				}
			}//for m
		}
	}
}

int Petal::IsCross(Vector l1, Vector l2, Vector *pp)
{
	double f1, f2, f3, f4;
	double a1, b1, c1, a2, b2, c2;
	Vector s1, s2;
	s1 = *m_p0; s2 = m_Apex;
	a1 = s2.m_dy - s1.m_dy;
	b1 = -(s2.m_dx - s1.m_dx);
	c1 = -a1*s1.m_dx - b1*s1.m_dy;
	a2 = l2.m_dy - l1.m_dy;
	b2 = -(l2.m_dx - l1.m_dx);
	c2 = -a2*l1.m_dx - b2*l1.m_dy;
	f1 = (l1 - s1) ^ (s2 - s1);//s1,l1,s2
	f2 = (l2 - s1) ^ (s2 - s1);//s1,l2,s2
	f3 = (s2 - l1) ^ (l2 - l1);//l1,s2,l2
	f4 = (s1 - l1) ^ (l2 - l1);//l1,s1,l2

	if (f1*f2 <= 0 && f3*f4 <= 0 || f1 == 0)//s2上的不要
	{
		if (pp != NULL)
		{
			if (a1*b2 - a2*b1 > 0 || a1*b2 - a2*b1 < 0)
			{
				pp->m_dx = (c2*b1 - c1*b2) / (a1*b2 - a2*b1);
				pp->m_dy = (c1*a2 - c2*a1) / (a1*b2 - a2*b1);
				pp->m_dx = ErrorCorrect(pp->m_dx, l1.m_dx, l2.m_dx);
				pp->m_dy = ErrorCorrect(pp->m_dy, l1.m_dy, l2.m_dy);
			}
			else
			{
				if (lattice[int(l1.m_dx)][int(l1.m_dy)].m_iSol == -1)//两线重叠，直接认为交点在固体点上？？？
				{
					*pp = l1;
				}
				else
				{
					*pp = l2;
				}
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}


void Petal::FindLinkAlongLine(int *iLink)//画长线找的交点
{
	int i, j, iPx, iPy;
	int m;
	int l;
	double x1, x2;
	Vector pp;
	l = *iLink;      //count Boundary Number in fluid
	m_ixn = (m_p0->m_dx<m_Apex.m_dx) ? int(m_p0->m_dx) : int(m_Apex.m_dx);
	m_ixm = (m_Apex.m_dx>m_p0->m_dx) ? int(m_Apex.m_dx) : int(m_p0->m_dx);
	if (m_iType > 0)
	{
		m_iyn = int(m_Apex.m_dy); m_iym = int(m_p0->m_dy);
	}
	else
	{
		m_iyn = int(m_p0->m_dy); m_iym = int(m_Apex.m_dy);
	}
	m_ixn--; m_ixm = (m_ixm + 1) < iEnd_Row ? m_ixm + 1 : iEnd_Row - 1; m_iyn--; m_iym++;//区域扩大
	for (i = m_ixn; i <= m_ixm; i++)
	{
		for (j = m_iyn; j <= m_iym; j++)
		{
			if (OnLeftSide(i, j))
			{
				x1 = Requestx(j);//
				for (m = 1; m < 9; m++)
				{
					iPx = i + iPrjx[m];//m是由i,j指向iPx,iPy
					iPy = j + iPrjy[m];//if(m_link[l].m_iSolx==204&&m_link[l].m_iSoly==37&&m==8) {Show(m_p0->m_dx,"xx");Show(m_p0->m_dy,"xy");Show(m_Apex.m_dx,"ax");Show(m_Apex.m_dy,"ay");}
					if (IsCross(Vector(i, j), Vector(iPx, iPy), &pp) && !CrossMem(Vector(i, j), Vector(iPx, iPy)))//去掉与膜也相交的重复
					{
						if (InRegionX(iPx))//(i,j) and (iPx,iPy)
						{
							x2 = Requestx(iPy);//
							if ((i - x1)*(iPx - x2) <= 0)//i,j fluid node,x1<x(y1)&&x2>x(y2)
							{
								if ((i - x1) <= 0)
								{
									m_link[l].m_iDir = iRev[m];//流体指向固体					  					
									m_link[l].m_iSolx = i;
									m_link[l].m_iSoly = j;
									m_link[l].m_iFlux = iPx; //found fluid node
									m_link[l].m_iFluy = iPy; //-1~n
								}
								else
								{
									m_link[l].m_iDir = m;//流体指向固体					  					
									m_link[l].m_iSolx = iPx;
									m_link[l].m_iSoly = iPy;
									m_link[l].m_iFlux = i; //found fluid node
									m_link[l].m_iFluy = j; //-1~n
								}
								switch (m)
								{
								case 1:
								case 2:
									m_link[l].m_bf.m_dy = iPy;
									m_link[l].m_bf.m_dx = Requestx(iPy);
									break;
								case 3:
								case 4:
									if (fabs(m_dk) < 1e17)
									{
										m_link[l].m_bf.m_dx = iPx;
										m_link[l].m_bf.m_dy = m_p0->m_dy + m_dk*(iPx - m_p0->m_dx);
									}
									else
									{
										if (lattice[i][j].m_iSol >= 0)
										{
											m_link[l].m_bf.m_dx = i;
											m_link[l].m_bf.m_dy = j;
										}
										else
										{
											m_link[l].m_bf.m_dx = iPx;
											m_link[l].m_bf.m_dy = iPy;
										}
									}
									break;
								case 5:
								case 6:
									if (fabs(m_dk) < 1e17)
									{
										m_link[l].m_bf.m_dx = (iPy - m_p0->m_dy + m_dk*m_p0->m_dx + iPx) / (1 + m_dk);
										m_link[l].m_bf.m_dy = (iPy*m_dk + m_p0->m_dy - m_dk*(m_p0->m_dx - iPx)) / (1 + m_dk);
									}
									else
									{
										m_link[l].m_bf.m_dx = pp.m_dx;
										m_link[l].m_bf.m_dy = pp.m_dy;
									}
									break;
								case 7:
								case 8:
									if (fabs(m_dk) < 1e17)
									{
										m_link[l].m_bf.m_dx = (iPy - m_p0->m_dy + m_dk*m_p0->m_dx - iPx) / (m_dk - 1);
										m_link[l].m_bf.m_dy = (m_dk*(iPy - iPx + m_p0->m_dx) - m_p0->m_dy) / (m_dk - 1);
									}
									else
									{
										m_link[l].m_bf.m_dx = pp.m_dx;
										m_link[l].m_bf.m_dy = pp.m_dy;
									}
									break;
								default:
									;
								}//if(m_link[l].m_iSolx==204&&m_link[l].m_iSoly==37&&m==8) Show(iPy,"xx");  //for switch
								if (Inside(Vector(m_link[l].m_bf.m_dx,
									m_link[l].m_bf.m_dy), Vector(i, j), Vector(iPx, iPy))
									&& OnlyInsideOne(Vector(m_link[l].m_bf.m_dx,
									m_link[l].m_bf.m_dy)))
								{
									m_link[l].m_dDlf =
										sqrt((m_link[l].m_bf.m_dx - i)*(m_link[l].m_bf.m_dx - i)
										+ (m_link[l].m_bf.m_dy - j)*(m_link[l].m_bf.m_dy - j)) /
										sqrt((iPx - i)*(iPx - i) + (iPy - j)*(iPy - j));
									l++;
								}
							}	//for if	
						}
					}    //for if				
				} //for m 
			}
		}//for j
	}//for i
	*iLink = l;
}

int Petal::Inside(Vector pp, Vector p1, Vector p2)//交点要在两个点这间
{
	//	if(p1.m_dx==204&&p1.m_dy==46&&p2.m_dx==205&&p2.m_dy==47) Show(pp.m_dy,"xx");

	if ((pp.m_dx <= p2.m_dx&&pp.m_dx >= p1.m_dx || pp.m_dx <= p1.m_dx&&pp.m_dx >= p2.m_dx)
		&& (pp.m_dy <= p2.m_dy&&pp.m_dy >= p1.m_dy || pp.m_dy <= p1.m_dy&&pp.m_dy >= p2.m_dy) &&
		(pp.m_dx <= m_Apex.m_dx&&pp.m_dx >= m_p0->m_dx || pp.m_dx <= m_p0->m_dx&&pp.m_dx >= m_Apex.m_dx)
		&& (pp.m_dy <= m_Apex.m_dy&&pp.m_dy >= m_p0->m_dy || pp.m_dy <= m_p0->m_dy&&pp.m_dy >= m_Apex.m_dy))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


double Petal::Requestx(double y)
{
	if (m_Apex.m_dy > m_p0->m_dy || m_Apex.m_dy < m_p0->m_dy)
	{
		if (fabs(m_dk) >= 1e17)
		{
			return m_p0->m_dx;
		}
		else
		{
			return m_p0->m_dx + (y - m_p0->m_dy) / m_dk;
		}
	}
	else
	{
		return -1e16;
	}
}

int Petal::OnLeftSide(int i, int j)
{
	if (i > m_p0->m_dx - 2 && i <= Requestx(j))//右点是流体点，边界点看为固体点。
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int Petal::Inside(Vector v)
{
	if ((v.m_dx >= m_p0->m_dx&&v.m_dx <= m_Apex.m_dx&&m_p0->m_dx <= m_Apex.m_dx
		|| v.m_dx <= m_p0->m_dx&&v.m_dx >= m_Apex.m_dx&&m_p0->m_dx >= m_Apex.m_dx) &&
		(v.m_dy >= m_p0->m_dy&&v.m_dy <= m_Apex.m_dy&&m_p0->m_dy <= m_Apex.m_dy
		|| v.m_dy <= m_p0->m_dy&&v.m_dy >= m_Apex.m_dy&&m_p0->m_dy >= m_Apex.m_dy))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Petal::OnlyInsideOne(Vector v)
{
	if (Inside(v) && (!m_Opposite->Inside(v) || m_iType > 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int Petal::CrossMem(Vector v1, Vector v2)
{
	Vector pp;
	if (m_pSeg->m_prev != NULL)
	{
		if ((m_pSeg->m_prev)->IsCross(v1, v2, &pp) || m_pSeg->IsCross(v1, v2, &pp))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (m_pSeg->IsCross(v1, v2, &pp))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void Petal::ReactionNO(int iSection)
{
	int i;
	BoundaryLattice Outer, Inner, dDisapp;
	Vector v1, v2, v, vc, vv, vd;
	v = (m_Apex - *m_p0) / iSection;
	for (i = 0; i < iSection; i++)
	{
		v1 = (*m_p0) + (v*i);
		Outer.Clear();
		Inner.Clear();
		Outer.m_dx = v1.m_dx; Outer.m_dy = v1.m_dy;//段位置
		Inner = Outer;
		Nearest(&Outer, &Inner);//左边内，右边外
		ReactionNO(&Outer); ReactionNO(&Inner);//两面反应
	}
}

void Petal::ReactionNO(BoundaryLattice* NearLattice)
{
	double dks, dProduct;
	dks = dkNO * h;
	if (m_lattice[NearLattice->m_ix][NearLattice->m_iy].m_bReaction != 2)//valve 还没反应
	{

		dProduct = dks*fabs(Stress(NearLattice->m_ix, NearLattice->m_iy)) //查stress
			/ (m_lattice[NearLattice->m_ix][NearLattice->m_iy].m_dDen*(2 * dTau - 1) / 6);
		if (!(m_lattice[NearLattice->m_ix][NearLattice->m_iy].m_dcNO <= LIMITNO) &&
			NOBuf[NearLattice->m_ix][NearLattice->m_iy] + dProduct > NODecay)
		{
			NOBuf[NearLattice->m_ix][NearLattice->m_iy] = NODecay;
		}
		else
		{
			NOBuf[NearLattice->m_ix][NearLattice->m_iy] += dProduct;
		}
		dcNO[(lNowStep / 50) % 100] += m_lattice[NearLattice->m_ix][NearLattice->m_iy].m_dcNO + dProduct;//累加式，务必在膜之后
		m_lattice[NearLattice->m_ix][NearLattice->m_iy].m_bReaction = 2;//valve 已经反应
	}
}

double Petal::Stress(int i, int j)
{
	Vector v;
	int k;
	v = Distance(i, j) - *m_p0;
	k = int(v.Mod() / m_ss);
	if (OnLeftSide(i, j))//inner
	{
		return m_fi[k] / m_ss;//力除面积得stress
	}
	else
	{
		return m_fo[k] / m_ss;
	}
}
Vector Petal::Distance(int i, int j)//垂直交点
{
	Vector v1, n1, v2, n2, v;
	v1 = Vector(i, j);
	n1 = Vector(cos(m_dTheta + dhPI), sin(m_dTheta + dhPI));
	v2 = *m_p0;
	n2 = Vector(cos(m_dTheta), sin(m_dTheta));
	if (Segment::IsCross(v1, n1, v2, n2, &v))
	{
		return v;
	}
	else
	{
		printf("求垂直距离出错");
		return Vector(0, 0);
	}
}


void Petal::SaveForAnimate(fstream *ff)
{
	BoundaryLattice Outer, Inner, dDisapp;
	Vector v1, v2, v, vc, vv, vd;
	(*ff) << m_p0->m_dx << " " << m_p0->m_dy << " " <<
		m_dTheta << " ";
	/*v = (m_Apex - *m_p0) / iSegValve;
	for (i = 0; i < iSegValve; i++)
	{
	v1 = (*m_p0) + (v*i);
	Outer.Clear();
	Inner.Clear();
	Outer.m_dx = v1.m_dx; Outer.m_dy = v1.m_dy;
	Inner.m_dx = v1.m_dx; Inner.m_dy = v1.m_dy;
	Nearest(&Outer, &Inner);
	(*ff) << (m_lattice[Outer.m_ix][Outer.m_iy].m_dcNO+
	m_lattice[Inner.m_ix][Inner.m_iy].m_dcNO)/2<<" ";//两面反应
	}*/
	(*ff) << endl;
}

fstream& operator<<(fstream& fs, Petal& v)
{
	fs.write((char*)(&v.m_dOmigar), sizeof(v.m_dOmigar));
	fs.write((char*)(&v.m_dTheta), sizeof(v.m_dTheta));
	fs.write((char*)(&v.m_dTheta0), sizeof(v.m_dTheta0));
	fs.write((char*)(&v.m_iType), sizeof(v.m_iType));
	return fs;
}

fstream& operator>>(fstream& fs, Petal& v)
{
	fs.read((char*)(&v.m_dOmigar), sizeof(v.m_dOmigar));
	fs.read((char*)(&v.m_dTheta), sizeof(v.m_dTheta));
	fs.read((char*)(&v.m_dTheta0), sizeof(v.m_dTheta0));
	fs.read((char*)(&v.m_iType), sizeof(v.m_iType));
	return fs;
}