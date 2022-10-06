
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include "lymph_h.h"
#include "kernel.h"
#include "math.h"
//#include "StdAfx.h"
#include "Cuda_Link.cu" 
#include "Cuda_Segment.cu" 
#include "Cuda_Vector.cu" 


#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <fstream>
using namespace std;

double dTest;
int iMemSize = sizeof(lattice_str)*WIDTH_E*HEIGHT;


//parameter* dev_p;//所有的指针都是在host上，cuda分内存在 GPU上
parameter *dev_p;
lattice_str(*dev_a)[HEIGHT] = NULL;
lattice_str(*dev_a_bak)[HEIGHT] = NULL;
double(*dev_NOBuf)[HEIGHT] = NULL;
Cuda_Segment* dev_sm1 = NULL;//up
Cuda_Segment* dev_sm2 = NULL;//down
Cuda_Link *dev_link = NULL;
int* dev_linknum;
lattice_link(*dev_laln)[HEIGHT] = NULL;
int *dev_ixn;
int *dev_iyn;
Valve_str * dev_valve;
double *dev_test;





extern "C" int InitCudaParameter(int gpuid, parameter* InitData, lattice_str la[WIDTH_E][HEIGHT], Cuda_Segment memup[SEGNUM_E],
	Cuda_Segment memdown[SEGNUM_E], double pNO[WIDTH_E][HEIGHT], Cuda_Link* plink, int iMyID)
{
	cudaError_t cudaStatus;
	int devID, iCount = 8;
	cudaDeviceProp deviceProp;
	cudaStatus = cudaGetDeviceCount(&iCount);
	printf("CudaNumber is %d\n", iCount);
	if (iCount > 0)
	{
		devID = (iMyID % 8) % iCount;
	}
	else
	{
		return 0;
	}
	cudaGetDeviceProperties(&deviceProp, devID);
	if (deviceProp.computeMode == cudaComputeModeProhibited)
	{
		fprintf(stderr, "Error: device is running in <Compute Mode Prohibited>, no threads can use ::cudaSetDevice().\n");
		exit(EXIT_SUCCESS);
	}
	cudaStatus = cudaSetDevice(devID);
	printf("Use GPU %d\n", devID);
	cudaStatus = cudaMalloc((void**)&dev_valve, sizeof(Valve_str));
	cudaStatus = cudaMalloc((void**)&dev_p, sizeof(parameter));
	cudaStatus = cudaMemcpy((void**)dev_p, InitData, sizeof(parameter), cudaMemcpyHostToDevice);
	//cudaStatus = cudaMalloc((void**)&dev_a, iMemSize);
	cudaStatus = cudaMalloc((void**)&dev_a_bak, iMemSize);
	cudaStatus = cudaMalloc((void**)&dev_sm1, SEGNUM_E *  sizeof(Cuda_Segment));//分内存
	cudaStatus = cudaMalloc((void**)&dev_sm2, SEGNUM_E *  sizeof(Cuda_Segment));//分内存
	//cudaStatus = cudaMalloc((void**)& dev_NOBuf, WIDTH * HEIGHT* sizeof(double));//分内存
	//cudaStatus = cudaMalloc((void**)& dev_link, LINKNUM* sizeof(Cuda_Link));//分内存
	cudaStatus = cudaMalloc((void**)& dev_linknum, sizeof(int));//分内存
	cudaStatus = cudaMalloc((void**)& dev_laln, WIDTH_E * HEIGHT*sizeof(lattice_link));//分内存
	cudaStatus = cudaMalloc((void**)& dev_ixn, sizeof(int));
	cudaStatus = cudaMalloc((void**)& dev_iyn, sizeof(int));
	cudaStatus = cudaMalloc((void**)& dev_test, sizeof(double));
	cudaStatus = cudaHostRegister(la, iMemSize,
		cudaHostRegisterMapped); if (cudaStatus != cudaSuccess)	{ return 0; }
	//cudaStatus = cudaHostRegister(memup, SEGNUM *  sizeof(Cuda_Segment),
	//	cudaHostRegisterMapped); if (cudaStatus != cudaSuccess)	{ return 0; }
	//cudaStatus = cudaHostRegister(memdown, SEGNUM *  sizeof(Cuda_Segment),
	//	cudaHostRegisterMapped); if (cudaStatus != cudaSuccess)	{ return 0; }
	cudaStatus = cudaHostRegister(pNO, WIDTH_E*HEIGHT*sizeof(double),
		cudaHostRegisterMapped); if (cudaStatus != cudaSuccess)	{ return 0; }
	cudaStatus = cudaHostRegister(plink, LINKNUM* sizeof(Cuda_Link),
		cudaHostRegisterMapped); if (cudaStatus != cudaSuccess)	{ return 0; }
	cudaStatus = cudaHostGetDevicePointer((void**)&dev_a, la, 0); if (cudaStatus != cudaSuccess)	{ return 0; }
	//cudaStatus = cudaHostGetDevicePointer((void**)&dev_sm1, memup, 0); if (cudaStatus != cudaSuccess)	{ return 0; }
	//cudaStatus = cudaHostGetDevicePointer((void**)&dev_sm2, memdown, 0); if (cudaStatus != cudaSuccess)	{ return 0; }
	cudaStatus = cudaHostGetDevicePointer((void**)& dev_NOBuf, pNO, 0); if (cudaStatus != cudaSuccess)	{ return 0; }
	cudaStatus = cudaHostGetDevicePointer((void**)& dev_link, plink, 0); if (cudaStatus != cudaSuccess)	{ return 0; }
	cudaStatus = cudaGetLastError(); if (cudaStatus != cudaSuccess)	{ return 0; }
	if (cudaStatus != cudaSuccess)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//接口函数
extern "C" int CudaCollide()
{
	cudaError_t cudaStatus = addWithCuda(1);//Collide ID:1
	if (cudaStatus != cudaSuccess)
	{
		return 0;
	}
	return 1;
}

extern "C" int CudaCalculate()
{
	cudaError_t cudaStatus = addWithCuda(2);//Calculate ID:2
	if (cudaStatus != cudaSuccess)
	{
		return 0;
	}
	return 1;
}

extern "C" int CudaStream()
{
	cudaError_t cudaStatus = addWithCuda(3);//Stream ID:3
	if (cudaStatus != cudaSuccess)
	{
		return 0;
	}
	return 1;
}

/*extern "C" int CudaStress(Cuda_Segment memup[SEGNUM], Cuda_Segment memdown[SEGNUM])
{
cudaError_t cudaStatus = addWithCuda(4, memup, memdown);//Calculate Stress ID:4
if (cudaStatus != cudaSuccess)
{
return 0;
}
return 1;
}*/


extern "C" int CudaReactionNO(Valve_str * valve)
{
	cudaError_t cudaStatus;
	dim3 Grid(WIDTH, 1, 1);//The Grid includes 1 Block 
	dim3 Threads(HEIGHT, 1, 1);//1 Block includes W*H Threads
	cudaStatus = cudaMemcpy(dev_valve, valve, sizeof(Valve_str), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess)
	{
		return 0;
	}
	addKernelReactionNO << < Grid, Threads >> >(dev_a, dev_sm1, dev_sm2, dev_valve, dev_NOBuf, dev_p);
	cudaDeviceSynchronize();
	cudaStatus = cudaGetLastError();
	if (cudaStatus == cudaSuccess)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

extern "C" int CudaDiffusionNO()
{
	cudaError_t cudaStatus = addWithCuda(5);//Diffusion NO ID:5
	if (cudaStatus != cudaSuccess)
	{
		return 0;
	}
	return 1;
}

extern "C" int CudaFindLink(int * linknum,
	Cuda_Segment memup[SEGNUM_E], Cuda_Segment memdown[SEGNUM_E], parameter* p)
{
	int i;
	int m_ixn, m_ixm, m_iyn, m_iym;
	dim3 Grid(WIDTH, 1, 1);//The Grid includes 1 Block 
	dim3 Threads(HEIGHT, 1, 1);//1 Block includes W*H Threads
	dim3 gg(2, 1, 1), th(SEGNUM_E, 1, 1);
	cudaError_t cudaStatus;
	cudaDeviceSynchronize();
	if (p->iMyID == 0)
	{
		m_ixn = int(memup[LEFTVALVE].m_pst.m_dx); m_ixm = int(memup[LEFTVALVE].m_pst.m_dx); m_iyn = int(memdown[LEFTVALVE].m_pst.m_dy); m_iym = int(memup[LEFTVALVE].m_pst.m_dy);
	}
	else
	{
		m_ixn = int(memup[0].m_pst.m_dx); m_ixm = int(memup[0].m_pst.m_dx); m_iyn = int(memdown[0].m_pst.m_dy); m_iym = int(memup[0].m_pst.m_dy);
	}
	for (i = p->iBegin_Mem; i<p->iEnd_Mem; i++)
	{
		if (m_ixn>memup[i].m_pst.m_dx)
		{
			m_ixn = int(memup[i].m_pst.m_dx);
		}
		if (m_ixm<memup[i].m_pst.m_dx)
		{
			m_ixm = int(memup[i].m_pst.m_dx + 0.5);
		}
		if (m_iyn>memdown[i].m_pst.m_dy)
		{
			m_iyn = int(memdown[i].m_pst.m_dy);
		}
		if (m_iym < memup[i].m_pst.m_dy)
		{
			m_iym = int(memup[i].m_pst.m_dy + 0.5);
		}
	}
	m_ixn = (m_ixn <= p->iBegin_Row) ? p->iBegin_Row : m_ixn--; m_ixm = (m_ixm >= p->iEnd_Row - 1) ? p->iEnd_Row - 1 : m_ixm++;
	m_iyn = (m_iyn <= 0) ? 0 : m_iyn--; m_iym = (m_iym >= HEIGHT - 1) ? HEIGHT - 1 : m_iym++;
	cudaStatus = cudaMemcpy(dev_ixn, &m_ixn, sizeof(int), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(dev_iyn, &m_iyn, sizeof(int), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(dev_sm1, memup, SEGNUM_E *  sizeof(Cuda_Segment), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(dev_sm2, memdown, SEGNUM_E *  sizeof(Cuda_Segment), cudaMemcpyHostToDevice);
	addKernelInitMem << < gg, th >> >(dev_sm1, dev_sm2, dev_p, dev_test);
	cudaDeviceSynchronize();
	Grid.x = m_ixm - m_ixn + 1;
	Threads.x = m_iym - m_iyn + 1;
	i = 0;
	cudaStatus = cudaMemcpy(dev_linknum, &i, sizeof(int), cudaMemcpyHostToDevice);
	addKernelFindLink << < Grid, Threads >> >(dev_a, dev_sm1, dev_sm2,
		dev_laln, dev_ixn, dev_iyn, dev_linknum, dev_p, dev_test);
	cudaDeviceSynchronize();
	addKernelCopyLink << < Grid, Threads >> >(dev_laln, dev_ixn, dev_iyn, dev_link, dev_p, dev_test);
	cudaDeviceSynchronize();
	cudaStatus = cudaMemcpy(linknum, dev_linknum, sizeof(int), cudaMemcpyDeviceToHost);
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess)
	{
		return 0;
	}
	return 1;
}

//Cuda 进程调度
cudaError_t addWithCuda(int iAction)
{
	dim3 Grid(WIDTH, 1, 1);//The Grid includes 1 Block 
	dim3 Threads(HEIGHT, 1, 1);//1 Block includes W*H Threads
	cudaError_t cudaStatus;
	//cudaStatus = cudaMemcpy(dev_a, la, iMemSize, cudaMemcpyHostToDevice); if (cudaStatus != cudaSuccess) goto Error; //考进去
	switch (iAction)
	{
	case 1://ID:1, Collide
		addKernelCollide << < Grid, Threads >> >(dev_a, dev_p);
		cudaStatus = cudaDeviceSynchronize();
		break;
	case 2://ID:2, Calculate
		addKernelCalculate << < Grid, Threads >> >(dev_a, dev_p);
		cudaStatus = cudaDeviceSynchronize();
		break;
	case 3://ID:3, Stream
		cudaStatus = cudaMemcpy(dev_a_bak, dev_a, iMemSize, cudaMemcpyDeviceToDevice);//???
		addKernelStream << < Grid, Threads >> >(dev_a, dev_a_bak, dev_p);
		cudaStatus = cudaDeviceSynchronize();
		break;
	case 4://ID:4, Calculate stress
		/*cudaStatus = cudaMemcpy(dev_sm1, memup, SEGNUM *  sizeof(Cuda_Segment), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_sm2, memdown, SEGNUM *  sizeof(Cuda_Segment), cudaMemcpyHostToDevice);
		addKernelStress << < Grid, Threads >> >(dev_a, dev_sm1, dev_sm2, dev_p);
		cudaStatus = cudaDeviceSynchronize();*/
		break;
	case 5://ID:5, Diffusion NO
		//cudaStatus = cudaMemcpy(dev_NOBuf, pNO, WIDTH * HEIGHT* sizeof(double), cudaMemcpyHostToDevice);
		addKernelDeffusionNO << < Grid, Threads >> >(dev_a, dev_NOBuf, dev_p);
		cudaDeviceSynchronize();//此处必须同步，然后才能计算总NO
		addKernelAddupNO << < Grid, Threads >> >(dev_a, dev_NOBuf, dev_p);
		cudaStatus = cudaDeviceSynchronize();
		break;
	default:;
	}
	cudaStatus = cudaDeviceSynchronize();// if (cudaStatus != cudaSuccess) goto Error;
	//cudaStatus = cudaMemcpy(la, dev_a, iMemSize, cudaMemcpyDeviceToHost); //if (cudaStatus != cudaSuccess) goto Error;
	if (iAction == 5)
	{
		//cudaMemcpy(pNO, dev_NOBuf, WIDTH * HEIGHT* sizeof(double), cudaMemcpyDeviceToHost);
	}
	return cudaStatus;
}

__device__ void feq(double* dR, int Order, double Den, double Velx, double Vely, parameter* p)
{
	double dfeq;
	double dDotMet;
	dDotMet = p->iPrjx[Order] * Velx + p->iPrjy[Order] * Vely;
	dfeq = Den*p->dCoe[Order] * (1 + 3 * dDotMet + 4.5*dDotMet*dDotMet -
		1.5*(Velx*Velx + Vely*Vely));
	*dR = dfeq;
}

__global__ void addKernelCollide(lattice_str  la[WIDTH_E][HEIGHT], parameter* p)
{
	int i, j, k;
	double dfeq;
	i = blockIdx.x + p->iBegin_Row;
	j = threadIdx.x;
	if (i < p->iEnd_Row&&j < HEIGHT)
	{
		if (la[i][j].m_iSol < 0 && la[i][j].m_ipr != 2)
		{
			for (k = 0; k < 9; k++)
			{
				feq(&dfeq, k, la[i][j].m_dDen, la[i][j].m_v.m_dx, la[i][j].m_v.m_dy, p);
				la[i][j].m_df[k] = la[i][j].m_df[k] - (la[i][j].m_df[k] - dfeq) / p->Tau
					+ p->dGravity*cos(p->dGravityAngle)*la[i][j].m_dDen* p->iPrjx[k] / 6.0
					+ p->dGravity*sin(p->dGravityAngle)*la[i][j].m_dDen* p->iPrjy[k] / 6.0;
			}
		}
	}
}

__global__ void addKernelCalculate(lattice_str  la[WIDTH_E][HEIGHT], parameter* p)//已经用寄存器变更缓存全局变量
{
	int i, j, k;
	double dfeq;
	i = blockIdx.x + p->iBegin_Row;
	j = threadIdx.x;
	la[i][j].m_bReaction = 0;//0
	double dDen = 0, dvx = 0, dvy = 0, df[9];//定义局部变量，Cuda将作寄存器处理
	if (la[i][j].m_iSol < 0 && la[i][j].m_ipr != 2)
	{
		for (k = 0; k < 9; k++)
		{
			df[k] = la[i][j].m_df[k];
		}
		for (k = 0; k<9; k++)
		{
			dDen += df[k];
			dvx += df[k] * p->iPrjx[k];
			dvy += df[k] * p->iPrjy[k];
		}
		if (dDen >10)
		{
			printf("Too large Density Proc = %d, x= %d,y=%d,d= %f\n", p->iMyID, i,j,dDen);
		}
		dvx /= dDen;
		dvy /= dDen;
		la[i][j].m_dDen = dDen;
		la[i][j].m_v.m_dx = dvx;
		la[i][j].m_v.m_dy = dvy;
	}
	else
	{
		la[i][j].m_dDen = 1000;
		la[i][j].m_v.m_dx = 0;
		la[i][j].m_v.m_dy = 0;
		for (k = 0; k < 9; k++)
		{
			la[i][j].m_df[k] = 1000;
		}
	}
	if (j<CENTER + 2 && j>CENTER - 2 && i>p->iBegin_Row + 2 && j < p->iEnd_Row - 2)//单独一点夹在两膜间
	{
		if (la[i][j].m_iSol == -3 && la[i][j + 1].m_iSol != -3
			&& la[i][j - 1].m_iSol != -3 && la[i - 1][j + 1].m_iSol != -3
			&& la[i - 1][j - 1].m_iSol != -3)
		{
			la[i][j].m_dDen = (la[i][j + 1].m_dDen + la[i][j - 1].m_dDen) / 2;
			for (k = 0; k < 9; k++)
			{
				feq(&dfeq, k, la[i][j].m_dDen, 0, 0, p);
				la[i][j].m_df[k] = dfeq;
			}
			la[i][j].m_v.m_dx = 0;
			la[i][j].m_v.m_dy = 0;
		}
	}
}

__global__ void addKernelStream(lattice_str  la[WIDTH_E][HEIGHT], lattice_str  la_bak[WIDTH_E][HEIGHT], parameter* p)//无法加速
{
	int i, j, k;
	i = blockIdx.x + p->iBegin_Row;
	j = threadIdx.x;
	if (la[i][j].m_iSol < 0 && la[i][j].m_ipr != 2)
	{
		for (k = 1; k < 9; k++)
		{
			la[i][j].m_df[k] = la_bak[i - p->iPrjx[k]][j - p->iPrjy[k]].m_df[k];
		}
	}
}

__global__ void addKernelStress(lattice_str  la[WIDTH][HEIGHT], Cuda_Segment sm1[SEGNUM], Cuda_Segment sm2[SEGNUM], parameter* p)//不用了
{
	int i, j;
	double v0, vj, vjj, dely;
	i = blockIdx.x;
	j = threadIdx.x;
	if (j == 0)//下边界
	{
		la[i][j].m_Stress = la[i][j + 1].m_v.m_dx - 0;
	}
	else if (j == HEIGHT - 1)//上边界
	{
		la[i][j].m_Stress = 0 - la[i][HEIGHT - 2].m_v.m_dx;
	}//j=1~H-2
	else if (la[i][j + 1].m_iSol == la[i][j - 1].m_iSol || j == 1 || j == HEIGHT - 2)//同种区域内
	{
		la[i][j].m_Stress = (la[i][j + 1].m_v.m_dx - la[i][j - 1].m_v.m_dx) / 2.0;
	}//j=1~H-2
	else if (la[i][j + 1].m_iSol*la[i][j - 1].m_iSol == 2 && la[i][j].m_iSol<0)//-1,-2
	{
		if (i >= sm1[0].m_pst.m_dx)//不同种区域在膜的周围
		{
			if (j>HEIGHT / 2)
			{
				dely = VirticalDistance(sm1, i, j);//上膜
			}
			else
			{
				dely = VirticalDistance(sm2, i, j);//下膜
			}
			vj = la[i][j].m_v.m_dx;
			if (la[i][j].m_iSol == la[i][j + 1].m_iSol)
			{
				if (fabs(dely) < 0.1)
				{
					la[i][j].m_Stress = la[i][j + 1].m_v.m_dx - la[i][j].m_v.m_dx;
				}
				else
				{
					vjj = la[i][j + 1].m_v.m_dx;
					v0 = vj*(1 - dely) + dely*vjj;
					la[i][j].m_Stress = v0 / (2 * dely);
				}
			}
			else
			{
				if (fabs(dely) < 0.1)
				{
					la[i][j].m_Stress = la[i][j].m_v.m_dx - la[i][j - 1].m_v.m_dx;
				}
				else
				{
					vjj = la[i][j - 1].m_v.m_dx;
					v0 = vj*(1 - dely) + dely*vjj;
					la[i][j].m_Stress = -v0 / (2 * dely);
				}
			}
		}
	}//-1,-2,//j=1~H-2
	else if (la[i][j + 1].m_iSol >= 0 && la[i][j - 1].m_iSol < 0 && la[i][j].m_iSol < 0)//0,4
	{
		la[i][j].m_Stress = (0 - la[i][j - 1].m_v.m_dx) / 2.0;
	}
	else if (la[i][j - 1].m_iSol >= 0 && la[i][j + 1].m_iSol < 0 && la[i][j].m_iSol < 0)//0,4
	{
		la[i][j].m_Stress = (la[i][j + 1].m_v.m_dx - 0) / 2.0;
	}
	else if (la[i][j + 1].m_iSol >= 0 && la[i][j - 1].m_iSol >= 0 && la[i][j].m_iSol<0)
	{
		la[i][j].m_Stress = 0;
	}//j=1~H-2
	else if (j>HEIGHT / 2)//j=H/2~H-2
	{
		if (la[i][j - 1].m_iSol < 0)
		{
			if (la[i][j - 1].m_iSol == la[i][j - 2].m_iSol&&la[i][j - 1].m_iSol == la[i][j - 3].m_iSol)
			{
				la[i][j].m_Stress = (la[i][j - 3].m_v.m_dx - la[i][j - 1].m_v.m_dx) / 2 - la[i][j - 2].m_v.m_dx;//外推
			}
			else
			{
				la[i][j].m_Stress = 0 - la[i][j - 1].m_v.m_dx;
			}
		}
		else
		{
			la[i][j].m_Stress = 0;
		}
	}//j=1~H/2
	else
	{
		if (la[i][j + 1].m_iSol < 0)
		{
			if (la[i][j + 1].m_iSol == la[i][j + 2].m_iSol&&la[i][j + 1].m_iSol == la[i][j + 3].m_iSol)
			{
				la[i][j].m_Stress = la[i][j + 2].m_v.m_dx - (la[i][j + 3].m_v.m_dx - la[i][j + 1].m_v.m_dx) / 2;//外推
			}
			else
			{
				la[i][j].m_Stress = la[i][j + 1].m_v.m_dx - 0;//是否外推一下？
			}
		}
		else
		{
			la[i][j].m_Stress = 0;
		}
	}
}


__global__ void addKernelDeffusionNO(lattice_str  la[WIDTH_E][HEIGHT], double  NOBuf[WIDTH_E][HEIGHT], parameter* p)//已经用缓存加速
{
	int i, j;
	double dDeltaNO, dLaplace;
	Cuda_Vector Grad, v;
	i = blockIdx.x + p->iBegin_Row;
	j = threadIdx.x;
	double Nij, N1j, Ni1, N2j, Ni2;
	Nij = la[i][j].m_dcNO;
	N1j = (i <= p->iBegin_Row&&p->iMyID == 0) ? 0 : la[i - 1][j].m_dcNO;
	Ni1 = (j > 0) ? la[i][j - 1].m_dcNO : 0;
	N2j = (i >= p->iEnd_Row - 1 && p->iMyID == p->iNumProcs - 1) ? 0 : la[i + 1][j].m_dcNO;
	Ni2 = (j<HEIGHT - 1) ? la[i][j + 1].m_dcNO : 0;
	v = la[i][j].m_v;
	if (i >= ((p->iMyID == 0) ? p->iBegin_Row + 1 : p->iBegin_Row)
		&& i <= ((p->iMyID == p->iNumProcs - 1) ? p->iEnd_Row - 2 : p->iEnd_Row - 1)
		&& j>0 && j < HEIGHT - 1)//1 - W-2, 1 - H-2
	{
		dLaplace = N1j - 2 * Nij + N2j + Ni1 - 2 * Nij + Ni2;
		if (v.m_dx>0)
		{
			Grad.m_dx = Nij - N1j;
		}
		else
		{
			Grad.m_dx = N2j - Nij;
		}
		if (v.m_dy > 0)
		{
			Grad.m_dy = Nij - Ni1;
		}
		else
		{
			Grad.m_dy = Ni2 - Nij;
		}
		dDeltaNO = p->NOSpread*dLaplace - p->NODecay*Nij - v*Grad;//dks
		NOBuf[i][j] += dDeltaNO;
	}//中央
	else if (j == 0 && ((p->iMyID == 0) ? i != p->iBegin_Row : 1)
		&& ((p->iMyID == p->iNumProcs - 1) ? i != p->iEnd_Row - 1 : 1))//j=0, i!=0,i!=w-1,除去两头的下水平线
	{
		NOBuf[i][0] += p->NOSpread*(Ni2 - 3 * Nij + N2j + N1j) - p->NODecay*Nij;
	}//下
	else if (j == HEIGHT - 1 && ((p->iMyID == 0) ? i != p->iBegin_Row : 1)
		&& ((p->iMyID == p->iNumProcs - 1) ? i != p->iEnd_Row - 1 : 1))//j=H-1,i!=0,i!=W-1,除去两头的上水平线
	{
		NOBuf[i][HEIGHT - 1] += p->NOSpread*(Ni1 - 3 * Nij + N2j + N1j) - p->NODecay*Nij;
	}//上
	else if (i == p->iBegin_Row &&p->iMyID == 0 && j != 0 && j != HEIGHT - 1)//除去两头的左竖直线
	{
		NOBuf[i][j] += p->NOSpread*(N2j - 3 * Nij + Ni2 + Ni1) - p->NODecay*Nij;
	}//左
	else if (i == p->iEnd_Row - 1 && p->iMyID == p->iNumProcs - 1 && j != 0 && j != HEIGHT - 1)//除去两头的右竖直线
	{
		NOBuf[i][j] += p->NOSpread*(N1j - 3 * Nij + Ni2 + Ni1) - p->NODecay*Nij;
	}//右
	else if (i == p->iBegin_Row && p->iMyID == 0 && j == 0)//i=0,j=0，左下
	{
		NOBuf[i][j] += p->NOSpread*(Ni2 - 2 * Nij + N2j) - p->NODecay*Nij;
	}//左下
	else if (i == p->iBegin_Row && p->iMyID == 0 && j == HEIGHT - 1)//i=0,j=H-1，左上
	{
		NOBuf[i][j] += p->NOSpread*(Ni1 - 2 * Nij + N2j) - p->NODecay*Nij;
	}//左上
	else if (i == p->iEnd_Row - 1 && p->iMyID == p->iNumProcs - 1 && j == 0)//右下
	{
		NOBuf[i][j] += p->NOSpread*(Ni2 - 2 * Nij + N1j) - p->NODecay*Nij;
	}//右下
	else if (i == p->iEnd_Row - 1 && p->iMyID == p->iNumProcs - 1 && j == HEIGHT - 1)//右上
	{
		NOBuf[i][j] += p->NOSpread*(Ni1 - 2 * Nij + N1j) - p->NODecay*Nij;
	}//右上
	else
	{
		printf("NO spreading Error\n");
	}
}

__global__ void addKernelAddupNO(lattice_str  la[WIDTH_E][HEIGHT], double  NOBuf[WIDTH_E][HEIGHT], parameter* p)
{
	int i, j;
	i = blockIdx.x + p->iBegin_Row;
	j = threadIdx.x;
	double N;
	N = la[i][j].m_dcNO + NOBuf[i][j];//从反冲区再拷过来
	if (N<0)
	{
		//printf("NO<0,No=%f,i=%d,j=%d,type=%d,ID=%d,set to 0\n", NOBuf[i][j], i, j, la[i][j].m_bReaction,p->iMyID);
		N = 0;
	}
	/*else if (N>1)
	{
		N = 1;
	}*/
	if (i == p->iBegin_Row + 1 && p->iMyID == 0 || j == 1 || i == p->iEnd_Row - 2 && p->iMyID == p->iNumProcs - 1 || j == HEIGHT - 2)//0 边界
	{
		N = 0;
	}
	la[i][j].m_dcNO = N;
}

__global__ void addKernelInitMem(Cuda_Segment* mem1, Cuda_Segment* mem2, parameter * p, double * Test)
{
	int i, j;
	i = blockIdx.x;
	j = threadIdx.x;
	if (i == 0)
	{
		mem1[j].init(mem1, &j, p);
	}
	else if (i == 1)
	{
		mem2[j].init(mem2, &j, p);
	}
}

__global__ void addKernelFindLink(lattice_str  la[WIDTH_E][HEIGHT], Cuda_Segment* mem1, Cuda_Segment* mem2, lattice_link link[WIDTH_E][HEIGHT],
	int *ixn, int* iyn, int * linknum, parameter* p, double * Test)
{
	int i, j, L, m, iSol, iFol, iSeg;
	int iFlux, iFluy;
	//cudaError_t cudaStatus;
	double ddy;
	int iNearest = 0;
	Cuda_Vector pp, v1, v2;
	lattice_link blink;
	i = blockIdx.x + (*ixn);
	j = threadIdx.x + (*iyn);
	link[i][j].m_i = 0;
	blink = link[i][j];
	blink.m_i = 0;
	iSol = la[i][j].m_iSol; //*Test = mem1[2].m_pst.m_dx;
	if (iSol == -1 || iSol == 0 || iSol == 4 && OnMem(i, j, mem1, mem2) || iSol == -3)
	{
		iSeg = BEGINMEM + int(double(i - p->iBegin_Row)*(p->iEnd_Mem - 1 - BEGINMEM)
			/ (p->iEnd_Row - 1 - p->iBegin_Row) + 0.5);
		for (m = 1; m < 9; m++)//m的方向由固体指向流体
		{
			iFlux = i + p->iPrjx[m];  //fluid node -1~n
			iFluy = j + p->iPrjy[m];  //iFluy -1~n
			iNearest = iSeg;
			if (iSeg < p->iEnd_Mem - 1)
			{
				if (iFluy >= (HEIGHT - 1) / 2)
				{
					ddy = fabs(mem1[iSeg + 1].m_pst.m_dy + (mem1[iSeg].m_pst.m_dy
						- mem1[iSeg + 1].m_pst.m_dy) / (mem1[iSeg].m_pst.m_dx
						- mem1[iSeg + 1].m_pst.m_dx)*(iFlux - mem1[iSeg + 1].m_pst.m_dx) - iFluy);
				}
				else
				{
					ddy = fabs(mem2[iSeg + 1].m_pst.m_dy + (mem2[iSeg].m_pst.m_dy
						- mem2[iSeg + 1].m_pst.m_dy) / (mem2[iSeg].m_pst.m_dx
						- mem2[iSeg + 1].m_pst.m_dx)*(iFlux - mem2[iSeg + 1].m_pst.m_dx) - iFluy);
				}
			}
			iFol = la[iFlux][iFluy].m_iSol;
			if (i < mem1[p->iBegin_Mem].m_pst.m_dx&&iFlux < mem1[p->iBegin_Mem].m_pst.m_dx
				|| i < mem1[p->iBegin_Mem].m_pst.m_dx&&iFlux >= mem1[p->iBegin_Mem].m_pst.m_dx&&
				iFol == -2 && iSol == -1 || i >= mem1[p->iBegin_Mem].m_pst.m_dx&&iFlux
				< mem1[p->iBegin_Mem].m_pst.m_dx&&
				iSol == -2 && iFol == -1)
			{
				continue;
			}
			if (InRegionX(iFlux, p) && iFluy >= 0 && iFluy < HEIGHT)
			{
				if (iSol == -1 && iFol == -2
					|| iSol == 0 && iFol < 0
					|| iSol == 4 && iFol < 0
					|| iSol == -3 && iFol == -2) //由管内指向管外
				{
					blink.m_ln[blink.m_i].m_iSolx = i;
					blink.m_ln[blink.m_i].m_iSoly = j;
					blink.m_ln[blink.m_i].m_iFlux = iFlux;
					blink.m_ln[blink.m_i].m_iFluy = iFluy;
					blink.m_ln[blink.m_i].m_iDir = p->iRev[m];//调整为从流体指向固体
					blink.m_ln[blink.m_i].m_iSegf = -1;
					for (L = (p->iMyID == 0) ? p->iBegin_Mem : p->iBegin_Mem - 1; L < ((p->iMyID == p->iNumProcs - 1) ? p->iEnd_Mem : p->iEnd_Mem + 1); L++)
					{
						if (iFluy >= (HEIGHT - 1) / 2)
						{
							v1.m_dx = iFlux; v1.m_dy = iFluy;
							v2.m_dx = i; v2.m_dy = j;
							if (_IsCross(la, &v1, &v2, &(mem1[L]), mem1[L].m_next, &pp, Test))
							{
								blink.m_ln[blink.m_i].m_bf = pp;
								blink.m_ln[blink.m_i].m_iSegf = L;
								blink.m_ln[blink.m_i].m_iType = 1;//链在上边膜
								if (blink.m_ln[blink.m_i].m_iDir != 3 && blink.m_ln[blink.m_i].m_iDir != 4)//非竖直方向
								{
									blink.m_ln[blink.m_i].m_dDlf = fabs(blink.m_ln[blink.m_i].m_bf.m_dx - blink.m_ln[blink.m_i].m_iFlux);
								}
								else
								{
									blink.m_ln[blink.m_i].m_dDlf = fabs(blink.m_ln[blink.m_i].m_bf.m_dy - blink.m_ln[blink.m_i].m_iFluy);
								}
								break;
							}
						}
						else
						{
							v1.m_dx = iFlux; v1.m_dy = iFluy;
							v2.m_dx = i; v2.m_dy = j;
							if (_IsCross(la, &v1, &v2, &(mem2[L]), mem2[L].m_next, &pp, Test))
							{
								blink.m_ln[blink.m_i].m_bf = pp; 
								blink.m_ln[blink.m_i].m_iSegf = L;
								blink.m_ln[blink.m_i].m_iType = -1;//链在下边膜
								if (blink.m_ln[blink.m_i].m_iDir != 3 && blink.m_ln[blink.m_i].m_iDir != 4)//非竖直方向
								{
									blink.m_ln[blink.m_i].m_dDlf = fabs(blink.m_ln[blink.m_i].m_bf.m_dx - blink.m_ln[blink.m_i].m_iFlux);
								}
								else
								{
									blink.m_ln[blink.m_i].m_dDlf = fabs(blink.m_ln[blink.m_i].m_bf.m_dy - blink.m_ln[blink.m_i].m_iFluy);
								}	
								break;
							}
						}
						if (L == p->iEnd_Mem - 1 && (p->iMyID == p->iNumProcs - 1 || p->iMyID == 0))//固体点在段上端点，实数无法判断
						{
							if (iSol >= 0)//在段上端点
							{
								blink.m_ln[blink.m_i].m_bf = Cuda_Vector(i, j);
								blink.m_ln[blink.m_i].m_iSegf = iNearest;
								blink.m_ln[blink.m_i].m_dDlf = 1;//边界刚好在固体点上;
								if (iSol == 0 && (iFol == -3 || iFol == -1))
								{
									la[iFlux][iFluy].m_bReaction = 1;
								}
							}
							else if (iSol == -1 || iSol == -3)//非常靠近(i,j),计算误差无法判断
							{
								blink.m_ln[blink.m_i].m_bf = Cuda_Vector(i, j);
								blink.m_ln[blink.m_i].m_iSegf = iNearest;
								blink.m_ln[blink.m_i].m_dDlf = 1;//边界刚好在固体点上;
								la[i][j].m_bReaction = 1;
							}
							else
							{
								printf("boundary not find Procs=%d,x=%d,y=%d,fx=%d,fy=%d,s0y=%f,s1y=%f,s2y=%f,is=%d,if=%d,%f\n", p->iMyID, i, j, iFlux, iFluy, mem1[2].m_pst.m_dy, mem2[154].m_pst.m_dy, mem2[155].m_pst.m_dy, iSol, iFol, ddy);
							}
						}
					}//for L
					if (iSol == -1 || (iSol == -3 && iFol == -2))
					{
						la[i][j].m_bReaction = 1;//内膜要产NO
					}
					else if (iSol == 0 && (iFol == -3 || iFol == -1))
					{
						la[iFlux][iFluy].m_bReaction = 1;
					}
					else if ((iSol == 0 || iSol == 4) &&
						la[i - p->iPrjx[m]][j - p->iPrjy[m]].m_iSol == -1)
					{
						la[i - p->iPrjx[m]][j - p->iPrjy[m]].m_bReaction = 1;
					}
					if (blink.m_ln[blink.m_i].m_iSegf >= 0)
					{
						blink.m_i++;
					}
					if (blink.m_i > LALNNUM)
					{
						printf("Link Number> LALNNUM\n");
					}
				}//if
			}//if
		}//for m
		link[i][j] = blink;
		link[i][j].m_ior = atomicAdd(linknum, blink.m_i);
	}
}


__device__ int OnMem(int i, int j, Cuda_Segment* mem1, Cuda_Segment* mem2)
{
	if (SamePoint(i, j, mem1[LEFTVALVE].m_pst) ||
		SamePoint(i, j, mem1[RIGHTVALVE].m_pst) ||
		SamePoint(i, j, mem2[LEFTVALVE].m_pst) ||
		SamePoint(i, j, mem2[RIGHTVALVE].m_pst))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

__device__ int SamePoint(int i, int j, Cuda_Vector v)
{
	if (i >= v.m_dx&&i <= v.m_dx&&j >= v.m_dy&&j <= v.m_dy)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

__global__ void  addKernelCopyLink(lattice_link link[WIDTH_E][HEIGHT], int *ixn, int* iyn, Cuda_Link* link0, parameter* p, double* Test)
{
	int i, j, k;
	i = blockIdx.x + *ixn;
	j = threadIdx.x + *iyn;
	for (k = 0; k < link[i][j].m_i; k++)
	{
		link0[link[i][j].m_ior + k] = link[i][j].m_ln[k];
	}
}


__global__ void addKernelReactionNO(lattice_str  la[WIDTH_E][HEIGHT], Cuda_Segment* mem1, Cuda_Segment* mem2, Valve_str * valve, double  NOBuf[WIDTH_E][HEIGHT], parameter* p)//1:膜,2:内点,3:外点
{
	int i, j, iSeg;
	double dks, dProduct, dstr = 0, dx;
	i = blockIdx.x + p->iBegin_Row;
	j = threadIdx.x;
	dks = p->dkNO*p->h;
	if (la[i][j].m_bReaction > 0)
	{
		if (la[i][j].m_bReaction == 1)//上下膜
		{
			iSeg = BEGINMEM + int(double(i - p->iBegin_Row)*(p->iEnd_Mem - 1 - BEGINMEM)
				/ (p->iEnd_Row - 1 - p->iBegin_Row) + 0.5);
			if (iSeg >= 0 && iSeg < SEGNUM_E)
			{
				if (j >= CENTER)
				{
					dstr = mem1[iSeg].m_fi;
				}
				else
				{
					dstr = mem2[iSeg].m_fi;
				}
			}
			else
			{
				printf("SegNum Wrong : %d\n", iSeg);
			}
		}
		else if (la[i][j].m_bReaction == 2)//valve内
		{
			dx = (H2 - H1) * 2.0 / VALSEGNUM;
			if (valve->m_lActive &&i < mem1[RIGHTVALVE].m_pst.m_dx)//左
			{
				iSeg = int((i - valve->m_dLx0) / dx + 0.5);
				if (iSeg >= 0 && iSeg < VALSEGNUM)
				{
					if (j >= CENTER)//上
					{
						dstr = valve->m_dULfi[iSeg];
					}
					else
					{
						dstr = valve->m_dDLfi[iSeg];
					}
				}
				else
				{
					printf("Valve left SegNum Wrong : %d\n", iSeg);
				}
			}
			else  if (valve->m_rActive)
			{
				iSeg = int((i - valve->m_dRx0) / dx + 0.5);
				if (iSeg >= 0 && iSeg < VALSEGNUM)
				{
					if (j >= CENTER)//上
					{
						dstr = valve->m_dURfi[iSeg];
					}
					else
					{
						dstr = valve->m_dDRfi[iSeg];
					}
				}
				else
				{
					printf("Valve right SegNum Wrong : %d\n", iSeg);
				}
			}
		}
		else if (la[i][j].m_bReaction == 3)//valve外
		{
			dx = (H2 - H1) * 2.0 / VALSEGNUM;
			if (valve->m_lActive &&i < mem1[RIGHTVALVE].m_pst.m_dx)//左
			{
				iSeg = int((i - valve->m_dLx0) / dx + 0.5);
				if (iSeg >= 0 && iSeg < VALSEGNUM)
				{
					if (j >= CENTER)//上
					{
						dstr = valve->m_dULfo[iSeg];
					}
					else
					{
						dstr = valve->m_dDLfo[iSeg];
					}
				}
				else
				{
					printf("Valve right SegNum Wrong : %d\n", iSeg);
				}
			}
			else if (valve->m_rActive)
			{
				iSeg = int((i - valve->m_dRx0) / dx + 0.5);
				if (iSeg >= 0 && iSeg < VALSEGNUM)
				{
					if (j >= CENTER)//上
					{
						dstr = valve->m_dURfo[iSeg];
					}
					else
					{
						dstr = valve->m_dDRfo[iSeg];
					}
				}
				else
				{
					printf("Valve right SegNum Wrong : %d\n", iSeg);
				}
			}
		}
		else
		{
			dstr = 0;
		}
		dProduct = dks*fabs(dstr) / (la[i][j].m_dDen*(2 * p->Tau - 1) / 6);
		/*if (!(la[i][j].m_dcNO <= LIMITNO) && dProduct > p->NODecay)
		{
			dProduct = p->NODecay;
		}*/
		NOBuf[i][j] = dProduct;
		la[i][j].m_bReaction = 0;
	}
	else
	{
		NOBuf[i][j] = 0;
	}
}



__device__ int  _IsCross(lattice_str  la[WIDTH_E][HEIGHT], Cuda_Vector* l1, Cuda_Vector* l2, Cuda_Segment* seg, Cuda_Segment* next, Cuda_Vector *pp, double *Test)//判断s1s2和l1l2的交点
{
	Cuda_Vector s1, s2;
	if (next != NULL)
	{
		double f1, f2, f3, f4;
		double a1, b1, c1, a2, b2, c2;
		s1 = seg->m_pst;
		s2 = next->m_pst;
		a1 = s2.m_dy - s1.m_dy;
		b1 = -(s2.m_dx - s1.m_dx);
		c1 = -a1*s1.m_dx - b1*s1.m_dy;
		a2 = l2->m_dy - l1->m_dy;
		b2 = -(l2->m_dx - l1->m_dx);
		c2 = -a2*l1->m_dx - b2*l1->m_dy;
		f1 = (*l1 - s1) ^ (s2 - s1);//s1,l1,s2
		f2 = (*l2 - s1) ^ (s2 - s1);//s1,l2,s2
		f3 = (s2 - *l1) ^ (*l2 - *l1);//l1,s2,l2
		f4 = (s1 - *l1) ^ (*l2 - *l1);//l1,s1,l2
		if (f1*f2 < 0 && f3*f4<0 || f1 == 0 && _OnSegment(&s1, &s2, l1) || f2 == 0 && _OnSegment(&s1, &s2, l2)
			|| f3 == 0 && _OnSegment(l1, l2, &s2) || f4 == 0 && _OnSegment(l1, l2, &s1))//s2上的不要
		{
			if (pp != NULL)
			{
				if (a1*b2 - a2*b1>0 || a1*b2 - a2*b1 < 0)
				{
					pp->m_dx = (c2*b1 - c1*b2) / (a1*b2 - a2*b1);
					pp->m_dy = (c1*a2 - c2*a1) / (a1*b2 - a2*b1);
					pp->m_dx = _ErrorCorrect(pp->m_dx, l1->m_dx, l2->m_dx);
					pp->m_dy = _ErrorCorrect(pp->m_dy, l1->m_dy, l2->m_dy);
				}
				else
				{
					if (la[int(l1->m_dx)][int(l1->m_dy)].m_iSol == -1)//两线重叠，直接认为交点在固体点上？？？
					{
						*pp = *l1;
					}
					else
					{
						*pp = *l2;
					}
				}
			}
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

__device__ int  _OnSegment(Cuda_Vector* p1, Cuda_Vector* p2, Cuda_Vector* p3)
{
	double dxm, dxn, dym, dyn;
	if (p1->m_dx < p2->m_dx)
	{
		dxn = p1->m_dx;
		dxm = p2->m_dx;
	}
	else
	{
		dxn = p2->m_dx;
		dxm = p1->m_dx;
	}
	if (p1->m_dy < p2->m_dy)
	{
		dyn = p1->m_dy;
		dym = p2->m_dy;
	}
	else
	{
		dyn = p2->m_dy;
		dym = p1->m_dy;
	}
	if (p3->m_dx<dxn || p3->m_dx>dxm || p3->m_dy<dyn || p3->m_dy>dym)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

__device__ double _ErrorCorrect(double x, double x1, double x2)//已知x位于x1和x2之间，计算x如果超出这个范围可能导致致命结果，必须修正
{
	if (x1 <= x2)
	{
		if (x <= x1)
		{
			return  x1;
		}
		else if (x >= x2)
		{
			return x2;
		}
		else
		{
			return x;
		}
	}
	else
	{
		if (x <= x2)
		{
			return x2;
		}
		else if (x >= x1)
		{
			return x1;
		}
		else
		{
			return x;
		}
	}
}


__device__ double VirticalDistance(Cuda_Segment sm[SEGNUM_E], int i, int j)
{
	int m, ix, ii;
	double dely;
	ix = int((i - sm[0].m_pst.m_dx)*SEGNUM_E / (sm[SEGNUM_E - 1].m_pst.m_dx - sm[0].m_pst.m_dx));
	ii = (ix - 1 > 0) ? ix - 1 : 0;
	if (ix >= SEGNUM_E - 1) ix = SEGNUM_E - 1;
	for (m = ii; m < ix + 1; m++)
	{
		if (m < SEGNUM_E - 1)
		{
			if (i >= sm[m].m_pst.m_dx && i <= sm[m + 1].m_pst.m_dx)
			{
				break;
			}
		}
	}
	dely = sm[m].m_pst.m_dy + (sm[m + 1].m_pst.m_dy - sm[m].m_pst.m_dy) /
		(sm[m + 1].m_pst.m_dx - sm[m].m_pst.m_dx)*(i - sm[m].m_pst.m_dx) - j;
	return fabs(dely);
}

__device__ int InRegionX(double x, parameter* p)
{
	if (p->iMyID > 0 && p->iMyID < p->iNumProcs - 1 && x >= 0 && x < WIDTH_E)
	{
		return 1;
	}
	else if (p->iMyID == 0 && x >= p->iBegin_Row &&  x < WIDTH_E)
	{
		return 1;
	}
	else if (p->iMyID == p->iNumProcs - 1 && x >= 0 && x < p->iEnd_Row)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

extern "C" double GetValue()
{
	cudaMemcpy(&dTest, dev_test, sizeof(double), cudaMemcpyDeviceToHost);
	return double(dTest);

}

extern "C"  void freeMem()
{
	cudaFree(dev_p);
	//cudaFree(dev_a);
	cudaFree(dev_a_bak);
	//cudaFree(dev_NOBuf);
	cudaFree(dev_sm1);
	cudaFree(dev_sm2);
	//cudaFree(dev_link);
	cudaFree(dev_linknum);
	cudaFree(dev_laln);
	cudaFree(dev_ixn);
	cudaFree(dev_iyn);
	cudaFree(dev_valve);
	cudaFree(dev_test);
}


/*void Show1(int Par, char*ch)
{
char cc[50];
sprintf(cc, "%d", Par);
MessageBox(NULL, ch, cc, 1);
}

void Show1(double Par, char*ch)
{
char cc[50];
sprintf(cc, "%16.15e", Par);
//	MessageBox(NULL, ch, cc, MB_OK);
}

void Show1(float Par, char*ch)
{
char cc[50];
sprintf(cc, "%f", Par);
//	MessageBox(NULL, ch, cc, MB_OK);
}*/
