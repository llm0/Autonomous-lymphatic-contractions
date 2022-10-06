// Valve.h: interface for the Valve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALVE_H__5C6A1DDF_9783_41D2_9185_14A41A3A13A7__INCLUDED_)
#define AFX_VALVE_H__5C6A1DDF_9783_41D2_9185_14A41A3A13A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"
#include "lymph_h.h"
#include "ValveAnimate.h"


class Valve  
{
public:
	void ReadForAnimate(fstream* ff);
	void SaveForMap(fstream* ff);
	int m_lActive;
	int m_rActive;
	VectorAnimate m_MemDL[VALSEGNUM];
	VectorAnimate m_MemUL[VALSEGNUM];
	VectorAnimate m_MemDR[VALSEGNUM];
	VectorAnimate m_MemUR[VALSEGNUM];
	void Draw(CDC* pDC, CRect* pRect, double k, int Unit);
	Valve& Valve::operator=(const ValveAnimate& v);
	Valve();
	virtual ~Valve();

};

#endif // !defined(AFX_VALVE_H__5C6A1DDF_9783_41D2_9185_14A41A3A13A7__INCLUDED_)
