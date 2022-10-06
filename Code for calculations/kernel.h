#include "lymph_h.h"
#include "Cuda_Vector.h"
#include "Cuda_Link.h"
#include "Cuda_Segment.h"
#include "cudastr.h"
#define LALNNUM 8
#define BLOCK_SIZE 32
#include "Vector.h"



__device__ double _ErrorCorrect(double x, double x1, double x2);
extern "C" void freeMem();
void Show1(int, char*);
void Show1(double, char*);
void Show1(float, char*);

cudaError_t addWithCuda(int iAction);

extern "C" int InitCUDA();
__device__ void feq(int Order, double Den, double Velx, double Vely, parameter* p);
__device__ double VirticalDistance(Cuda_Segment sm[SEGNUM_E], int i, int j);
__global__ void addKernelCollide(lattice_str  la[WIDTH_E][HEIGHT], parameter* p);
__global__ void addKernelCalculate(lattice_str  la[WIDTH_E][HEIGHT], parameter* p);
__global__ void addKernelStream(lattice_str  la[WIDTH_E][HEIGHT], lattice_str la_bak[WIDTH_E][HEIGHT], parameter* p);
__global__ void addKernelReactionNO(lattice_str  la[WIDTH_E][HEIGHT], Cuda_Segment* mem1, Cuda_Segment* mem2, Valve_str * valve, double  NOBuf[WIDTH][HEIGHT], parameter* p);
__global__ void addKernelDeffusionNO(lattice_str  la[WIDTH_E][HEIGHT], double  NOBuf[WIDTH_E][HEIGHT], parameter* p);
__global__ void addKernelAddupNO(lattice_str  la[WIDTH_E][HEIGHT], double  NOBuf[WIDTH_E][HEIGHT], parameter* p);
__global__ void addKernelFindLink(lattice_str  la[WIDTH_E][HEIGHT], Cuda_Segment* mem1, Cuda_Segment* mem2, lattice_link link[WIDTH_E][HEIGHT], int* dev_ixn, int* dev_iyn, int* linknum,  parameter* p
	,double * Test);
__global__ void addKernelInitMem(Cuda_Segment* mem1, Cuda_Segment* mem2, parameter * p, double * dev_Test);
__global__ void  addKernelCopyLink(lattice_link dev_laln[WIDTH_E][HEIGHT], int* ixn, int* iyn, Cuda_Link* dev_link, parameter* p, double *Test);
__device__ int  _IsCross(lattice_str  la[WIDTH_E][HEIGHT], Cuda_Vector* l1, Cuda_Vector* l2, Cuda_Segment* seg, Cuda_Segment* next, Cuda_Vector *pp, double *Test);
__device__ int  _OnSegment(Cuda_Vector* p1, Cuda_Vector* p2, Cuda_Vector* p3);
__device__ int OnMem(int i, int j, Cuda_Segment* mem1, Cuda_Segment* mem2);
__device__ int SamePoint(int i, int j, Cuda_Vector v);
__device__ int InRegionX(double x, parameter* p);