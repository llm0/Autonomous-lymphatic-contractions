#include "lymph_h.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#if !defined(AFX_CUDA_LINK)
#define AFX_CUDA_LINK
#endif 
#pragma once
using namespace std;
class Cuda_Link
{
public:
	int m_iSegf;
	int m_iSegs;
	int m_iType;
	int m_iSolx;
	int m_iSoly;
	int m_iFlux;
	int m_iFluy;
	int m_iDir;
	Cuda_Vector m_bf;//Ò»Á´´©Ë«Ä¤
	Cuda_Vector m_bs;
	double m_dDlf;
	double m_dDls;
	int m_bdb;//Ë«Ä¤±êÖ¾
	__device__ Cuda_Link& operator = (const Cuda_Link &v);
};