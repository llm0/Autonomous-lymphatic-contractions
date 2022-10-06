#pragma once
#include "VectorAnimate.h"
#include <fstream>
using namespace std;
class LatticeAnimate
{
public:
	float m_cNO;
	float m_dDen;
	VectorAnimate m_v;
	void SaveAnimate(fstream* ff);
	LatticeAnimate& operator=(const LatticeAnimate& v);
	LatticeAnimate();
	~LatticeAnimate();
};

