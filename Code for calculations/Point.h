// Point.h: interface for the Line class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_POINT_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Point  
{
	
public:
	Point(int ix=0,int iy=0);
	virtual ~Point();
public://define members
	int m_ix;
	int m_iy;
public://define operators
	Point& operator = (const Point &v);
};

#endif // !defined(AFX_POINT_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)