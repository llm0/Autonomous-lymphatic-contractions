// Petal.h: interface for the Petal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETAL_H__4B63A259_B949_4066_B53C_1D102AEE7F5E__INCLUDED_)
#define AFX_PETAL_H__4B63A259_B949_4066_B53C_1D102AEE7F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"
#include "lymph_h.h"
#include "Point.h"
#include "Segment.h"
#include "PetalAnimate.h"
#include <fstream>

class Petal  
{
public:
	Vector m_p0;
	Vector m_Apex;
	double m_dTheta;
	double* m_NO;
	void Draw(CDC* pDC, CRect* pRect,double k);
	Vector Vertex();
	Petal& Petal::operator=(const PetalAnimate& v);
	Petal();
	virtual ~Petal();
	friend fstream& operator>>(fstream& fs, Petal& v);
	static double m_Length;
};

#endif // !defined(AFX_PETAL_H__4B63A259_B949_4066_B53C_1D102AEE7F5E__INCLUDED_)
