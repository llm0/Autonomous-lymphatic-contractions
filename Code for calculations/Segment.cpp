// Segment.cpp: implementation of the Segment class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "lymph.h"
#include "Segment.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//注意，所有静态变量在对象应用前必须初始化
double Segment::m_ymax;
double Segment::m_ymin;
int* Segment::m_iPrjx=NULL;
int* Segment::m_iPrjy=NULL;
int (*Segment::m_iPrjxy)[9]=NULL;
int* Segment::m_iRev=NULL;
double* Segment::m_dAngleE=NULL;
double* Segment::m_dCoe=NULL;
double Segment::m_dTau=0;
double Segment::m_dInTau=0;
double Segment::m_dDensity=0;
double Segment::m_dEpsilon=1;
double Segment::m_ddT=0;
double Segment::m_dl=0;
double Segment::m_dTheta0;
//double Segment::m_dm=0;
double Segment::m_dffx=0;
double Segment::m_dffy=0;
Lattice (*(Segment::m_Lattice))[HEIGHT];
Link Segment::m_LinkBuffer[MAXLINK];

Segment::Segment()
{
	m_il=0;
	m_pst=Vector(0,0);
	m_v=Vector(0,0);
	m_f=Vector(0,0);
	m_fi = 0;
	m_fo = 0;
	m_fm=0;
	m_db=0;
	m_prev=NULL;
	m_next=NULL;
	m_next1 = NULL;
	m_link=NULL;
	m_dcCa=0;
	m_dGrowth=0;
	m_GetForce = 0;
}

Segment::~Segment()
{
	if(m_link!=NULL)
	{
		delete []m_link;
	}
}

Segment& Segment::operator=(const Segment &v)
{
	m_GetForce = v.m_GetForce;
	m_pst = v.m_pst;
	m_pst_bak = v.m_pst_bak;
	m_da = v.m_da;
	m_da1 = v.m_da1;
	m_v = v.m_v;
	m_f = v.m_f;
	m_fa = v.m_fa;
	m_ka = v.m_ka;
	m_db = v.m_db;
	m_dCurve = v.m_dCurve;
	m_fi = v.m_fi;
	m_fo = v.m_fo;
	return *this;
}

MPI_File& operator<<(MPI_File& fs, Segment& v)//1i+2d+2d+2d+2d+2i=8d+3i
{
	MPI_Status Status;
	MPI_File_write(fs, (char*)&(v.m_id), sizeof(v.m_id), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_pst), sizeof(v.m_pst), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_v), sizeof(v.m_v), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_f), sizeof(v.m_f), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_dcCa), sizeof(v.m_dcCa), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_dGrowth), sizeof(v.m_dGrowth), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_bPassTh), sizeof(v.m_bPassTh), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_bCanMove), sizeof(v.m_bCanMove), MPI_BYTE, &Status);
	return fs;
}

MPI_File& operator>>(MPI_File& fs, Segment& v)
{
	MPI_Status Status;
	MPI_File_read(fs, (char*)&(v.m_id), sizeof(v.m_id), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_pst), sizeof(v.m_pst), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_v), sizeof(v.m_v), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_f), sizeof(v.m_f), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_dcCa), sizeof(v.m_dcCa), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_dGrowth), sizeof(v.m_dGrowth), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_bPassTh), sizeof(v.m_bPassTh), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_bCanMove), sizeof(v.m_bCanMove), MPI_BYTE, &Status);
	return fs;
}

void Segment::FindRec(int *ixn, int *iyn, int *ixm, int *iym, double *dxn, double *dyn, double *dxm, double *dym, int lb[2][2], int iType )
{
	Segment * Next;
	if (iType == 0)
	{
		Next = m_next;
	}
	else
	{
		Next = m_next1;
	}
	if(Next!=NULL)
	{
		if(m_pst.m_dx<Next->m_pst.m_dx)
		{
			*ixn=int(m_pst.m_dx);
			*ixm=int(Next->m_pst.m_dx+1.0);
			*dxn=m_pst.m_dx;
			*dxm=Next->m_pst.m_dx;
			lb[0][0]=1;
			lb[1][0]=0;
		}
		else
		{
			*ixn=int(Next->m_pst.m_dx);
			*ixm=int(m_pst.m_dx+1.0);
			*dxn=Next->m_pst.m_dx;
			*dxm=m_pst.m_dx;
			lb[0][0]=0;
			lb[1][0]=1;
		}
		if(m_pst.m_dy<Next->m_pst.m_dy)
		{
			*iyn=int(m_pst.m_dy);
			*iym=int(Next->m_pst.m_dy+1.0);
			*dyn=m_pst.m_dy;
			*dym=Next->m_pst.m_dy;
			lb[0][1]=1;
			lb[1][1]=0;

		}
		else
		{
			*iyn=int(Next->m_pst.m_dy);
			*iym=int(m_pst.m_dy+1.0);
			*dyn=Next->m_pst.m_dy;
			*dym=m_pst.m_dy;
			lb[0][1]=0;
			lb[1][1]=1;
		}
	}
}

void Segment::FindRec(int *ixn, int *iyn, int *ixm, int *iym, int iType)
{
	Segment * Next;
	if (iType == 0)
	{
		Next = m_next;
	}
	else
	{
		Next = m_next1;
	}
	if(Next!=NULL)
	{
		if(m_pst.m_dx<Next->m_pst.m_dx)
		{
			*ixn=int(m_pst.m_dx);
			*ixm=int(Next->m_pst.m_dx+1.0);
		}
		else
		{
			*ixn=int(Next->m_pst.m_dx);
			*ixm=int(m_pst.m_dx+1.0);
		}
		if(m_pst.m_dy<Next->m_pst.m_dy)
		{
			*iyn=int(m_pst.m_dy);
			*iym=int(Next->m_pst.m_dy+1.0);
		}
		else
		{
			*iyn=int(Next->m_pst.m_dy);
			*iym=int(m_pst.m_dy+1.0);
		}
	}
}

void Segment::DetermineDirection(int *iDir,Point*FluidNode, Point *SolidNode)
{
	switch(*iDir)
	{
	case 1:
	case 2:
		if(FluidNode->m_ix>SolidNode->m_ix)//固体指向流体
		{
			*iDir=1;
		}
		else
		{
			*iDir=2;
		}
		break;
	case 3:
	case 4:
		if(FluidNode->m_iy>SolidNode->m_iy)//固体指向流体
		{
			*iDir=3;
		}
		else
		{
			*iDir=4;
		}
		break;
	case 5:
	case 6:
		if(FluidNode->m_iy>SolidNode->m_iy)//固体指向流体
		{
			*iDir=6;
		}
		else
		{
			*iDir=5;
		}
		break;
	case 7:
	case 8:
		if(FluidNode->m_iy>SolidNode->m_iy)//固体指向流体
		{
			*iDir=7;
		}
		else
		{
			*iDir=8;
		}
		break;
	default:
		printf(("确定方向错误"));
	}

}

int Segment::IsInner(Point p1,Point p2,Vector pp,int iDir)
{
	if((iDir==1||iDir==2)&&(pp.m_dx<=p2.m_ix&&pp.m_dx>=p1.m_ix||
		pp.m_dx>=p2.m_ix&&pp.m_dx<=p1.m_ix))
	{
		return 1;
	}
	else if((iDir==3||iDir==4)&&(pp.m_dy<=p2.m_iy&&pp.m_dy>=p1.m_iy||
		pp.m_dy>=p2.m_iy&&pp.m_dy<=p1.m_iy))
	{
		return 1;
	}
	else if((pp.m_dx<=p2.m_ix&&pp.m_dx>=p1.m_ix||
		pp.m_dx>=p2.m_ix&&pp.m_dx<=p1.m_ix)&&
		(pp.m_dy<=p2.m_iy&&pp.m_dy>=p1.m_iy||
		pp.m_dy>=p2.m_iy&&pp.m_dy<=p1.m_iy))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

Point Segment::FindInerPoint(Vector p1,Vector p2,int iDir)
{
	Point p;
	if(iDir==1||iDir==2)
	{	
		p.m_ix=int((p1.m_dx+p2.m_dx)/2.0);
		p.m_iy=int(p1.m_dy+0.1);//保证水平方向上iy取值正确
	}
	else if(iDir==3||iDir==4)
	{
		p.m_ix=int(p1.m_dx+0.1);//保证竖直方向上ix取值正确
		p.m_iy=int((p1.m_dy+p2.m_dy)/2.0);
		
	}
	else if(iDir==5)
	{
		p.m_ix=int((p1.m_dx+p2.m_dx)/2.0);
		p.m_iy=int((p1.m_dy+p2.m_dy)/2.0-10000)+10000;//让y和x朝一个方向取点；
	}
	else if(iDir==6)
	{
		p.m_ix=int((p1.m_dx+p2.m_dx)/2.0-10000)+10000;//让x和y朝一个方向取点；
		p.m_iy=int((p1.m_dy+p2.m_dy)/2.0);//让y和x朝一个方向取点；
	}
	else
	{
		p.m_ix=int((p1.m_dx+p2.m_dx)/2.0);
		p.m_iy=int((p1.m_dy+p2.m_dy)/2.0);//剩下7和8x,y同向
	}
	if(iDir!=1&&iDir!=2)//非水平方向
	{
		if(p.m_iy<p1.m_dy&&p.m_iy<p2.m_dy)
		{
			p.m_iy++;
		}
		else if(p.m_iy>p1.m_dy&&p.m_iy>p2.m_dy)
		{
			p.m_iy--;
		}
	}
	if(iDir!=3&&iDir!=4)//非竖直方向
	{
		if(p.m_ix<p1.m_dx&&p.m_ix<p2.m_dx)//在左边
		{
			p.m_ix++;
		}
		else if(p.m_ix>p1.m_dx&&p.m_ix>p2.m_dx)//在右边
		{
			p.m_ix--;
		}
	}
	return p;
}

void Segment::HydroAcc()//只有y方向有运动，x方向无运动,包括Ca+力也在其中
{
	if(m_prev!=NULL)//高密度下，不用加粘性，此时时间平滑是不稳定的
	{
		m_fa.m_dy=((m_f.m_dy+m_prev->m_f.m_dy)/2.0+m_fm)/m_dm;//每一点存的是下一段的力，所以要和前一点取平均
	}
	else
	{
		m_fa.m_dy=m_f.m_dy/2.0/m_dm+m_fm/m_dm;
	}
	m_fa.m_dx = 0; 
}


void Segment::ElastAcc(int j)//包含3种弹性力：收缩弹性，拉伸弹性，限制弹性，有问题，因为要用到坐标算半径，所以一定要备份坐标以保证并行
{	
	double dR,l1,l2,RR0;
	double dch;
	Vector dd1,dd2;
	Vector s1,s,s2;
	Segment * Next;
	Next = m_next;
	dR = LymVessle.m_MemUp.m_segment[int(m_id)].m_pst_bak.m_dy - LymVessle.m_MemDown.m_segment[int(m_id)].m_pst_bak.m_dy;
/*	if (iMyID == iNumProcs - 1 &&m_pst.m_dx >= SolValve.m_MemUR.m_Seg[0]->m_pst.m_dx)
	{
		dch = Shrink * 4;// +(100 * Shrink - Shrink) / (LymVessle.m_MemUp.m_segment[SEGNUM - LEFTVALVE].m_pst.m_dx - SolValve.m_MemUR.m_Seg[0]->m_pst.m_dx)
			//*(m_pst.m_dx - SolValve.m_MemUR.m_Seg[0]->m_pst.m_dx);
	}
	else*/
	{
		dch = Shrink;
	}
	if (iMyID == iNumProcs - 1 && m_id >= SEGNUM - LEFTVALVE)//72/2+1
	{
		RR0 = DCa / 2;
	}
	else
	{
		RR0 = dR0 / 2;
	}
	l1=0;l2=0;
	{
		if (m_iType == 1)
		{
			if (m_pst.m_dy >= m_ymin + DELTA)
			{
				m_ka.m_dy += -dch*dStregth*(LymVessle.m_MemUp.m_segment[int(m_id)].m_pst_bak.m_dy -
					((HEIGHT - 1) / 2.0 + RR0)) / m_dm;//(dR-dR0)/m_dm;//Show(m_ka.m_dy,"fy");//收缩弹性
			}
			else
			{
				m_ka.m_dy += -dch*dStregth*(LymVessle.m_MemUp.m_segment[int(m_id)].m_pst_bak.m_dy -
					((HEIGHT - 1) / 2.0 + RR0)) / m_dm
					*pow(DELTA / (m_pst.m_dy - m_ymin), 11);
			}
		}
		else if(m_iType==-1)
		{
			if (m_pst.m_dy <= m_ymax - DELTA)
			{
				m_ka.m_dy += -dch*dStregth*(LymVessle.m_MemDown.m_segment[int(m_id)].m_pst_bak.m_dy - ((HEIGHT - 1) / 2.0 - RR0)) / m_dm;//(dR-dR0)/m_dm;//收缩弹性
			}
			else
			{
				m_ka.m_dy += -dch*dStregth*(LymVessle.m_MemDown.m_segment[int(m_id)].m_pst_bak.m_dy - ((HEIGHT - 1) / 2.0 - RR0)) / m_dm
					*pow(DELTA / (m_ymax - m_pst.m_dy), 11);
			}
		}
	}
	if(m_prev!=NULL)
	{
		l1=m_pst_bak.m_dy-m_prev->m_pst_bak.m_dy;
	}
	if(Next!=NULL)
	{
		l2=m_pst_bak.m_dy-Next->m_pst_bak.m_dy;
	}
	m_ka.m_dy+=-Bending*(l1+l2)/m_dm;//拉伸弹性
}

void Segment::VescoAcc()
{
	Vector vv,vv1,vv2;
	if (m_prev != NULL)
	{
		vv1 = m_v- m_prev->m_v;
	}
	if (m_next != NULL)
	{
		vv2 = m_v - m_next->m_v;
	}
	vv = -(vv1 + vv2)*Viscous / m_dm;
	m_ka+=vv;
	m_ka += -m_v*ViscousMem / m_dm;
}

void Segment::Angle(int iType)
{
	Segment * Next;
	if (iType == 0)
	{
		Next = m_next;
		if (Next != NULL)
		{
			m_da = atan2(Next->m_pst.m_dy - m_pst.m_dy, Next->m_pst.m_dx - m_pst.m_dx);
		}
	}
	else
	{
		Next = m_next1;
		if (Next != NULL)
		{
			m_da1 = atan2(Next->m_pst.m_dy - m_pst.m_dy, Next->m_pst.m_dx - m_pst.m_dx);
		}
	}
}

int Segment::Curvature(int iType)
{
	Vector p1,p2,po,s,s1,s2,ss,pp;
	Segment * Next;
	if (iType == 0)
	{
		Next = m_next;
	}
	else
	{
		Next = m_next1;
	}
	if(m_prev!=NULL&&Next!=NULL)
	{
		s1=m_prev->m_pst;
		s=m_pst;
		s2=Next->m_pst;
		p1=(s1+s)*0.5;//中点1
		p2=(s+s2)*0.5;//中点2
		s1=s-s1;
		ss=s1;
		s1.Roll(dhPI);//前段的垂直线
		s2=s2-s;
		s2.Roll(dhPI);//后段的垂直线
		if(IsCross(p1,s1,p2,s2,&po))//p1,s1;p2,s2两条中垂线
		{
			pp=po-s;//po为两线段中垂线交点,ss为前点指向当前点的矢量
			if((pp^ss)>0)
			{
				m_dCurve=-1.0/pp.Mod();
			}
			else
			{
				m_dCurve=1.0/pp.Mod();
			}
		}
		else
		{
			m_dCurve=0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

double Segment::Momentum(int iType)
{
	Vector s1,s,s2,f1,f2;
	Segment * Next;
	if (iType == 0)
	{
		Next = m_next;
	}
	else
	{
		Next = m_next1;
	}
	if(m_prev!=NULL&&Next!=NULL)
	{
		s1=m_prev->m_pst;
		s=m_pst;
		s2=Next->m_pst;
		s2=Next->m_pst;
		s2=s2-s;
		s1=s1-s;
		f1=m_prev->m_ka*m_dm;
		f2=Next->m_ka*m_dm;
		f1.Roll(-PI/2);f2.Roll(-PI/2);
		return s1*f1+s2*f2;
	}
	else
	{
		return 0;
	}
}

void Segment::Move(int j)
{	
	m_v += (m_fa + m_ka)*m_ddT; 
	if (m_iType >= 2 && m_iType <= 6 || m_iType >= -6 && m_iType <= -2)//Valve
	{
		m_v.m_dx = 0;
	}
	m_pst+=m_v*m_ddT;
	if (m_iType >= 2 && m_iType <= 6)//Valve 中间限制位置
	{
		m_v.m_dx = 0;
		if (m_pst.m_dy < (HEIGHT - 1) / 2.0 + DELTA *0.5)
		{
			m_pst.m_dy = (HEIGHT - 1) / 2.0 + DELTA *0.5;
			if (m_v.m_dy < 0)
			{
				m_v.m_dy = 0;
			}
		}
	}
	else if (m_iType >= -6 && m_iType <= -2)
	{
		m_v.m_dx = 0;
		if (m_pst.m_dy >(HEIGHT - 1) / 2.0 - DELTA *0.5)
		{
			m_pst.m_dy = (HEIGHT - 1) / 2.0 - DELTA *0.5;
			if (m_v.m_dy > 0)
			{
				m_v.m_dy = 0;
			}
		}
	}
}


void Segment::Force(int iType)//最近点插值法
{	
	int bin=0, bout=0;
	int k,m,n;
	double Press[2][2],Delta[2][2]={1,0,0,1},FluidVel[2],WallVel[2],ds[2],dPi;
	double df[2],ss;
	BoundaryLattice LeftB,RightB;
	Segment * Next;
	double da;
	Vector v,fout;
	if (iType == 0)
	{
		Next = m_next;
		da = m_da;
	}
	else
	{
		Next = m_next1;
		da = m_da1;
	}
	if(Next!=NULL)
	{
		LeftB.Clear();
		RightB.Clear();
		v = Next->m_pst - m_pst;
		ss=v.Mod();
		LeftB.m_dx=((m_pst+Next->m_pst)/2.0).m_dx;//位置
		LeftB.m_dy=((m_pst+Next->m_pst)/2.0).m_dy;
		RightB.m_dx=LeftB.m_dx;
		RightB.m_dy=LeftB.m_dy;
		//Extrapolate(&LeftB,&RightB);	
		Nearest(&LeftB,&RightB,iType);//外点，内点
		WallVel[0]=m_v.m_dx;
		WallVel[1]=m_v.m_dy;
		df[0]=0;df[1]=0;
		FluidVel[0]=LeftB.m_v.m_dx;FluidVel[1]=LeftB.m_v.m_dy;
		if(m_iType>0)
		{
			ds[0]=ss*cos(da-PI/2);ds[1]=ss*sin(da-PI/2);//上膜的面积法线方向向下
		}
		else
		{
			ds[0]=ss*cos(da+PI/2);ds[1]=ss*sin(da+PI/2);//下膜的面积法线方向向上
		}
		if (LeftB.m_iSol != -4)//找不到最近点
		{
			for (m = 0; m < 2; m++)
			{
				for (n = 0; n < 2; n++)
				{
					dPi = 0;
					for (k = 0; k < 9; k++)
					{
						dPi += (m_iPrjxy[m][k] - FluidVel[m])*(m_iPrjxy[n][k] - FluidVel[n])*LeftB.m_df[k];
					}
					Press[m][n] = -1.0 / 6.0 / dTau*LeftB.m_dDen*Delta[m][n] - (1 - 0.5 / dTau)*dPi;
				}
			}
			for (m = 0; m < 2; m++)
			{
				for (n = 0; n < 2; n++)
				{
					df[m] += (-Press[m][n] + LeftB.m_dDen*FluidVel[m] *
						(FluidVel[n] - WallVel[n]))*ds[n];
				}
			}
			m_fo = (df[0] * v.m_dx + df[1] * v.m_dy) / ss/ss;//v没归一,算出沿面积的应力
			fout.m_dx = df[0]; fout.m_dy = df[1];
			bout = 1; 
		}
		else
		{
			bout = 0;
		}
		if (RightB.m_iSol != -4)
		{
			FluidVel[0]=RightB.m_v.m_dx;FluidVel[1]=RightB.m_v.m_dy;
			ds[0]=-ds[0];ds[1]=-ds[1];
			for(m=0;m<2;m++)
			{
				for(n=0;n<2;n++)
				{
					dPi=0;
					for(k=0;k<9;k++)
					{
						dPi+=(m_iPrjxy[m][k]-FluidVel[m])*(m_iPrjxy[n][k]-FluidVel[n])*RightB.m_df[k];
					}
					Press[m][n]=-1.0/6.0/dTau*RightB.m_dDen*Delta[m][n]-(1-0.5/dTau)*dPi;
				}
			}
			for(m=0;m<2;m++)
			{
				for(n=0;n<2;n++)
				{
					df[m]+=(-Press[m][n]+RightB.m_dDen*FluidVel[m]*
						(FluidVel[n]-WallVel[n]))*ds[n];
				}
			}
			bin = 1;
		}
		else
		{
			bin = 0;
		}
		m_f.m_dx = df[0]; m_f.m_dy = df[1]; 
		m_fi = ((df[0] - fout.m_dx) * v.m_dx + (df[1] - fout.m_dy) * v.m_dy) / ss/ss;//v没归一,算出沿面积的应力
		if (fabs(m_f.m_dy) > 2) { printf("Proc:%d,Type:%d,SegID:%d,px:%f,Segment Force >2\n", iMyID, m_iType, m_id, Next->m_pst.m_dx); }
		if (bin&&bout)
		{
			m_GetForce = 0;
		}
		else if (!bin)
		{
			m_GetForce = 1;
		}
		else if (!bout)
		{
			m_GetForce = 2;
		}
		else
		{
			m_GetForce = 3;
		}
		/*if (m_iType >= 2 && m_iType <= 6&& m_pst.m_dy < CENTER + DELTA ||
			m_iType >= -6 && m_iType <= -2&& m_pst.m_dy>CENTER - DELTA)
		{
			m_GetForce = 1;
		}*/
	}//null
	else
	{
		m_f=Vector(0,0);
		if (iType != 0 )
		{
			m_GetForce = m_prev->m_GetForce;
		}
	}
	if (m_GetForce != 0)
	{
		m_f.m_dy = 0;
	}
}



void Segment::Nearest(BoundaryLattice *Outer, BoundaryLattice *Inner, int iType)
{
	int m, n, ilr, iPx, iPy, nn;
	double dx, dy, kkl0, kkr0, kk;
	dx = Outer->m_dx;
	dy = Outer->m_dy;
	nn = 1;
	Outer->m_iSol = -4;
	Inner->m_iSol = -4;
	kkl0 = 10;
	kkr0 = 10;
	for (m = -nn; m <= nn; m++)
	{
		for (n = -nn; n <= nn; n++)
		{
			ilr = 0;
			iPx = int(dx + 0.5) + m;//Minimium,第一点
			iPy = int(dy + 0.5) + n;
			if (InRegionX(iPx) && iPy > 0 && iPy<HEIGHT - 1)
			{
				if (lattice[iPx][iPy].m_iSol <= 0 && lattice[iPx][iPy].m_ipr != 2)
				{
					kk = sqrt((iPx - dx)*(iPx - dx) + (iPy - dy)*(iPy - dy));//距离
					if ((m_iType == -1 || m_iType == 1) && iType == 0)
					{
						if (lattice[iPx][iPy].m_iSol == -2 && lattice[iPx][iPy].m_ipr != 2)//外点，看为流体
						{
							if (Outer->m_iSol == -4 || kkl0>kk)
							{
								kkl0 = kk;
								*Outer = lattice[iPx][iPy];
								Outer->m_ix = iPx; Outer->m_iy = iPy;
							}
						}
						else if (lattice[iPx][iPy].m_iSol == -1)//内点，看为固体
						{
							if (Inner->m_iSol == -4 || kkr0 > kk)
							{
								kkr0 = kk;
								*Inner = lattice[iPx][iPy];
								Inner->m_ix = iPx; Inner->m_iy = iPy;
							}
						}
					}
					else if (iType == 1)
					{
						if (lattice[iPx][iPy].m_iSol == -1)//外点，看为流体
						{
							if (Outer->m_iSol == -4 || kkl0 > kk)
							{
								kkl0 = kk;
								*Outer = lattice[iPx][iPy];//ilx=iPx;ily=iPy;
								Outer->m_ix = iPx; Outer->m_iy = iPy;
							}
						}
						else if (lattice[iPx][iPy].m_iSol == -3)//内点，看为固体
						{
							if (Inner->m_iSol == -4 || kkr0 > kk)
							{
								kkr0 = kk;
								*Inner = lattice[iPx][iPy];
								Inner->m_ix = iPx; Inner->m_iy = iPy;
							}
						}
					}
				}
			}
		}
	}
	if (Outer->m_iSol != -4 && Inner->m_iSol != -4)
	{
		//break;
	}
	//	nn++;
	if (nn > 1)
	{
		//Show(1,("Segment NearestError"));Show(dx,("x"));Show(dy,("y"));
		//Show(m_iType, "aa"); Show(lattice[int(dx)][int(dy)].m_iSol , "type");
	}
}


double Segment::feqs(int Order,double Den,Vector bfv,Vector fv)
{
	double dfeq;
	double dDotMetBF;
	double dDotMetF;
	dDotMetBF=Vector(m_iPrjx[Order],m_iPrjy[Order])*bfv;
	dDotMetF=Vector(m_iPrjx[Order],m_iPrjy[Order])*fv;
	dfeq=Den*m_dCoe[Order]*(1+3*dDotMetBF+4.5*dDotMetF*
		dDotMetF-1.5*pow(fv.Mod(),2));
	return dfeq;
}


int Segment::OnSegment(Vector p1,Vector p2,Vector p3)
{
	double dxm,dxn,dym,dyn;
	if(p1.m_dx<p2.m_dx)
	{
		dxn=p1.m_dx;
		dxm=p2.m_dx;
	}
	else
	{
		dxn=p2.m_dx;
		dxm=p1.m_dx;
	}
	if(p1.m_dy<p2.m_dy)
	{
		dyn=p1.m_dy;
		dym=p2.m_dy;
	}
	else
	{
		dyn=p2.m_dy;
		dym=p1.m_dy;
	}
	if(p3.m_dx<dxn||p3.m_dx>dxm||p3.m_dy<dyn||p3.m_dy>dym)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Segment::IsCross(Vector l1,Vector l2,Vector *pp,int iType)//判断s1s2和l1l2的交点
{
	Vector s1,s2;
	Segment * Next;
	if (iType == 0)
	{
		Next = m_next;
	}
	else
	{
		Next = m_next1;
	}
	if(Next!=NULL)
	{
		double f1,f2,f3,f4;
		double a1,b1,c1,a2,b2,c2;
		s1=m_pst;
		s2=Next->m_pst;
		a1=s2.m_dy-s1.m_dy;
		b1=-(s2.m_dx-s1.m_dx);
		c1=-a1*s1.m_dx-b1*s1.m_dy;
		a2=l2.m_dy-l1.m_dy;
		b2=-(l2.m_dx-l1.m_dx);
		c2=-a2*l1.m_dx-b2*l1.m_dy;
		f1=(l1-s1)^(s2-s1);//s1,l1,s2
		f2=(l2-s1)^(s2-s1);//s1,l2,s2
		f3=(s2-l1)^(l2-l1);//l1,s2,l2
		f4=(s1-l1)^(l2-l1);//l1,s1,l2
		if(f1*f2<0&&f3*f4<0||f1==0&&OnSegment(s1,s2,l1)||f2==0&&OnSegment(s1,s2,l2)
			||f3==0&&OnSegment(l1,l2,s2)||f4==0&&OnSegment(l1,l2,s1))//s2上的不要
		{
			if(pp!=NULL)
			{
				if(a1*b2-a2*b1>0||a1*b2-a2*b1<0)
				{
					pp->m_dx=(c2*b1-c1*b2)/(a1*b2-a2*b1);
					pp->m_dy=(c1*a2-c2*a1)/(a1*b2-a2*b1);
					pp->m_dx=ErrorCorrect(pp->m_dx,l1.m_dx,l2.m_dx);
					pp->m_dy=ErrorCorrect(pp->m_dy,l1.m_dy,l2.m_dy);
				}
				else
				{
					if(lattice[int(l1.m_dx)][int(l1.m_dy)].m_iSol==-1)//两线重叠，直接认为交点在固体点上？？？
					{
						*pp=l1;
					}
					else
					{
						*pp=l2;
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
	else
	{
		return 0;
	}
}

int Segment::IsCross(Vector p1,Vector n1,Vector p2,Vector n2,Vector* pR)//p1,p2为点;n1,n2为方向
{
	double a1,b1,c1,a2,b2,c2;
	a1=n1.m_dy;
	b1=-n1.m_dx;
	c1=-a1*p1.m_dx-b1*p1.m_dy;
	a2=n2.m_dy;
	b2=-n2.m_dx;
	c2=-a2*p2.m_dx-b2*p2.m_dy;
	if(a1*b2-a2*b1>0||a1*b2-a2*b1<0)//两线非平行
	{
		pR->m_dx=(c2*b1-c1*b2)/(a1*b2-a2*b1);
		pR->m_dy=(c1*a2-c2*a1)/(a1*b2-a2*b1);
		return 1;
	}
	else
	{ 
		return 0;
	}
}


/*int Segment::IsOnLine(Vector pp)
{
	Vector s1,s2;
	double f;
	double a1,b1,c1;
	if(m_next!=NULL)
	{
		s1=m_pst;
		s2=m_next->m_pst;
		a1=s2.m_dy-s1.m_dy;
		b1=-(s2.m_dx-s1.m_dx);
		c1=-a1*s1.m_dx-b1*s1.m_dy;
		f=a1*pp.m_dx+b1*pp.m_dy+c1;//f1>0,l1 at s12 right,else left
		if(Cut(f)==0)//大于y的交点
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}*/

double  Segment::Cut(double v)//Avoid inconsistent in real number calculate
{
	if(fabs(v)<ZERO)
	{
		return 0;
	}
	else
	{
		return v;
	}
}

int  Segment::IsOneLink(Vector p1,Vector p2)
{
	if(int(p1.m_dx+ZERO)==int(p2.m_dx+ZERO)
		&&int(p1.m_dy+ZERO)==int(p2.m_dy+ZERO))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void Segment::CalciumForce()
{
	double dk,dR;
	BoundaryLattice Outer,Inner;
	//dkm = 0.05;// 0.04;//0.03;//0.06;// 0.12;//  0.1;// 0.08;// 0.06;// 0.03;//0.02;//  NOF:0.006;// 0.004;// 0.01;// 0.004;// 0.008L;// 0.01L;// 0.001;//0.0002sm; 0.01;// 0.015;// 0.01;//0.02;// 0.02;// 
	dk = 0.3;// 1;// 4;//1;
	dR = (LymVessle.m_MemUp.m_segment[m_id].m_pst.m_dy - LymVessle.m_MemDown.m_segment[m_id].m_pst.m_dy)/2;
	Outer.m_dx=m_pst.m_dx;Outer.m_dy=m_pst.m_dy;
	Nearest(&Outer,&Inner);//m_dcCa=1;
	if(m_iType>0)
	{
		m_fm = -dkm*dStregth*(m_dcCa / (1 + m_dcCa))*2*
			(2*dR/(DCa/2+dR))* (1.0/(1+dk*Inner.m_dcNO));//上膜的力向下
	}
	else
	{
		m_fm = dkm* dStregth* (m_dcCa / (1 + m_dcCa))*2*
			(2*dR/(DCa/2+dR))* (1.0/(1+dk*Inner.m_dcNO));//下膜的力向上
	}
}

void Segment::ReactionCa(double *dCaBuff)
{
	int iDelta; //fstream fa;
	double dR,dDeltaCa,dProduct,dDisapp,dRl,dNonlinear;
	BoundaryLattice Outer,Inner;
	dR = LymVessle.m_MemUp.m_segment[int(m_id)].m_pst.m_dy - LymVessle.m_MemDown.m_segment[int(m_id)].m_pst.m_dy;
	dRl = m_ymin - m_ymax; 
	Outer.m_dx=m_pst.m_dx;Outer.m_dy=m_pst.m_dy;
	Nearest(&Outer,&Inner);
	if(m_dGrowth>0&&m_dcCa>=CaThreshold&&m_dcCa-m_dGrowth<CaThreshold)
	{
		m_bPassTh=1;
	}
	if(m_dGrowth<0)
	{
		m_bPassTh=0;
	}
	if(m_dGrowth>0&&m_bPassTh)
	{
		iDelta=1;
	}
	else
	{
		iDelta=0;
	}
	if(m_dcCa>0)
	{
		/*if (m_bCanMove&&dR > dR0)
		{
			m_bCanMove = 2;
		}
		else if (dR < dR0 -2 && m_bCanMove == 2)
		{
			m_bCanMove = 1;
		}
		if (m_bCanMove == 2)*/
		/*if (iMyID == iNumProcs - 1 &&//最右边容易胀，不要
			m_pst.m_dx >= SolValve.m_MemUR.m_Seg[0]->m_pst.m_dx)
		{
			dNonlinear = 0;
		}
		else*/
		{
			dNonlinear = pow((dR - dRl) / (DCa - dRl), 11.0);
		}
		dProduct=CaProduct
			+ CaProduct*dNonlinear
			//+ 2*CaPeak*dNonlinear
			+ 10 * CaPeak*iDelta;
		if (Inner.m_iSol != -4)
		{
			dDisapp = CaDecay*(1 + CaNODecay*m_Lattice[Inner.m_ix][Inner.m_iy].m_dcNO);
		}
		else if (Outer.m_iSol != -4)
		{
			dDisapp = CaDecay*(1 + CaNODecay*m_Lattice[Outer.m_ix][Outer.m_iy].m_dcNO);
		}
		else
		{
			dDisapp = CaDecay*(1 + CaNODecay*0);
			printf("ReactionCa Error\n");
		}
		if(!(m_dcCa<=LIMITCA)&&dProduct>dDisapp)//最多是1
		{
			//dProduct=dDisapp;
			dDeltaCa = 0;
		}
		else
		{
			dDeltaCa = -dDisapp*m_dcCa + dProduct;
		}
		/*if (iMyID == 1 && m_iType == 1 && m_id == 54)
		{
			fa.open("aa.txt", ios::app | ios::out);
			fa << lNowStep << " " << iDelta << " " << dProduct << " " << dDisapp << " " << m_Lattice[Outer.m_ix][Outer.m_iy].m_dcNO << endl;
			fa.close();
		}*/
	}
	else
	{
		printf(("cCa<0"));//dDeltaCa=0;
	}
	*dCaBuff=dDeltaCa;
}

void Segment::ReactionNO(double rr)//not use
{
	BoundaryLattice Outer,Inner,dDisapp;
	double dks,dProduct,dBaseLine;
	dks = dkNO*h;// 500 * h;// 150 * h;// 70 * h;// 20 * h;//8//50;//50;//16;//10;//25;//50;//250;//25;//50
	dBaseLine=0;//0.00001;
	//if(m_bCanMove)
	{
		Outer.m_dx=m_pst.m_dx;Outer.m_dy=m_pst.m_dy;
		Nearest(&Outer,&Inner);
		if(m_Lattice[Inner.m_ix][Inner.m_iy].m_bReaction != 1)//膜还没反应
		{
			dProduct=dks*fabs(m_Lattice[Inner.m_ix][Inner.m_iy].m_Stress)+dBaseLine;
			if(!(m_Lattice[Inner.m_ix][Inner.m_iy].m_dcNO<=LIMITNO)&&dProduct>NODecay)
			{
				dProduct=NODecay;
			}
			NOBuf[Inner.m_ix][Inner.m_iy]=dProduct;//if(lNowStep==50) Show(dks*fabs(rr-dDeltaR)/rr/rr+dBaseLine,"aa");
			dcNO[(lNowStep/50)%100]+=m_Lattice[Inner.m_ix][Inner.m_iy].m_dcNO+dProduct;
			m_Lattice[Inner.m_ix][Inner.m_iy].m_bReaction = 1;//膜已经反应
		}
	}
}

void Segment::Stress()
{

}

int Segment::Triger()
{
	if(m_dGrowth>0&&m_dcCa>=CaThreshold&&m_dcCa-m_dGrowth<CaThreshold)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
