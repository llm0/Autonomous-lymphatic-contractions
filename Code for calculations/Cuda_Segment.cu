#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "lymph_h.h"
#include "math.h"
//#include "StdAfx.h"
#include "Cuda_Segment.h"

__device__ void Cuda_Segment::init(Cuda_Segment* mem, int* iSeg, parameter * p)//链的方向为管外指向管内，管外看为流体
{
	if (*iSeg != 0)
	{
		if (p->iMyID == 0 && *iSeg >= p->iBegin_Mem)
		{
			this->m_prev = NULL;
		}
		else
		{
			this->m_prev = &mem[*iSeg - 1];
		}
	}
	else
	{
		this->m_prev = NULL;
	}
	
	if (*iSeg != SEGNUM_E - 1 )
	{
		if (p->iMyID == p->iNumProcs - 1 && *iSeg == p->iEnd_Mem-1)
		{
			this->m_next = NULL;
		}
		else
		{
			this->m_next = &mem[*iSeg + 1];
		}
	}
	else
	{
		this->m_next = NULL;
	}
}
