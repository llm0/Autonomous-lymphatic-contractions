// Line.h: interface for the Line class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_LINE_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"

class Line  
{
public:
	Line(Vector p1=Vector(0,0),Vector p2=Vector(1,1));
	virtual ~Line();
public://define members
	Vector m_p1;
	Vector m_p2;
	double m_dk;//dk大于100时用m_dk_re;
	double m_dk_re;
	int m_iHole;//第偶数个洞是半通，第奇数个洞是全通
	int m_iType; //线种类：上线：2，下线1，竖直左-1，竖直右：-2
public://define operators
	Line& operator = (const Line &v);
public:
	BOOL IsHole(int ix,int ix0);
	void init();
//static void Collide();
};

#endif // !defined(AFX_LINE_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
