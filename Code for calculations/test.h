#include "lymph_h.h"
#include "Cuda_Vector.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Cuda_Segment.h"
#if !defined(AFX_CUDA_TEST)
#define AFX_CUDA_TEST
#endif 
#pragma once
using namespace std;
class Cuda_Test
{
public:
	int m_id;//
	int m_il;//1
	int m_iType;//段所在膜的种类：上管膜：1，下管膜-1，上瓣膜：2，下瓣膜：-2
	int m_bPassTh;//2
	int m_bCanMove;//
	int m_GetForce;//3 0两面，1:里面,2外面

	Cuda_Vector m_pst;//4,5位置
	Cuda_Vector m_pst_bak;//6,7
	double m_da;//8角度
	double m_da1;//9角度
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
	Cuda_Segment* m_prev;//
	Cuda_Segment* m_next;//26
	Cuda_Segment* m_next1;//
	double* m_Opposite;//27
	Link* m_link;//
	int*m_Left[2];//28
	int*m_Right[2];
	//29,地址是4字节
};