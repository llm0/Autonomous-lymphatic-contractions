#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "lymph_h.h"
#include "math.h"
//#include "StdAfx.h"
#include "Cuda_Vector.h"

__device__  Cuda_Vector::Cuda_Vector(double x,double y)
{
	m_dx = x;
	m_dy = y;
}
__device__ Cuda_Vector& Cuda_Vector:: operator =(const Cuda_Vector &v)
{
	m_dx = v.m_dx;
	m_dy = v.m_dy;
	return *this;
}
__device__ Cuda_Vector& Cuda_Vector:: operator =(const Vector &v)
{
	m_dx = v.m_dx;
	m_dy = v.m_dy;
	return *this;
}

__device__ Cuda_Vector Cuda_Vector:: operator-(Cuda_Vector v)
{
	Cuda_Vector R;
	R.m_dx = m_dx - v.m_dx;
	R.m_dy = m_dy - v.m_dy;
	return R;
}

__device__ double Cuda_Vector::operator^(const Cuda_Vector &v)
{
	double dR;
	dR = m_dx*v.m_dy - m_dy*v.m_dx;
	return dR;
}

__device__ double Cuda_Vector::operator*(const Cuda_Vector &v)
{
	double dR;
	dR = m_dx*v.m_dx + m_dy*v.m_dy;
	return dR;
}