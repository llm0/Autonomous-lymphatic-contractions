// Membrane.cpp: implementation of the Membrane class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "lymph.h"
#include "Membrane.h"
#include "math.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double Membrane::m_dDeltax;
int* Membrane::m_iPrjx;
int* Membrane::m_iPrjy;
int(*(Membrane::m_iPrjxy))[9];
int* Membrane::m_iRev;
Lattice(*(Membrane::m_lattice))[HEIGHT];
Link Membrane::m_LinkBuffer[MAXLINK];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Membrane::Membrane(double dx1, double dy1, double dx2, double dy2)
{
	m_dx1 = dx1;
	m_dy1 = dy1;
	m_dx2 = dx2;
	m_dy2 = dy2;
}

Membrane::~Membrane()
{

}

MPI_File& operator<<(MPI_File& fs, Membrane& v)//4d+1i+SEGNUM_E*
{
	int i;
	MPI_Status Status;
	MPI_File_write(fs, (char*)&(v.m_dx1), sizeof(v.m_dx1), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_dy1), sizeof(v.m_dy1), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_dx2), sizeof(v.m_dx2), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_dy2), sizeof(v.m_dy2), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_iType), sizeof(v.m_iType), MPI_BYTE, &Status);
	for (i = 0; i < SEGNUM_E; i++)
	{
		fs << v.m_segment[i];
	}
	return fs;
}

MPI_File& operator>>(MPI_File& fs, Membrane& v)
{
	int i;
	MPI_Status Status;
	MPI_File_read(fs, (char*)&(v.m_dx1), sizeof(v.m_dx1), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_dy1), sizeof(v.m_dy1), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_dx2), sizeof(v.m_dx2), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_dy2), sizeof(v.m_dy2), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_iType), sizeof(v.m_iType), MPI_BYTE, &Status);
	for (i = 0; i < SEGNUM_E; i++)
	{
		fs >> v.m_segment[i];
	}
	return fs;
}

void Membrane::Pack(char* buffer, int  size, int * position)
{
	MPI_Pack((char*)&(m_dx1), 4, MPI_DOUBLE, buffer, size, position, MPI_COMM_WORLD);
	MPI_Pack((char*)&(m_iType), 2, MPI_INT, buffer, size, position, MPI_COMM_WORLD);
	MPI_Pack((char*)&(m_segment[0].m_id), SEGNUM_E, One_Mem_data_type, buffer, size, position, MPI_COMM_WORLD);
}

void Membrane::Unpack(char* buffer, int size, int * position)
{
	MPI_Unpack(buffer, size, position, (char*)&(m_dx1), 4, MPI_DOUBLE, MPI_COMM_WORLD);
	MPI_Unpack(buffer, size, position, (char*)&(m_iType), 2, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(buffer, size, position, (char*)&(m_segment[0].m_id), SEGNUM_E, One_Mem_data_type, MPI_COMM_WORLD);
}

int Membrane::GetInt(double x)
{
	return int(x + 0.5);
}

void Membrane::init(int bf)
{
	int i, dDeltax;
	double dDeltay;
	dDeltax = GetInt((m_dx2 - m_dx1) / (iEnd_Mem-1 - iBegin_Mem));
	m_dDeltax = dDeltax;
	Segment::m_dl = dDeltax;
	dDeltay = (m_dy2 - m_dy1) / (iEnd_Mem-1 - iBegin_Mem);
	for (i = 0; i < SEGNUM_E; i++)
	{
		m_segment[i].m_dm = dDeltax*dMemDensity;
	}
	if (!bf)
	{
		m_segment[iBegin_Mem].m_pst.m_dx = m_dx1;
		m_segment[iBegin_Mem].m_pst.m_dy = m_dy1;
		m_segment[0].m_pst.m_dx = 1;
		m_segment[0].m_pst.m_dy = m_dy1;
		for (i = 0; i<SEGNUM_E; i++)
		{
			if (i <= iBegin_Mem&&iMyID == 0)
			{
				m_segment[i].m_prev = NULL;
				m_segment[i].m_bCanMove = 0;
			}
			else if (i != 0)
			{
				m_segment[i].m_pst.m_dx = GetInt(m_segment[i - 1].m_pst.m_dx) + dDeltax;
				m_segment[i].m_prev = &(m_segment[i - 1]);
			}
			if (iMyID == iNumProcs - 1 && i >= SEGNUM - LEFTVALVE)//72/2+1
			{
				if (m_iType>0)
				{
					m_segment[i].m_pst.m_dy = (HEIGHT - 1) / 2.0 + (DCa - 1) / 2.0;
				}
				else
				{
					m_segment[i].m_pst.m_dy = (HEIGHT - 1) / 2.0 - (DCa - 1) / 2.0;
				}
			}
			else
			{
				m_segment[i].m_pst.m_dy = m_segment[0].m_pst.m_dy;
			}
			
			if (iMyID == iNumProcs - 1 && i >= SEGNUM - LEFTVALVE)//72/2+1
			{
				m_segment[i].m_bCanMove = 0;
			}
			else if (!(iMyID == 0 && i <= iBegin_Mem))
			{
				m_segment[i].m_bCanMove = 1;
			}
			m_segment[i].m_id = i;
			m_segment[i].m_iType = m_iType;
			m_segment[i].m_dcCa = Ca0;
			m_segment[i].m_bPassTh = 0;
			if (i >= iEnd_Mem - 1&&iMyID == iNumProcs-1)
			{
				m_segment[i].m_next = NULL;//最后一段没有后
			}
			else if (i!=SEGNUM_E-1)
			{
				m_segment[i].m_next = &(m_segment[i + 1]);
			}	
		}
	}
	else
	{
		for (i = 0; i < SEGNUM_E; i++)
		{
			//m_segment[i].m_dcCa = LIMITCA;
			m_segment[i].m_iType = m_iType;
			if (i == iBegin_Mem&&iMyID == 0)
			{
				m_segment[i].m_prev = NULL;
			}
			else if (i > 0)
			{
				m_segment[i].m_prev = &(m_segment[i - 1]);
			}
			if (i == iEnd_Mem - 1 && iMyID == iNumProcs - 1)
			{
				m_segment[i].m_next = NULL;
			}
			else if (i < SEGNUM_E - 1)
			{
				m_segment[i].m_next = &(m_segment[i + 1]);
			}
		}
	}
	m_segment[0].m_prev = NULL;//最前一段没有前
	m_segment[SEGNUM_E - 1].m_next = NULL;
}

void Membrane::FindLink()//注意两个瓣只能用线段法找链
{
	int i, j, L, m, iLink;
	int iBegin, iFlux, iFluy;
	Vector p;
	double dd, dd1;
	dd = 10;
	int iNearest = 0;
	Angle();
	m_ixn = int(m_segment[iBegin_Mem].m_pst.m_dx); m_ixm = int(m_segment[iBegin_Mem].m_pst.m_dx); m_iyn = int(m_segment[iBegin_Mem].m_pst.m_dy); m_iym = int(m_segment[iBegin_Mem].m_pst.m_dy);
	if (iMyID == 0)
	{
		iBegin = iBegin_Mem + 1;
	}
	else
	{
		iBegin = iBegin_Mem;
	}
	for (i = iBegin; i<iEnd_Mem; i++)
	{
		if (m_ixn>m_segment[i].m_pst.m_dx)
		{
			m_ixn = int(m_segment[i].m_pst.m_dx);
		}
		else if (m_ixm<m_segment[i].m_pst.m_dx)
		{
			m_ixm = int(m_segment[i].m_pst.m_dx + 0.5);
		}
		if (m_iyn>m_segment[i].m_pst.m_dy)
		{
			m_iyn = int(m_segment[i].m_pst.m_dy);
		}
		else if (m_iym < m_segment[i].m_pst.m_dy)
		{
			m_iym = int(m_segment[i].m_pst.m_dy + 0.5);
		}
	}
	m_ixn--; m_ixm++; m_iyn--; m_iym++;
	iLink = 0;
	for (i = m_ixn; i < m_ixm; i++)
	{
		for (j = m_iyn; j < m_iym; j++)
		{
			for (m = 1; m < 9; m++)//m的方向由固体指向流体
			{
				iFlux = i + m_iPrjx[m];  //fluid node -1~n
				iFluy = j + m_iPrjy[m];  //iFluy -1~n
				if (iFlux >= 0 && iFlux < iEnd_Row&&iFluy >= 0 && iFluy < HEIGHT)
				{
					if (m_lattice[i][j].m_iSol >= -7 && m_lattice[i][j].m_iSol <= -6 && m_lattice[iFlux][iFluy].m_iSol <= -8) //Boundary
					{
						m_link[iLink].m_iSolx = i;
						m_link[iLink].m_iSoly = j;
						m_link[iLink].m_iFlux = iFlux;
						m_link[iLink].m_iFluy = iFluy;
						m_link[iLink].m_iDir = m_iRev[m];//调整为从流体指向固体
						m_lattice[i][j].m_iSol = -7;
						m_lattice[iFlux][iFluy].m_iSol = -8;
						for (L = iBegin_Mem; L < iEnd_Mem; L++)
						{
							dd1 = (i - m_segment[L].m_pst.m_dx)*(i - m_segment[L].m_pst.m_dx)
								+ (j - m_segment[L].m_pst.m_dy)*(j - m_segment[L].m_pst.m_dy);//求出最近固体点的段
							if (dd1 < dd)
							{
								iNearest = m_segment[L].m_id;
								dd = dd1;
							}
							if (m_segment[L].IsCross(Vector(iFlux, iFluy), Vector(i, j), &p))
							{
								m_link[iLink].m_bf = p;
								m_link[iLink].m_iSegf = L;
								if (m_link[iLink].m_iDir != 3 && m_link[iLink].m_iDir != 4)//非竖直方向
								{
									m_link[iLink].m_dDlf = fabs(m_link[iLink].m_bf.m_dx - m_link[iLink].m_iFlux);
								}
								else
								{
									m_link[iLink].m_dDlf = fabs(m_link[iLink].m_bf.m_dy - m_link[iLink].m_iFluy);
								}
								break;
							}
							if (L == iEnd_Mem - 1)//固体点在段上端点，实数无法判断
							{
								if (dd < 0.01)//在段上端点，距离应该很小
								{
									m_link[iLink].m_bf = Vector(i, j);
									m_link[iLink].m_iSegf = iNearest;
									m_link[iLink].m_dDlf = 1;//边界刚好在固体点上;
								}
								else
								{
									printf("找交点出错");
								}
							}
						}//for l
						iLink++;
						if (iLink > LINKNUM)
						{
							printf("细胞链数超过了设定值");
						}
					}
				}
			}//for m
		}
	}
	m_il = iLink;
}

int Membrane::Between(int ix, int *i, int *j)
{
	int k, m; 
	k = int((ix - m_pMemUp->m_dx1) / Membrane::m_dDeltax + 0.5) + iBegin_Mem;//找到第几段膜
	if (ix < m_pMemUp->m_dx1) return 0;
	for (m =((k - 2) >= iBegin_Mem) ? k - 1 : iBegin_Mem ; m <= ((k + 2 < iEnd_Mem) ? k + 2 : iEnd_Mem - 1) ; m++)//在k前后两个段的范围内找
	{
		if (ix >= m_dx1 + (m - iBegin_Mem)*m_dDeltax&&ix <= m_dx1 + (m + 1 - iBegin_Mem)*m_dDeltax)
		{
			if (m < ((iMyID == iNumProcs - 1) ? iEnd_Mem - 1 : iEnd_Mem))
			{
				*i = m; *j = m + 1;
			}
			else
			{
				*i = m - 1; *j = m;
			}
			return 1;
		}
	}
	return 0;
}

void Membrane::Angle()
{
	int i;
	for (i = 0; i < SEGNUM_E; i++)
	{
		m_segment[i].Angle();
	}
}

void Membrane::CalciumForce()
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		if (m_segment[i].m_bCanMove)
		{
			m_segment[i].CalciumForce();
		}
	}
}

void Membrane::BackupPosition()
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		m_segment[i].m_pst_bak = m_segment[i].m_pst;
	}
}

void Membrane::ReactionCa(double *dCaBuff)
{
	int i;
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		if (m_segment[i].m_bCanMove)
		{
			m_segment[i].ReactionCa(&(dCaBuff[i]));
		}
	}
}

void Membrane::ReactionNO(double rr)
{
	int i;
	for (i = iBegin_Mem; i<iEnd_Mem; i++)
	{
		m_segment[i].ReactionNO(rr);
	}
}
double Membrane::VirticalDistance(int i, int j)
{
	int m, ix, ii;
	double dely;
	ix = int((i - m_dx1)*(iEnd_Mem - iBegin_Mem - 1) / (m_dx2 - m_dx1));
	ii = (ix - 1>0) ? ix - 1 : 0;
	for (m = ii; m < ix + 1; m++)
	{
		if (m < iEnd_Mem - 1)
		{
			if (i >= m_segment[m].m_pst.m_dx&&i <= m_segment[m + 1].m_pst.m_dx)
			{
				break;
			}
		}
	}
	dely = m_segment[m].m_pst.m_dy + (m_segment[m + 1].m_pst.m_dy - m_segment[m].m_pst.m_dy) /
		(m_segment[m + 1].m_pst.m_dx - m_segment[m].m_pst.m_dx)*(i - m_segment[m].m_pst.m_dx) - j;
	return fabs(dely);
}

void Membrane::CalciumSpread(double *dCaBuff)
{
	int i;

	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		if (m_segment[i - 1].m_bCanMove&&m_segment[i + 1].m_bCanMove)
		{
			dCaBuff[i] += dSpCa*(m_segment[i - 1].m_dcCa - 2 * m_segment[i].m_dcCa + m_segment[i + 1].m_dcCa);
		}
		else if ((!m_segment[i - 1].m_bCanMove) && m_segment[i].m_bCanMove)
		{
			dCaBuff[i] += dSpCa*(m_segment[i + 1].m_dcCa - m_segment[i].m_dcCa);
		}
		else if (m_segment[i].m_bCanMove && (!m_segment[i + 1].m_bCanMove))
		{
			dCaBuff[i] += dSpCa*(m_segment[i - 1].m_dcCa - m_segment[i].m_dcCa);
		}
	}
	for (i = iBegin_Mem; i < iEnd_Mem; i++)
	{
		m_segment[i].m_dGrowth = dCaBuff[i];
		m_segment[i].m_dcCa += dCaBuff[i];
	}
}


void Membrane::FindNbLattice()
{
	/*	int i;
		BoundaryLattice Outer,Inner
		for(i=0;i<SEGNUM;i++)
		{
		Outer.m_dx=m_segment[i].m_pst.m_dx;Outer.m_dy=m_segment[i].m_pst.m_dy;
		Nearest(&Outer,&Inner);
		if(m_lattice[Inner.m_ix][Inner.m_iy].m_n1<0)
		{
		m_lattice[Inner.m_ix][Inner.m_iy].m_n1=i;
		}
		else
		{
		m_lattice[Inner.m_ix][Inner.m_iy].m_n2=i;
		}

		}*/
}