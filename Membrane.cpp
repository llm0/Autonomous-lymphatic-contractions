// Membrane.cpp: implementation of the Membrane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Membrane.h"
#include "math.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Membrane::Membrane()
{
}

Membrane::~Membrane()
{

}

int Membrane::Between(int ix, int *i, int *j)
{
	int k, m;
	k = int((ix - m_dx1) / 2 + 0.5);//找到第几段膜
	if (ix < m_dx1) return 0;
	for (m = ((k - 2) >= 0) ? k - 1 : 0; m <= ((k + 2 < SEGNUM) ? k + 2 : SEGNUM - 1); m++)//在k前后两个段的范围内找
	{
		if (ix >= m_dx1 + (m - 0)*2&&ix <= m_dx1 + (m + 1 - 0)*2)
		{
			if (m < SEGNUM)
			{
				*i = m; *j = m + 1;
			}
			else
			{
				*i = m - 1; *j = m;
			}
			return 1;
		}
	}
	return 0;
}


