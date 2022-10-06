#pragma once
#include "Lattice.h"
#include "VectorAnimate.h"
class LatticeAnimate
{
public:
	float m_cNO;
	float m_dDen;
	VectorAnimate m_v;
	void SaveAnimate(fstream* ff);
	LatticeAnimate& operator=(const Lattice& v);
	LatticeAnimate();
	~LatticeAnimate();
};

