// Segment.h: interface for the Segment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENT_H__EB79A7E0_E937_11D6_8461_00E04C392A24__INCLUDED_)
#define AFX_SEGMENT_H__EB79A7E0_E937_11D6_8461_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Link.h"
#include "Point.h"
#include "Lattice.h"
#include "math.h"
#include <fstream>
#include "lymph_h.h"
#include "BoundaryLattice.h"
#include "Vector.h"
#define MAXLINK 100
#define PI 3.1415926535897932

class Segment  
{
public:
	Segment();
	~Segment();
public://define member
	int m_id;//
	int m_il;//1
	int m_iType;//段所在膜的种类：上管膜：1，下管膜-1，上瓣膜：2，下瓣膜：-2
	int m_bCanMove;//

	
	int m_bPassTh;//
	int m_GetForce;//1 0两面，1:里面,2外面
	
	Vector m_pst;//2,3置
	Vector m_pst_bak;//4,5
	double m_da;//6角度
	double m_da1;//7角度
	Vector m_v;//8,9
	Vector m_f;//10,11
	Vector m_fa;//12,13流体作用产生的加速度
	Vector m_ka;//14,15弹性力产生的加速度
	double m_db;//16
	double m_dCurve;//17
	double m_fm;//18
	double m_dcCa;//19
	double m_dGrowth;//20
	double m_fi;//21
	double m_fo;//22
	double m_dm;//23

	Segment* m_prev;//24
	Segment* m_next;//25
	Segment* m_next1;//26
	Segment* m_Opposite;//27
	Link* m_link;//28
	int (*m_Left)[2];//29
	int (*m_Right)[2];//30
	
	static double m_ymax;
	static double m_ymin;
	static int* m_iPrjx;
	static int* m_iPrjy;
	static int (*m_iPrjxy)[9];
	static int* m_iRev;
	static double* m_dAngleE;
	static double* m_dCoe;
	static double m_dTau;
	static double m_dInTau;
	static double m_dDensity;
	static double m_dEpsilon;
	static double m_Shrink;//收缩弹性
	static double m_Confine;//限制弹性;
	static double m_ddT;
	static double m_dl;
	static double m_dTheta0;
	//static double m_dm;
	static double m_dffx;
	static double m_dffy;
	static Lattice (*m_Lattice)[HEIGHT];
	static Link m_LinkBuffer[MAXLINK];
public://define function
	int Triger();
	void Stress();
	void ReactionNO(double rr);
	void ReactionCa(double *dCaBuff);
	void CalciumForce();
	void DrawArrow(Vector v1, Vector v2);
	Point FindInerPoint(Vector p1,Vector p2,int iDir);
	static int IsInner(Point p1, Point p2, Vector pp, int iDir);
	void DetermineDirection(int *iDir,Point*FluidNode, Point *SolidNode);
	void Force(int iType=0);
	void HydroAcc();
	void HydroAvr();
	void ElastAcc(int j);
	void VescoAcc();
	void Angle(int iType=0);
	int Curvature(int iType=0);
	double Momentum(int iType=0);
	void Move(int j);
	void Cut();
	void Insert();
	void Nearest(BoundaryLattice *Outer, BoundaryLattice *Inner, int iType=0);
	int LeftToRight(Link* pLink);
	int IsCross(Vector l1, Vector l2, Vector *pp = NULL, int iType=0);
	static int IsCross(Vector p1,Vector n1,Vector p2,Vector n2,Vector* pR);
	int OnSegment(Vector p1,Vector p2,Vector p3);
	static double feqs(int,double,Vector bfv,Vector fv);
	static double Cut(double v);
	int IsOneLink(Vector p1,Vector p2);
	Segment& operator=(const Segment &v);
	friend MPI_File& operator<<(MPI_File& fs, Segment& v);
	friend MPI_File& operator>>(MPI_File& fs, Segment& v);
private:
	void FindRec(int *ixn, int *iyn, int *ixm, int *iym, double *dxn, double *dxm, double *dyn, double *dym, int lb[2][2], int iType=0);
	void FindRec(int *ixn, int *iyn, int *ixm, int *iym, int iType=0);
};

#endif // !defined(AFX_SEGMENT_H__EB79A7E0_E937_11D6_8461_00E04C392A24__INCLUDED_)
