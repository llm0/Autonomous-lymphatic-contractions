#include "stdafx.h"
#include "ValveAnimate.h"


ValveAnimate::ValveAnimate()
{
}


ValveAnimate::~ValveAnimate()
{
}

void ValveAnimate::SaveForMap(fstream* ff)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		if (!m_lActive&&i <= 1)
		{
			continue;
		}
		else if (!m_rActive&&i >= 2)
		{
			continue;
		}
		for(j=0;j<VALSEGNUM;j++)
		{
			*ff<<m_p[i][j].m_fx<<" "<<m_p[i][j].m_fy<<endl;
		}
		*ff<<endl;
	}
}

/*ValveAnimate& ValveAnimate::operator=(const Valve& v)
{
	m_Petal[0] = v.m_PetalDL;
	m_Petal[1] = v.m_PetalDR;
	m_Petal[2] = v.m_PetalUL;
	m_Petal[3] = v.m_PetalUR;
	return *this;
}*/