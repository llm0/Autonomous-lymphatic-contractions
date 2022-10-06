//#include "stdafx.h"
#include "ValveAnimate.h"


ValveAnimate::ValveAnimate()
{
}


ValveAnimate::~ValveAnimate()
{
}

ValveAnimate& ValveAnimate::operator=(const SolftValve& v)
{
	int i;
	m_lActive = v.m_lActive;
	m_rActive = v.m_rActive;
	for (i = 0; i < VALSEGNUM; i++)
	{
		if (m_lActive)
		{
			m_p[0][i] = v.m_MemDL.m_Seg[i]->m_pst;
			m_p[1][i] = v.m_MemUL.m_Seg[i]->m_pst;
		}
		else
		{
			m_p[0][i] = VectorAnimate(0,0);
			m_p[1][i] = VectorAnimate(0, 0);
		}
		if (m_rActive)
		{
			m_p[2][i] = v.m_MemDR.m_Seg[i]->m_pst;
			m_p[3][i] = v.m_MemUR.m_Seg[i]->m_pst;
		}
		else
		{
			m_p[2][i] = VectorAnimate(0, 0);
			m_p[3][i] = VectorAnimate(0, 0);
		}
	}
	return *this;
}