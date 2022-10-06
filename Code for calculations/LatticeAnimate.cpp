//#include "stdafx.h"
#include "LatticeAnimate.h"


LatticeAnimate::LatticeAnimate()
{
}


LatticeAnimate::~LatticeAnimate()
{
}

LatticeAnimate& LatticeAnimate::operator=(const Lattice& v)
{
	m_cNO = float(v.m_dcNO);
	m_dDen = float(v.m_dDen);
	m_v = v.m_v;
	return *this;
}

void LatticeAnimate::SaveAnimate(fstream* ff)
{
	ff->write((char*) this, sizeof(LatticeAnimate));
}
