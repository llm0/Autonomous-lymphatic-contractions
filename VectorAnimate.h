#pragma once

class VectorAnimate
{
public:
	float m_fx;
	float m_fy;
public:
	VectorAnimate(double x=0, double y=0);
	VectorAnimate operator*(const float &v);
	~VectorAnimate();
};

