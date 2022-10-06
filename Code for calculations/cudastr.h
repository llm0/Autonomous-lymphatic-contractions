#include "Cuda_Vector.h"
#include "Cuda_Link.h"
#pragma once
#define LALNNUM 8
struct lattice_str
{
	int m_iSol;//
	int m_iOld;//10
	int m_bReaction;//
	int m_ipr;
	double m_df[9];//1-9
	Cuda_Vector m_v;
	double m_dDen;//14
	double m_dcNO;//15
	double m_Stress;//16
};

struct parameter
{
	int iPrjx[9];// = { 0, 1, -1, 0, 0, 1, -1, 1, -1 };//补齐8字节
	int iPrjy[9];// = { 0, 0, 0, 1, -1, -1, 1, 1, -1 };//9
	int iRev[9];
	int Reserve;//双精度对齐
	double dCoe[9];// = { W0, W1, W1, W1, W1, W2, W2, W2, W2 };//27
	double Tau;// = 0.75;//28
	double NOSpread;//29
	double NODecay;//30
	double dkNO;
	double h;
	int iMyID;//use by MPI
	int iBegin_Row;
	int iEnd_Row;
	int iBegin_Mem;
	int iEnd_Mem;
	int iNumProcs;
	double dGravity;
	double dGravityAngle;
};

struct Valve_str
{
	int m_lActive;
	int m_rActive;
	double m_dLx0;
	double m_dRx0;
	double m_dDLfi[VALSEGNUM];
	double m_dDLfo[VALSEGNUM];
	double m_dULfi[VALSEGNUM];
	double m_dULfo[VALSEGNUM];
	double m_dDRfi[VALSEGNUM];
	double m_dDRfo[VALSEGNUM];
	double m_dURfi[VALSEGNUM];
	double m_dURfo[VALSEGNUM];
};

struct lattice_link
{
	int m_i;
	int m_ior;
	Cuda_Link m_ln[LALNNUM];
};

struct data_str
{
	int iSize;
	int iOffset;
};