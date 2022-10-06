#pragma once
#include "PetalAnimate.h"
#include "SolftValve.h"
class ValveAnimate
{
public:
public:
	int m_lActive;
	int m_rActive;
	VectorAnimate m_p[4][VALSEGNUM];
public:
	ValveAnimate& operator=(const SolftValve& v);
	ValveAnimate();
	~ValveAnimate();
};

