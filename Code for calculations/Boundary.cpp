//#include "stdafx.h"
#include "Boundary.h"
#include "Link.h"
#include "Common.h"
#include "Lattice.h"

/*Boundary::Boundary(int LinNum,int Px,int Py)
{  
	iLinNum=LinNum;
	iPx=Px;
	iPy=Py;
}*/
Boundary& Boundary::operator =(const Boundary& v)
{
	int i;
	m_iLinNum=v.m_iLinNum;
	m_ix=v.m_ix;
	m_iy=v.m_iy;
	for(i=0;i<8;i++)
	{
		m_link[i]=v.m_link[i];
	}
	return *this;
}

