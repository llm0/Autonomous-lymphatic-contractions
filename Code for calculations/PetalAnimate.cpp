//#include "stdafx.h"
#include "PetalAnimate.h"


PetalAnimate::PetalAnimate()
{
}


PetalAnimate::~PetalAnimate()
{
}

PetalAnimate& PetalAnimate::operator=(const Petal& v)
{
	m_p0 = *v.m_p0;
	m_Theta = v.m_dTheta;
	return *this;
}