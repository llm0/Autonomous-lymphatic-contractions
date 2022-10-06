#include "stdafx.h"
#include "Point.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Point::Point(int ix,int iy)
{
	m_ix=ix;
	m_iy=iy;
}
Point::~Point()
{

}

Point& Point::operator=(const Point &v)
{
	m_ix = v.m_ix;
	m_iy = v.m_iy;
	return *this;
}