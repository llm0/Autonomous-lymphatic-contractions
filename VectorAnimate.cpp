#include "stdafx.h"
#include "VectorAnimate.h"


VectorAnimate::VectorAnimate(double x, double y)
{
	m_fx = x;
	m_fy = y;
}

VectorAnimate VectorAnimate:: operator *(const float &v)
{
	VectorAnimate R;
	R.m_fx = m_fx*v;
	R.m_fy = m_fy*v;
	return R;
}

VectorAnimate::~VectorAnimate()
{
}
