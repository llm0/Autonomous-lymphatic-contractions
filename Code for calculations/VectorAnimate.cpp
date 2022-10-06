//#include "stdafx.h"
#include "VectorAnimate.h"


VectorAnimate::VectorAnimate(float x, float y)
{
	m_fx = x;
	m_fy = y;
}


VectorAnimate::~VectorAnimate()
{
}

VectorAnimate& VectorAnimate:: operator =(const Vector &v)
{
	m_fx = float(v.m_dx);
	m_fy = float(v.m_dy);
	return *this;
}
