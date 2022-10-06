#pragma once
#include "PetalAnimate.h"
#include "lymph_h.h"
#include <fstream>
using namespace std;
class ValveAnimate
{
public:
	BOOL m_lActive;
	BOOL m_rActive;
	VectorAnimate m_p[4][VALSEGNUM];
public:
//	ValveAnimate& ValveAnimate::operator=(const Valve& v);
	ValveAnimate();
	~ValveAnimate();
	void SaveForMap(fstream* ff);
};

