//#include "stdafx.h"
#include "SolftValve.h"
#include "Common.h"
Lattice(*SolftValve::m_lattice)[HEIGHT];
int* SolftValve::m_iPrjx;
int* SolftValve::m_iPrjy;
int(*(SolftValve::m_iPrjxy))[9];
int* SolftValve::m_iRev;
double* SolftValve::m_dCoe;
double SolftValve::m_dTau;
double SolftValve::m_dDensity;
double SolftValve::m_kb = 0.01;
double SolftValve::m_dDeltax;

SolftValve::SolftValve()
{
	m_SegDL0 = (Segment**)&(m_MemDL.m_Seg[0]);//m_Seg[0]的地址转存
	m_SegUL0 = (Segment**)&(m_MemUL.m_Seg[0]);
	m_SegDR0 = (Segment**)&(m_MemDR.m_Seg[0]);
	m_SegUR0 = (Segment**)&(m_MemUR.m_Seg[0]);
	m_lActive = 1;
	m_rActive = 1;
	InitMem();
}

SolftValve::~SolftValve()
{
}

void SolftValve::InitMem()
{
	m_MemDL.InitMem();
	m_MemUL.InitMem();
	m_MemDR.InitMem();
	m_MemUR.InitMem();
}

void SolftValve::Init(int bFromFile)
{
	int i;
	if (m_lActive)
	{
		m_MemDL.m_iType = -2;
		m_MemUL.m_iType = 2;
		m_MemDL.Init(bFromFile);
		m_MemUL.Init(bFromFile);
	}
	if (m_rActive)
	{
		m_MemDR.m_iType = -3;
		m_MemUR.m_iType = 3;
		m_MemDR.Init(bFromFile);
		m_MemUR.Init(bFromFile);
	}
	for (i = 1; i < VALSEGNUM; i++)
	{
		if (m_lActive)
		{
			m_MemDL.m_Seg[i]->m_Opposite = m_MemUL.m_Seg[i];
			m_MemUL.m_Seg[i]->m_Opposite = m_MemDL.m_Seg[i];
		}
		if (m_rActive)
		{
			m_MemDR.m_Seg[i]->m_Opposite = m_MemUR.m_Seg[i];
			m_MemUR.m_Seg[i]->m_Opposite = m_MemDR.m_Seg[i];
		}
	}
	if (m_lActive)
	{
		m_dDeltax = m_MemDL.m_Seg[1]->m_pst.m_dx - m_MemDL.m_Seg[0]->m_pst.m_dx;
	}
	else if (m_rActive)
	{
		m_dDeltax = m_MemDR.m_Seg[1]->m_pst.m_dx - m_MemDR.m_Seg[0]->m_pst.m_dx;
	}
	else
	{
		m_dDeltax = 0;
	}
	ValveSegment::m_dm = m_dDeltax*dValveMemDensity;
}

void SolftValve::FirstDrawValve()//on:4,in:-3,out:-1
{
	int i, j, m;
	int iInner;
	double dx1[2], dx2[2];
	if (m_lActive)
	{
		dx1[0] = m_MemUL.m_Seg[0]->m_pst.m_dx; dx2[0] = m_MemUL.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	if (m_rActive)
	{
		dx1[1] = m_MemUR.m_Seg[0]->m_pst.m_dx; dx2[1] = m_MemUR.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	for (m = 0; m < 2; m++)//two valve
	{
		if (!m_lActive&&m == 0)//左无效
		{
			continue;
		}
		if (!m_rActive&&m == 1)//右无效
		{
			continue;
		}
		for (i = int(dx1[m]) - 1; i < ((int(dx2[m]) + 1 < WIDTH - 1) ? int(dx2[m]) + 1 : WIDTH - 1); i++)//最左边点留给了瓣膜
		{
			for (j = 1; j < HEIGHT - 1; j++)
			{
				iInner = IsInner(i, j);
				if (iInner == 1)//在两段膜之间
				{
					m_lattice[i][j].m_iSol = -3;
				}
				else if (iInner == 0)//膜上的点
				{
					m_lattice[i][j].m_iSol = 4;
					m_lattice[i][j].m_v = Vector(0, 0);
				}
			}
		}
	}
}

void SolftValve::DrawValve()//on:4,in:-3,out:-1
{
	int i, j, m;
	int iInner;
	double dx1[2], dx2[2];
	if (m_lActive)
	{
		dx1[0] = m_MemUL.m_Seg[0]->m_pst.m_dx; dx2[0] = m_MemUL.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	if (m_rActive)
	{
		dx1[1] = m_MemUR.m_Seg[0]->m_pst.m_dx; dx2[1] = m_MemUR.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	for (m = 0; m < 2; m++)//two valve
	{
		if (!m_lActive&&m == 0)//左无效
		{
			continue;
		}
		if (!m_rActive&&m == 1)//右无效
		{
			continue;
		}
		for (i = int(dx1[m]) - 1; i < ((int(dx2[m]) + 1 < WIDTH - 1) ? int(dx2[m]) + 1 : WIDTH - 1); i++)//最左边点留给了瓣膜
		{
			for (j = 1; j < HEIGHT - 1; j++)
			{
				iInner = IsInner(i, j);
				if (iInner == 1)//在两段膜之间
				{
					if (m_lattice[i][j].m_iSol == -1 || m_lattice[i][j].m_iSol == 4)
					{
						m_lattice[i][j].m_iSol = 3;//新出现内点，要由内点外推
					}
				}
				else if (iInner == -1 && i >= dx1[m] && i <= dx2[m] && (m_lattice[i][j].m_iSol == -3 ||
					m_lattice[i][j].m_iSol == 4))
				{
					m_lattice[i][j].m_iSol = 1;//新出现外点，要由外点外推
				}
				else if (iInner == 0)//膜上的点
				{
					m_lattice[i][j].m_iSol = 4;
					m_lattice[i][j].m_v = Vector(0, 0);
				}
			}
		}
	}
}

void SolftValve::Remark()
{
	int i, j, m;
	int iInner;
	double dx1[2], dx2[2];
	if (m_lActive)
	{
		dx1[0] = m_MemUL.m_Seg[0]->m_pst.m_dx; dx2[0] = m_MemUL.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	if (m_rActive)
	{
		dx1[1] = m_MemUR.m_Seg[0]->m_pst.m_dx; dx2[1] = m_MemUR.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	for (m = 0; m < 2; m++)//two valve
	{
		if (!m_lActive&&m == 0)//左无效
		{
			continue;
		}
		if (!m_rActive&&m == 1)//右无效
		{
			continue;
		}
		for (i = int(dx1[m]) - 1; i < ((int(dx2[m]) + 1 < WIDTH - 1) ? int(dx2[m]) + 1 : WIDTH - 1); i++)//最左边点留给了瓣膜
		{
			for (j = 1; j < HEIGHT - 1; j++)
			{
				iInner = IsInner(i, j);
				if (iInner == 1)//在两段膜之间
				{
					if (m_lattice[i][j].m_iSol == 1)
					{
						m_lattice[i][j].m_iSol = -3;
					}
				}
				else if (i <= dx2[m] && i >= dx1[m] && m_lattice[i][j].m_iSol == 3)
				{
					m_lattice[i][j].m_iSol = -1;
				}
			}
		}
	}
}

int SolftValve::IsInner(int ix, int iy)
{
	int m, n;
	double dx1, dx2;
	ValveSegment ** SegU, **SegD;
	if (ix >= m_MemDL.m_Seg[0]->m_pst.m_dx&&ix <= m_MemDL.m_Seg[VALSEGNUM - 1]->m_pst.m_dx)
	{
		dx1 = m_MemDL.m_Seg[0]->m_pst.m_dx;
		dx2 = m_MemDL.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
		SegU = m_MemUL.m_Seg;
		SegD = m_MemDL.m_Seg;
	}
	else if (ix >= m_MemDR.m_Seg[0]->m_pst.m_dx&&ix <= m_MemDR.m_Seg[VALSEGNUM - 1]->m_pst.m_dx)
	{
		dx1 = m_MemDR.m_Seg[0]->m_pst.m_dx;
		dx2 = m_MemDR.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
		SegU = m_MemUR.m_Seg;
		SegD = m_MemDR.m_Seg;
	}
	if (Between(ix, &m, &n))//找到位于哪两段膜之间
	{
		if (ix >= dx1&&ix <= dx2&&ix != WIDTH - 1)//必须在膜的左右之间
		{
			if (iy<SegU[m]->m_pst.m_dy + (SegU[n]->m_pst.m_dy//上膜的下面
				- SegU[m]->m_pst.m_dy) / (SegU[n]->m_pst.m_dx
				- SegU[m]->m_pst.m_dx)*(ix - SegU[m]->m_pst.m_dx) &&

				iy>SegD[m]->m_pst.m_dy + (SegD[n]->m_pst.m_dy//下膜的上面
				- SegD[m]->m_pst.m_dy) / (SegD[n]->m_pst.m_dx
				- SegD[m]->m_pst.m_dx)*(ix - SegD[m]->m_pst.m_dx))
			{
				return 1;
			}
			else if (iy > SegU[m]->m_pst.m_dy + (SegU[n]->m_pst.m_dy//上膜的下面
				- SegU[m]->m_pst.m_dy) / (SegU[n]->m_pst.m_dx
				- SegU[m]->m_pst.m_dx)*(ix - SegU[m]->m_pst.m_dx) ||

				iy < SegD[m]->m_pst.m_dy + (SegD[n]->m_pst.m_dy//下膜的上面
				- SegD[m]->m_pst.m_dy) / (SegD[n]->m_pst.m_dx
				- SegD[m]->m_pst.m_dx)*(ix - SegD[m]->m_pst.m_dx))
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

int SolftValve::Between(int ix, int *i, int *j)
{
	int k, m, n;
	double dx1, dx2;
	if (ix >= m_MemDL.m_Seg[0]->m_pst.m_dx&&ix <= m_MemDL.m_Seg[VALSEGNUM - 1]->m_pst.m_dx)
	{
		dx1 = m_MemDL.m_Seg[0]->m_pst.m_dx;
		dx2 = m_MemDL.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	else if (ix >= m_MemDR.m_Seg[0]->m_pst.m_dx&&ix <= m_MemDR.m_Seg[VALSEGNUM - 1]->m_pst.m_dx)
	{
		dx1 = m_MemDR.m_Seg[0]->m_pst.m_dx;
		dx2 = m_MemDR.m_Seg[VALSEGNUM - 1]->m_pst.m_dx;
	}
	else
	{
		return 0;
	}
	k = int((ix - dx1) / m_dDeltax + 0.5);//找到第几段膜
	n = (k + 2 < VALSEGNUM) ? k + 2 : VALSEGNUM - 1;
	for (m = ((k - 2) >= 0) ? k - 1 : 0; m <= n; m++)//在k前后两个段的范围内找
	{
		if (ix >= dx1 + m*m_dDeltax&&ix <= dx1 + (m + 1)*m_dDeltax)
		{
			if (m < VALSEGNUM - 1)
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


void SolftValve::FindLink(ValveSegment** su, ValveSegment**sd)
{
	int i, j, L, m, n, idel;
	int iFlux, iFluy;
	Vector p;
	double dd, dd1;
	int iNearest = 0;
	idel = int(1.0 / m_dDeltax) + 1;
	m_ixn = int(su[0]->m_pst.m_dx); m_ixm = int(su[0]->m_pst.m_dx);
	m_iyn = int(su[0]->m_pst.m_dy); m_iym = int(su[0]->m_pst.m_dy);
	for (i = 0; i<VALSEGNUM; i++)//找出整段膜所在区域
	{
		if (m_ixn>su[i]->m_pst.m_dx)//找上膜所在范围
		{
			m_ixn = int(su[i]->m_pst.m_dx);
		}
		else if (m_ixm<su[i]->m_pst.m_dx)
		{
			m_ixm = int(su[i]->m_pst.m_dx + 0.5);
		}
		if (m_iyn>su[i]->m_pst.m_dy)
		{
			m_iyn = int(su[i]->m_pst.m_dy);
		}
		else if (m_iym<su[i]->m_pst.m_dy)
		{
			m_iym = int(su[i]->m_pst.m_dy + 0.5);
		}

		if (m_ixn>sd[i]->m_pst.m_dx)//找下膜所在范围
		{
			m_ixn = int(sd[i]->m_pst.m_dx);
		}
		else if (m_ixm<sd[i]->m_pst.m_dx)
		{
			m_ixm = int(sd[i]->m_pst.m_dx + 0.5);
		}
		if (m_iyn>sd[i]->m_pst.m_dy)
		{
			m_iyn = int(sd[i]->m_pst.m_dy);
		}
		else if (m_iym < sd[i]->m_pst.m_dy)
		{
			m_iym = int(sd[i]->m_pst.m_dy + 0.5);
		}
	}
	m_ixn--; m_ixm = (m_ixm + 1) < WIDTH ? m_ixm + 1 : WIDTH - 1; m_iyn--; m_iym++;//区域扩大
	for (i = m_ixn; i <= m_ixm; i++)//扫描区域
	{
		for (j = m_iyn; j <= m_iym; j++)//扫描区域
		{
			if (m_lattice[i][j].m_iSol == -3 ||
				(m_lattice[i][j].m_iSol == 4 && !OnMem(i, j))
				|| m_lattice[i][j].m_iSol == -1 && j == int(CENTER))
			{
				for (m = 1; m < 9; m++)//m的方向由固体指向流体
				{
					iFlux = i + m_iPrjx[m];  //fluid node -1~n
					iFluy = j + m_iPrjy[m];  //iFluy -1~n
					if (i < su[0]->m_pst.m_dx || iFlux < su[0]->m_pst.m_dx
						|| i > su[VALSEGNUM - 1]->m_pst.m_dx || iFlux > su[VALSEGNUM - 1]->m_pst.m_dx)
					{
						continue;
					}
					if (InRegionX(iFlux) && iFluy >= 0 && iFluy < HEIGHT)
					{
						if (m_lattice[i][j].m_iSol == -3 && m_lattice[iFlux][iFluy].m_iSol == -1
							|| m_lattice[i][j].m_iSol == 4 && m_lattice[iFlux][iFluy].m_iSol < 0
							|| (m_lattice[i][j].m_iSol == -1 && m_lattice[iFlux][iFluy].m_iSol == -1
							&& j == int(CENTER) && iFluy == int(CENTER) + 1)
							&& i <= su[VALSEGNUM - 1]->m_pst.m_dx
							&&iFlux < su[VALSEGNUM - 1]->m_pst.m_dx) //里面是固体，外面是流体，由管内指向管外
						{
							m_link[m_il].m_iSolx = i;
							m_link[m_il].m_iSoly = j;
							m_link[m_il].m_iFlux = iFlux;
							m_link[m_il].m_iFluy = iFluy;
							m_link[m_il].m_iDir = m_iRev[m];//调整为从流体指向固体
							if (m_lattice[i][j].m_iSol == -1)
							{
								m_link[m_il].m_bdb = 1;
								m_link[m_il].m_iSegs = -1;
							}
							else
							{
								m_link[m_il].m_bdb = 0;
							}
							m_link[m_il].m_iSegf = -1;
							dd = 10;
							for (L = 0; L < VALSEGNUM; L++)
							{
								if (iFluy >= CENTER)//因固体点在里面，只能用流体点判断
								{
									dd1 = (i - su[L]->m_pst.m_dx)*(i - su[L]->m_pst.m_dx)
										+ (j - su[L]->m_pst.m_dy)*(j - su[L]->m_pst.m_dy);//求出最近固体点的段
									if (dd1 < dd)
									{
										iNearest = su[L]->m_id;
										dd = dd1;
									}
									if (su[L]->IsCross(Vector(iFlux, iFluy), Vector(i, j), &p, 1))
									{
										m_link[m_il].m_bf = p;
										m_link[m_il].m_iSegf = L;
										m_link[m_il].m_iType = su[L]->m_iType;
										if (m_link[m_il].m_bdb)
										{
											for (n = ((L - idel > 0) ? L - idel : 0); n < ((L + idel < VALSEGNUM) ? L + idel : VALSEGNUM - 1); n++)
											{
												if (sd[n]->IsCross(Vector(iFlux, iFluy), Vector(i, j), &p, 1))
												{
													m_link[m_il].m_bs = p;
													m_link[m_il].m_iSegs = n;
													break;
												}
											}
											if (m_link[m_il].m_iSegs == -1)
											{
												m_link[m_il].m_bdb = 0;//只穿过一张膜
											}
										}
										if (m_link[m_il].m_iDir != 3 && m_link[m_il].m_iDir != 4)//非竖直方向
										{
											m_link[m_il].m_dDlf = fabs(m_link[m_il].m_bf.m_dx - m_link[m_il].m_iFlux);
											if (m_link[m_il].m_bdb)
											{
												m_link[m_il].m_dDls = fabs(m_link[m_il].m_bs.m_dx - m_link[m_il].m_iSolx);
											}
										}
										else
										{
											m_link[m_il].m_dDlf = fabs(m_link[m_il].m_bf.m_dy - m_link[m_il].m_iFluy);
											if (m_link[m_il].m_bdb)
											{
												m_link[m_il].m_dDls = fabs(m_link[m_il].m_bs.m_dy - m_link[m_il].m_iSoly);
											}
										}
										break;
									}
								}
								else
								{
									dd1 = (i - sd[L]->m_pst.m_dx)*(i - sd[L]->m_pst.m_dx)
										+ (j - sd[L]->m_pst.m_dy)*(j - sd[L]->m_pst.m_dy);//求出最近固体点的段
									if (dd1 < dd)
									{
										iNearest = sd[L]->m_id;
										dd = dd1;
									}
									if (sd[L]->IsCross(Vector(iFlux, iFluy), Vector(i, j), &p, 1))
									{
										m_link[m_il].m_bf = p;
										m_link[m_il].m_iSegf = L;
										m_link[m_il].m_iType = sd[L]->m_iType;
										if (m_link[m_il].m_iDir != 3 && m_link[m_il].m_iDir != 4)//非竖直方向
										{
											m_link[m_il].m_dDlf = fabs(m_link[m_il].m_bf.m_dx - m_link[m_il].m_iFlux);
										}
										else
										{
											m_link[m_il].m_dDlf = fabs(m_link[m_il].m_bf.m_dy - m_link[m_il].m_iFluy);
										}
										break;
									}
								}
								if (L == VALSEGNUM - 1)//固体点在段上端点，实数无法判断
								{
									if (dd < 0.01)//在段上端点，距离应该很小
									{
										m_link[m_il].m_bf = Vector(i, j);
										m_link[m_il].m_iSegf = iNearest;
										m_link[m_il].m_dDlf = 1;//边界刚好在固体点上;
									}
									else
									{
										//Show(lNowStep, "找交点出错");
										//Show(float(dd), "dd"); Show(i, "sx"); Show(j, "sy"); Show(iFlux, "fx"); Show(iFluy, "i"); Show(j, "j");
									}
								}
							}//for l
							if (m_lattice[i][j].m_iSol == -3)
							{
								m_lattice[i][j].m_bReaction = 2;//内膜要产NO
							}
							else if ((m_lattice[i][j].m_iSol == 0 || m_lattice[i][j].m_iSol == 4) &&
								m_lattice[i - m_iPrjx[m]][j - m_iPrjy[m]].m_iSol == -3)
							{
								m_lattice[i - m_iPrjx[m]][j - m_iPrjy[m]].m_bReaction = 2;
							}
							if (m_lattice[iFlux][iFluy].m_iSol == -1)
							{
								m_lattice[iFlux][iFluy].m_bReaction = 3;//内膜要产NO
							}
							if (m_link[m_il].m_iSegf >= 0 && (m_link[m_il].m_bdb ? m_link[m_il].m_iSegs >= 0 : 1))
							{
								m_il++;
							}
							if (m_il > LINKNUM)
							{
								printf("细胞链数超过了设定值");
							}
						}//if
					}//if
				}//for m
			}//for if
		}//for j
	}//for i
}

void SolftValve::FindLink()
{
	m_il = 0;
	Angle();
	if (m_lActive)
	{
		FindLink(m_MemUL.m_Seg, m_MemDL.m_Seg);
	}
	m_il0 = m_il;
	if (m_rActive)
	{
		FindLink(m_MemUR.m_Seg, m_MemDR.m_Seg);
	}
}

int SolftValve::OnMem(int i, int j)
{
	if (SamePoint(i, j, m_MemDL.m_Seg[0]->m_pst) ||
		SamePoint(i, j, m_MemUL.m_Seg[0]->m_pst) ||
		SamePoint(i, j, m_MemDR.m_Seg[0]->m_pst) ||
		SamePoint(i, j, m_MemUR.m_Seg[0]->m_pst))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int SolftValve::SamePoint(int i, int j, Vector v)
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

void SolftValve::Angle(ValveSegment** v)
{
	int i;
	for (i = 0; i < VALSEGNUM; i++)
	{
		if (v[i]->m_next1 != NULL)
		{
			v[i]->m_da1 = atan2(v[i]->m_next1->m_pst.m_dy
				- v[i]->m_pst.m_dy, v[i]->m_next1->m_pst.m_dx
				- v[i]->m_pst.m_dx);
		}
	}
}

void SolftValve::Angle()
{
	Angle(m_MemDL.m_Seg); Angle(m_MemUL.m_Seg);
	Angle(m_MemDR.m_Seg); Angle(m_MemUR.m_Seg);
}

void SolftValve::Collide()
{
	int i, j;
	Vector wv, bfv, fv;
	double dDen, dKappa;
	double dLinBuff[2];
	Link link[2];
	ValveSegment **su, **sd;
	for (i = 0; i < m_il; i++)//扫描链//1
	{
		if (i == 0 && m_lActive)
		{
			su = m_MemUL.m_Seg;
			sd = m_MemDL.m_Seg;
		}
		else if (i == m_il0&&m_rActive)
		{
			su = m_MemUR.m_Seg;
			sd = m_MemDR.m_Seg;
		}
		for (j = 0; j<2; j++)//0外面是流体，里面是固体，1反过来//2
		{
			if (j == 0)
			{
				link[0] = m_link[i];
			}
			else
			{
				link[1] = -m_link[i];
			}
			if (link[j].m_iType >0)//链在上边膜
			{
				if (su[link[j].m_iSegf]->m_next1 != NULL)
				{
					wv = (su[link[j].m_iSegf]->m_v + su[link[j].m_iSegf]->m_next1->m_v) / 2.0;
				}
				else
				{
					wv = su[link[j].m_iSegf]->m_v;
				}
			}
			else if (link[j].m_iType < 0)//链在下边膜
			{
				if (sd[link[j].m_iSegf]->m_next1 != NULL)
				{
					wv = (sd[link[j].m_iSegf]->m_v + sd[link[j].m_iSegf]->m_next1->m_v) / 2.0;
					/*if (lNowStep == 209 && i == 305 && m_iRev[link[j].m_iDir] == 4)
					{
					Show(sd[link[j].m_iSegf]->m_v.m_dy, "ss0"); Show(sd[link[j].m_iSegf]->m_next1->m_iType, "ss");
					}*/
				}
				else
				{
					wv = sd[link[j].m_iSegf]->m_v;
				}
			}
			fv = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_v;
			dDen = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_dDen;
			if ((m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol != 0) &&
				(m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]//方向为流体指向固体
				[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_iSol == -1 && j == 0 ||
				m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]//方向为流体指向固体
				[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_iSol == -3 && j == 1))//附近有流体点//3
			{
				if (link[j].m_dDlf >= 0.5)//外推//4
				{
					bfv = fv*((link[j].m_dDlf - 1) / link[j].m_dDlf)
						+ wv / link[j].m_dDlf;
					dKappa = (2 * link[j].m_dDlf - 1) / m_dTau;
				}
				else//直接用//4
				{
					if (j == 0)//外点//5
					{
						if (m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]
							[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_iSol == -1)//仍然在外
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
					else//内点//5
					{
						if (m_lattice[link[j].m_iFlux - m_iPrjx[link[j].m_iDir]]
							[link[j].m_iFluy - m_iPrjy[link[j].m_iDir]].m_iSol == -3)//仍然在内
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
			else//附近无流体 或 链反射//3
			{
				dLinBuff[j] = m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_df[link[j].m_iDir]
					- 2 * m_dCoe[link[j].m_iDir] * 3 * m_dDensity*(Vector(m_iPrjx[link[j].m_iDir], m_iPrjy[link[j].m_iDir])*wv);
				/*if(m_lattice[link[j].m_iFlux][link[j].m_iFluy].m_iSol>=0)
				{
				MessageBox(NULL,"Error","出去啦",MB_OK);
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
			//if (lNowStep == 114605 && link[j].m_iSolx == 316 && link[j].m_iSoly == 28 && m_iRev[link[j].m_iDir] == 4) { Show(i, "i"); Show(dLinBuff[j], "ss"); }
		}//for i
	}//1
}


void SolftValve::Force()
{
	if (m_lActive)
	{
		m_MemDL.Force();
		m_MemUL.Force();
	}
	if (m_rActive)
	{
		m_MemDR.Force();
		m_MemUR.Force();
	}
}

void SolftValve::HydroAcc()
{
	int i;
	for (i = 1; i < VALSEGNUM; i++)//第一段不动
	{
		if (m_lActive)
		{
			m_MemDL.m_Seg[i]->HydroAcc();
			m_MemUL.m_Seg[i]->HydroAcc();
		}
		if (m_rActive)
		{
			m_MemDR.m_Seg[i]->HydroAcc();
			m_MemUR.m_Seg[i]->HydroAcc();
		}
	}
}

void SolftValve::Move(int j)
{
	int i;
	for (i = 1; i < VALSEGNUM; i++)//第一段在膜上
	{
		if (m_lActive)
		{
			m_MemDL.m_Seg[i]->Move(j);
			m_MemUL.m_Seg[i]->Move(j);
		}
		if (m_rActive)
		{
			m_MemDR.m_Seg[i]->Move(j);
			m_MemUR.m_Seg[i]->Move(j);
		}
	}
}

void SolftValve::ElastAcc()
{
	int i, n1, n2, m1, m2;
	for (i = 1; i < VALSEGNUM; i++)//第一段不动
	{
		if (m_lActive)
		{
			m_MemDL.m_Seg[i]->m_ka = Vector(0, 0);
			m_MemUL.m_Seg[i]->m_ka = Vector(0, 0);
		}
		if (m_rActive)
		{
			m_MemDR.m_Seg[i]->m_ka = Vector(0, 0);
			m_MemUR.m_Seg[i]->m_ka = Vector(0, 0);
		}
	}
	n1 = 0, n2 = 0; m1 = 0; m2 = 0;
	if (m_lActive)
	{
		for (i = 0; i < VALSEGNUM; i++)//统计发生接触的段数
		{
			if (m_MemDL.m_Seg[i]->m_GetForce != 0)
			{
				if (m_MemDL.m_Seg[i]->m_v.m_dy <= 0)
				{
					n2++;
				}
				else
				{
					n1++;
				}
			}
			if (m_MemUL.m_Seg[i]->m_GetForce != 0)
			{
				if (m_MemUL.m_Seg[i]->m_v.m_dy >= 0)
				{
					m1++;
				}
				else
				{
					m2++;
				}
			}
		}
		for (i = 1; i < VALSEGNUM; i++)//不要取0
		{
			m_MemDL.m_Seg[i]->ElastAcc(n1, n2);//>0在前
			m_MemUL.m_Seg[i]->ElastAcc(m1, m2);
		}
	}
	n1 = 0, n2 = 0; m1 = 0; m2 = 0;
	if (m_rActive)
	{
		for (i = 0; i < VALSEGNUM; i++)//第一段不动
		{
			if (m_MemDR.m_Seg[i]->m_GetForce != 0)
			{
				if (m_MemDR.m_Seg[i]->m_v.m_dy <= 0)
				{
					n2++;
				}
				else
				{
					n1++;
				}
			}
			if (m_MemUR.m_Seg[i]->m_GetForce != 0)
			{
				if (m_MemUR.m_Seg[i]->m_v.m_dy >= 0)
				{
					m1++;
				}
				else
				{
					m2++;
				}
			}
		}
		for (i = 1; i < VALSEGNUM; i++)//不能取0
		{
			m_MemDR.m_Seg[i]->ElastAcc(n1, n2);
			m_MemUR.m_Seg[i]->ElastAcc(m1, m2);
		}
	}
	LimitFirstPoint();
}

void SolftValve::LimitFirstPoint()
{
	if (m_lActive)
	{
		m_MemDL.LimitFirstPoint();
		m_MemUL.LimitFirstPoint();
	}
	if (m_rActive)
	{
		m_MemDR.LimitFirstPoint();
		m_MemUR.LimitFirstPoint();
	}
}

void SolftValve::Update()
{
	m_MemDR.Update();
	m_MemUR.Update();
}

void SolftValve::VescoAcc()
{
	int i;
	for (i = 1; i < VALSEGNUM; i++)//第一段不动
	{
		if (m_lActive)
		{
			m_MemDL.m_Seg[i]->VescoAcc();
			m_MemUL.m_Seg[i]->VescoAcc();
		}
		if (m_rActive)
		{
			m_MemDR.m_Seg[i]->VescoAcc();
			m_MemUR.m_Seg[i]->VescoAcc();
		}
	}
}

void SolftValve::Contact()
{
	int i;
	double dy, df,ff;
	for (i = 1; i < VALSEGNUM; i++)
	{
		dy = m_MemUL.m_Seg[i]->m_pst.m_dy - m_MemDL.m_Seg[i]->m_pst.m_dy;
		if (dy < DELTA)
		{
			if (iClose != 1&&iMyID==iNumProcs-1)
			{
				iClose = 1;
			}
			df = m_MemUL.m_Seg[i]->m_ka.m_dy - m_MemDL.m_Seg[i]->m_ka.m_dy;
			df *= pow(DELTA / dy, 11);
			ff = (fabs(m_MemUL.m_Seg[i]->m_f.m_dy + m_MemUL.m_Seg[i]->m_ka.m_dy*m_MemUL.m_Seg[i]->m_dm) +
				fabs(m_MemDL.m_Seg[i]->m_f.m_dy + m_MemDL.m_Seg[i]->m_ka.m_dy*m_MemDL.m_Seg[i]->m_dm)) / 2;
			if (df>2 * ff)
			{
				df = 2 * ff;
			}
			m_MemUL.m_Seg[i]->m_ka.m_dy = df / 2;
			m_MemDL.m_Seg[i]->m_ka.m_dy = -df / 2;
		}
	}
}

void SolftValve::Pack(char* buffer, int size, int* position)
{
	MPI_Pack((char*)&(m_MemDL.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, buffer, size, position, MPI_COMM_WORLD);
	MPI_Pack((char*)&(m_MemUL.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, buffer, size, position, MPI_COMM_WORLD);
	MPI_Pack((char*)&(m_MemDR.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, buffer, size, position, MPI_COMM_WORLD);
	MPI_Pack((char*)&(m_MemUR.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, buffer, size, position, MPI_COMM_WORLD);
}

void SolftValve::Unpack(char* buffer, int size, int* position)
{
	MPI_Unpack(buffer, size, position, (char*)&(m_MemDL.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, MPI_COMM_WORLD);
	MPI_Unpack(buffer, size, position, (char*)&(m_MemUL.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, MPI_COMM_WORLD);
	MPI_Unpack(buffer, size, position, (char*)&(m_MemDR.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, MPI_COMM_WORLD);
	MPI_Unpack(buffer, size, position, (char*)&(m_MemUR.m_Seg[1]->m_id), VALSEGNUM - 1, One_Valve_Mem_data_type, MPI_COMM_WORLD);
}

MPI_File& operator<<(MPI_File& fs, SolftValve& v)//4*
{
	if (v.m_lActive)
	{
		fs << v.m_MemDL;
		fs << v.m_MemUL;
	}
	if (v.m_rActive)
	{
		fs << v.m_MemDR;
		fs << v.m_MemUR;
	}
	return fs;
}

MPI_File& operator>>(MPI_File& fs, SolftValve& v)
{
	if (v.m_lActive)
	{
		fs >> v.m_MemDL;
		fs >> v.m_MemUL;
	}
	if (v.m_rActive)
	{
		fs >> v.m_MemDR;
		fs >> v.m_MemUR;
	}
	return fs;
}