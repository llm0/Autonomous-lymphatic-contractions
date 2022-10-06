#pragma once
#include "VectorAnimate.h"
#include "Petal.h"
class PetalAnimate
{
public:
	VectorAnimate m_p0;
	double m_Theta;
	PetalAnimate& operator=(const Petal& v);
	PetalAnimate();
	~PetalAnimate();
};

