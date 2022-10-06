#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "lymph_h.h"
#include "math.h"
//#include "StdAfx.h"
#include "Cuda_Link.h"
__device__ Cuda_Link& Cuda_Link:: operator =(const Cuda_Link &v)
{
	m_iSegf = v.m_iSegf;
	m_iSegs = v.m_iSegs;
	m_iType = v.m_iType;
	m_iSolx = v.m_iSolx;
	m_iSoly = v.m_iSoly;
	m_iFlux = v.m_iFlux;
	m_iFluy = v.m_iFluy;
	m_iDir = v.m_iDir;
	m_bf = v.m_bf;
	m_bs = v.m_bs;
	m_dDlf = v.m_dDlf;
	m_dDls = v.m_dDls;
	m_bdb = v.m_bdb;
	return *this;
}