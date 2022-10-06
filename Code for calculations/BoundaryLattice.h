// BoundaryLattice.h: interface for the BoundaryLattice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDARYLATTICE_H__CD772641_5F71_11D7_8461_00E04C392A24__INCLUDED_)
#define AFX_BOUNDARYLATTICE_H__CD772641_5F71_11D7_8461_00E04C392A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Lattice.h"

class BoundaryLattice : public Lattice  
{
public:
	int m_ix;
	int m_iy;
	//double m_dcNO;
	//double m_dcCa;
	double m_da;
	double m_dx;
	double m_dy;
	BoundaryLattice();
	virtual ~BoundaryLattice();
	Lattice& operator=(const BoundaryLattice& v);
	BoundaryLattice& operator=(const Lattice& v);

};

#endif // !defined(AFX_BOUNDARYLATTICE_H__CD772641_5F71_11D7_8461_00E04C392A24__INCLUDED_)
