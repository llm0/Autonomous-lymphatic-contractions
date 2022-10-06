// Lattice.h: interface for the Lattice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LATTICE_H__01398C4F_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_LATTICE_H__01398C4F_0E8B_11D6_845F_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <fstream>
#include "Vector.h"
#include "mpi.h"
class Lattice  
{
public:
	Lattice(int Px=0,int Py=0,double Den=0);
	~Lattice();
public:
	int m_iSol;//
	int m_iOld;//1
	int m_bReaction;//0还没反应，1膜反应，2valve反应
	int m_ipr;//2,porous

	double m_df[9];//3-11
	Vector m_v;//12-13
	double m_dDen;//14
	double m_dcNO;//15
	double m_Stress;//16

	static double m_dTau;
	static int* m_iPrjx;
	static int* m_iPrjy;
	static double* m_dCoe;
	static int m_iSize;
	static double* m_dGravity;
	static double* m_dGravityAngle;

public:
	void Collide();
	void Clear();
	Lattice& operator=(const Lattice& v);
	friend MPI_File& operator<<(MPI_File& fs, Lattice& v);
	friend MPI_File& operator>>(MPI_File& fs, Lattice& v);
	static double feq(int Order,double Den,double Velx,double Vely);
	static double feqs(int Order,double Den,double BFVelx,double BFVely,double FVelx,double FVely);
	static double feqs(int,double,Vector bfv,Vector fv);
};

#endif // !defined(AFX_LATTICE_H__01398C4F_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
