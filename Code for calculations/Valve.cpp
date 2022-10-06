// Valve.cpp: implementation of the Valve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lymph.h"
#include "Valve.h"
#include "Common.h"


Lattice (*Valve::m_lattice)[HEIGHT];
int Valve::m_ixn;
int Valve::m_iyn;
int Valve::m_ixm;
int Valve::m_iym;
int* Valve::m_iPrjx;
int* Valve::m_iPrjy;
int (*(Valve::m_iPrjxy))[9];
int* Valve::m_iRev;
double Valve::m_dTau;
double* Valve::m_dCoe;
double Valve::m_dDensity;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Valve::Valve()
{

}

Valve::~Valve()
{
	delete m_PetalDL.m_fi;
	delete m_PetalDL.m_fo;
	delete m_PetalUL.m_fi;
	delete m_PetalUL.m_fo;
	delete m_PetalDR.m_fi;
	delete m_PetalDR.m_fo;
	delete m_PetalUR.m_fi;
	delete m_PetalUR.m_fo;

}

void Valve::init(BOOL bf)
{
	int i,j,iInner;
	Vector v1;
	Petal::m_link=m_link;
	m_PetalDL.m_p0 = &(LymVessle.m_MemDown.m_segment[0].m_pst);
	m_PetalDL.m_pSeg = &(LymVessle.m_MemDown.m_segment[0]);
	m_PetalUL.m_p0 = &(LymVessle.m_MemUp.m_segment[0].m_pst);
	m_PetalUL.m_pSeg = &(LymVessle.m_MemUp.m_segment[0]);
	m_PetalDL.m_Opposite = &m_PetalUL; m_PetalUL.m_Opposite = &m_PetalDL;
	m_PetalDR.m_Opposite = &m_PetalUR; m_PetalUR.m_Opposite = &m_PetalDR;

	m_PetalDR.m_p0 = &(LymVessle.m_MemDown.m_segment[RIGHTVALVE].m_pst);
	m_PetalDR.m_pSeg = &(LymVessle.m_MemDown.m_segment[RIGHTVALVE]);
	m_PetalUR.m_p0 = &(LymVessle.m_MemUp.m_segment[RIGHTVALVE].m_pst);
	m_PetalUR.m_pSeg = &(LymVessle.m_MemUp.m_segment[RIGHTVALVE]);
	if (!bf)
	{
		m_PetalDL.m_dTheta0 = dhPI-dDelta;//60.0/360.0*2*3.1425926;//;///
		m_PetalUL.m_dTheta0 = -m_PetalDL.m_dTheta0;
		m_PetalDR.m_dTheta0 = m_PetalDL.m_dTheta0;//10.0/360.0*2*3.1425926;
		m_PetalUR.m_dTheta0 = -m_PetalDR.m_dTheta0;
		m_PetalDL.m_dTheta = m_PetalDL.m_dTheta0;
		m_PetalUL.m_dTheta = -m_PetalDL.m_dTheta;
		m_PetalDR.m_dTheta = m_PetalDR.m_dTheta0;
		m_PetalUR.m_dTheta = -m_PetalDR.m_dTheta0;
		m_PetalDL.m_iType = -2; m_PetalUL.m_iType = 2; m_PetalDR.m_iType = -3; m_PetalUR.m_iType = 3;
		LymVessle.m_MemDown.m_segment[RIGHTVALVE].m_bCanMove = FALSE;
		LymVessle.m_MemUp.m_segment[RIGHTVALVE].m_bCanMove = FALSE;
		LymVessle.m_MemDown.m_segment[0].m_bCanMove = FALSE;
		LymVessle.m_MemUp.m_segment[0].m_bCanMove = FALSE;
		for (i = RIGHTVALVE; i < SEGNUM; i++)
		{
			LymVessle.m_MemDown.m_segment[i].m_bCanMove = FALSE;
			LymVessle.m_MemUp.m_segment[i].m_bCanMove = FALSE;
		}
		for (i = 0; i<int((LymVessle.m_MemUp.m_dy1 - LymVessle.m_MemDown.m_dy1) 
			/ 2.0 / (LymVessle.m_MemDown.m_dx2
			- LymVessle.m_MemDown.m_dx1)*SEGNUM); i++)//减少边界效应，valve前后一半径不动??????
		{
			LymVessle.m_MemDown.m_segment[i].m_bCanMove = FALSE;
			LymVessle.m_MemUp.m_segment[i].m_bCanMove = FALSE;
		}

	/*	for (i = RIGHTVALVE - int((LymVessle.m_MemUp.m_dy1 - LymVessle.m_MemDown.m_dy1) / 2.0 /
			(LymVessle.m_MemDown.m_dx2 - LymVessle.m_MemDown.m_dx1)*SEGNUM); i<RIGHTVALVE; i++)//减少边界效应，valve前后一半径不动
		{
			LymVessle.m_MemDown.m_segment[i].m_bCanMove = FALSE;
			LymVessle.m_MemUp.m_segment[i].m_bCanMove = FALSE;
		}*/
	}
	m_PetalDL.Vertex();
	m_PetalUL.Vertex();
	m_PetalDR.Vertex();
	m_PetalUR.Vertex();
	Petal::m_dI=1.0/3.0*dValveDensity*pow((m_PetalDL.m_Apex-(*m_PetalDL.m_p0)).Mod(),3);
	if (!bf)
	{
		for (i = int(m_PetalDL.m_p0->m_dx); i < int(m_PetalDL.m_Apex.m_dx) + 1; i++)//初始化左瓣膜，最左边一排归瓣膜，找链要找到它
		{
			for (j = 0; j < HEIGHT; j++)
			{
				iInner = IsInner(i, j);
				if (iInner == 1)//在两段膜之间
				{
					m_lattice[i][j].m_iSol = -3;//首次初始化必须-3
				}
				else if (iInner == 0 && j >= m_PetalDL.m_p0->m_dy&&j <= m_PetalUL.m_p0->m_dy)
				{
					m_lattice[i][j].m_iSol = 4;
				}
			}
		}
		for (i = int(m_PetalDR.m_p0->m_dx); i < int(m_PetalDR.m_Apex.m_dx) + 1; i++)//初始化右瓣膜，
		{
			for (j = 0; j < HEIGHT; j++)
			{
				iInner = IsInner(i, j);
				if (iInner == 1)//在两段膜之间
				{
					m_lattice[i][j].m_iSol = -3;//新出现内点，要由内点外推
				}
				else if (iInner == 0 && j >= m_PetalDR.m_p0->m_dy&&j <= m_PetalUR.m_p0->m_dy)
				{
					m_lattice[i][j].m_iSol = 4;
				}
			}
		}
	}
}


void Valve::Move()
{
	double aa;
	aa=(m_PetalDL.m_dBelta-m_PetalUL.m_dBelta)/2;
	m_PetalDL.m_dBelta=aa;
	m_PetalUL.m_dBelta=-aa;
	aa=(m_PetalDR.m_dBelta-m_PetalUR.m_dBelta)/2;
	m_PetalDR.m_dBelta=aa;
	m_PetalUR.m_dBelta=-aa;
	m_PetalDL.TurnRound();
	m_PetalUL.TurnRound();
	m_PetalDR.TurnRound();
	m_PetalUR.TurnRound();
}

void Valve::DrawValve()
{
	int i,j,iInner;
	m_PetalDL.Vertex();
	m_PetalUL.Vertex();
	m_PetalDR.Vertex();
	m_PetalUR.Vertex();
	for(i=int(m_PetalDL.m_p0->m_dx);i<int(m_PetalDL.m_Apex.m_dx)+1;i++)//画左瓣膜，最左边一排归瓣膜
	{
		for(j=0;j<HEIGHT;j++)
		{
			iInner=IsInner(i,j);
			if(iInner==1)//在两段膜之间
			{
				if(m_lattice[i][j].m_iSol==-1||m_lattice[i][j].m_iSol==4)
				{
					m_lattice[i][j].m_iSol=3;//新出现内点，要由内点外推
				}
			}
			else if((iInner==-1||m_lattice[i][j].m_iSol==4)&&(i<m_PetalDL.m_Apex.m_dx&&i>m_PetalDL.m_p0->m_dx&&m_lattice[i][j].m_iSol==-3))
			{
				m_lattice[i][j].m_iSol=1;//新出现外点，要由外点外推
			}
			else if(iInner==0&&j>=m_PetalDL.m_p0->m_dy&&j<=m_PetalUL.m_p0->m_dy)
			{
				m_lattice[i][j].m_iSol=4;
				m_lattice[i][j].m_v = Vector(0, 0);
			}
		}
	}
	for(i=int(m_PetalDR.m_p0->m_dx);i<int(m_PetalDR.m_Apex.m_dx)+1;i++)//画右瓣膜
	{
		for(j=0;j<HEIGHT;j++)
		{
			iInner=IsInner(i,j);
			if(iInner==1)//在两段膜之间
			{
				if(m_lattice[i][j].m_iSol==-1||m_lattice[i][j].m_iSol==4)
				{
					m_lattice[i][j].m_iSol=3;//新出现内点，要由内点外推
				}
			}
			else if((iInner==-1||m_lattice[i][j].m_iSol==4)&&(i<m_PetalDR.m_Apex.m_dx&&i>m_PetalDR.m_p0->m_dx&&m_lattice[i][j].m_iSol==-3))
			{
				m_lattice[i][j].m_iSol=1;//新出现外点，要由外点外推
			}
			else if(iInner==0&&j>=m_PetalDR.m_p0->m_dy&&j<=m_PetalUR.m_p0->m_dy)
			{
				m_lattice[i][j].m_iSol=4;
			}
		}
	}
}

void Valve::Force()
{
	m_PetalDL.Force(iSegValve);
	m_PetalUL.Force(iSegValve);
	m_PetalDR.Force(iSegValve);
	m_PetalUR.Force(iSegValve);
}

void Valve::Remark()
{
	int i,j;
	for(i=int(m_PetalDL.m_p0->m_dx);i<int(m_PetalDL.m_Apex.m_dx)+1;i++)//左瓣膜
	{
		for(j=0;j<HEIGHT;j++)
		{
			if(IsInner(i,j)==1)//在两瓣膜之间
			{
				if(m_lattice[i][j].m_iSol==1)//出现在瓣膜内部的新点
				{
					m_lattice[i][j].m_iSol=-3;
				}
			}
			else if(i<=m_PetalDL.m_Apex.m_dx&&i>=m_PetalDL.m_p0->m_dx&&m_lattice[i][j].m_iSol==3)//出现在瓣膜外部的新点，不会干涉到膜外，因为膜外新点标记是2
			{
				m_lattice[i][j].m_iSol=-1;
			}
		}
	}
	for(i=int(m_PetalDR.m_p0->m_dx);i<int(m_PetalDR.m_Apex.m_dx)+1;i++)//右瓣膜
	{
		for(j=0;j<HEIGHT;j++)
		{
			if(IsInner(i,j)==1)//在两瓣膜之间
			{
				if(m_lattice[i][j].m_iSol==1)//出现在瓣膜内部的新点
				{
					m_lattice[i][j].m_iSol=-3;
				}
			}
			else if(i<=m_PetalDR.m_Apex.m_dx&&i>=m_PetalDR.m_p0->m_dx&&m_lattice[i][j].m_iSol==3)//出现在瓣膜外部的新点，不会干涉到膜外，因为膜外新点标记是2
			{
				m_lattice[i][j].m_iSol=-1;
			}
		}
	}
}

void Valve::FindLink()
{
	int iLink;
	iLink=0;
	m_PetalDL.FindLinkAlongLine(&iLink);//左下
	m_PetalUL.FindLinkAlongLine(&iLink);//左上
	m_PetalDR.FindLinkAlongLine(&iLink);//右下
	m_PetalUR.FindLinkAlongLine(&iLink);
	m_il=iLink;
}

int Valve::IsInner(int ix, int iy)//排除掉膜上的点
{
	int iOnDL,iOnUL,iOnDR,iOnUR;
	iOnDL=OnLine(ix,iy,*m_PetalDL.m_p0,m_PetalDL.m_Apex);//1线上方，0线上面，-1线下方,10线段两端
	iOnUL=OnLine(ix,iy,*m_PetalUL.m_p0,m_PetalUL.m_Apex);
	iOnDR=OnLine(ix,iy,*m_PetalDR.m_p0,m_PetalDR.m_Apex);
	iOnUR=OnLine(ix,iy,*m_PetalUR.m_p0,m_PetalUR.m_Apex);
	if((ix<=m_PetalDL.m_Apex.m_dx&&ix>=m_PetalDL.m_p0->m_dx&&iOnUL==-1&&iOnDL==1||
	ix<=m_PetalDR.m_Apex.m_dx&&ix>=m_PetalDR.m_p0->m_dx&&iOnUR==-1&&iOnDR==1))
	{
		return 1;
	}
	else if(ix<=m_PetalDL.m_Apex.m_dx&&ix>=m_PetalDL.m_p0->m_dx&&(iOnUL==1||iOnDL==-1)||
		ix<=m_PetalDR.m_Apex.m_dx&&ix>=m_PetalDR.m_p0->m_dx&&(iOnUR==1||iOnDR==-1))
	{
		return -1;
	}
	else if((ix<=m_PetalDL.m_Apex.m_dx&&ix>=m_PetalDL.m_p0->m_dx)&&(iOnDL==0||iOnUL==0)||
		ix<=m_PetalDR.m_Apex.m_dx&&ix>=m_PetalDR.m_p0->m_dx&&(iOnDR==0||iOnUR==0))
	{
		return 0;
	}
	else if(iOnDL==0||iOnUL==0)
	{
		return 3;
	}
	else
	{
		return 10;
	}
}

int Valve::OnLine(int ix,int iy,Vector v1,Vector v2)//1线上方，0线上面，-1线下方,10线段两端
{
	if(v1.m_dx>v2.m_dx||v1.m_dx<v2.m_dx)
	{
		if(iy>v1.m_dy+(v2.m_dy-v1.m_dy)/(v2.m_dx-v1.m_dx)*(ix-v1.m_dx))
		{//	if(ix==204&&iy==37&&v1.m_dx>150&&v2.m_dy<50) Show(v2.m_dy,"aasas");
			return 1;
		}
		else if(iy<v1.m_dy+(v2.m_dy-v1.m_dy)/(v2.m_dx-v1.m_dx)*(ix-v1.m_dx))
		{
			return -1;
		}
		else
		{
			if(Between(ix,v1.m_dx,v2.m_dx)&&Between(iy,v1.m_dy,v2.m_dy))
			{
				return 0;
			}
			else
			{
				return 10;
			}
		}
	}
	else if(ix>v1.m_dx)
	{
		return 1;
	}
	else if(ix<v1.m_dx)
	{
		return -1;
	}
	else
	{
		if(Between(ix,v1.m_dx,v2.m_dx)&&Between(iy,v1.m_dy,v2.m_dy))
		{
			return 0;
		}
		else
		{
			return 10;
		}
	}
}
void Valve::Collide()
{
	int i,j;
	Vector wv,bfv,fv,rr;
	double dDen,dKappa;
	double dLinBuff[2];
	Link link[2];
	for(i=0;i<m_il;i++)//扫描链//1
	{
		
		switch(m_link[i].m_iType)//链在上边膜
		{
		case -2://左下
			rr=Vector(m_link[i].m_dBoux,m_link[i].m_dBouy)-(*m_PetalDL.m_p0);
			wv=rr.Roll(PI/2)*m_PetalDL.m_dOmigar;
			break;
		case 2://左上
			rr=Vector(m_link[i].m_dBoux,m_link[i].m_dBouy)-(*m_PetalUL.m_p0);
			wv=rr.Roll(PI/2)*m_PetalUL.m_dOmigar;
			break;
		case -3://右下
			rr=Vector(m_link[i].m_dBoux,m_link[i].m_dBouy)-(*m_PetalDR.m_p0);
			wv=rr.Roll(PI/2)*m_PetalDR.m_dOmigar;
			break;
		case 3://右上
			rr=Vector(m_link[i].m_dBoux,m_link[i].m_dBouy)-(*m_PetalUR.m_p0);
			wv=rr.Roll(PI/2)*m_PetalUR.m_dOmigar;
			break;
		default:
			;
		}
		for(j=0;j<2;j++)//0外面是流体，里面是固体，1反过来//2
		{
			if(j==0)
			{
				link[0]=m_link[i];
			}
			else
			{
				link[1]=-m_link[i];
			}
			fv = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_v;
			dDen=m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_dDen;
			if(m_lattice[link[j].m_iFlux-m_iPrjx[link[j].m_iDir]]//方向为流体指向固体
				[link[j].m_iFluy-m_iPrjy[link[j].m_iDir]].m_iSol==-1&&j==0||
				m_lattice[link[j].m_iFlux-m_iPrjx[link[j].m_iDir]]//方向为流体指向固体
				[link[j].m_iFluy-m_iPrjy[link[j].m_iDir]].m_iSol<0&&j==1)//附近有流体点//3
			{
				if(link[j].m_dDelta>=0.5)//外推//4
				{
					bfv=fv*((link[j].m_dDelta-1)/link[j].m_dDelta)
						+wv/link[j].m_dDelta;
					dKappa=(2*link[j].m_dDelta-1)/m_dTau;
				}
				else//直接用//4
				{	if(j==0)//外点//5
					{
						if(m_lattice[link[j].m_iFlux-m_iPrjx[link[j].m_iDir]]
							[link[j].m_iFluy-m_iPrjy[link[j].m_iDir]].m_iSol==-1)//仍然在外
						{
							bfv = m_lattice[link[j].m_iFlux-m_iPrjx[link[j].m_iDir]]
								[link[j].m_iFluy-m_iPrjy[link[j].m_iDir]].m_v;
						}
						else
						{
							bfv = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_v;
						}
						dKappa=(2*link[j].m_dDelta-1)/(m_dTau-2);
					}
					else//内点//5
					{
						if(m_lattice[link[j].m_iFlux-m_iPrjx[link[j].m_iDir]]
							[link[j].m_iFluy-m_iPrjy[link[j].m_iDir]].m_iSol==-3)//仍然在内
						{
							bfv = m_lattice[link[j].m_iFlux-m_iPrjx[link[j].m_iDir]]
								[link[j].m_iFluy-m_iPrjy[link[j].m_iDir]].m_v;
						}
						else
						{
							bfv = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_v;
						}
						dKappa=(2*link[j].m_dDelta-1)/(m_dTau-2);
					}//5
				}//4
				dLinBuff[j]=double(1-dKappa)*
					m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_df[link[j].m_iDir]
					+dKappa*Lattice::feqs(link[j].m_iDir,dDen,bfv,fv)
					-6*dDen*m_dCoe[link[j].m_iDir]*(Vector(m_iPrjx[link[j].m_iDir],
					m_iPrjy[link[j].m_iDir])*wv);
			}// if//3
			else//附近无流体 或 链反射//3
			{	dLinBuff[j]=m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_df[link[j].m_iDir]
					-2*m_dCoe[link[j].m_iDir]*3*m_dDensity*(Vector(m_iPrjx[link[j].m_iDir],m_iPrjy[link[j].m_iDir])*wv);
				/*if(m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol>=0)
				{
					MessageBox(NULL,"Error","出去啦",MB_OK);
				}*/
			}//3
		}//2
		for(j=0;j<2;j++)
		{
			if(j==0)
			{
				link[0]=m_link[i];
			}
			else
			{
				link[1]=-m_link[i];
			}
		//		if(link[j].m_iSolx==60&&link[j].m_iSoly==51&&link[j].m_iFlux==61&&link[j].m_iFluy==50) {Show(dLinBuff[j],"df");}
			if(m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol<0)
			{
				m_lattice[link[j].m_iSolx][link[j].m_iSoly].m_df[m_iRev[link[j].m_iDir]]=dLinBuff[j];
			}
		}//for i
	}//1
}

void Valve::ElaticRotate()
{
	m_PetalDL.ElasticForce();
	m_PetalUL.ElasticForce();
	m_PetalDR.ElasticForce();
	m_PetalUR.ElasticForce();
}

void Valve::EndPoint()
{

}

BOOL Valve::OnLeftSide(int i, int j)
{
	if(m_PetalDL.OnLeftSide(i,j)&&m_PetalUL.OnLeftSide(i,j))
	{
		return TRUE;
	}
	if(m_PetalDR.OnLeftSide(i,j)&&m_PetalUR.OnLeftSide(i,j))
	{
		return TRUE;
	}
	return FALSE;

}

void Valve::ReactionNO()
{
	m_PetalDL.ReactionNO(int(dR0) * 2);
	m_PetalUL.ReactionNO(int(dR0) * 2);
	m_PetalDR.ReactionNO(int(dR0) * 2);
	m_PetalUR.ReactionNO(int(dR0) * 2);
}



void Valve::OutPutLink()
{
	int i;
	filep.open("f://link.txt",ios::out);
	filep.precision(18);
	for(i=0;i<m_il;i++)
	{
		filep<<m_link[i].m_iSolx<<" "<<m_link[i].m_iSoly<<" "<<m_link[i].m_iFlux
			<<" "<<m_link[i].m_iFluy<<" "<<m_link[i].m_dBoux<<" "<<m_link[i].m_dBouy
			<<" "<<m_link[i].m_dDelta<<endl;
	}
	filep.close();
}


BOOL Valve::Between(double x, double x1, double x2)
{
	if(x1<=x2)
	{
		if(x>=x1&&x<=x2)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if(x>=x2&&x<=x1)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

void Valve::Draw(CDC* pDC, CRect* pRect, double k)
{
	m_PetalDL.Draw(pDC, pRect, k);
	m_PetalDR.Draw(pDC, pRect, k);
	m_PetalUL.Draw(pDC, pRect, k);
	m_PetalUR.Draw(pDC, pRect, k);
}

void Valve :: InitMem()
{
	ValveDual.m_PetalDL.m_fi = new double[iSegValve];
	ValveDual.m_PetalDL.m_fo = new double[iSegValve];
	ValveDual.m_PetalUL.m_fi = new double[iSegValve];
	ValveDual.m_PetalUL.m_fo = new double[iSegValve];
	ValveDual.m_PetalDR.m_fi = new double[iSegValve];
	ValveDual.m_PetalDR.m_fo = new double[iSegValve];
	ValveDual.m_PetalUR.m_fi = new double[iSegValve];
	ValveDual.m_PetalUR.m_fo = new double[iSegValve];
}

void Valve::SaveForAnimate(fstream *ff)
{
	m_PetalDL.SaveForAnimate(ff);
	m_PetalDR.SaveForAnimate(ff);
	m_PetalUL.SaveForAnimate(ff);
	m_PetalUR.SaveForAnimate(ff);
}

fstream& operator<<(fstream& fs, Valve& v)
{
	fs << v.m_PetalDL;
	fs << v.m_PetalUL;
	fs << v.m_PetalDR;
	fs << v.m_PetalUR;
	return fs;
}

fstream& operator>>(fstream& fs, Valve& v)
{
	fs >> v.m_PetalDL;
	fs >> v.m_PetalUL;
	fs >> v.m_PetalDR;
	fs >> v.m_PetalUR;
	return fs;
}
