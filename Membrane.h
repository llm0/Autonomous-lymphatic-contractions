// Membrane.h: interface for the Membrane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBRANE_H__BDFBE8B8_03F0_40EF_A531_8AF9B46BB2DC__INCLUDED_)
#define AFX_MEMBRANE_H__BDFBE8B8_03F0_40EF_A531_8AF9B46BB2DC__INCLUDED_
#include "Segment.h"
#include <fstream>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Membrane  
{
public:
	double m_dx1 = 0;//膜的起点,fixed
	double m_dy1 = 0;
	double m_dx2 = 0;
	double m_dy2 = 0;//膜的终点,fixed
	Segment m_segment[SEGNUM];
	virtual ~Membrane();
	Membrane();
	int Between(int ix, int *i, int *j);
};

#endif // !defined(AFX_MEMBRANE_H__BDFBE8B8_03F0_40EF_A531_8AF9B46BB2DC__INCLUDED_)
