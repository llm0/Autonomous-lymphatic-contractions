#pragma once
#include "VectorAnimate.h"
#include "lymph_h.h"
class LymphAnimate
{
public:
	VectorAnimate m_p[2][SEGNUM];
	float m_Ca[2][SEGNUM];
	float m_NO[2][SEGNUM];
	int m_th[2][SEGNUM];
	LymphAnimate();
	~LymphAnimate();
};

