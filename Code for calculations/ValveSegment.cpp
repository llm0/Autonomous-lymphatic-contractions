//#include "stdafx.h"
#include "ValveSegment.h"
#include "Common.h"
double ValveSegment::m_dk;
double ValveSegment::m_dm;

ValveSegment::ValveSegment()
{
}


ValveSegment::~ValveSegment()
{
}

void ValveSegment::Init()
{
	int i;
	double dDeltax,dDeltay;
	double dAlpha0;
	dAlpha0 = 20.0 / 360 * PI;
	dDeltax = (H2 - H1) * 2.0 / VALSEGNUM;
	dDeltay = (H2 - H1) * 2.0*tan(dAlpha0) / VALSEGNUM;
	if (m_iType > 0)
	{
		dDeltay = -dDeltay;
	}
	for (i = 0; i < VALSEGNUM; i++)
	{

	}
}

void ValveSegment::ElastAcc(int n1,int n2)//包含3种弹性力：收缩弹性，拉伸弹性，限制弹性，有问题，因为要用到坐标算半径，所以一定要备份坐标以保证并行
{
	double l1, l2,dy01,dy02,dk1,dk2,dHard,dOpen,ff; 
	double dak,dlub;
	Vector dd1, dd2;
	Vector s1, s, s2; 
	int n; 
	dk1 = 0.4;// 0.2;// 0.1;// 0.1;
	dk2 = 0.0000001;// 0.001;// 0.001;
	dHard = 0;// 0.0004; //0.0002;//0;//  0.0008;// 0.001 / L - ;// 0.002 / L;// 0.02 / L;// 0.0002/S;
	dOpen = 2;//1.5;//3;//
	if (m_iType>0)//上面
	{
		if (m_pst.m_dy<m_ym-DELTA)
		{
			dak = -m_dk*(m_pst.m_dy - m_y0) / m_dm;
			if (m_pst.m_dy < CENTER + dOpen)
			{
				dak += -dHard*(m_pst.m_dy - (CENTER + dOpen)) / m_dm;
				if (m_pst.m_dy <= CENTER + DELTA)//abnormal
				{
					if (iClose != 1 && iMyID == iNumProcs - 1)
					{
						iClose = 1;
					}
					dak *= pow(DELTA/ (m_pst.m_dy - CENTER), 11);
					ff = (fabs(m_f.m_dy) + fabs(m_Opposite->m_f.m_dy)) / 2;
					if (ff > 1e-10)//ff非零？？？？？？？？
					{
						if (dak > 2 * ff)//dak>0
						{
							dak = 2 * ff;
						}
					}
				}
			}
		}
		else
		{
			dak= -m_dk*(m_pst.m_dy - m_y0)*pow(DELTA
				/ (m_ym  - m_pst.m_dy), 11) / m_dm;
		}
	}
	else//下面
	{
		if (m_pst.m_dy > m_ym + DELTA)
		{
			dak = -m_dk*(m_pst.m_dy - m_y0) / m_dm;
			if (m_pst.m_dy > CENTER - dOpen)
			{
				dak += -dHard*(m_pst.m_dy - (CENTER - dOpen)) / m_dm;
				if (m_pst.m_dy >= CENTER - DELTA)//normal
				{
					if (iClose != 1 && iMyID == iNumProcs - 1)
					{
						iClose = 1;
					}
					dak *= pow(DELTA / (CENTER - m_pst.m_dy), 11);
					ff = (fabs(m_f.m_dy) + fabs(m_Opposite->m_f.m_dy)) / 2;
					if (ff > 1e-10)//ff非零？？？？？？？
					{
						if (-dak > 2 * ff)//dak<0
						{
							dak = -2 * ff;
						}
					}
				}
			}
		}
		else
		{
			dak = -m_dk*(m_pst.m_dy - m_y0)*pow(DELTA
				/ (m_pst.m_dy - m_ym ), 11) / m_dm;
		}
	}
	m_ka.m_dy += dak;

	l1 = 0; l2 = 0;
	if (m_prev != NULL)//&&m_next1 != NULL)//两头不加弯弹性力，两头只有单边受力
	{
		if (m_GetForce == 0 || m_GetForce != 0 && ((m_prev != NULL) ?
			m_prev->m_GetForce == 0 : 0))
		{
			if (m_id != 1)
			{
				dy01 = m_y0 - ((ValveSegment*)m_prev)->m_y0;
			}
			else
			{
				dy01 = (m_y0 - ((m_iType > 0) ? H2 : H1));
			}
		}
		else
		{
			dy01 = 0;
		}
		l1 = m_pst.m_dy - m_prev->m_pst.m_dy - dy01;
	}
	if (m_next1 != NULL)//&&m_prev != NULL)//两头不加弯弹性力，两头只有单边受力
	{
		if (m_GetForce == 0)
		{
			dy02 = m_y0 - ((ValveSegment*)m_next1)->m_y0;
		}
		else
		{
			dy02 = 0;
		}
		l2 = m_pst.m_dy - m_next1->m_pst.m_dy - dy02;
	}
	m_ka.m_dy += (-kchange(dk1,dk2,(m_id - 1.0))*l1
		- kchange(dk1, dk2, m_id)* l2) / m_dm; //弯弹性
	if (m_id == 1)
	{
		m_prev->m_ka.m_dy += dk1*l1 / m_prev->m_dm;
	}
	if (m_GetForce != 0)//&& (m_prev != NULL&&m_prev->m_GetForce != 0))
	{
		if ((m_iType > 0) ? m_v.m_dy >= 0 : m_v.m_dy > 0)
		{
			n = n1;
		}
		else
		{
			n = n2;
		}
		dlub = -16 * n*n* dDensity*(2 * dTau - 1) / 6.0*m_v.m_dy*
			pow((m_pst.m_dx - m_prev->m_pst.m_dx) / 2
			/ fabs(m_pst.m_dy - m_Opposite->m_pst.m_dy), 3) / m_dm;
		if (fabs(dlub*m_ddT) > fabs(m_v.m_dy))
		{
			dlub = -m_v.m_dy / m_ddT;//阻力要求
		}
		m_ka.m_dy += dlub;
		/*if (iClose != 1&&iMyID==iNumProcs-1)
		{
			iClose = 1;
		}*/
	}
}

double ValveSegment::kchange(double k1, double k2,double x)
{
	//return (k1 - k2) * 2 / (1 + exp(2 * x / (VALSEGNUM - 1))) + k2;
	//return (k1 - k2) * 2 / (1 + exp(3 * x / (VALSEGNUM - 1))) + k2;
	//return (k1 - k2) * 2 / (1 + exp(4 * x / (VALSEGNUM - 1))) + k2;
	//return (k1 - k2) * 2 / (1 + exp(5 * x / (VALSEGNUM - 1))) + k2;
	return (k1 - k2) * 2 / (1 + exp(6 * x / (VALSEGNUM - 1))) + k2;
	//return (k1 - k2) * 2 / (1 + exp(7 * x / (VALSEGNUM - 1))) + k2;
}

void ValveSegment::HydroAcc()
{
	if (m_prev != NULL)
	{
		m_fa.m_dy = (m_f.m_dy + m_prev->m_f.m_dy) / 2.0  / m_dm;//每一点存的是下一段的力，所以要和前一点取平均
	}
	else
	{
		m_fa.m_dy = m_f.m_dy / 2.0 / m_dm ;
	}
	m_fa.m_dx = 0; 
}

void ValveSegment::VescoAcc()
{
	Vector vv, vv1, vv2;
	double dvs;
	dvs = Viscous;// ViscousValve;
	if (m_prev != NULL)
	{
		vv1 = m_v - m_prev->m_v;
	}
	if (m_next1 != NULL)
	{
		vv2 = m_v - m_next1->m_v;
	}
	if (m_id < VALSEGNUM - 2)
	{
		vv = -(vv1 + vv2)*dvs / m_dm;
	}
	else
	{
		if (m_id == VALSEGNUM - 2)
		{
			vv = -(vv1 * dvs + vv2 * dvs *1.0) / m_dm;//提高最后两段的粘性以稳定最后一段
		}
		else
		{
			vv = -vv1 *  dvs*1.0 / m_dm;//提高最后两段的粘性以稳定最后一段
		}
	}
	m_ka += vv;
}

/*void ValveSegment::VescoAcc()
{
	Vector vv, vv1 = Vector(0,0), vv2= Vector(0,0) , a1, a2;
	double dvs;
	dvs = Viscous/100;
	if (m_prev != NULL)
	{
		vv1 = m_v - m_prev->m_v;
	}
	if (m_next1 != NULL)
	{
		vv2 = m_v - m_next1->m_v;
	}
	if (m_id < VALSEGNUM - 1)
	{
		//vv = -(vv1 + vv2)*dvs / m_dm;
		a1 = -vv1*dvs / m_dm;
		a2 = -vv2*dvs / m_dm;
	}
	else
	{
		a1 = -vv1*dvs / m_dm;
		a2 = Vector(0,0);
		//vv = -vv1 *  dvs*1.0 / m_dm;//提高最后两段的粘性以稳定最后一段
	}
	if (fabs(a1.m_dy*m_ddT) > fabs(vv1.m_dy))
	{
		a1 = -vv1/m_ddT;
	}
	if (fabs(a2.m_dy*m_ddT) > fabs(vv2.m_dy))
	{
		a2 = -vv2 / m_ddT;
	}
	vv = a1 + a2;
	m_ka += vv;
}*/