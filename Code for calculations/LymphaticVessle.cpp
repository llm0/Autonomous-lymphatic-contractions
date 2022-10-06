// LymphaticVessle.cpp: implementation of the LymphaticVessle class.
//
//////////////////////////////////////////////////////////////////////

#include "LymphaticVessle.h"
#include "math.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Lattice(*(LymphaticVessle::m_lattice))[HEIGHT];
int* LymphaticVessle::m_iPrjx;
int* LymphaticVessle::m_iPrjy;
int(*(LymphaticVessle::m_iPrjxy))[9];
int* LymphaticVessle::m_iRev;
double* LymphaticVessle::m_dAngleE;
double* LymphaticVessle::m_dCoe;
double LymphaticVessle::m_dTau;
double LymphaticVessle::m_dDensity;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LymphaticVessle::LymphaticVessle()
{
	int m_il = 0;
}

LymphaticVessle::~LymphaticVessle()
{

}

void LymphaticVessle::DrawLymph_L()//Inner:-1,OutSide:-2,New inner:1,New outside:2 
{
	int i, j;
	int iInner;
	int ixm;
	if (iMyID == iNumProcs - 1)
	{
		ixm = ((int(m_MemUp.m_dx2) + 1 < iEnd_Row - 1) ? int(m_MemUp.m_dx2) + 1 : iEnd_Row - 1);//�ұ��б߽�
	}
	else
	{
		ixm = ((int(m_MemUp.m_dx2) + 1 < iEnd_Row) ? int(m_MemUp.m_dx2) + 1 : iEnd_Row);
	}
	for (i = int(m_MemUp.m_dx1) - 1; i < ixm; i++)//����ߵ������˰�Ĥ
	{
		for (j = 1; j < HEIGHT - 1; j++)
		{
			iInner = IsInner(i, j); 
			if (iInner == 1)//������Ĥ֮��
			{
				if (m_lattice[i][j].m_iSol == -2 || m_lattice[i][j].m_iSol == 0)
				{
					m_lattice[i][j].m_iSol = 1;//�³����ڵ㣬Ҫ���ڵ�����
				}
			}
			else if (iInner == -1 && i >= m_MemUp.m_dx1&&i <= m_MemUp.m_dx2 && (m_lattice[i][j].m_iSol == -1 ||
				m_lattice[i][j].m_iSol == 0))
			{
				m_lattice[i][j].m_iSol = 2;//�³�����㣬Ҫ���������
			}
			else if (iInner == 0)//Ĥ�ϵĵ�
			{
				m_lattice[i][j].m_iSol = 0;
				m_lattice[i][j].m_v = Vector(0, 0);
			}
		}
	}
}

int LymphaticVessle::IsInner(int ix, int iy)
{
	int m, n;
	if (m_MemUp.Between(ix, &m, &n))//�ҵ�λ��������Ĥ֮��
	{
		if (ix >= m_MemUp.m_dx1&&ix <= m_MemUp.m_dx2&&!(ix == iEnd_Row - 1 && iMyID == iNumProcs - 1))//������Ĥ������֮��
		{
			if (iy<m_MemUp.m_segment[m].m_pst.m_dy + (m_MemUp.m_segment[n].m_pst.m_dy//��Ĥ������
				- m_MemUp.m_segment[m].m_pst.m_dy) / (m_MemUp.m_segment[n].m_pst.m_dx
				- m_MemUp.m_segment[m].m_pst.m_dx)*(ix - m_MemUp.m_segment[m].m_pst.m_dx) &&

				iy>m_MemDown.m_segment[m].m_pst.m_dy + (m_MemDown.m_segment[n].m_pst.m_dy//��Ĥ������
				- m_MemDown.m_segment[m].m_pst.m_dy) / (m_MemDown.m_segment[n].m_pst.m_dx
				- m_MemDown.m_segment[m].m_pst.m_dx)*(ix - m_MemDown.m_segment[m].m_pst.m_dx))
			{
				return 1;
			}
			else if (iy > m_MemUp.m_segment[m].m_pst.m_dy + (m_MemUp.m_segment[n].m_pst.m_dy//��Ĥ������
				- m_MemUp.m_segment[m].m_pst.m_dy) / (m_MemUp.m_segment[n].m_pst.m_dx
				- m_MemUp.m_segment[m].m_pst.m_dx)*(ix - m_MemUp.m_segment[m].m_pst.m_dx) ||

				iy < m_MemDown.m_segment[m].m_pst.m_dy + (m_MemDown.m_segment[n].m_pst.m_dy//��Ĥ������
				- m_MemDown.m_segment[m].m_pst.m_dy) / (m_MemDown.m_segment[n].m_pst.m_dx
				- m_MemDown.m_segment[m].m_pst.m_dx)*(ix - m_MemDown.m_segment[m].m_pst.m_dx))
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}
	return -1;
}

void LymphaticVessle::init_LM(int bFromFile)
{
	int i, j, iInner;
	m_MemUp.m_pMemUp = &m_MemUp;
	m_MemDown.m_pMemDown = &m_MemDown;
	*(SolValve.m_SegDL0) = &(m_MemDown.m_segment[LEFTVALVE]);//seg[0]��ַ���ݸ�ֵ
	*(SolValve.m_SegUL0) = &(m_MemUp.m_segment[LEFTVALVE]);
	*(SolValve.m_SegDR0) = &(m_MemDown.m_segment[RIGHTVALVE]);
	*(SolValve.m_SegUR0) = &(m_MemUp.m_segment[RIGHTVALVE]);
	if (bFromFile)
	{
		m_MemUp.init(1);
		m_MemDown.init(1);
	}
	else
	{
		if (iMyID == 0)
		{
			m_MemUp.m_dx1 = PX2;
			m_MemDown.m_dx1 = PX2;
		}
		else
		{
			m_MemUp.m_dx1 = iBegin_Row;
			m_MemDown.m_dx1 = iBegin_Row;
		}
		m_MemUp.m_dy1 = H2;
		m_MemUp.m_dx2 = iEnd_Row-1; m_MemUp.m_dy2 = H2;
		m_MemDown.m_dy1 = H1;
		m_MemDown.m_dx2 = iEnd_Row-1; m_MemDown.m_dy2 = H1;
		m_MemUp.m_iType = 1; m_MemDown.m_iType = -1;
		m_MemUp.init(0);
		m_MemDown.init(0); 
		for (i = int(m_MemUp.m_dx1) - 1; i < int(m_MemUp.m_dx2) + 1; i++)
		{
			for (j = 0; j < HEIGHT; j++)
			{
				iInner = IsInner(i, j);
				if (iInner == 1)//������Ĥ֮��
				{
					m_lattice[i][j].m_iSol = -1;
				}
				else if (iInner == 0)
				{
					m_lattice[i][j].m_iSol = 0;
				}
			}
		}
	}
	for (i = 0; i < SEGNUM_E; i++)
	{
		m_MemUp.m_segment[i].m_Opposite = &(m_MemDown.m_segment[i]);
		m_MemDown.m_segment[i].m_Opposite = &(m_MemUp.m_segment[i]);
	}
}

void LymphaticVessle::FindLink_L()//���ķ���Ϊ����ָ����ڣ����⿴Ϊ����
{
	int i, j, L, m, iLink,iSeg,iSol,iFol;
	int iFlux, iFluy;
	Vector p;
	double ddy;
	int iNearest = 0;
	if (iMyID == 0)
	{
		m_ixn = int(m_MemUp.m_segment[LEFTVALVE].m_pst.m_dx); m_ixm = int(m_MemUp.m_segment[LEFTVALVE].m_pst.m_dx);
		m_iyn = int(m_MemUp.m_segment[LEFTVALVE].m_pst.m_dy); m_iym = int(m_MemUp.m_segment[LEFTVALVE].m_pst.m_dy);
	}
	else
	{
		m_ixn = int(m_MemUp.m_segment[0].m_pst.m_dx); m_ixm = int(m_MemUp.m_segment[0].m_pst.m_dx);
		m_iyn = int(m_MemUp.m_segment[0].m_pst.m_dy); m_iym = int(m_MemUp.m_segment[0].m_pst.m_dy);
	}
	for (i = iBegin_Mem; i<iEnd_Mem; i++)//�ҳ�����Ĥ��������
	{
		if (m_ixn>m_MemUp.m_segment[i].m_pst.m_dx)//����Ĥ���ڷ�Χ
		{
			m_ixn = int(m_MemUp.m_segment[i].m_pst.m_dx);
		}
		if (m_ixm<m_MemUp.m_segment[i].m_pst.m_dx)
		{
			m_ixm = int(m_MemUp.m_segment[i].m_pst.m_dx + 0.5);
		}
		if (m_iyn>m_MemUp.m_segment[i].m_pst.m_dy)
		{
			m_iyn = int(m_MemUp.m_segment[i].m_pst.m_dy);
		}
		if (m_iym<m_MemUp.m_segment[i].m_pst.m_dy)
		{
			m_iym = int(m_MemUp.m_segment[i].m_pst.m_dy + 0.5);
		}


		if (m_ixn>m_MemDown.m_segment[i].m_pst.m_dx)//����Ĥ���ڷ�Χ
		{
			m_ixn = int(m_MemDown.m_segment[i].m_pst.m_dx);
		}
		if (m_ixm<m_MemDown.m_segment[i].m_pst.m_dx)
		{
			m_ixm = int(m_MemDown.m_segment[i].m_pst.m_dx + 0.5);
		}
		if (m_iyn>m_MemDown.m_segment[i].m_pst.m_dy)
		{
			m_iyn = int(m_MemDown.m_segment[i].m_pst.m_dy);
		}
		if (m_iym < m_MemDown.m_segment[i].m_pst.m_dy)
		{
			m_iym = int(m_MemDown.m_segment[i].m_pst.m_dy + 0.5);
		}
	}
	m_ixn--; m_ixm = ((m_ixm + 1) < iEnd_Row ? m_ixm + 1 : iEnd_Row - 1); m_iyn--; m_iym++;//��������
	iLink = 0;
	for (i = m_ixn; i <= m_ixm; i++)//ɨ������
	{
		for (j = m_iyn; j <= m_iym; j++)//ɨ������
		{
			if (m_lattice[i][j].m_iSol == -1 || m_lattice[i][j].m_iSol == 0 ||
				(m_lattice[i][j].m_iSol == 4 && OnMem(i, j)) || m_lattice[i][j].m_iSol == -3
				)
			{
				iSeg = BEGINMEM + int(double(i - iBegin_Row)*(iEnd_Mem - 1 - BEGINMEM)
					/ (iEnd_Row - 1 - iBegin_Row) + 0.5);
				iSol = m_lattice[i][j].m_iSol;
				for (m = 1; m < 9; m++)//m�ķ����ɹ���ָ������
				{
					iFlux = i + m_iPrjx[m];  //fluid node -1~n
					iFluy = j + m_iPrjy[m];  //iFluy -1~n
					iFol = m_lattice[iFlux][iFluy].m_iSol;
					iNearest = iSeg;
					if (iSeg < iEnd_Mem - 1)
					{
						if (iFluy >= (HEIGHT - 1) / 2)
						{
							ddy = fabs(m_MemUp.m_segment[iSeg + 1].m_pst.m_dy + (m_MemUp.m_segment[iSeg].m_pst.m_dy
								- m_MemUp.m_segment[iSeg + 1].m_pst.m_dy) / (m_MemUp.m_segment[iSeg].m_pst.m_dx
								- m_MemUp.m_segment[iSeg + 1].m_pst.m_dx)*(iFlux - m_MemUp.m_segment[iSeg + 1].m_pst.m_dx) - iFluy);
						}
						else
						{
							ddy = fabs(m_MemDown.m_segment[iSeg + 1].m_pst.m_dy + (m_MemDown.m_segment[iSeg].m_pst.m_dy
								- m_MemDown.m_segment[iSeg + 1].m_pst.m_dy) / (m_MemDown.m_segment[iSeg].m_pst.m_dx
								- m_MemDown.m_segment[iSeg + 1].m_pst.m_dx)*(iFlux - m_MemDown.m_segment[iSeg + 1].m_pst.m_dx) - iFluy);
						}
					}
					if (i < m_MemUp.m_segment[iBegin_Mem].m_pst.m_dx&&iFlux < m_MemUp.m_segment[iBegin_Mem].m_pst.m_dx||
						i < m_MemUp.m_segment[iBegin_Mem].m_pst.m_dx&&iFlux >= m_MemUp.m_segment[iBegin_Mem].m_pst.m_dx&&
						iFol == -2 && iSol == -1||i >= m_MemUp.m_segment[iBegin_Mem].m_pst.m_dx&&iFlux 
						< m_MemUp.m_segment[iBegin_Mem].m_pst.m_dx&&
						iSol == -2 && iFol == -1)
					{
						continue;
					}
					if (InRegionX(iFlux) && iFluy >= 0 && iFluy < HEIGHT)
					{
						if (m_lattice[i][j].m_iSol == -1 && m_lattice[iFlux][iFluy].m_iSol == -2
							|| m_lattice[i][j].m_iSol == 0 && m_lattice[iFlux][iFluy].m_iSol < 0
							|| m_lattice[i][j].m_iSol == 4 && m_lattice[iFlux][iFluy].m_iSol < 0
							|| m_lattice[i][j].m_iSol == -3 && m_lattice[iFlux][iFluy].m_iSol == -2) //�ɹ���ָ�����
						{
							m_link[iLink].m_iSolx = i;
							m_link[iLink].m_iSoly = j;
							m_link[iLink].m_iFlux = iFlux;
							m_link[iLink].m_iFluy = iFluy;
							m_link[iLink].m_iDir = m_iRev[m];//����Ϊ������ָ�����
							m_link[iLink].m_iSegf = -1;
							for (L = (iMyID == 0) ? iBegin_Mem : iBegin_Mem - 1; L < ((iMyID == iNumProcs - 1) ? iEnd_Mem : iEnd_Mem+1); L++)//
							{
								if (iFluy >= (HEIGHT - 1) / 2)
								{
									if (m_MemUp.m_segment[L].IsCross(Vector(iFlux, iFluy), Vector(i, j), &p))
									{
										m_link[iLink].m_bf = p;
										m_link[iLink].m_iSegf = L;
										m_link[iLink].m_iType = 1;//�����ϱ�Ĥ
										if (m_link[iLink].m_iDir != 3 && m_link[iLink].m_iDir != 4)//����ֱ����
										{
											m_link[iLink].m_dDlf = fabs(m_link[iLink].m_bf.m_dx - m_link[iLink].m_iFlux);
										}
										else
										{
											m_link[iLink].m_dDlf = fabs(m_link[iLink].m_bf.m_dy - m_link[iLink].m_iFluy);
										}
										break;
									}
								}
								else
								{
									if (m_MemDown.m_segment[L].IsCross(Vector(iFlux, iFluy), Vector(i, j), &p))
									{
										m_link[iLink].m_bf = p;
										m_link[iLink].m_iSegf = L;
										m_link[iLink].m_iType = -1;//�����±�Ĥ
										if (m_link[iLink].m_iDir != 3 && m_link[iLink].m_iDir != 4)//����ֱ����
										{
											m_link[iLink].m_dDlf = fabs(m_link[iLink].m_bf.m_dx - m_link[iLink].m_iFlux);
										}
										else
										{
											m_link[iLink].m_dDlf = fabs(m_link[iLink].m_bf.m_dy - m_link[iLink].m_iFluy);
										}	
										break;
									}
								}
								if (L == iEnd_Mem - 1 && (iMyID == iNumProcs - 1 || iMyID==0))//������ڶ��϶˵㣬ʵ���޷��ж�
								{
									if (m_lattice[i][j].m_iSol >= 0)//�ڶ��϶˵㣬����Ӧ�ú�С
									{
										m_link[iLink].m_bf = Vector(i, j);
										m_link[iLink].m_iSegf = iNearest;
										m_link[iLink].m_dDlf = 1;//�߽�պ��ڹ������;
										if (m_lattice[i][j].m_iSol == 0 && (m_lattice[iFlux][iFluy].m_iSol == -3 || m_lattice[iFlux][iFluy].m_iSol == -1))
										{
											m_lattice[iFlux][iFluy].m_bReaction = 1;
										}
									}
									else if (iSol == -1 || iSol == -3)//�ǳ�����(i,j),��������޷��ж�
									{
										m_link[iLink].m_bf = Vector(i, j);
										m_link[iLink].m_iSegf = iNearest;
										m_link[iLink].m_dDlf = 1;//�߽�պ��ڹ������;
										m_lattice[i][j].m_bReaction = 1;
									}
									else
									{
										printf("find cross error Procs=%d,step=%ld,x=%d,y=%d,fx=%d,fy=%d,s=%d,f=%d\n",iMyID, lNowStep, i, j, iFlux, iFluy, m_lattice[i][j].m_iSol, m_lattice[iFlux][iFluy].m_iSol);
									}
								}
							}//for l
							
							if (m_lattice[i][j].m_iSol == -1 || (m_lattice[i][j].m_iSol == -3 
								&& m_lattice[iFlux][iFluy].m_iSol == -2))
							{
								m_lattice[i][j].m_bReaction = 1;//��ĤҪ��NO
							}
							else if (m_lattice[i][j].m_iSol == 0 && 
								(m_lattice[iFlux][iFluy].m_iSol == -3 || m_lattice[iFlux][iFluy].m_iSol == -1))
							{
								m_lattice[iFlux][iFluy].m_bReaction = 1;
							}
							else if ((m_lattice[i][j].m_iSol == 0 || m_lattice[i][j].m_iSol == 4) &&
								m_lattice[i - m_iPrjx[m]][j - m_iPrjy[m]].m_iSol == -1)
							{
								m_lattice[i - m_iPrjx[m]][j - m_iPrjy[m]].m_bReaction = 1;
							}
							if (m_link[iLink].m_iSegf >= 0)
							{
								iLink++;
							}
							if (iLink > LINKNUM)
							{
								printf("ϸ�������������趨ֵ");
							}
						}//if
					}//if
				}//for m
			}//for if
		}//for j
	}//for i
	m_il = iLink;
}


void LymphaticVessle::Collide_L()
{
	int i, j, iffx, iffy;
	Vector wv, bfv, fv;
	double dDen, dKappa;
	double dLinBuff[2];
	Link link[2]; 
	for (i = 0; i < m_il; i++)//ɨ����//1
	{
		if (m_link[i].m_iType == 1)//�����ϱ�Ĥ
		{
			if (m_MemUp.m_segment[m_link[i].m_iSegf].m_next != NULL)
			{
				wv = (m_MemUp.m_segment[m_link[i].m_iSegf].m_v + m_MemUp.m_segment[m_link[i].m_iSegf].m_next->m_v) / 2.0;
			}
			else
			{
				wv = m_MemUp.m_segment[m_link[i].m_iSegf].m_v;
			}
		}
		else if (m_link[i].m_iType == -1)//�����±�Ĥ
		{
			if (m_MemDown.m_segment[m_link[i].m_iSegf].m_next != NULL)
			{
				wv = (m_MemDown.m_segment[m_link[i].m_iSegf].m_v + m_MemDown.m_segment[m_link[i].m_iSegf].m_next->m_v) / 2.0;
			}
			else
			{
				wv = m_MemDown.m_segment[m_link[i].m_iSegf].m_v;
			}
		}
		for (j = 0; j < 2; j++)//0���������壬�����ǹ��壬1������//2
		{
			if (j == 0)
			{	
				link[0] = m_link[i];
			}
			else
			{
				link[1] = -m_link[i];
			}
			fv = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_v;
			dDen = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_dDen;
			iffx = link[j].m_iFlux - m_iPrjx[link[j].m_iDir];
			iffy = link[j].m_iFluy - m_iPrjy[link[j].m_iDir];
			if ((m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol != 0) &&//�Ѿ����߽�
				(m_lattice[iffx][iffy].m_iSol == -2 && j == 0 ||
				m_lattice[iffx][iffy].m_iSol == -1 && j == 1))//�����������//3
			{
				if (link[j].m_dDlf >= 0.5)//����//4
				{
					bfv = fv*((link[j].m_dDlf - 1) / link[j].m_dDlf)
						+ wv / link[j].m_dDlf;
					dKappa = (2 * link[j].m_dDlf - 1) / m_dTau;
				}
				else//ֱ����//4
				{
					if (j == 0)//���//5
					{
						if (m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]
							[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_iSol == -2)//��Ȼ����
						{
							bfv = m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]
								[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_v;
						}
						else
						{
							bfv = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_v;
						}
						dKappa = (2 * link[j].m_dDlf - 1) / (m_dTau - 2);
					}
					else//�ڵ�//5
					{
						if (m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]
							[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_iSol == -1)//��Ȼ����
						{
							bfv = m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]
								[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_v;
						}
						else
						{
							bfv = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_v;
						}
						dKappa = (2 * link[j].m_dDlf - 1) / (m_dTau - 2);
					}//5
				}//4
				dLinBuff[j] = double(1 - dKappa)*
					m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_df[link[j].m_iDir]
					+ dKappa*Lattice::feqs(link[j].m_iDir, dDen, bfv, fv)
					- 6 * dDen*m_dCoe[link[j].m_iDir] * (Vector(m_iPrjx[link[j].m_iDir],
					m_iPrjy[link[j].m_iDir])*wv);
			}// if//3
			else//���������� �� ������//3
			{
				dLinBuff[j] = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_df[link[j].m_iDir]
					- 2 * m_dCoe[link[j].m_iDir] * 3 * m_dDensity*(Vector(m_iPrjx[link[j].m_iDir], m_iPrjy[link[j].m_iDir])*wv);
				/*if(m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol>=0)
				{
				MessageBox(NULL,"Error","��ȥ��",MB_OK);
				}*/
			}//3
		}//2
		for (j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				link[0] = m_link[i];
			}
			else
			{
				link[1] = -m_link[i];
			}
			if (m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol < 0)
			{
				m_lattice[link[j].m_iSolx][link[j].m_iSoly].m_df[m_iRev[link[j].m_iDir]] = dLinBuff[j];
			}
		}//for i
	}//1
}

void LymphaticVessle::Force_M()
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)//���һ�β�����
	{
		if (m_MemUp.m_segment[i].m_bCanMove)
		{
			m_MemUp.m_segment[i].Force();
		}
		if (m_MemDown.m_segment[i].m_bCanMove)
		{
			m_MemDown.m_segment[i].Force();
		}
	}
}

void LymphaticVessle::HydroAcc_M()
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)//��һ�β���
	{
		if (m_MemUp.m_segment[i].m_bCanMove)
		{
			m_MemUp.m_segment[i].HydroAcc();
		}
		if (m_MemDown.m_segment[i].m_bCanMove)
		{
			m_MemDown.m_segment[i].HydroAcc();
		}
	}
}

void LymphaticVessle::Move_M(int j)
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)//��һ�β���
	{
		if (m_MemUp.m_segment[i].m_bCanMove)
		{
			m_MemUp.m_segment[i].Move(j);
		}
		if (m_MemDown.m_segment[i].m_bCanMove)
		{
			m_MemDown.m_segment[i].Move(j);
		}
	}
}

void LymphaticVessle::ElastAcc_M(int j)
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)//��һ�β���
	{
		if (m_MemUp.m_segment[i].m_bCanMove)
		{
			m_MemUp.m_segment[i].m_ka = Vector(0, 0);
		}
		if (m_MemDown.m_segment[i].m_bCanMove)
		{
			m_MemDown.m_segment[i].m_ka = Vector(0, 0);
		}
	}
	for (i = iBegin_Mem; i < iEnd_Mem; i++)//��һ�β���
	{
		if (m_MemUp.m_segment[i].m_bCanMove)
		{
			m_MemUp.m_segment[i].ElastAcc(j);
		}
		if (m_MemDown.m_segment[i].m_bCanMove)
		{
			m_MemDown.m_segment[i].ElastAcc(j);
		}
	}
}

void LymphaticVessle::VescoAcc_M(int j)
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)//��һ�β���
	{
		if (m_MemUp.m_segment[i].m_bCanMove)
		{
			m_MemUp.m_segment[i].VescoAcc();
		}
		if (m_MemDown.m_segment[i].m_bCanMove)
		{
			m_MemDown.m_segment[i].VescoAcc();
		}
	}
}


MPI_File & operator >>(MPI_File &fs, LymphaticVessle &v)//2*
{
	fs >> v.m_MemUp;
	fs >> v.m_MemDown;
	return fs;
}

MPI_File& operator <<(MPI_File &fs, LymphaticVessle &v)
{
	fs << v.m_MemUp;
	fs << v.m_MemDown;
	return fs;
}

void LymphaticVessle::Pack(char* buffer, int  size, int* position)
{
	m_MemUp.Pack(buffer, size, position);
	m_MemDown.Pack(buffer, size, position);
}

void LymphaticVessle::Unpack(char* buffer, int size, int* position)
{
	m_MemUp.Unpack(buffer, size, position);
	m_MemDown.Unpack(buffer, size, position);
}

void LymphaticVessle::Insert_L()
{
	int i, j, m, n, k;
	int ix1, iy1, ix2, iy2;
	for (i = int(m_MemUp.m_dx1) - 1; i < int(m_MemUp.m_dx2) + 1; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (m_lattice[i][j].m_iSol == 1 || m_lattice[i][j].m_iSol == 2 || m_lattice[i][j].m_iSol == 3)//���µ�
			{
				m = 0;
				m_lattice[i][j].Clear();
				for (k = 1; k < 9; k++)
				{
					ix1 = i + m_iPrjx[k];
					iy1 = j + m_iPrjy[k];
					if ((m_lattice[i][j].m_iSol == 1 && m_lattice[ix1][iy1].m_iSol == -1 ||
						m_lattice[i][j].m_iSol == 2 && m_lattice[ix1][iy1].m_iSol == -2 && m_lattice[ix1][iy1].m_ipr !=2) &&
						OnSameSide(j, iy1) ||
						m_lattice[i][j].m_iSol == 3 && (m_lattice[ix1][iy1].m_iSol == -3))//||
						//(ValveDual.OnLeftSide(ix1,iy1)&&m_lattice[ix1][iy1].m_iSol==-1)))//�ڵ�Ҫ�ڵ����ƣ����Ҫ���������,�ڿ�����ֱ����ʱ��������ڵ����
					{
						ix2 = ix1 + m_iPrjx[k];
						iy2 = iy1 + m_iPrjy[k];
						if (m_lattice[ix1][iy1].m_iSol == m_lattice[ix2][iy2].m_iSol&& m_lattice[ix2][iy2].m_ipr != 2)//Ҫ���Ƶ�����ͬΪ�ڵ�����
						{
							for (n = 0; n < 9; n++)
							{
								m_lattice[i][j].m_df[n] += 2 * m_lattice[ix1][iy1].m_df[n] - m_lattice[ix2][iy2].m_df[n];
							}
							m++;
						}
						else//����ֱ���ú���
						{
							for (n = 0; n < 9; n++)
							{
								m_lattice[i][j].m_df[n] += m_lattice[ix1][iy1].m_df[n];
							}
							m++;
						}
					}
				}//for k
				if (m != 0)
				{
					for (n = 0; n < 9; n++)
					{
						m_lattice[i][j].m_df[n] /= m;
						m_lattice[i][j].m_dDen += m_lattice[i][j].m_df[n];
						m_lattice[i][j].m_v.m_dx += m_iPrjx[n] * m_lattice[i][j].m_df[n];
						m_lattice[i][j].m_v.m_dy += m_iPrjy[n] * m_lattice[i][j].m_df[n];
					}
				}
				else
				{
					int ok = 0;
					for (k = 1; k < 9; k++)
					{
						ix1 = i + 3 * m_iPrjx[k];
						iy1 = j + 3 * m_iPrjy[k];
						if ((m_lattice[i][j].m_iSol == 1 && m_lattice[ix1][iy1].m_iSol == -1 ||
							m_lattice[i][j].m_iSol == 2 && m_lattice[ix1][iy1].m_iSol == -2 && m_lattice[ix1][iy1].m_ipr != 2)
							&& OnSameSide(j, iy1) ||
							m_lattice[i][j].m_iSol == 3 && m_lattice[ix1][iy1].m_iSol == -3)
						{
							m_lattice[i][j] = m_lattice[ix1][iy1];
							ok = 1;
						}
					}
					if (!ok)
					{
						if (m_lattice[i][j].m_iSol == 3)//Valve ��סһ��û������
						{
							m_lattice[i][j].m_dDen = dDensity;
							m_lattice[i][j].m_v = Vector(0, 0);
							for (k = 0; k < 9; k++)
							{
								m_lattice[i][j].m_df[k] = Lattice::feq(k, dDensity, 0, 0);
							}
						}
						else
						{
							printf("Insert Fluid error");
						}
					}
				}
			}
		}//for j
	}//for i
}

int LymphaticVessle::OnSameSide(int y1, int y2)
{
	if ((y1 - CENTER)*(y2 - CENTER) > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void LymphaticVessle::ReMark_L()
{
	int i, j;
	for (i = int(m_MemUp.m_dx1) - 1; i < int(m_MemUp.m_dx2) + 1; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (m_lattice[i][j].m_iSol == 1)//�������ڲ����µ�
			{
				m_lattice[i][j].m_iSol = -1;
			}
			else if (m_lattice[i][j].m_iSol == 2)//�������ⲿ���µ�
			{
				m_lattice[i][j].m_iSol = -2;
			}
			else if (m_lattice[i][j].m_iSol == 3)//�����ڰ�Ĥ�ڲ����µ�
			{
				m_lattice[i][j].m_iSol = -3;
			}
		}//for j
	}//for i
}

void LymphaticVessle::SavePosition(fstream *output)
{
	int i;
	if (output->is_open())
	{
		*output << SEGNUM << endl;
		for (i = 0; i < SEGNUM; i++)
		{
			*output << m_MemUp.m_segment[i].m_pst.m_dx << " " << m_MemUp.m_segment[i].m_pst.m_dy << " ";
			*output << m_MemDown.m_segment[i].m_pst.m_dx << " " << m_MemDown.m_segment[i].m_pst.m_dy << " ";
		}
		*output << endl;
	}
}

void LymphaticVessle::Angle_M()
{
	m_MemUp.Angle();
	m_MemDown.Angle();
}

void LymphaticVessle::CalciumForce_M()
{
	m_MemUp.CalciumForce();
	m_MemDown.CalciumForce();
}

void LymphaticVessle::BackupPosition_M()
{
	m_MemUp.BackupPosition();
	m_MemDown.BackupPosition();
}

void LymphaticVessle::CaReaction()
{
	m_MemUp.ReactionCa(dCaBuffUp);
	m_MemDown.ReactionCa(dCaBuffDown);
}

void LymphaticVessle::CaDiffusion_M()
{
	m_MemUp.CalciumSpread(dCaBuffUp);
	m_MemDown.CalciumSpread(dCaBuffDown);
}

void LymphaticVessle::SaveCa()
{
	int i;
	filep.open("f:\\a.txt", ios::out);
	filep.precision(18);
	for (i = 0; i < SEGNUM; i++)
	{
		filep << i << " " << m_MemUp.m_segment[i].m_dcCa << " " << m_MemDown.m_segment[i].m_dcCa << endl;
	}
	filep.close();
}

void LymphaticVessle::Activate_M()
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		if (m_MemUp.m_segment[i].Triger() && m_MemDown.m_segment[i].Triger())
		{
			m_MemUp.m_segment[i].m_bPassTh = 1;
			m_MemDown.m_segment[i].m_bPassTh = 1;
		}
		if (m_MemUp.m_segment[i].m_dGrowth < 0 || m_MemDown.m_segment[i].m_dGrowth < 0)
		{
			m_MemUp.m_segment[i].m_bPassTh = 0;
			m_MemDown.m_segment[i].m_bPassTh = 0;
		}
	}
}

void LymphaticVessle::AverageCaForce_M()
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		m_MemUp.m_segment[i].m_fm = (m_MemUp.m_segment[i].m_fm - m_MemDown.m_segment[i].m_fm) / 2;
		m_MemDown.m_segment[i].m_fm = -m_MemUp.m_segment[i].m_fm;
	}
}

void LymphaticVessle::CaAverage_M()
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		m_MemUp.m_segment[i].m_dcCa = (m_MemUp.m_segment[i].m_dcCa + m_MemDown.m_segment[i].m_dcCa) / 2;
		m_MemDown.m_segment[i].m_dcCa = m_MemUp.m_segment[i].m_dcCa;
	}
}

void LymphaticVessle::AverageStress_L()
{
	int i;
	BoundaryLattice OuterUp, InnerUp, OuterDown, InnerDown;
	double dd;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		OuterUp.m_dx = m_MemUp.m_segment[i].m_pst.m_dx;
		OuterUp.m_dy = m_MemUp.m_segment[i].m_pst.m_dy;
		m_MemUp.m_segment[i].Nearest(&OuterUp, &InnerUp);
		OuterDown.m_dx = m_MemDown.m_segment[i].m_pst.m_dx;
		OuterDown.m_dy = m_MemDown.m_segment[i].m_pst.m_dy;
		m_MemDown.m_segment[i].Nearest(&OuterDown, &InnerDown);
		dd = (lattice[InnerUp.m_ix][InnerUp.m_iy].m_dcNO +
			lattice[InnerDown.m_ix][InnerDown.m_iy].m_dcNO) / 2.0;
		lattice[InnerUp.m_ix][InnerUp.m_iy].m_dcNO = dd;
		lattice[InnerDown.m_ix][InnerDown.m_iy].m_dcNO = dd;
	}
}

void LymphaticVessle::SaveForAnimate(fstream *ff)
{
	int i;
	BoundaryLattice Outer, Inner;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		(*ff) << m_MemUp.m_segment[i].m_pst.m_dx << " " << m_MemUp.m_segment[i].m_pst.m_dy << " "
			<< m_MemUp.m_segment[i].m_dcCa << " ";
		Outer.m_dx = m_MemUp.m_segment[i].m_pst.m_dx;
		Outer.m_dy = m_MemUp.m_segment[i].m_pst.m_dy;
		m_MemUp.m_segment[i].Nearest(&Outer, &Inner);
		(*ff) << lattice[Inner.m_ix][Inner.m_iy].m_dcNO << " " << m_MemUp.m_segment[i].m_bPassTh << " ";

		(*ff) << m_MemDown.m_segment[i].m_pst.m_dx << " " << m_MemDown.m_segment[i].m_pst.m_dy << " "
			<< m_MemDown.m_segment[i].m_dcCa << " ";
		Outer.m_dx = m_MemDown.m_segment[i].m_pst.m_dx;
		Outer.m_dy = m_MemDown.m_segment[i].m_pst.m_dy;
		m_MemDown.m_segment[i].Nearest(&Outer, &Inner);
		(*ff) << lattice[Inner.m_ix][Inner.m_iy].m_dcNO << " " << m_MemDown.m_segment[i].m_bPassTh << " ";
	}
}

void LymphaticVessle::ForAnimate(LymphAnimate* anid)
{
	int i;
	BoundaryLattice Outer, Inner;
	for (i = BEGINMEM; i < iEnd_Mem; i++)
	{
		anid->m_p[0][i - BEGINMEM] = m_MemUp.m_segment[i].m_pst;
		anid->m_Ca[0][i - BEGINMEM] = float(m_MemUp.m_segment[i].m_dcCa);
		Outer.m_dx = m_MemUp.m_segment[i].m_pst.m_dx;
		Outer.m_dy = m_MemUp.m_segment[i].m_pst.m_dy;
		if (m_MemUp.m_segment[i].m_bCanMove)
		{
			m_MemUp.m_segment[i].Nearest(&Outer, &Inner);
			anid->m_NO[0][i - BEGINMEM] = float(lattice[Inner.m_ix][Inner.m_iy].m_dcNO);
		}
		else
		{
			anid->m_NO[0][i - BEGINMEM] = 0;
		}
		anid->m_th[0][i - BEGINMEM] = m_MemUp.m_segment[i].m_bPassTh;
		anid->m_p[1][i - BEGINMEM] = m_MemDown.m_segment[i].m_pst;
		anid->m_Ca[1][i - BEGINMEM] = float(m_MemDown.m_segment[i].m_dcCa);
		Outer.m_dx = m_MemDown.m_segment[i].m_pst.m_dx;
		Outer.m_dy = m_MemDown.m_segment[i].m_pst.m_dy;
		if (m_MemDown.m_segment[i].m_bCanMove)
		{
			m_MemDown.m_segment[i].Nearest(&Outer, &Inner);
			anid->m_NO[1][i - BEGINMEM] = float(lattice[Inner.m_ix][Inner.m_iy].m_dcNO);
		}
		else
		{
			anid->m_NO[1][i - BEGINMEM] = 0;
		}
		anid->m_th[1][i - BEGINMEM] = m_MemDown.m_segment[i].m_bPassTh;
	}
}

int LymphaticVessle::OnMem(int i, int j)
{
	if (SamePoint(i, j, m_MemUp.m_segment[0].m_pst) ||
		SamePoint(i, j, m_MemUp.m_segment[RIGHTVALVE].m_pst) ||
		SamePoint(i, j, m_MemDown.m_segment[0].m_pst) ||
		SamePoint(i, j, m_MemDown.m_segment[RIGHTVALVE].m_pst))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int LymphaticVessle::SamePoint(int i, int j, Vector v)
{
	if (i >= v.m_dx&&i <= v.m_dx&&j >= v.m_dy&&j <= v.m_dy)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void LymphaticVessle::CorrectX()
{
	int i;
	for (i = iEnd_Mem - 1; i <= iEnd_Mem; i++)
	{
		m_MemUp.m_segment[i].m_pst.m_dx = m_MemUp.m_segment[i - 1]
			.m_pst.m_dx + m_MemUp.m_dDeltax;
		m_MemUp.m_segment[i].m_pst_bak.m_dx = m_MemUp.m_segment[i - 1]
			.m_pst_bak.m_dx + m_MemUp.m_dDeltax;
		m_MemDown.m_segment[i].m_pst.m_dx = m_MemDown.m_segment[i - 1]
			.m_pst.m_dx + m_MemDown.m_dDeltax;
		m_MemDown.m_segment[i].m_pst_bak.m_dx = m_MemDown.m_segment[i - 1]
			.m_pst_bak.m_dx + m_MemDown.m_dDeltax;
	}
	m_MemUp.m_segment[iBegin_Mem - 1].m_pst.m_dx = m_MemUp.m_segment[iBegin_Mem]
		.m_pst.m_dx	- m_MemUp.m_dDeltax;
	m_MemUp.m_segment[iBegin_Mem - 1].m_pst_bak.m_dx = m_MemUp.m_segment[iBegin_Mem]
		.m_pst_bak.m_dx	- m_MemUp.m_dDeltax;
	m_MemDown.m_segment[iBegin_Mem - 1].m_pst.m_dx = m_MemDown.m_segment[iBegin_Mem]
		.m_pst.m_dx	- m_MemDown.m_dDeltax;
	m_MemDown.m_segment[iBegin_Mem - 1].m_pst_bak.m_dx = m_MemDown.m_segment[iBegin_Mem]
		.m_pst_bak.m_dx	- m_MemDown.m_dDeltax;
}