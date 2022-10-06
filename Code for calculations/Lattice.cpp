// Lattice.cpp: implementation of the Lattice class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Lattice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int* Lattice::m_iPrjx;
int* Lattice::m_iPrjy;
double* Lattice::m_dCoe;
double Lattice::m_dTau;
int Lattice::m_iSize;
double* Lattice::m_dGravity;
double* Lattice::m_dGravityAngle;

Lattice::Lattice(int Px,int Py,double Den)
{
	int i;
	m_dDen=Den;
	m_iSol=-2;//先全是外部
	m_v = Vector(0, 0);
	m_Stress=0;
	m_bReaction=0;
	for(i=0;i<9;i++)
	{
		m_df[i]=0;
	}
}

Lattice::~Lattice()
{
	m_Stress=0;
}

double Lattice::feq(int Order,double Den,double Velx,double Vely)
{
	double dfeq;
	double dDotMet;
	dDotMet=m_iPrjx[Order]*Velx+m_iPrjy[Order]*Vely;
	dfeq=Den*m_dCoe[Order]*(1+3*dDotMet+4.5*dDotMet*dDotMet-
		1.5*(Velx*Velx+Vely*Vely));
	return dfeq;
}

double Lattice::feqs(int Order,double Den,double BFVelx,double BFVely,double FVelx,double FVely)
{
	double dfeq;
	double dDotMetBF;
	double dDotMetF;
	dDotMetBF=m_iPrjx[Order]*BFVelx+m_iPrjy[Order]*BFVely;
	dDotMetF=m_iPrjx[Order]*FVelx+m_iPrjy[Order]*FVely;
	dfeq=Den*m_dCoe[Order]*(1+3*dDotMetBF+4.5*dDotMetF*
		dDotMetF-1.5*(FVelx*FVelx+FVely*FVely));
	return dfeq;
}

double Lattice::feqs(int Order,double Den,Vector bfv,Vector fv)
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

void Lattice::Collide()
{
	int i;
	if(m_iSol<0)
	{
		for(i=0;i<9;i++)
		{
			m_df[i] = m_df[i] - (m_df[i] - feq(i, m_dDen, m_v.m_dx, m_v.m_dy)) / m_dTau + (*m_dGravity*cos(*m_dGravityAngle)*m_dDen)*m_iPrjx[i] / 6.0
				+ (*m_dGravity*sin(*m_dGravityAngle)*m_dDen)*m_iPrjy[i] / 6.0;
		}
	}
}

void Lattice::Clear()
{
	int i;
	m_dDen=0;
	m_v = Vector(0, 0);
	for(i=0;i<9;i++)
	{
		m_df[i]=0;
	}
}

Lattice& Lattice::operator=(const Lattice& v)
{
	int i;
	m_dDen=v.m_dDen;
	m_iSol=v.m_iSol;
	m_iOld=v.m_iOld;
	m_v=v.m_v;
	for(i=0;i<9;i++)
	{
		m_df[i]=v.m_df[i];
	}
	return *this;
}

MPI_File& operator<<(MPI_File& fs, Lattice& v)//9d+2i+2d+1i+2d=13d+3i
{
	MPI_Status Status;
	MPI_File_write(fs, (char*)&(v.m_df), 9 * sizeof(double), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_iSol), sizeof(v.m_iSol), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_iOld), sizeof(v.m_iOld), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_v), sizeof(v.m_v), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_dDen), sizeof(v.m_dDen), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_bReaction), sizeof(v.m_bReaction), MPI_BYTE, &Status);
	MPI_File_write(fs, (char*)&(v.m_dcNO), sizeof(v.m_dcNO), MPI_BYTE, &Status);
	return fs;
}

MPI_File& operator>>(MPI_File& fs, Lattice& v)
{
	MPI_Status Status;
	MPI_File_read(fs, (char*)&(v.m_df), 9 * sizeof(double), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_iSol), sizeof(v.m_iSol), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_iOld), sizeof(v.m_iOld), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_v), sizeof(v.m_v), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_dDen), sizeof(v.m_dDen), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_bReaction), sizeof(v.m_bReaction), MPI_BYTE, &Status);
	MPI_File_read(fs, (char*)&(v.m_dcNO), sizeof(v.m_dcNO), MPI_BYTE, &Status);
	return fs;
}
