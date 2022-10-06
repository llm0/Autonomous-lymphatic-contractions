// Petal.h: interface for the Petal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETAL_H__4B63A259_B949_4066_B53C_1D102AEE7F5E__INCLUDED_)
#define AFX_PETAL_H__4B63A259_B949_4066_B53C_1D102AEE7F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Lattice.h"
#include "Vector.h"
#include "lymph_h.h"
#include "Link.h"
#include "Point.h"
#include "BoundaryLattice.h"
#define MAXLINK 100
#include "Segment.h"

class Petal  
{
public:
	double m_dBelta;
	double m_dElasticMoment;
	double m_dOmigar;
	double m_dTheta;
	double m_dTheta0;
	Vector* m_p0;//0点系于膜上，随膜运动
	Vector m_Apex;
	int m_iType;
	double m_dk;//dk大于100时用m_dk_re;
	double m_dk_re;
	Petal* m_Opposite;
	Segment* m_pSeg;
	double *m_fi;
	double *m_fo;
	static double m_Length;
	static double m_dI;
	static int m_ixn;
	static int m_iyn;
	static int m_ixm;
	static int m_iym;
	static Lattice(*m_lattice)[HEIGHT];
	static int* m_iPrjx;
	static int* m_iPrjy;
	static int(*m_iPrjxy)[9];
	static int* m_iRev;
	static Link* m_link;
	static double m_ddT;
	static double m_ss;

	int CrossMem(Vector v1,Vector v2);
	int OnlyInsideOne(Vector v);
	int Inside(Vector v);
	int OnLeftSide(int i, int j);
	double Requestx(double y);
	void ElasticForce();
	void Nearest(BoundaryLattice *Left,BoundaryLattice *Right,int i);
	int IsCross(Vector l1, Vector l2, Vector *pp);
	void FindLinkInArea(int *iLink);
	void FindLinkAlongLine(int *iLink);
	int Inside(Vector pp,Vector p1,Vector p2);
	void Force(int iSection);
	void TurnRound();
	void ReactionNO(int iSection);
	void ReactionNO(BoundaryLattice* NearLattice);
	friend fstream& operator<<(fstream& fs, Petal& v);
	friend fstream& operator>>(fstream& fs, Petal& v);
	Vector Vertex();
	double Stress(int i, int j);
	Vector Distance(int i, int j);
	void SaveForAnimate(fstream *ff);
	//void Draw(CDC* pDC, CRect* pRect, double k);
	Petal();
	virtual ~Petal();
};

#endif // !defined(AFX_PETAL_H__4B63A259_B949_4066_B53C_1D102AEE7F5E__INCLUDED_)
