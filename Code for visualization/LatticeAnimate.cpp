#include "stdafx.h"
#include "LatticeAnimate.h"


LatticeAnimate::LatticeAnimate()
{
}


LatticeAnimate::~LatticeAnimate()
{
}

LatticeAnimate& LatticeAnimate::operator=(const LatticeAnimate& v)
{
	m_cNO = v.	m_cNO;
	m_dDen = v.m_dDen;
	m_v = v.m_v;
	return *this;
}

void LatticeAnimate::SaveAnimate(fstream* ff)
{
	ff->write((char*) this, sizeof(LatticeAnimate));
}
