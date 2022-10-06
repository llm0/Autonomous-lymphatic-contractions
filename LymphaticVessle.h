// LymphaticVessle.h: interface for the LymphaticVessle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LYMPHATICVESSLE_H__E4FAC06A_F41E_4EB1_952A_58E7D4A8B797__INCLUDED_)
#define AFX_LYMPHATICVESSLE_H__E4FAC06A_F41E_4EB1_952A_58E7D4A8B797__INCLUDED_
#include <fstream>
#include "Membrane.h"
#include "LymphAnimate.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LymphaticVessle  
{
public:
	void SaveForMap(fstream* ff);
	void ReadForAnimate(fstream* ff);
	int m_ixn;
	int m_iyn;
	int m_ixm;
	int m_iym;
	Membrane m_MemUp;
	Membrane m_MemDown;
	LymphaticVessle& LymphaticVessle::operator=(const LymphAnimate& v);
	LymphaticVessle();
	int IsInner(int ix, int iy);
	virtual ~LymphaticVessle();

};

#endif // !defined(AFX_LYMPHATICVESSLE_H__E4FAC06A_F41E_4EB1_952A_58E7D4A8B797__INCLUDED_)
