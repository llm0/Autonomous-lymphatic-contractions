// Valve.h: interface for the Valve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALVE_H__5C6A1DDF_9783_41D2_9185_14A41A3A13A7__INCLUDED_)
#define AFX_VALVE_H__5C6A1DDF_9783_41D2_9185_14A41A3A13A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Lattice.h"
#include "Vector.h"
#include "lymph_h.h"
#include "Petal.h"
#include "Link.h"


class Valve  
{
public:
	void SaveForAnimate(fstream *ff);
	BOOL Between(double x,double x1,double x2);
	void OutPutLink();
	BOOL OnLeftSide(int i,int j);
	Petal m_PetalDL;
	Petal m_PetalUL;
	Petal m_PetalDR;
	Petal m_PetalUR;
	Link m_link[LINKNUM];
	int m_il;
	static Lattice (*m_lattice)[HEIGHT];
	static int m_ixn;
	static int m_iyn;
	static int m_ixm;
	static int m_iym;
	static int* m_iPrjx;
	static int* m_iPrjy;
	static int (*m_iPrjxy)[9];
	static int* m_iRev;
	static double m_dTau;
	static double* m_dCoe;
	static double m_dDensity;
	void EndPoint();
	void ElaticRotate();
	void Collide();
	void FindLink();
	void Remark();
	void Force();
	void DrawValve();
	void Move();
	void ReactionNO();
	void init(BOOL bf);
	int OnLine(int ix,int iy,Vector v1,Vector v2);
	void InitMem();
	void Draw(CDC* pDC, CRect* pRect, double k);
	Valve();
	int IsInner(int ix, int iy);
	friend fstream& operator<<(fstream& fs, Valve& v);
	friend fstream& operator>>(fstream& fs, Valve& v);
	virtual ~Valve();

};

#endif // !defined(AFX_VALVE_H__5C6A1DDF_9783_41D2_9185_14A41A3A13A7__INCLUDED_)
