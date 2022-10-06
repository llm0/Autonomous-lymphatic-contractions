// Membrane.h: interface for the Membrane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBRANE_H__BDFBE8B8_03F0_40EF_A531_8AF9B46BB2DC__INCLUDED_)
#define AFX_MEMBRANE_H__BDFBE8B8_03F0_40EF_A531_8AF9B46BB2DC__INCLUDED_
#include "Segment.h"
#include <fstream>
#include "Link.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Membrane  
{
public:
	double m_dx1;//膜的起点,fixed
	double m_dy1;
	double m_dx2;
	double m_dy2;//膜的终点,fixed
	int m_iType;//段所在膜的种类：上管膜：1，下管膜-1，上瓣膜：2，下瓣膜：-2
	static int* m_iPrjx;
	static int* m_iPrjy;
	static int (*m_iPrjxy)[9];
	static int* m_iRev;
	static Lattice (*m_lattice)[HEIGHT];
	static Link m_LinkBuffer[MAXLINK];
	Membrane* m_pMemUp;
	Membrane* m_pMemDown;
	int m_ixn;
	int m_iyn;
	int m_ixm;
	int m_iym;
	int m_il;
	static double m_dDeltax;
	Segment m_segment[SEGNUM_E];
	Link m_link[LINKNUM];
	Membrane(double dx1=0,double dy1=0,double dx2=10,double dy2=10);
	virtual ~Membrane();
	friend MPI_File& operator<<(MPI_File& fs, Membrane& v);
	friend MPI_File& operator>>(MPI_File& fs, Membrane& v);
	void FindNbLattice();
	void CalciumSpread(double *);
	double VirticalDistance(int i, int j);
	void ReactionCa(double *dCaBuff);
	void ReactionNO(double rr);
	void BackupPosition();
	void CalciumForce();
	void Angle();
	int Between(int ix, int* i, int* j);
	void init(int bf);
	void FindLink();
	void Pack(char* buffer, int size, int * position);
	void Unpack(char* buffer, int size, int * position);
	int GetInt(double x);
};

#endif // !defined(AFX_MEMBRANE_H__BDFBE8B8_03F0_40EF_A531_8AF9B46BB2DC__INCLUDED_)
