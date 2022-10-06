#pragma once
#include "lymph_h.h"
#include "PermeatAnimate.h"
#include "ValveAnimate.h"
#include "LatticeAnimate.h"
#include "zlib.h"
#include "LymphAnimate.h"
#define ZLIB_WINAPI
class AnimateData
{
public:
	int m_lNowStep;//linux long占8字节，windows long,int都只占4字节
	PermeatAnimate m_Permeat;
	ValveAnimate m_Valve;
	LatticeAnimate m_Lattice[WIDTH][HEIGHT];
	LymphAnimate m_lymani;
	float m_v[HEIGHT][3];
	static Bytef *m_bf;
	static uLongf m_dl;
	static uLongf m_bfl;
	static uLongf m_bfl0;
	int zip();
	int unzip();
	void StoreV();
public:
	AnimateData();
	~AnimateData();
};

