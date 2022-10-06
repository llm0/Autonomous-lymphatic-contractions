// Line.h: interface for the Line class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_LINE_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"
#include "Lattice.h"
#include "Boundary.h"

class Line  
{
public:
	Line(Vector p1=Vector(0,0),Vector p2=Vector(1,1));
	~Line();
public://define members
	Vector m_p1;
	Vector m_p2;
	int m_iHole;//第偶数个洞是半通，第奇数个洞是全通
	int m_iCycle;
	int m_iType; //线种类：上线：2，下线1，竖直左-1，竖直右：-2
	int m_Reserve;
	double m_dk;//dk大于100时用m_dk_re;
	double m_dk_re;
public://define operators
	Line& operator = (const Line &v);
public:
	int IsHole(int ix,int ix0);
	int IsCross(Vector l1, Vector l2, Vector *pp);
	void init();
	double distance(Vector,Vector*);
	void FindLink(int *BouNum,Boundary *Boud );
	int Inside(Vector pp,Vector p1,Vector p2);
	void Oneway();
	double PartialBounce(double in, double out, double dFraction);
	void Pack(char* buffer, int  size, int* position);
	void Unpack(char* buffer, int  size, int* position);
	void recalculate(int i, int j);
	friend MPI_File& operator<<(MPI_File& fs, Line& v);
	friend MPI_File& operator>>(MPI_File& fs, Line& v);
//static void Collide();
};

#endif // !defined(AFX_LINE_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
