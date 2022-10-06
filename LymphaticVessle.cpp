// LymphaticVessle.cpp: implementation of the LymphaticVessle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LymphaticVessle.h"
#include "math.h"
#include "Common.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LymphaticVessle::LymphaticVessle()
{
}

LymphaticVessle::~LymphaticVessle()
{

}

void LymphaticVessle::ReadForAnimate(fstream* ff)
{
	int i;
	for(i=0;i<SEGNUM;i++)
	{
		*ff>>m_MemUp.m_segment[i].m_pst.m_dx>>m_MemUp.m_segment[i].m_pst.m_dy
			>>m_MemUp.m_segment[i].m_dcCa>>m_MemUp.m_segment[i].m_dcNO>>m_MemUp.m_segment[i].m_bPassTh;
		*ff>>m_MemDown.m_segment[i].m_pst.m_dx>>m_MemDown.m_segment[i].m_pst.m_dy
			>>m_MemDown.m_segment[i].m_dcCa>>m_MemDown.m_segment[i].m_dcNO>>m_MemDown.m_segment[i].m_bPassTh;
	}
}

void LymphaticVessle::SaveForMap(fstream* ff)
{
	int i,j;
	for (j = 0; j < iUnit; j++)
	{
		for (i = 0; i < SEGNUM; i++)
		{
			if (j>0 || i >= LEFTVALVE)
			{
				*ff << pAniData[j].m_lymani.m_p[0][i].m_fx - BEGINROW + j*WIDTH << " " << pAniData[j].m_lymani.m_p[0][i].m_fy << " "
					<< pAniData[j].m_lymani.m_Ca[0][i] << " " << pAniData[j].m_lymani.m_NO[0][i] << endl;
			}
		}
	}
	*ff << endl;
	for (j = 0; j < iUnit; j++)
	{
		for (i = 0; i < SEGNUM; i++)
		{
			if (j>0 || i >= LEFTVALVE)
			{
				*ff << pAniData[j].m_lymani.m_p[1][i].m_fx - BEGINROW + j*WIDTH << " " << pAniData[j].m_lymani.m_p[1][i].m_fy << " "
					<< pAniData[j].m_lymani.m_Ca[1][i] << " " << pAniData[j].m_lymani.m_NO[1][i] << endl;
			}
		}
	}
}

LymphaticVessle& LymphaticVessle::operator=(const LymphAnimate& v)
{
	int i;
	for(i=0;i<SEGNUM;i++)
	{
		m_MemUp.m_segment[i].m_pst=v.m_p[0][i];
		m_MemDown.m_segment[i].m_pst=v.m_p[1][i];
		m_MemUp.m_segment[i].m_dcCa=v.m_Ca[0][i];
		m_MemDown.m_segment[i].m_dcCa=v.m_Ca[1][i];
		m_MemUp.m_segment[i].m_dcNO=v.m_NO[0][i];
		m_MemDown.m_segment[i].m_dcNO=v.m_NO[1][i];
		m_MemUp.m_segment[i].m_bPassTh=v.m_th[0][i];
		m_MemDown.m_segment[i].m_bPassTh=v.m_th[1][i];
	}
	m_MemUp.m_dx1 = m_MemUp.m_segment[0].m_pst.m_dx - BEGINROW;
	m_MemDown.m_dx1 = m_MemDown.m_segment[0].m_pst.m_dx - BEGINROW;
	m_MemUp.m_dy1 = m_MemUp.m_segment[0].m_pst.m_dy;
	m_MemDown.m_dy1 = m_MemDown.m_segment[0].m_pst.m_dy;
	return *this;
}

int LymphaticVessle::IsInner(int ix, int iy)
{
	int m, n;
	if (m_MemUp.Between(ix, &m, &n))//找到位于哪两段膜之间
	{
		if (iy<m_MemUp.m_segment[m].m_pst.m_dy + (m_MemUp.m_segment[n].m_pst.m_dy//上膜的下面
			- m_MemUp.m_segment[m].m_pst.m_dy) / (m_MemUp.m_segment[n].m_pst.m_dx
			- m_MemUp.m_segment[m].m_pst.m_dx)*(ix - m_MemUp.m_segment[m].m_pst.m_dx) &&

			iy>m_MemDown.m_segment[m].m_pst.m_dy + (m_MemDown.m_segment[n].m_pst.m_dy//下膜的上面
			- m_MemDown.m_segment[m].m_pst.m_dy) / (m_MemDown.m_segment[n].m_pst.m_dx
			- m_MemDown.m_segment[m].m_pst.m_dx)*(ix - m_MemDown.m_segment[m].m_pst.m_dx))
		{
			return 1;
		}
		else if (iy > m_MemUp.m_segment[m].m_pst.m_dy + (m_MemUp.m_segment[n].m_pst.m_dy//上膜的上面
			- m_MemUp.m_segment[m].m_pst.m_dy) / (m_MemUp.m_segment[n].m_pst.m_dx
			- m_MemUp.m_segment[m].m_pst.m_dx)*(ix - m_MemUp.m_segment[m].m_pst.m_dx) ||

			iy < m_MemDown.m_segment[m].m_pst.m_dy + (m_MemDown.m_segment[n].m_pst.m_dy//下膜的下面
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
	return -1;
}