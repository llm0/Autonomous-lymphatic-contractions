#pragma once
#include "Vector.h"
class VectorAnimate
{
public:
	float m_fx;
	float m_fy;
	VectorAnimate& operator=(const Vector &v);
public:
	VectorAnimate(float x = 0,float y = 0);
	~VectorAnimate();
};

