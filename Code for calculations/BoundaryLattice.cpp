// BoundaryLattice.cpp: implementation of the BoundaryLattice class.
//
//////////////////////////////////////////////////////////////////////

#include "BoundaryLattice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BoundaryLattice::BoundaryLattice()
{
	m_dx=0;
	m_dy=0;
	m_da=0;
}

BoundaryLattice::~BoundaryLattice()
{

}

Lattice& BoundaryLattice::operator=(const BoundaryLattice& v)
{
	int i;
	m_dDen=v.m_dDen;
	m_iSol=v.m_iSol;
	m_v=v.m_v;
	for(i=0;i<9;i++)
	{
		m_df[i]=v.m_df[i];
	}
	return *this;
}

BoundaryLattice& BoundaryLattice::operator=(const Lattice& v)
{
	int i;
	m_dDen=v.m_dDen;
	m_dcNO=v.m_dcNO;
	m_iSol=v.m_iSol;
	m_v=v.m_v;
	for(i=0;i<9;i++)
	{
		m_df[i]=v.m_df[i];
	}
	return *this;
}