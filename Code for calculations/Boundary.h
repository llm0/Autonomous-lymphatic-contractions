#ifndef _BOUNDARY
#define _BOUNDARY
#include "Link.h"

class Boundary
{
 
public:
//	Boundary();
	int m_iLinNum;
	int m_ix;
	int m_iy;


	Link m_link[8];

	Boundary& operator =(const Boundary& v);
};
#endif

