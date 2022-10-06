#pragma once
#include "lymph_h.h"
#include "PermeatAnimate.h"
#include "ValveAnimate.h"
#include "LatticeAnimate.h"
#include "LymphAnimate.h"
#include "zlib.h"
#define ZLIB_WINAPI
class AnimateData
{
public:
	long m_lNowStep;
	PermeatAnimate m_Permeat;
	ValveAnimate m_Valve;
	LatticeAnimate m_Lattice[WIDTH][HEIGHT];
	LymphAnimate m_lymani;
	float m_v[HEIGHT][3];
	static Bytef* m_bf;
	static uLongf m_dl;
	static uLongf m_bfl;
	int zip();
	int unzip();
	void DistributeData();
public:
	AnimateData();
	~AnimateData();
};

