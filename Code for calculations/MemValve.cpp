//#include "stdafx.h"
#include "MemValve.h"
#include "Common.h"

double MemValve::m_dDeltax;
MemValve::MemValve()
{
}


MemValve::~MemValve()
{
}

MPI_File& operator<<(MPI_File& fs, MemValve& v)//(VALSEGNUM-1)*
{
	int i;
	for (i = 1; i < VALSEGNUM; i++)
	{
		fs <<* v.m_Seg[i];
	}
	return fs;
}

MPI_File& operator>>(MPI_File& fs, MemValve& v)
{
	int i;
	for (i = 1; i < VALSEGNUM; i++)
	{
		fs >> *v.m_Seg[i];
	}
	return fs;
}

void MemValve::InitMem()
{
	int i;
	for (i = 1; i < VALSEGNUM; i++)
	{
		m_Seg[i] = m_SegData + i - 1;
	}
}

void MemValve::Init(int bFromFile)//注意 m_Seg[0]是segment类型，不要调用ValeSegment的东西
{
	int i;
	double dDeltax;
	dDeltax = (H2 - H1) * 2.0 / VALSEGNUM;
	for (i = 1; i < VALSEGNUM; i++)
	{
		m_Seg[i]->m_y0 = y0(i, m_iType, dDeltax);
		if (!bFromFile)
		{
			m_Seg[i]->m_pst = Vector(m_Seg[0]->m_pst.m_dx + i*dDeltax, m_Seg[i]->m_y0);
		}
		m_Seg[i]->m_ym = yLimit(i, m_iType);
		m_Seg[i]->m_iType = m_iType;
		m_Seg[i]->m_id = i;
	}//Show(m_Seg[VALSEGNUM - 1]->m_y0, "a");
	if (m_Seg[VALSEGNUM - 1]->m_pst.m_dx>iEnd_Row - 1) printf("Valve 突出去了");
	if (m_iType > 0 && m_Seg[VALSEGNUM - 1]->m_y0 <= CENTER) printf("平衡位置太靠中间");
	for (i = 0; i < VALSEGNUM; i++)
	{
		if (i != VALSEGNUM - 1)
		{
			m_Seg[i]->m_next1 = m_Seg[i + 1];
		}
		else
		{
			m_Seg[i]->m_next1 = NULL;
		}
		if (i == 0)
		{
			m_Seg[i]->m_prev = NULL;
		}
		else
		{
			m_Seg[i]->m_prev = m_Seg[i - 1];
		}
	}
}


double MemValve::y0(int i, int iType, double dDeltax)
{
	if (iType > 0)
	{
		//return H2 - sqrt(i*dDeltax / 0.6); //0.97);0.35close);//
		return H2 - sqrt(i*dDeltax / 0.6);
		//return H2 - 0.15*i*dDeltax;
	}
	else
	{
		//return H1 + sqrt(i*dDeltax / 0.6);// 0.97);0.35close);//
		return H1 + sqrt(i*dDeltax / 0.6);
		//return H1 + 0.15*i*dDeltax;
	}
}
double MemValve::yLimit(int i, int iType)
{
	double dfirst;
	if (iType > 0)
	{
		dfirst = (m_Seg[0]->m_pst.m_dy > H2) ? m_Seg[0]->m_pst.m_dy : H2;
	}
	else
	{
		dfirst = (m_Seg[0]->m_pst.m_dy < H1) ? m_Seg[0]->m_pst.m_dy : H1;
	}
	if (iType > 0)
	{
		//return dfirst - sqrt((m_Seg[i]->m_pst.m_dx - dfirst) / 0.8);
		return H2 - sqrt((m_Seg[i]->m_pst.m_dx - m_Seg[0]->m_pst.m_dx) / 0.8);
	}
	else
	{
		//return dfirst + sqrt((m_Seg[i]->m_pst.m_dx - dfirst) / 0.8);
		return H1 + sqrt((m_Seg[i]->m_pst.m_dx - m_Seg[0]->m_pst.m_dx) / 0.8);
	}
}


int MemValve::OnMem(int i, int j)
{
	if (SamePoint(i, j, m_Seg[0]->m_pst))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int  MemValve::SamePoint(int i, int j, Vector v)
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

void  MemValve::Angle()
{
	int i;
	for (i = 0; i < VALSEGNUM-1; i++)
	{
		m_Seg[i]->m_da1 = atan2(m_Seg[i+1]->m_pst.m_dy
			- m_Seg[i]->m_pst.m_dy, m_Seg[i+1]->m_pst.m_dx
			- m_Seg[i]->m_pst.m_dx);
	}
}


void MemValve::Force()
{
	int i;
	for (i = 1; i<VALSEGNUM; i++)//最后一段不能算,第一段也不算
	{
		m_Seg[i]->Force(1); 
	}

}

void MemValve::HydroAcc()
{
	int i;
	for (i = 1; i<VALSEGNUM; i++)//第一段不动，第一段也不算
	{
		m_Seg[i]->HydroAcc();
	}
}

void MemValve::Move(int j)
{
	int i;
	for (i = 1; i<VALSEGNUM; i++)//第一段在膜上
	{
		m_Seg[i]->Move(j);
	}
}

void MemValve::ElastAcc(int j,int n1,int n2)
{
	int i;
	for (i = 1; i<VALSEGNUM; i++)//第一段不动
	{
		m_Seg[i]->m_ka = Vector(0, 0);
	}
	for (i = 0; i<VALSEGNUM; i++)
	{
		m_Seg[i]->ElastAcc(n1,n2);
	}
}

void MemValve::VescoAcc(int j)
{
	int i;
	for (i = 0; i<VALSEGNUM; i++)
	{
		m_Seg[i]->VescoAcc();
	}
}

void MemValve::Update()
{
	int i;
	for (i = 1; i < VALSEGNUM; i++)
	{
		//m_Seg[i]->m_y0 = y0(i, m_iType, m_dDeltax);
		m_Seg[i]->m_ym = yLimit(i, m_iType);
	}
}

void MemValve::LimitFirstPoint()
{
	if (m_iType>0)
	{
		if (m_Seg[0]->m_pst.m_dy>H2)//&&m_Seg[0]->m_pst.m_dy<H2+DELTA)
		{
			/*m_Seg[0]->m_ka.m_dy += -m_Seg[1]->m_dk*(m_Seg[0]->m_pst.m_dy - H2)*pow(DELTA
			/ (H2+DELTA - m_Seg[0]->m_pst.m_dy), 11) / m_Seg[0]->m_dm;*/
			if (m_Seg[0]->m_v.m_dy>0)
			{
				m_Seg[0]->m_ka.m_dy += -2 * m_Seg[0]->m_v.m_dy*iNewton;
			}
		}
	}
	else
	{
		if (m_Seg[0]->m_pst.m_dy < H1)// && m_Seg[0]->m_pst.m_dy >H1 - DELTA / 2)
		{
			/*m_Seg[0]->m_ka.m_dy += -m_Seg[1]->m_dk*(m_Seg[0]->m_pst.m_dy - H1)*pow(DELTA
			/ (m_Seg[0]->m_pst.m_dy-(H1-DELTA)), 11) / m_Seg[0]->m_dm;*/
			if (m_Seg[0]->m_v.m_dy<0)
			{
				m_Seg[0]->m_ka.m_dy += -2 * m_Seg[0]->m_v.m_dy*iNewton;
			}
		}
	}
}