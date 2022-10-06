#include "lymph_h.h"
#include "Vector.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#if !defined(AFX_CUDA_VECTOR)
#define AFX_CUDA_VECTOR
#endif 
#pragma once
using namespace std;
class Cuda_Vector
{
public:
	double m_dx;
	double m_dy;
	__device__ Cuda_Vector(double x=0, double y=0);
	__device__ Cuda_Vector& operator = (const Cuda_Vector &v);
	__device__ Cuda_Vector& operator = (const Vector &v);
	__device__ Cuda_Vector operator-(Cuda_Vector v);
	__device__ double operator^(const Cuda_Vector &v);
	__device__ double operator*(const Cuda_Vector &v);
};