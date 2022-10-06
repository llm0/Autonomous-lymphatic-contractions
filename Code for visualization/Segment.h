// Segment.h: interface for the Segment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENT_H__EB79A7E0_E937_11D6_8461_00E04C392A24__INCLUDED_)
#define AFX_SEGMENT_H__EB79A7E0_E937_11D6_8461_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Point.h"
#include "math.h"
#include <fstream>
#include "lymph_h.h"
#include "Vector.h"
#define MAXLINK 100
#define PI 3.1415926535897932
class Segment  
{
public:
	Segment();
	virtual ~Segment();
public://define member
	Vector m_pst;//Œª÷√
	double m_dcCa;
	double m_dcNO;
	BOOL m_bPassTh;
private:
};

#endif // !defined(AFX_SEGMENT_H__EB79A7E0_E937_11D6_8461_00E04C392A24__INCLUDED_)
