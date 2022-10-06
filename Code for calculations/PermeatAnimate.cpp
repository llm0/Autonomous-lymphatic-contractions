//#include "stdafx.h"
#include "PermeatAnimate.h"


PermeatAnimate& PermeatAnimate::operator=(const PermeatLine& v)
{
	m_iHole = v.m_LineUp.m_iHole;
	m_p[0][0] = v.m_LineUp.m_p1;
	m_p[0][1] = v.m_LineUp.m_p2;
	m_p[1][0] = v.m_LineDown.m_p1;
	m_p[1][1] = v.m_LineDown.m_p2;
	m_p[2][0] = v.m_LineV.m_p1;
	m_p[2][1] = v.m_LineV.m_p2;
	return *this;
}
PermeatAnimate::PermeatAnimate()
{
}


PermeatAnimate::~PermeatAnimate()
{
}
