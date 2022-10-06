#pragma once
#include "VectorAnimate.h"
#include "PermeatLine.h"
class PermeatAnimate
{
public:
	int m_iHole;
	VectorAnimate m_p[3][2];
	PermeatAnimate& operator=(const PermeatLine& v);
	PermeatAnimate();
	~PermeatAnimate();
};

