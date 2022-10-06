#include "lymph_h.h"
#include "Cuda_Link.h"
#include "Cuda_Vector.h"
#include "cudastr.h"
#if !defined(AFX_CUDA_SEGMENT)
#define AFX_CUDA_SEGMENT
#endif 
#pragma once
using namespace std;
class Cuda_Segment
{
public:
	int m_id;//
	int m_il;//1
	int m_iType;//段所在膜的种类：上管膜：1，下管膜-1，上瓣膜：2，下瓣膜：-2
	int m_bCanMove;//,总共256字节
	
	int m_bPassTh;//2
	int m_GetForce;//3
	Cuda_Vector m_pst;//4,5位置
	Cuda_Vector m_pst_bak;//6，7
	double m_da;//8角度
	double m_dal;//9
	Cuda_Vector m_v;//10,11
	Cuda_Vector m_f;//12,13
	Cuda_Vector m_fa;//14,15流体作用产生的加速度
	Cuda_Vector m_ka;//16,17弹性力产生的加速度
	double m_db;//18
	double m_dCurve;//19
	double m_fm;//20
	double m_dcCa;//21
	double m_dGrowth;//22
	double m_fi;//23
	double m_fo;//24
	double m_dm;//25
	Cuda_Segment*  m_prev;//
	Cuda_Segment*  m_next;//26
	Cuda_Segment*  m_next1;//
	Cuda_Segment*  m_Opposite;//27
	Cuda_Link*  m_link;//
	int (*m_Left)[2];//29
	int (*m_Right)[2];//30,地址是4字节


	__device__ void init(Cuda_Segment* mem, int* iSeg, parameter* p);
};