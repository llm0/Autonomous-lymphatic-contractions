
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include "math.h"
#include <fstream>
#include "time.h"
#include "lbmc.h"
#include "Common.h"
#include "Evolute.h"
#include "CUDA_Transfer.h"
#include "LatticeAnimate.h"
#include "MPIComm.h"
#include <sstream>
#include <string.h>

using namespace std;

void InitAllStatic()
{
	//Valve::m_lattice=lattice;
	PermeatLine::m_lattice = lattice;
	PermeatLine::m_iPrjx = iPrjx;
	PermeatLine::m_iPrjy = iPrjy;
	PermeatLine::m_iPrjxy = iPrjxy;
	PermeatLine::m_iRev = iRev;
	PermeatLine::m_pBoud = new Boundary[BOUDNUM];
	LymphaticVessle::m_lattice = lattice;
	LymphaticVessle::m_iPrjx = iPrjx;
	LymphaticVessle::m_iPrjy = iPrjy;
	LymphaticVessle::m_iPrjxy = iPrjxy;
	LymphaticVessle::m_iRev = iRev;
	LymphaticVessle::m_dAngleE = dAngle;
	LymphaticVessle::m_dCoe = dCoe;
	LymphaticVessle::m_dTau = dTau;
	LymphaticVessle::m_dDensity = dDensity;
	
	SolftValve::m_lattice = lattice;
	SolftValve::m_iPrjx = iPrjx;
	SolftValve::m_iPrjy = iPrjy;
	SolftValve::m_iPrjxy = iPrjxy;
	SolftValve::m_iRev = iRev;
	SolftValve::m_dTau = dTau;
	SolftValve::m_dCoe = dCoe;
	SolftValve::m_dDensity = dDensity;
	ValveSegment::m_dk = 0.00000001;// 0.000001;

	Membrane::m_lattice = lattice;
	Membrane::m_iPrjx = iPrjx;
	Membrane::m_iPrjy = iPrjy;
	Membrane::m_iPrjxy = iPrjxy;
	Membrane::m_iRev = iRev;

	Segment::m_ddT = 1.0 / double(iNewton);//开始初始化Segment的静态变量
	Segment::m_dDensity = dDensity;
	Segment::m_dTau = dTau;
	Segment::m_dEpsilon = dEpsilon;
	Segment::m_iPrjx = iPrjx;
	Segment::m_iPrjy = iPrjy;
	Segment::m_iPrjxy = iPrjxy;
	Segment::m_iRev = iRev;
	Segment::m_dAngleE = dAngle;
	Segment::m_dCoe = dCoe;
	Segment::m_Lattice = lattice;//Segment的静态变量初始化结束
	Segment::m_ymax = H1 + LIMIT;
	Segment::m_ymin = H2 - LIMIT;
	Lattice::m_dTau = dTau;//开始初始化Lattice的静态变量
	Lattice::m_iPrjx = iPrjx;
	Lattice::m_iPrjy = iPrjy;
	Lattice::m_dCoe = dCoe;
	Lattice::m_iSize = sizeof(Lattice);//Lattice的静态变量初始化结束
	Lattice::m_dGravity = &dGravity;
	Lattice::m_dGravityAngle = &dGravityAngle;
}

void input()
{
	lStep = 500000000;
	dTau = 0.75;//0.75;//0.6;
	dEpsilon = 1;
	dMemDensity = 80;//1000;//500;//  80;// 2000;// 1000;// 800;// 80;//1//1.5;//0.133;//3;//1.33;//0.5;//0.5;//0.133;//0.0458;//10;
	Shrink = 0.015;// 0.004;// 0.002;//0.001;//NOF:0.00005; //0.00004he;// 0.00005S; //0.00005;// 0.00025;// 0.00025;//0.0005;// 0.00001;// 0.0005;//0.0005;1e-6
	Bending = 0.2;// 0.1;//0.15;//0.1;//0.1;
	Viscous = 0.0001;// 0.05;//0.5; // 0.5;//6;// 0.1;// NOF:0.05;// 0.1;// 0.1;// 0.05; //0.01;// 0.001;//0.0001;//fluid  0.08333 
	ViscousMem = 0;// 0.5;// 0.5;// 5;
	ViscousValve = 0;// 0.5;
	dDensity = 1.0;
	CaThreshold = 0.1;// 0.094;// 0.08;// 0.11;// 0.05;// 0.035;//balance:0.036=CaProduct/CaDecay;// 0.015;// 0.013;//0.012;// 0.1;// 0.1;//1;//0.2;// 0.2;

	CaDecay = 0.0005*h;// 0.00005*h;//0.0002*h;//0.00006//0.00004//0.0001;
	CaProduct = 0.00003*h;// 0.00003*h; //0.000018*h; //0.000016*h; //0.000012*h; //0.0000012*h;// 0.0000016*h;// 0.0000011*h;// 0.000004*hL;//0.000001*h//s; //0.0000012*h; //0.000012*h;//2*h;//0.000025*h; //0.0001*h;//0.00006//0.00015

	NOSpread = 0.001/4;// 0.015;//0.01;//0.0015*h;//0.001;
	NODecay = 0.0001*h;//0.00001*h;// 0.000007*hs;//0.00001*hl//0.000005*h;// 0.00002*h;//0.000002//0.00018*h;//0.00005;//0.00018;//0.0001;//0.000005*h;//0.002;
	CaNODecay = 0.5;// 0.22;// 0.25;//0.136;// 5.45;// win; 5.5linuxOK;// // 5.3OK;////5.2;//4;// 5.2;// 5.5L;//5S;//100;// 5;// 3.2;// 2.9;//3// 4;// 6;//1.5;//2.3;//10;//20;//35;//25//250;
	DCa = -1;// -1;// -4;// 0;// -8;// +0;// +4;// +4;// -2;//+4;//22 Ca直径相对于基准直径
	dR0 = +0;// +5;// 8;// +4;// 10;//+4;//2;//淋巴管的基准直径22
	CaPeak = 0.15*h; //0.015*h;//org//0.00055*h;//0.0006*h; //0.0007*h; //0.0005*h;//3  0.1*h;// 0.05*h; //0.03*h;// 0.02*h; //0.0004;//0.
	dSpCa = 2.0855e-6;// 2.3775e-5;// 0.004;  //org// 0.015;// 0.00004;//  0.015;// 0.015;// 0.1;//0.01;//0.001;//0.4;
}

void Init()
{
	int i, j, k;
	Vector v;
	input();
	dDiameter = H2 - H1;
	DCa += dDiameter;
	dR0 += dDiameter;
	srand(1);//time(NULL)
	dGravity = -980.0 * pow((2 * dTau - 1) / 6 / 0.01, 2)
		*pow(0.01 / 25, 3) * 3;
	//dOutletDensity = 1 - 3 * dGravity*(WIDTH - 1); dGravity = 0; printf("DensityOut = %lf\n", dOutletDensity);
	dGravityAngle = 0*PI/6;
	//printf("ddd=%f\n", dGravity);
	/*if (iMyID == 0)//iNumProcs-1
	{
		SolValve.m_lActive = 0;
		SolValve.m_rActive = 1;
	}
	else if (iMyID == iNumProcs-1)
	{
		SolValve.m_lActive = 1;//1
		SolValve.m_rActive = 0;//0
	}
	else*/
	{
		SolValve.m_lActive = 1;//1
		SolValve.m_rActive = 1;//0
	}
	if (iMyID > 0)//this block calculate first row and last row
	{
		iLeft = iMyID - 1;//below block
	}
	else
	{
		iLeft = MPI_PROC_NULL;//no below block now
	}
	if (iMyID < iNumProcs - 1)
	{
		iRight = iMyID + 1;//up block
	}
	else
	{
		iRight = MPI_PROC_NULL;//no up block now;
	}
	pDataSize = new data_str[iNumProcs];
	//system("mkdir \"dat\"");
	InitAllStatic();
	SolValve.InitMem();
	lStartStep = 0;
	dhPI = atan2(1, 0);
	TranPara(&pa);
	bCanCuda = InitCudaParameter(GPUID, &pa, lattice, LymVessle.m_MemUp.m_segment, LymVessle.m_MemDown.m_segment, NOBuf, LymVessle.m_link, iMyID+GPUst);
	MPIInitComm();
	//bCanCuda = 0;//不用Cuda
	for (i = 0; i < 100; i++)
	{
		dcCa[i] = -10;
	}
	if (InitFromFile && ReadData(chrName))
	{
		lStartStep++;//Step has been done,will do step+1
		if (iMyID == 0)
		{
			PerLine.init(1);//先定好左边管道线
			PerLine.DrawPermeatArea();
		}
		for (i = 0; i < WIDTH_E; i++)
		{
			if (i < iBegin_Row || i >= iEnd_Row)
			{
				for (j = 0; j < HEIGHT; j++)
				{
					if (iMyID == 0 && i == iBegin_Row || iMyID == iNumProcs - 1 && i == iEnd_Row - 1 || j == 0 || j == HEIGHT - 1) //No direction has boudary
					{
						lattice[i][j].m_iSol = 5;//认定为边界
						lattice[i][j].m_v.m_dx = 0;
						lattice[i][j].m_v.m_dy = 0;
						for (k = 0; k < 9; k++)
						{
							lattice[i][j].m_df[k] = -1000000;//Lattice::feq(k,dDensity,0,0);
						}
					}
					else
					{
						lattice[i][j].m_dcNO = NO0;
						lattice[i][j].m_dDen = dDensity;
						lattice[i][j].m_v.m_dx = 0;
						lattice[i][j].m_v.m_dy = 0;
						for (k = 0; k < 9; k++)
						{
							lattice[i][j].m_df[k] = Lattice::feq(k, lattice[i][j].m_dDen, 0, 0);
						}
						lattice[i][j].m_iSol = -2;
					}
				}
			}
		}
		LymVessle.init_LM(1); //先初始化好膜和段的位置，格子才能初始化
		SolValve.Init(1);
		SolValve.FirstDrawValve(); 
		lStopLeak = lStartStep; 
	}
	else
	{
		for (i = 0; i < WIDTH_E; i++)
		{
			for (j = 0; j < HEIGHT; j++)
			{
				lattice[i][j].m_dcNO = NO0;
				if (iMyID == 0 && i == iBegin_Row || iMyID == iNumProcs - 1 && i == iEnd_Row-1 ||j == 0 || j == HEIGHT - 1) //No direction has boudary
				{
					lattice[i][j].m_iSol = 5;//认定为边界
					lattice[i][j].m_v.m_dx = 0;
					lattice[i][j].m_v.m_dy = 0;
					for (k = 0; k < 9; k++)
					{
						lattice[i][j].m_df[k] = -1000000;//Lattice::feq(k,dDensity,0,0);
					}
				}
				else
				{
					lattice[i][j].m_dDen = dDensity;
					lattice[i][j].m_v.m_dx = 0;
					lattice[i][j].m_v.m_dy = 0;
					for (k = 0; k < 9; k++)
					{
						lattice[i][j].m_df[k] = Lattice::feq(k, lattice[i][j].m_dDen, 0, 0);
					}
					lattice[i][j].m_iSol = -2;
					if (rand() / double(RAND_MAX) <= dPorous&&i >= iBegin_Row&&i < iEnd_Row-1)
					{
						lattice[i][j].m_ipr = 1;
					}
					else
					{
						lattice[i][j].m_ipr = 0;
					}
				}
			}
		}
		if (iMyID == 0)
		{
			PerLine.init(0);//先定好左边管道线
			PerLine.DrawPermeatArea();
		}
		LymVessle.init_LM(0); //先初始化好膜和段的位置，格子才能初始
		SolValve.Init(0); 
		SolValve.FirstDrawValve(); 
	}
	lNowStep = lStartStep;
	SolValve.DrawValve(); //要从里到外画
	LymVessle.DrawLymph_L(); 
	LymVessle.BackupPosition_M();
	if (iMyID == 0)
	{
		PerLine.DrawPermeatArea();
		PerLine.FindLink();
	}
}

int ReadData(char* cc)
{
	int i, j, s5, iOffset;
	MPI_File File;
	MPI_Status Status;
	stringstream stream;
	string str;
	int size, position = 0;
	char *buffer;
	char ch[10] = { "native" };
	SetDataSize(&size, &s5);
	if (iMyID == 0)
	{
		iOffset = 0;
	}
	else
	{
		iOffset = iMyID*size + s5;
	}
	if (iMyID == 0)
	{
		size = size + s5;
	}
	printf("Offset is %d\n", iOffset);
	MPI_File_open(MPI_COMM_WORLD, cc, MPI_MODE_RDONLY, MPI_INFO_NULL, &File);
	MPI_File_set_view(File, iOffset, MPI_CHAR, MPI_PACKED, ch, MPI_INFO_NULL);
	buffer = new char[size];
	MPI_File_read(File, (char*)buffer, size, MPI_PACKED, &Status);
	if (Status.MPI_ERROR)
	{
		printf("Read input file error(step)\n");
	}
	MPI_Unpack(buffer, size, &position, (char*)&(lStartStep), 1, MPI_LONG, MPI_COMM_WORLD);
	MPI_Unpack(buffer, size, &position, (char*)&(lattice[iBegin_Row][0].m_iSol), WIDTH, Lattice_data_type0, MPI_COMM_WORLD);
	LymVessle.Unpack(buffer, size, &position);
	printf("Mem posotion is %f\n", LymVessle.m_MemUp.m_segment[SEGNUM_E - 1].m_pst.m_dx);
	SolValve.Unpack(buffer, size, &position);
	if (iMyID == 0)
	{
		PerLine.Unpack(buffer, size, &position);
		printf("permeat line posotion is %f\n", PerLine.m_LineUp.m_p1.m_dy);
	}
	delete[]buffer;
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_close(&File);
	for (i = 0; i < SEGNUM_E; i++)
	{
		LymVessle.m_MemUp.m_segment[i].m_pst_bak = LymVessle.m_MemUp.m_segment[i].m_pst;
		LymVessle.m_MemDown.m_segment[i].m_pst_bak = LymVessle.m_MemDown.m_segment[i].m_pst;
	}
	CommunicateLattice();
	printf("StartStep is %ld\n", lStartStep);
	return 1;
}

void TranPara(parameter* aa)
{
	int i;
	for (i = 0; i < 9; i++)
	{
		aa->iPrjx[i] = iPrjx[i];
		aa->iPrjy[i] = iPrjy[i];
		aa->iRev[i] = iRev[i];
		aa->dCoe[i] = dCoe[i];
	}
	aa->Tau = dTau;
	aa->NOSpread = NOSpread;
	aa->NODecay = NODecay;
	aa->dkNO = dkNO;
	aa->h = h;
	aa->iMyID = iMyID;
	aa->iBegin_Row = iBegin_Row;
	aa->iEnd_Row = iEnd_Row;
	aa->iBegin_Mem = iBegin_Mem;
	aa->iEnd_Mem = iEnd_Mem;
	aa->iNumProcs = iNumProcs;
	aa->dGravity = dGravity;
	aa->dGravityAngle = dGravityAngle;
}

void TranferValve(Valve_str* valve)
{
	int i;
	valve->m_lActive = SolValve.m_lActive;
	valve->m_rActive = SolValve.m_rActive;
	if (SolValve.m_lActive)
	{
		valve->m_dLx0 = SolValve.m_MemDL.m_Seg[0]->m_pst.m_dx;
	}
	else
	{
		valve->m_dLx0 = 0;
	}
	if (SolValve.m_rActive)
	{
		valve->m_dRx0 = SolValve.m_MemDR.m_Seg[0]->m_pst.m_dx;
	}
	else
	{
		valve->m_dRx0 = 0;
	}
	for (i = 0; i < VALSEGNUM; i++)
	{
		if (SolValve.m_lActive)
		{
			valve->m_dDLfi[i] = SolValve.m_MemDL.m_Seg[i]->m_fi;
			valve->m_dDLfo[i] = SolValve.m_MemDL.m_Seg[i]->m_fo;
			valve->m_dULfi[i] = SolValve.m_MemUL.m_Seg[i]->m_fi;
			valve->m_dULfo[i] = SolValve.m_MemUL.m_Seg[i]->m_fo;
		}
		else
		{
			valve->m_dDLfi[i] = 0;
			valve->m_dDLfo[i] = 0;
			valve->m_dULfi[i] = 0;
			valve->m_dULfo[i] = 0;
		}
		if (SolValve.m_rActive)
		{
			valve->m_dDRfi[i] = SolValve.m_MemDR.m_Seg[i]->m_fi;
			valve->m_dDRfo[i] = SolValve.m_MemDR.m_Seg[i]->m_fo;
			valve->m_dURfi[i] = SolValve.m_MemUR.m_Seg[i]->m_fi;
			valve->m_dURfo[i] = SolValve.m_MemUR.m_Seg[i]->m_fo;
		}
		else
		{
			valve->m_dDRfi[i] = 0;
			valve->m_dDRfo[i] = 0;
			valve->m_dURfi[i] = 0;
			valve->m_dURfo[i] = 0;
		}
	}
}
double Collide(int x, int y, int Order)
{
	int iFx;
	int iFy;//oposit direction
	iFx = x + iPrjx[Order];
	//iFx=GetModx(x+iPrjx[Order]);
	//iFy=GetMody(y+iPrjy[Order]);
	iFy = y + iPrjy[Order];
	if (iFx >= 0 && iFx <= WIDTH_E - 1 && iFy > 0 && iFy < HEIGHT - 1)
	{
		return Lattice::feq(Order, dInletDensity, 0, 0);
	}
	else
	{
		return 1e90;
	}
}

int FluidNode(int i,int j)
{
	if (i >= 0 && i < WIDTH_E&&j >= 0 && j < HEIGHT&&lattice[i][j].m_iSol == -2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void BounCollide()//treat with x,y direction boundary
{
	int i, k, iPx, iPy;
	double b_pb;
	b_pb = 0.9;//0.5;
	if (iMyID == 0 || iMyID == iNumProcs - 1)
	{
		for (i = 0; i < HEIGHT; i++)//y
		{
			for (k = 1; k < 9; k++)
			{
				iPy = i + iPrjy[k];
				if (iMyID == 0)
				{
					iPx = iBegin_Row + iPrjx[k];
					if (FluidNode(iPx, iPy))
					{
						lattice[iBegin_Row][i].m_df[k] = Collide(iBegin_Row, i, k) *(1 - b_pb)
							+ b_pb*lattice[iPx][iPy].m_df[iRev[k]];
					}
				}
				else
				{
					iPx = iEnd_Row - 1 + iPrjx[k];
					if (i <= LymVessle.m_MemUp.m_segment[iEnd_Mem - 1].m_pst.m_dy&&i
						>= LymVessle.m_MemDown.m_segment[iEnd_Mem - 1].m_pst.m_dy)
					{
						if (iPx < iEnd_Row&&iPy >= 0 && iPy < HEIGHT&&lattice[iPx][iPy].m_iSol == -2)
						{
							lattice[iEnd_Row - 1][i].m_df[k] = Collide(iEnd_Row - 1, i, k)*(1 - b_pb)
								+ b_pb*lattice[iPx][iPy].m_df[iRev[k]];
						}
						else
						{
							lattice[iEnd_Row - 1][i].m_df[k] = Lattice::feq(k, dOutletDensity, 0, 0);//lattice[iPx][iPy].m_df[iRev[k]];
						}
					}
					else
					{
						if (iPx < iEnd_Row&&iPy >= 0 && iPy < HEIGHT&&lattice[iPx][iPy].m_iSol == -1)
						{
							lattice[iEnd_Row - 1][i].m_df[k] = Lattice::feq(k, dOutletDensity, 0, 0);
						}
						else if(lattice[iPx][iPy].m_iSol == -2)
						{
							lattice[iEnd_Row - 1][i].m_df[k] = Collide(iEnd_Row - 1, i, k) *(1 - b_pb)
								+ b_pb*lattice[iPx][iPy].m_df[iRev[k]];
						}
					}
				}
			}
		}
	}
	for (i = 0; i < WIDTH_E; i++)//x
	{
		for (k = 1; k < 9; k++)
		{	
			iPx = i + iPrjx[k];
			iPy = 0 + iPrjy[k];
			if (FluidNode(iPx, iPy))
			{
				lattice[i][0].m_df[k] = Collide(i, 0, k) *(1 - b_pb)
					+ b_pb*lattice[iPx][iPy].m_df[iRev[k]];
			}
			iPx = i + iPrjx[k];
			iPy = HEIGHT - 1 + iPrjy[k];
			if (FluidNode(iPx, iPy))
			{
				lattice[i][HEIGHT - 1].m_df[k] = Collide(i, HEIGHT - 1, k) *(1 - b_pb)
					+ b_pb*lattice[iPx][iPy].m_df[iRev[k]];
			}
		}
	}
}



void OutLet() //treat with y diretion boundary
{
	int iy1, iy2;
	if (iMyID == iNumProcs - 1)
	{
		iy1 = int(LymVessle.m_MemDown.m_segment[iEnd_Mem - 1].m_pst.m_dy + 0.5) + 2;//42,41的2，4，6用局域平衡处理，
		iy2 = int(LymVessle.m_MemUp.m_segment[iEnd_Mem - 1].m_pst.m_dy + 0.5) - 2;//58，59的2，4，6用局域平衡处理
		BoundPress(iy1, iy2);
	}
}

void Stream()//可以用Cuda加速
{
	int i;
	int j;
	int k;
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			for (k = 1; k < 9; k++)
			{
				if (lattice[i][j].m_iSol < 0 && lattice[i][j].m_ipr!=2)
				{
					latbuf[i][j][k - 1] = lattice[i - iPrjx[k]][
						j - iPrjy[k]].m_df[k];
				}
			}//for k
		}//for i
	}//for j
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			for (k = 1; k < 9; k++)
			{
				if (lattice[i][j].m_iSol < 0 && lattice[i][j].m_ipr != 2)
				{
					lattice[i][j].m_df[k] = latbuf[i][j][k - 1];
				}
			}//for k
		}//for i
	}//for j
}

void Calculate()//可以用Cuda加速
{
	int i, j, k;
	dTotal = 0;
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			lattice[i][j].m_bReaction = 0;
			if (lattice[i][j].m_iSol < 0 && lattice[i][j].m_ipr != 2)
			{
				lattice[i][j].m_dDen = 0;
				lattice[i][j].m_v.m_dx = 0;
				lattice[i][j].m_v.m_dy = 0;
				for (k = 0; k<9; k++)
				{
					lattice[i][j].m_dDen += lattice[i][j].m_df[k];
					lattice[i][j].m_v.m_dx += lattice[i][j].m_df[k]
						* iPrjx[k];
					lattice[i][j].m_v.m_dy += lattice[i][j].m_df[k]
						* iPrjy[k];
				}
				if (fabs(lattice[i][j].m_dDen) / dDensity>1.2
					|| lattice[i][j].m_dDen < 0.9*dDensity)
				{
					/*if (lattice[i][j].m_iSol == -3)//被Valve孤立的格点是不稳定的。
					{
					lattice[i][j].m_dDen = dDensity;
					lattice[i][j].m_v = Vector(0, 0);
					for (k = 0; k < 9; k++)
					{
					lattice[i][j].m_df[k] = Lattice::feq(k, dDensity, 0, 0);
					}
					}*/
					//else
					{
						printf("iMyID=%d,x=%d,y=%d,Density=%f>20\n", iMyID, i, j, lattice[i][j].m_dDen);
						//	exit(1);
					}
				}
				if (lattice[i][j].m_iSol < 0)
				{
					dTotal += lattice[i][j].m_dDen;
				}
				lattice[i][j].m_v.m_dx /= lattice[i][j].m_dDen;
				lattice[i][j].m_v.m_dy /= lattice[i][j].m_dDen;
			}// if
			else
			{
				lattice[i][j].m_dDen = 1000;
				lattice[i][j].m_v.m_dx = 0;
				lattice[i][j].m_v.m_dy = 0;
				for (k = 0; k < 9; k++)
				{
					lattice[i][j].m_df[k] = 1000;
				}
			}
			if (j<CENTER + 2 && j>CENTER - 2 && i>iBegin_Row + 2 && j < iEnd_Row - 2)//单独一点夹在两膜间
			{
				if (lattice[i][j].m_iSol == -3 && lattice[i][j + 1].m_iSol != -3
					&& lattice[i][j - 1].m_iSol != -3 && lattice[i - 1][j + 1].m_iSol != -3
					&& lattice[i - 1][j - 1].m_iSol != -3)
				{
					lattice[i][j].m_dDen = (lattice[i][j + 1].m_dDen + lattice[i][j - 1].m_dDen) / 2;
					for (k = 0; k < 9; k++)
					{
						lattice[i][j].m_df[k] = Lattice::feq(k, lattice[i][j].m_dDen, 0, 0);
					}
					lattice[i][j].m_v = Vector(0, 0);
				}
			}
		}//for j
	}//for i
}

int main(int argc, char ** argv)
{
	int i, j;// , m;
	long lTime0, lTime1;
	fstream fPosition, fOmega;// , ff;
	char cc[50];
	//MPI_Request Request_Force[2];
	//MPI_Status Status_Force;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &iNumProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &iMyID);
	MPI_Get_processor_name(cc, &iLen);
	printf("The processor is %s,the process ID is %d \n", cc, iMyID);
	printf("The number of process is %d\n ", iNumProcs);
	printf("Arg Number is %d\n ", argc);
	iBegin_Row = 3;
	iEnd_Row = WIDTH_E - 3;
	iEnd_Mem = SEGNUM_E - 1;
	if (argc > 1)
	{
		if (argv[1][0] == '-')
		{
			if (argv[1][1] == 'n')
			{
				strcpy(chrName, argv[2]);
				GPUst = atol(chrName);
				MPI_Bcast(&GPUst, 1, MPI_INT, 0, MPI_COMM_WORLD);
				if (argc > 3)
				{
					strcpy(chrName, argv[3]);
					printf("Input file is %s\n", chrName);
					InitFromFile = 1;
					MPI_Bcast(&chrName, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
				}
			}
		}
		else
		{
			strcpy(chrName, argv[1]);
			printf("Input file is %s\n", chrName);
			InitFromFile = 1;
			MPI_Bcast(&chrName, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		InitFromFile = 0;
	}
	if (iMyID == 0)
	{
		iBegin_Mem = LEFTVALVE;
	}
	else
	{
		iBegin_Mem = BEGINMEM;
	}
	Init();
	TobeSame(); 
	CommunicateLattice();
	ActivePorous(); 
	CommunicateLattice();
	InsertNonActivePorous();
	CommunicateLattice();
	bDrawNow = 0;
	lTime0 = long(time(NULL)); //lStep = 1;
	iTotal = iCountP;
	/*if (lStartStep<iTotal)
	{
		iCountP = iTotal - lStartStep;
	}
	else
	{
		iCountP = 0;
	}*/
	dkm = dkm0;
	for (lNowStep = lStartStep; lNowStep <= lStep + 1; lNowStep++)
	{
		if (iCountP > 0 && lNowStep >= 3e6)//time>4s
		{
			if (lNowStep == 3e6)
			{
				DataBak(0);
			}
			IncreasPress();
			iCountP--;
		}
		CommunicateLattice();
		CommunicateMem(); 
		LymVessle.Angle_M();
		LymVessle.Force_M(); //fi,fo,f,getfore change
		LymVessle.CalciumForce_M();// f change LymVessle.AverageCaForce();
		SolValve.Force(); 
		CommunicateMem();
		LymVessle.HydroAcc_M();//用到近邻的f,更新fa
		SolValve.HydroAcc(); //lStep = 4000;
		iClose = 0;
		for (j = 0; j < iNewton; j++)//Newtonian 时步
		{
			LymVessle.ElastAcc_M(j); //用到近邻的p,更新ka
			LymVessle.VescoAcc_M(j);//用到近邻的v,更新ka
			SolValve.ElastAcc();
			SolValve.VescoAcc();
			LymVessle.Move_M(j);
			SolValve.Move(j);
			LymVessle.BackupPosition_M();
			CommunicateMem();
		}
		ActivePorous();
		SolValve.DrawValve();//从里到外画
		LymVessle.DrawLymph_L();//运动之后会出-3(外变内),-4(内变外)。
		CommunicateLattice();
		InsertNonActivePorous();
		CommunicateLattice();
		LymVessle.Insert_L();// if (iMyID == 0) SaveDensity(1);//1要由-1外推,2要由-2外推，3要由-3外推，瓣膜不用Insert也用ReMark，因为它夹在两膜之间用淋巴管处理就行了
		CommunicateLattice(); 
		SolValve.Remark();
		LymVessle.ReMark_L();//清除中间态标识,1变-1,2变-2，3变-3
		CommunicateLattice(); //CUDA1
		if (!(bCanCuda&&CudaFindLink(&(LymVessle.m_il),
			LymVessle.m_MemUp.m_segment, LymVessle.m_MemDown.m_segment, &pa)))//cuda
		{
			bCanCuda = 0;
			LymVessle.FindLink_L();//运动完毕，重新找Link
		} 
		CommunicateLattice();
		SolValve.FindLink(); 
		LymVessle.AverageStress_L();
		CommunicateLattice();
		TranferValve(&valve); //CUDA2
		if (!(bCanCuda&&CudaReactionNO(&valve)))
		{
			bCanCuda = 0;
			ReactionNO();
		}
		CommunicateLattice();//CUDA3
		if (!(bCanCuda&&CudaDiffusionNO()))//cuda
		{
			bCanCuda = 0;
			DeffusionNO();//只能用上一步的流速，保证同步性
		}
		CommunicateLattice();
		LymVessle.CaReaction();//Ca reaction and spreading
		CommunicateMem();
		LymVessle.CaDiffusion_M();
		CommunicateMem();
		LymVessle.CaAverage_M();
		CommunicateMem();//CUDA4
		if (!(bCanCuda&&CudaCollide()))//cuda
		{
			bCanCuda = 0;
			for (i = iBegin_Row; i < iEnd_Row; i++) //x direction has no  boundary
			{
				for (j = 0; j < HEIGHT; j++) //y direction has boundary
				{
					if (lattice[i][j].m_iSol < 0 && lattice[i][j].m_ipr != 2)
					{
						lattice[i][j].Collide(); //Fluid Cllide
					}
				}
			}
		}
		CommunicateLattice();
		BounCollide(); 
		LymVessle.Collide_L(); 
		PorousCollide();
		CommunicateLattice(); 
		if (iMyID == 0)
		{
			PerLine.Collide();
		}
		SolValve.Collide(); 
		if (iMyID == 0)
		{
			PerLine.Oneway();
		}//CUDA5
		if (iMyID == iNumProcs - 1)
		{
			PartialBounceback(iEnd_Row-5);// int(LymVessle.m_MemUp.m_segment[SEGNUM - LEFTVALVE].m_pst.m_dx));
		}
		if (!(bCanCuda&&CudaStream()))//cuda
		{
			bCanCuda = 0;
			Stream();	//Fluid Stream
		}
		OutLet();	 //x direction boudary
		CommunicateLattice();//CUDA6
		if (!(bCanCuda&&CudaCalculate()))//cuda
		{
		    bCanCuda = 0;
			Calculate();
		}
		if (iMyID == iNumProcs - 1)
		{
			Average(iEnd_Row - 4, iEnd_Row - 3);
			Average(iEnd_Row - 7, iEnd_Row - 6);
		}
		CommunicateLattice(); // CommunicateAll();
		lTime1 = long(time(NULL));
		if (lTime1 - lTime0 > BAKTIME || lNowStep == lSave)
		{
			lTime0 = lTime1;
			DataBak(1);
		} 
		if (lNowStep % 2000 == 0)
		{
			if (iMyID == 0)
			{
				Flux();
			}
			SaveForAnimate();
		}
		if (lNowStep % 2000== 0)
		{
			bDrawNow = 1;
		}
		else
		{
			bDrawNow = 0;
		}
		if (bDrawNow)
		{
			DataShow();
		}
		iStep++;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	freeMem();
	delete pDataSize;
	return 0;
}


void FileOpen()
{
	filep.open((".\\dat\\fv.dat"), ios::out | ios::in);
	filep.seekp(0, ios::end);
}

void FileSave()
{
	if (filep.is_open())
	{
		/*for(i=0;i<iSegNum;i++)
		{
		filep<<segment[i].m_dx<<" "<<segment[i].m_dy<<endl;
		}*/
		filep.close();
	}
}

void DataBak(int iNormal)
{
	int s5, iOffset;
	MPI_File File;
	MPI_Status Status;
	stringstream stream;
	string str;
	int size, position = 0;
	char *buffer;
	char cc[10] = { "native" };
	char aa[10];
	//sprintf(aa, "%d", iFile); iFile++;
	//stream << aa;
	stream << iNumProcs;
	if (iFile == 0)
	{
		stream << 'A';
		iFile = 1;
	}
	else
	{
		stream << 'B';
		iFile = 0;
	}

	if (iNormal)
	{
		str = "Bak" + stream.str() + ".dat";
	}
	else
	{
		str = "Bak" + stream.str() + "Step.dat";
	}
	SetDataSize(&size, &s5);
	if (iMyID == 0)
	{
		iOffset = 0;
	}
	else
	{
		iOffset = iMyID* size + s5;
	}
	if (iMyID == 0)
	{
		size = size + s5;
	}
	MPI_File_open(MPI_COMM_WORLD, (char*)str.data(), MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &File);
	MPI_File_set_view(File, iOffset, MPI_CHAR, MPI_CHAR, cc, MPI_INFO_NULL);
	buffer = new char[size];
	MPI_Pack((char*)&(lNowStep), 1, MPI_LONG, buffer, size, &position, MPI_COMM_WORLD);
	MPI_Pack((char*)&(lattice[iBegin_Row][0].m_iSol), WIDTH, Lattice_data_type0, buffer, size, &position, MPI_COMM_WORLD);
	LymVessle.Pack(buffer, size, &position);
	SolValve.Pack(buffer, size, &position);
	if (iMyID == 0)
	{
		PerLine.Pack(buffer, size, &position);
	}
	MPI_File_write(File, buffer, size, MPI_PACKED, &Status);
	if (Status.MPI_ERROR)
	{
		printf("Save Bakup file error\n");
	}
	delete[]buffer;
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_close(&File);
}

/*void DataBak()
{
	int i, s5, iOffset;
	MPI_File File;
	MPI_Status Status;
	stringstream stream;
	string str;
	int size, position = 0, pp1;
	char *buffer;
	char cc[10] = { "native" };
	stream << iNumProcs;
	if (iFile == 0)
	{
		stream << 'A';
		iFile = 1;
	}
	else
	{
		stream << 'B';
		iFile = 0;
	}
	str = "Bak" + stream.str() + ".dat";
	SetDataSize(&size, &s5);
	if (iMyID == 0)
	{
		iOffset = 0;
	}
	else
	{
		iOffset = iMyID* size + s5;
	}
	if (iMyID == 0)
	{
		size = size + s5;
	}
	MPI_File_open(MPI_COMM_WORLD, (char*)str.data(), MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &File);
	MPI_File_set_view(File, iOffset, MPI_CHAR, MPI_CHAR, cc, MPI_INFO_NULL);
	buffer = new char[size];
	MPI_Pack((char*)&(lNowStep), 1, MPI_LONG, buffer, size, &position, MPI_COMM_WORLD);
	MPI_Pack((char*)&(lattice[iBegin_Row][0].m_iSol), WIDTH, Lattice_data_type0, buffer, size, &position, MPI_COMM_WORLD);
	LymVessle.Pack(buffer, size, &position);
	SolValve.Pack(buffer, size, &position);
	if (iMyID == 0)
	{
		PerLine.Pack(buffer, size, &position);
	}
	MPI_File_write(File, buffer, size, MPI_PACKED, &Status);
	if (Status.MPI_ERROR)
	{
		printf("Save Bakup file error\n");
	}
	delete[]buffer;
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_close(&File);
}*/

void SetDataSize(int* size, int* s5)
{
	int s1, s2, s3, s4, ss;//MPI_DOUBLE:4,double:8
	ss = sizeof(Segment);
	s1 = sizeof(long);
	s2 = 16 * sizeof(double);
	s3 = 2 * (4 * sizeof(double)+2 * sizeof(int)+SEGNUM_E*ss);
	s4 = 4 * (VALSEGNUM - 1) * sizeof(ValveSegment);
	*s5 = 3 * (6 * sizeof(double));
	*size = s1 + s2*WIDTH*HEIGHT + s3 + s4;
}


void DataShow()
{
	printf("step=%ld,Valve tip speed=%f, Can Cuda=%d\n", lNowStep, LymVessle.m_MemUp.m_segment[RIGHTVALVE].m_v.m_dy, bCanCuda);
}


void BoundPress(int iy1, int iy2)
{
	double dux2, dDen2;
	int i;
	dDen2 = dOutletDensity;
	for (i = iy1; i <= iy2; i++) //eq
	{
		dux2 = -1 + (lattice[iEnd_Row - 2][i].m_df[0] + lattice[iEnd_Row - 2][i].m_df[3]
			+ lattice[iEnd_Row - 2][i].m_df[4] + 2 * (lattice[iEnd_Row - 2][i].m_df[1]
			+ lattice[iEnd_Row - 2][i].m_df[5] + lattice[iEnd_Row - 2][i].m_df[7])) / dDen2;
		lattice[iEnd_Row - 2][i].m_df[2] = lattice[iEnd_Row - 2][i].m_df[1] - 6 * dDen2*W1*dux2;
		lattice[iEnd_Row - 2][i].m_df[6] = lattice[iEnd_Row - 2][i].m_df[5] - 0.5*(lattice[iEnd_Row - 2][i].m_df[3]
			- lattice[iEnd_Row - 2][i].m_df[4]) - (0.5 - 3 * W1)*dDen2*dux2;
		lattice[iEnd_Row - 2][i].m_df[8] = lattice[iEnd_Row - 2][i].m_df[7] + 0.5*(lattice[iEnd_Row - 2][i].m_df[3]
			- lattice[iEnd_Row - 2][i].m_df[4]) - (0.5 - 3 * W1)*dDen2*dux2;
	}
}


void test()
{
	int i; int j;
	for (i = 0; i < WIDTH_E; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (j < 10 && lattice[i][j].m_iSol < 0)
			{
				printf("Step");
			}
		}
	}
}

void ReactionNO()//1:膜,2:内点,3:外点
{
	int i, j, iSeg;
	double dks, dProduct, dstr=0, dx;
	dks = dkNO*h;
	dcNO[(lNowStep / 50) % 100] = 0;
	if ((lNowStep / 50) % 100 == 0)
	{
		for (i = 1; i < 100; i++)
		{
			dcNO[i] = -10;
		}
	}
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j<HEIGHT; j++)
		{
			if (lattice[i][j].m_bReaction>0)
			{
				if (lattice[i][j].m_bReaction == 1)//上下膜
				{
					iSeg = BEGINMEM+int(double(i - iBegin_Row)*(iEnd_Mem - 1 - BEGINMEM)
						/ (iEnd_Row - 1 - iBegin_Row) + 0.5);
					if (j >= CENTER)
					{
						dstr = LymVessle.m_MemUp.m_segment[iSeg].m_fi;
					}
					else
					{
						dstr = LymVessle.m_MemDown.m_segment[iSeg].m_fi;
					}
				}
				else if (lattice[i][j].m_bReaction == 2)//valve内
				{
					dx = (H2 - H1) * 2.0 / VALSEGNUM;
					if (SolValve.m_lActive&&i < LymVessle.m_MemUp.m_segment[RIGHTVALVE].m_pst.m_dx)//左
					{
						iSeg = int((i - SolValve.m_MemDL.m_Seg[0]->m_pst.m_dx) / dx + 0.5);
						if (j >= CENTER)//上
						{
							dstr = SolValve.m_MemUL.m_Seg[iSeg]->m_fi;
						}
						else
						{
							dstr = SolValve.m_MemDL.m_Seg[iSeg]->m_fi;
						}
					}
					else if (SolValve.m_rActive)
					{
						iSeg = int((i - SolValve.m_MemDR.m_Seg[0]->m_pst.m_dx) / dx + 0.5);
						if (j >= CENTER)//上
						{
							dstr = SolValve.m_MemUR.m_Seg[iSeg]->m_fi;
						}
						else
						{
							dstr = SolValve.m_MemDR.m_Seg[iSeg]->m_fi;
						}
					}
				}
				else if (lattice[i][j].m_bReaction == 3)//valve外
				{
					dx = (H2 - H1) * 2.0 / VALSEGNUM;
					if (SolValve.m_lActive && i < LymVessle.m_MemUp.m_segment[RIGHTVALVE].m_pst.m_dx)//左
					{
						iSeg = int((i - SolValve.m_MemDL.m_Seg[0]->m_pst.m_dx) / dx + 0.5);
						if (j >= CENTER)//上
						{
							dstr = SolValve.m_MemUL.m_Seg[iSeg]->m_fo;
						}
						else
						{
							dstr = SolValve.m_MemDL.m_Seg[iSeg]->m_fo;
						}
					}
					else if (SolValve.m_rActive)
					{
						iSeg = int((i - SolValve.m_MemDR.m_Seg[0]->m_pst.m_dx) / dx + 0.5);
						if (j >= CENTER)//上
						{
							dstr = SolValve.m_MemUR.m_Seg[iSeg]->m_fo;
						}
						else
						{
							dstr = SolValve.m_MemDR.m_Seg[iSeg]->m_fo;
						}
					}
				}
				dProduct = dks*fabs(dstr) / (lattice[i][j].m_dDen*(2 * dTau - 1) / 6);
				/*if (!(lattice[i][j].m_dcNO <= LIMITNO) && dProduct > NODecay)
				{
					dProduct = NODecay;
				}*/
				NOBuf[i][j] = dProduct;
				dcNO[(lNowStep / 50) % 100] += lattice[i][j].m_dcNO + dProduct;
				lattice[i][j].m_bReaction = 0;
			}
			else
			{
				NOBuf[i][j] = 0;
			}
		}
	}
	dcCa[(lNowStep / 50) % 100] = LymVessle.m_MemUp.m_segment[20].m_dcCa;//dTotalCa;
	if ((lNowStep / 50) % 100 == 0)
	{
		for (i = 1; i < 100; i++)
		{
			dcCa[i] = -10;
		}
	}
}

void DeffusionNO()//NO能穿透膜的模型//可以用Cuda加速
{
	int i, j, ib, ie;
	double dDeltaNO, dLaplace;
	Vector Grad;
	if (iMyID == 0)
	{
		ib = iBegin_Row + 1; ie = iEnd_Row;
	}
	else if (iMyID == iNumProcs - 1)
	{
		ib = iBegin_Row; ie = iEnd_Row - 1;
	}
	else
	{
		ib = iBegin_Row; ie = iEnd_Row;
	}
	for (i = ib; i < ie; i++) //x direction has no  boundary
	{
		for (j = 1; j<HEIGHT - 1; j++) //y direction has boundary
		{

			dLaplace = lattice[i - 1][j].m_dcNO - 2 * lattice[i][j].m_dcNO + lattice[i + 1][j].m_dcNO
				+ lattice[i][j - 1].m_dcNO - 2 * lattice[i][j].m_dcNO + lattice[i][j + 1].m_dcNO;
			if (lattice[i][j].m_v.m_dx>0)
			{
				Grad.m_dx = lattice[i][j].m_dcNO - lattice[i - 1][j].m_dcNO;
			}
			else
			{
				Grad.m_dx = lattice[i + 1][j].m_dcNO - lattice[i][j].m_dcNO;
			}
			if (lattice[i][j].m_v.m_dy > 0)
			{
				Grad.m_dy = lattice[i][j].m_dcNO - lattice[i][j - 1].m_dcNO;
			}
			else
			{
				Grad.m_dy = lattice[i][j + 1].m_dcNO - lattice[i][j].m_dcNO;
			}
			dDeltaNO = NOSpread*dLaplace - NODecay*lattice[i][j].m_dcNO
				- Vector(lattice[i][j].m_v.m_dx, lattice[i][j].m_v.m_dy)*Grad;//dks
			NOBuf[i][j] += dDeltaNO;
		}
	}
	for (i = iBegin_Row; i < iEnd_Row; i++) //x direction has no  boundary
	{
		if (!(i == iBegin_Row&&iMyID == 0) && !(i == iEnd_Row - 1 && iMyID == iNumProcs - 1))
		{
			NOBuf[i][0] += NOSpread*(lattice[i][1].m_dcNO - 3 * lattice[i][0].m_dcNO
				+ lattice[i + 1][0].m_dcNO + lattice[i - 1][0].m_dcNO)
				- NODecay*lattice[i][0].m_dcNO;
			NOBuf[i][HEIGHT - 1] += NOSpread*(lattice[i][HEIGHT - 2].m_dcNO - 3 * lattice[i][HEIGHT - 1].m_dcNO
				+ lattice[i + 1][HEIGHT - 1].m_dcNO + lattice[i - 1][HEIGHT - 1].m_dcNO)
				- NODecay*lattice[i][HEIGHT - 1].m_dcNO;
		}
		else if (i == iBegin_Row && iMyID == 0)
		{
			NOBuf[i][0] += NOSpread*(lattice[i][1].m_dcNO - 2 * lattice[i][0].m_dcNO
				+ lattice[i + 1][0].m_dcNO)
				- NODecay*lattice[i][0].m_dcNO;
			NOBuf[i][HEIGHT - 1] += NOSpread*(lattice[i][HEIGHT - 2].m_dcNO - 2 * lattice[i][HEIGHT - 1].m_dcNO
				+ lattice[i + 1][HEIGHT - 1].m_dcNO) - NODecay*lattice[i][HEIGHT - 1].m_dcNO;
		}
		else
		{
			NOBuf[i][0] += NOSpread*(lattice[i][1].m_dcNO - 2 * lattice[i][0].m_dcNO
				+ lattice[i - 1][0].m_dcNO) - NODecay*lattice[i][0].m_dcNO;
			NOBuf[i][HEIGHT - 1] += NOSpread*(lattice[i][HEIGHT - 2].m_dcNO - 2 * lattice[i][HEIGHT - 1].m_dcNO
				+ lattice[i - 1][HEIGHT - 1].m_dcNO) - NODecay*lattice[i][HEIGHT - 1].m_dcNO;
		}
	}

	for (j = 0; j < HEIGHT; j++) //x direction has no  boundary
	{
		if (j != 0 && j != HEIGHT - 1)
		{
			if (iMyID == 0)
			{
				NOBuf[iBegin_Row][j] += NOSpread*(lattice[iBegin_Row + 1][j].m_dcNO - 3 * lattice[iBegin_Row][j].m_dcNO +
					lattice[iBegin_Row][j + 1].m_dcNO + lattice[iBegin_Row][j - 1].m_dcNO)
					- NODecay*lattice[iBegin_Row][j].m_dcNO;
			}
			if (iMyID == iNumProcs - 1)
			{
				NOBuf[iEnd_Row - 1][j] += NOSpread*(lattice[iEnd_Row - 2][j].m_dcNO - 3 * lattice[iEnd_Row - 1][j].m_dcNO +
					lattice[iEnd_Row - 1][j + 1].m_dcNO + lattice[iEnd_Row - 1][j - 1].m_dcNO)
					- NODecay*lattice[iEnd_Row - 1][j].m_dcNO;
			}
		}
	}
	for (i = iBegin_Row; i < iEnd_Row; i++) //x direction has no  boundary
	{
		for (j = 0; j < HEIGHT; j++) //y direction has boundary
		{
			lattice[i][j].m_dcNO += NOBuf[i][j];//从反冲区再拷过来
			if (lattice[i][j].m_dcNO<0)
			{
				printf("NO<0,No=%f,set to 0\n", lattice[i][j].m_dcNO);
				lattice[i][j].m_dcNO = 0;
			}
			else if (lattice[i][j].m_dcNO>1)
			{
				//lattice[i][j].m_dcNO = 1;
			}
			if (i == iBegin_Row + 1 && iMyID == 0 || j == 1 || i == iEnd_Row - 2 && iMyID == iNumProcs - 1 || j == HEIGHT - 2)
			{
				lattice[i][j].m_dcNO = 0;
			}
		}
	}
}


/*void DeffusionNO()//NO不能穿透膜的模型
{
int i,j;
double dDeltaNO,dLaplace;
Vector Grad;
for(i=0;i<WIDTH;i++) //x direction has no  boundary
{
for(j=0;j<HEIGHT;j++) //y direction has boundary
{
if(lattice[i-1][j].m_iSol<0&&lattice[i+1][j].m_iSol<0&&lattice[i][j-1].m_iSol<0
&&lattice[i+1][j].m_iSol<0&&lattice[i+1][j].m_iSol<0&&lattice[i][j+1].m_iSol<0)//流体
{
if(lattice[i-1][j].m_iSol==lattice[i+1][j].m_iSol&&lattice[i][j-1].m_iSol==lattice[i+1][j].m_iSol&&
lattice[i+1][j].m_iSol==lattice[i][j+1].m_iSol||BetweenValve(i,j))//管内外同一区域，管内不同区域：瓣膜口
{
dLaplace=lattice[i-1][j].m_dcNO-2*lattice[i][j].m_dcNO+lattice[i+1][j].m_dcNO
+lattice[i][j-1].m_dcNO-2*lattice[i][j].m_dcNO+lattice[i][j+1].m_dcNO;
}
else
{
dLaplace=0;
}
}
if(lattice[i][j].m_v.m_dx>0)
{
if(lattice[i][j].m_iSol==lattice[i-1][j].m_iSol||BetweenValve(i,j))
{
Grad.m_dx=lattice[i][j].m_dcNO-lattice[i-1][j].m_dcNO;
}
else
{
Grad.m_dx=0;
}
}
else if(lattice[i+1][j].m_iSol<0)
{
if(lattice[i+1][j].m_iSol==lattice[i][j].m_iSol||BetweenValve(i,j))
{
Grad.m_dx=lattice[i+1][j].m_dcNO-lattice[i][j].m_dcNO;
}
else
{
Grad.m_dx=0;
}
}
else
{
Grad.m_dx=0;
}
if(lattice[i][j].m_v.m_dy>0)
{
if(lattice[i][j-1].m_iSol==lattice[i][j].m_iSol||(BetweenValve(i,j)&&BetweenValve(i,j-1)))
{
Grad.m_dy=lattice[i][j].m_dcNO-lattice[i][j-1].m_dcNO;
}
else
{
Grad.m_dy=0;
}
}
else if(lattice[i][j+1].m_iSol<0)
{
if(lattice[i][j+1].m_iSol==lattice[i][j].m_iSol||(BetweenValve(i,j)&&BetweenValve(i,j+1)))
{
Grad.m_dy=lattice[i][j+1].m_dcNO-lattice[i][j].m_dcNO;
}
else
{
Grad.m_dy=0;
}
}
else
{
Grad.m_dy=0;
}
dDeltaNO=NOSpread*dLaplace-NODecay*lattice[i][j].m_dcNO//lattice[i][j].m_Stress)
-Vector(lattice[i][j].m_v.m_dx,lattice[i][j].m_v.m_dy)*Grad;//dks
CaNOBuf[i][j]=lattice[i][j].m_dcNO+dDeltaNO;
}
}
for(i=0;i<WIDTH;i++) //x direction has no  boundary
{
for(j=0;j<HEIGHT;j++) //y direction has boundary
{
if(lattice[i][j].m_iSol<0)
{
lattice[i][j].m_dcNO=CaNOBuf[i][j];//从反冲区再拷过来
}
}
}
}*/

/*int BetweenValve(int i,int j)
{
if((j>=ValveDual.m_PetalDL.m_Apex.m_dy&&j<=ValveDual.m_PetalUL.m_Apex.m_dy
&&fabs(i-ValveDual.m_PetalDL.m_Apex.m_dx)<ValveDual.m_PetalDL.m_Length+5)
||(j>=ValveDual.m_PetalDR.m_Apex.m_dy&&j<=ValveDual.m_PetalUR.m_Apex.m_dy
&&fabs(i-ValveDual.m_PetalDR.m_Apex.m_dx)<ValveDual.m_PetalDR.m_Length+5))
{
return 1;
}
else
{
return 0;
}
}*/


void ShearStress()//可以用Cuda加速，没有用到的函数
{
	int i, j;
	double v0, vj, vjj, dely;
	for (i = 0; i < WIDTH; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (j == 0)//下边界
			{
				lattice[i][j].m_Stress = lattice[i][j + 1].m_v.m_dx - 0;
			}
			else if (j == HEIGHT - 1)//上边界
			{
				lattice[i][j].m_Stress = 0 - lattice[i][HEIGHT - 2].m_v.m_dx;
			}
			else if (lattice[i][j + 1].m_iSol == lattice[i][j - 1].m_iSol || j == 1 || j == HEIGHT - 2)//同种区域内
			{
				lattice[i][j].m_Stress = (lattice[i][j + 1].m_v.m_dx - lattice[i][j - 1].m_v.m_dx) / 2.0;
			}
			else if (lattice[i][j + 1].m_iSol*lattice[i][j - 1].m_iSol == 2 && lattice[i][j].m_iSol<0)//-1,-2
			{
				if (i >= LymVessle.m_MemUp.m_dx1)//不同种区域在膜的周围
				{
					if (j>HEIGHT / 2)
					{
						dely = LymVessle.m_MemUp.VirticalDistance(i, j);
					}
					else
					{
						dely = LymVessle.m_MemDown.VirticalDistance(i, j);
					}
					vj = lattice[i][j].m_v.m_dx;
					if (lattice[i][j].m_iSol == lattice[i][j + 1].m_iSol)
					{
						if (fabs(dely) < 0.1)
						{
							lattice[i][j].m_Stress = lattice[i][j + 1].m_v.m_dx - lattice[i][j].m_v.m_dx;
						}
						else
						{
							vjj = lattice[i][j + 1].m_v.m_dx;
							v0 = vj*(1 - dely) + dely*vjj;
							lattice[i][j].m_Stress = v0 / (2 * dely);
						}
					}
					else
					{
						if (fabs(dely) < 0.1)
						{
							lattice[i][j].m_Stress = lattice[i][j].m_v.m_dx - lattice[i][j - 1].m_v.m_dx;
						}
						else
						{
							vjj = lattice[i][j - 1].m_v.m_dx;
							v0 = vj*(1 - dely) + dely*vjj;
							lattice[i][j].m_Stress = -v0 / (2 * dely);
						}
					}
				}
			}//-1,-2
			else if (lattice[i][j + 1].m_iSol >= 0 && lattice[i][j - 1].m_iSol < 0 && lattice[i][j].m_iSol < 0)//0,4
			{
				lattice[i][j].m_Stress = (0 - lattice[i][j - 1].m_v.m_dx) / 2.0;
			}
			else if (lattice[i][j - 1].m_iSol >= 0 && lattice[i][j + 1].m_iSol < 0 && lattice[i][j].m_iSol < 0)//0,4
			{
				lattice[i][j].m_Stress = (lattice[i][j + 1].m_v.m_dx - 0) / 2.0;
			}
			else if (lattice[i][j + 1].m_iSol >= 0 && lattice[i][j - 1].m_iSol >= 0 && lattice[i][j].m_iSol<0)
			{
				lattice[i][j].m_Stress = 0;
			}
			else if (j>HEIGHT / 2)
			{
				if (lattice[i][j - 1].m_iSol < 0)
				{
					if (lattice[i][j - 1].m_iSol == lattice[i][j - 2].m_iSol&&lattice[i][j - 1].m_iSol == lattice[i][j - 3].m_iSol)
					{
						lattice[i][j].m_Stress = (lattice[i][j - 3].m_v.m_dx - lattice[i][j - 1].m_v.m_dx) / 2 - lattice[i][j - 2].m_v.m_dx;//外推
					}
					else
					{
						lattice[i][j].m_Stress = 0 - lattice[i][j - 1].m_v.m_dx;
					}
				}
				else
				{
					lattice[i][j].m_Stress = 0;
				}
			}
			else
			{
				if (lattice[i][j + 1].m_iSol < 0)
				{
					if (lattice[i][j + 1].m_iSol == lattice[i][j + 2].m_iSol&&lattice[i][j + 1].m_iSol == lattice[i][j + 3].m_iSol)
					{
						lattice[i][j].m_Stress = lattice[i][j + 2].m_v.m_dx - (lattice[i][j + 3].m_v.m_dx - lattice[i][j + 1].m_v.m_dx) / 2;//外推
					}
					else
					{
						lattice[i][j].m_Stress = lattice[i][j + 1].m_v.m_dx - 0;//是否外推一下？
					}
				}
				else
				{
					lattice[i][j].m_Stress = 0;
				}
			}
		}
	}
}

void SaveDensity(int k)
{
	int i, j;
	filep.open("aa.txt", ios::out);
	//filep << li1.m_LineV.m_p2.m_dx << " " << li1.m_LineV.m_iType << endl;
	for (i = 1; i < VALSEGNUM; i++)
	{
		filep << i << " " <<SolValve.m_MemUR.m_Seg[i]->m_pst.m_dx << " " << SolValve.m_MemUR.m_Seg[i]->m_pst.m_dy <<" "<< SolValve.m_MemUR.m_Seg[i]->m_v.m_dx << " "
			<< SolValve.m_MemUR.m_Seg[i]->m_v.m_dy << endl;
	}
	//	for(j=HEIGHT-1;j>=0;j--)
	//	for(j=0;j<HEIGHT/2;j++)
	/*for (j = 0; j < HEIGHT; j++)
	{
		for (i = 0; i < WIDTH_E; i++)
		{
			//filep<<lattice[i][j].m_dcNO-lattice[i][HEIGHT-1-j].m_dcNO<<" ";
			//filep<<lattice[i][j].m_v.m_dy+lattice[i][HEIGHT-1-j].m_v.m_dy<<" ";
			//if (lattice[i][j].m_dDen < 900 && lattice[i][j].m_dDen!=0)
			{
				//filep << lattice[i][j].m_bReaction << " ";
				//filep << lattice[i][j].m_iSol << " ";
				//filep << lattice[i][j].m_dcNO << " ";
				filep << lattice[i][j].m_dDen << " ";
				//filep << lattice[i][j].m_v.m_dx << " ";
				//filep << lattice[i][j].m_ipr << " ";
			}
			//else
			{
			//	filep << 1 << " ";
			}
		}
		filep << endl;
	}*/
	/*for (i = 0; i<LymVessle.m_il; i++)
	{
	filep << LymVessle.m_link[i].m_iSolx << " " << LymVessle.m_link[i].m_iSoly << " "
	<< LymVessle.m_link[i].m_iFlux << " " << LymVessle.m_link[i].m_iFluy << " "
	<< LymVessle.m_link[i].m_iSegs << " " << LymVessle.m_link[i].m_iSegf << " "
	<< LymVessle.m_link[i].m_bdb << " " << LymVessle.m_link[i].m_dDlf << " "
	<< LymVessle.m_link[i].m_dDls << " " << LymVessle.m_link[i].m_iDir << " "
	<< LymVessle.m_link[i].m_iType << " " << LymVessle.m_link[i].m_bf.m_dx << " "
	<< LymVessle.m_link[i].m_bf.m_dy << " " << LymVessle.m_link[i].m_bs.m_dx << " "
	<< LymVessle.m_link[i].m_bs.m_dy <<  " "
	<< endl;
	}*/
	/*	for (i = 0; i < 9; i++)
		{
			if (iMyID == 0)
			{
				filep << i << " " << lattice[467][19].m_df[i] << endl;
			}
			else
			{
				filep << i << " " << lattice[3][19].m_df[i] << endl;
			}
		}*/
	filep.close();
}

void TestDiscloseInit()
{
	int i, j, k;
	for (i = 0; i < WIDTH; i++)
	{
		for (j = 0; j<HEIGHT; j++)
		{
			if (lattice[i][j].m_iSol == -2 ||
				lattice[i][j].m_iSol == -1 && (i <= PX2 ||
				i>SolValve.m_MemDR.m_Seg[0]->m_pst.m_dx) ||
				lattice[i][j].m_iSol == -3 && i < WIDTH / 2)
			{
				lattice[i][j].m_dDen = 1;
			}
			else if (lattice[i][j].m_iSol == -1 || lattice[i][j].m_iSol == -3)
			{
				lattice[i][j].m_dDen = 1.5;
			}
			lattice[i][j].m_v.m_dx = 0;
			lattice[i][j].m_v.m_dy = 0;
			for (k = 0; k < 9; k++)
			{
				lattice[i][j].m_df[k] = Lattice::feq(k, lattice[i][j].m_dDen, 0, 0);
			}
		}
	}
}

double Flux()
{
	int i, j, ix, ix1, ix2;
	double Q;
	fstream ff;
	Q = 0;
	ix1 = int(0.35*WIDTH); ix2 = int(0.8*WIDTH);
	ix = int(WIDTH*9.0 / 10);
	for (j = 0; j < HEIGHT; j++)
	{
		for (i = ix1; i <= ix2; i++)
		{
			if (lattice[i][j].m_iSol == -1)
			{
				Q += lattice[i][j].m_v.m_dx;
			}
		}
	}
	Q /= (ix2 - ix1 + 1);
	ff.open(("Flux.txt"), ios::app | ios::out);
	ff.precision(18);
	ff << lNowStep << " " << Q << " " << LymVessle.m_MemUp.m_segment[(iBegin_Mem+iEnd_Mem)/2].m_dcCa << endl;
	ff.close();
	return Q;
}

void SaveNO()
{
	fstream ff;
	int i, j;
	ff.open((".\\dat\\NO.txt"), ios::out);
	ff.precision(18);
	for (j = HEIGHT - 1; j >= 0; j--)
	{
		for (i = 0; i < WIDTH; i++)
		{

			ff << lattice[i][j].m_dcNO << " ";
		}
		ff << endl;
	}
	ff.close();
}

void CommunicateLattice()//重复一格
{
	MPI_Barrier(MPI_COMM_WORLD);//!important
	MPI_Ibsend(&(lattice[iEnd_Row - 4][0].m_iSol), 1, Lattice_data_type
		, iRight, iTag[0], MPI_COMM_WORLD, Request);//send shared data to right 0,2
	MPI_Ibsend(&(lattice[iBegin_Row+1][0].m_iSol), 1, Lattice_data_type
		, iLeft, iTag[1], MPI_COMM_WORLD, Request + 1);//send shared data to left 1,3
	MPI_Irecv(&(lattice[iBegin_Row - 3][0].m_iSol), 1, Lattice_data_type, iLeft
		, iTag[0], MPI_COMM_WORLD, Request + 2);//receive shared data from left 0,2
	MPI_Irecv(&(lattice[iEnd_Row][0].m_iSol), 1, Lattice_data_type
		, iRight, iTag[1], MPI_COMM_WORLD, Request + 3);//receive shared data from right 1,3
	MPI_Waitall(4, Request, Status);
	MPI_Barrier(MPI_COMM_WORLD);
}

void TobeSame()//重复一格
{
	MPI_Barrier(MPI_COMM_WORLD);//!important
	MPI_Ibsend(&(lattice[iEnd_Row - 1][0].m_iSol), 1, Lattice_data_type0
		, iRight, iTag[0], MPI_COMM_WORLD, Request);//send shared data to right 0,2
	MPI_Irecv(&(lattice[iBegin_Row][0].m_iSol), 1, Lattice_data_type0, iLeft
		, iTag[0], MPI_COMM_WORLD, Request + 1);//receive shared data from left 0,2
	MPI_Waitall(2, Request, Status);
	MPI_Barrier(MPI_COMM_WORLD);
}

void CommunicateMem()//
{//E=SN-1,E-1缓存，E-2交换
	MPI_Barrier(MPI_COMM_WORLD);//!important
	MPI_Ibsend(&(LymVessle.m_MemUp.m_segment[iEnd_Mem - 2].m_bPassTh), 1, Mem_data_type
		, iRight, iTag[0], MPI_COMM_WORLD, Request);//send shared data to right 0,2
	MPI_Ibsend(&(LymVessle.m_MemUp.m_segment[iBegin_Mem].m_bPassTh), 1, Mem_data_type
		, iLeft, iTag[1], MPI_COMM_WORLD, Request + 1);//send shared data to left 1,3
	MPI_Irecv(&(LymVessle.m_MemUp.m_segment[iBegin_Mem - 1].m_bPassTh), 1, Mem_data_type, iLeft
		, iTag[0], MPI_COMM_WORLD, Request + 2);//receive shared data from left 0,2
	MPI_Irecv(&(LymVessle.m_MemUp.m_segment[iEnd_Mem-1].m_bPassTh), 1, Mem_data_type
		, iRight, iTag[1], MPI_COMM_WORLD, Request + 3);//receive shared data from right 1,3
	MPI_Waitall(4, Request, Status);
	MPI_Barrier(MPI_COMM_WORLD);//!important
	LymVessle.CorrectX();
}

/*void CommunicateAll()//重复一格
{
	MPI_Barrier(MPI_COMM_WORLD);//!important
	MPI_Ibsend(&(lattice[iEnd_Row - 4][0].m_iSol), 1, All_data_type
		, iRight, iTag[0], MPI_COMM_WORLD, Request);//send shared data to right 0,2
	MPI_Ibsend(&(lattice[iBegin_Row+1][0].m_iSol), 1, All_data_type
		, iLeft, iTag[1], MPI_COMM_WORLD, Request + 1);//send shared data to left 1,3
	MPI_Irecv(&(lattice[iBegin_Row - 3][0].m_iSol), 1, All_data_type, iLeft
		, iTag[0], MPI_COMM_WORLD, Request + 2);//receive shared data from left 0,2
	MPI_Irecv(&(lattice[iEnd_Row][0].m_iSol), 1, All_data_type
		, iRight, iTag[1], MPI_COMM_WORLD, Request + 3);//receive shared data from right 1,3
	MPI_Waitall(4, Request, Status);
	MPI_Barrier(MPI_COMM_WORLD);//!important
	LymVessle.CorrectX();
}*/

void SaveForAnimate()
{
	MPI_File File;
	MPI_Status Status;
	int i, j;
	MPI_Offset Offset;
	stringstream stream;
	string str;
	char cc[10] = { "native" };
	stream << iNumProcs;
	str = "animate" + stream.str()+ "ChValve.txt";
	anidata.m_lNowStep = lNowStep;
	anidata.m_Permeat = PerLine;
	anidata.m_Valve = SolValve;
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			anidata.m_Lattice[i - iBegin_Row][j] = lattice[i][j];
		}
	}
	LymVessle.ForAnimate(&anidata.m_lymani);
	anidata.StoreV();
	anidata.m_bfl = anidata.m_bfl0;
	if (anidata.zip() != Z_OK)
	{
		printf("压缩失败"); exit(1);
	} 
	pDataSize[iMyID].iSize = anidata.m_bfl;//int linux windows 都是4字节
	for (i = 0; i < iNumProcs; i++)
	{
		MPI_Bcast(&(pDataSize[i].iSize), 1, MPI_INT, i, MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	pDataSize[0].iOffset = 0;
	for (i = 1; i < iNumProcs; i++)
	{
		pDataSize[i].iOffset = pDataSize[i - 1].iOffset + sizeof(pDataSize[i - 1].iSize) +pDataSize[i - 1].iSize;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_open(MPI_COMM_WORLD, (char*)str.data(), MPI_MODE_RDWR | MPI_MODE_APPEND | MPI_MODE_CREATE, MPI_INFO_NULL, &File);
	if (iMyID == 0)
	{
		MPI_File_get_position(File, &Offset);
	}
	MPI_Bcast(&Offset, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
	MPI_File_set_view(File, pDataSize[iMyID].iOffset + Offset, MPI_CHAR, MPI_CHAR, cc, MPI_INFO_NULL);
	MPI_File_write(File, (char*)&(pDataSize[iMyID].iSize), 1, MPI_INT, &Status);
	MPI_File_write(File, (char*)anidata.m_bf, anidata.m_bfl, MPI_BYTE, &Status);
	printf("Data write %ld\n", anidata.m_bfl);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_close(&File);
}

void ChangePress()
{
	//if (lNowStep %20475000  == 0)
	{
		switch ((lNowStep / 6554166) % 3)
		{
		case 0:
			dInletDensity = 1.0;
			dOutletDensity = (1.0 - 1 * 0);
			break;
		case 1:
			dInletDensity = 1.0;
			dOutletDensity = (1.0 - 1 * 0.0002); //-1 * 0.001);
			break;
		case 2:
			dInletDensity = 1.0;
			dOutletDensity = (1.0 + 1 * 0.0002);// 0.0005);
			break;
		default:
			;
		}
	}
}

int IsPorous(int i,int j)
{
	int k,ix,iy;
	for (k = 1; k < 9; k++)
	{
		ix = i + iPrjx[k];
		iy = j + iPrjy[k];
		if (ix < 0 || ix >= WIDTH_E - 1 || iy < 0 || iy >= HEIGHT - 1)
		{
			return 0;
		}
		if (lattice[ix][iy].m_iSol != -2)
		{
			return 0;
		}
	}
	return 1;
}

void ActivePorous()
{
	int i, j, k, ix, iy;
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (lattice[i][j].m_ipr)
			{
				if (IsPorous(i, j))
				{
					lattice[i][j].m_ipr = 2;
				}
				else if (lattice[i][j].m_ipr == 2)
				{
					lattice[i][j].m_ipr = 3;
				}
			}
		}
	}
}

void InsertNonActivePorous()
{
	int i, j, k, ix, iy, m, n, ix1, iy1, ix2, iy2;
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (lattice[i][j].m_ipr == 3)//待外推点
			{
				m = 0;
				lattice[i][j].Clear();
				for (k = 1; k < 9; k++)
				{
					ix1 = i + iPrjx[k];
					iy1 = j + iPrjy[k];
					if (lattice[ix1][iy1].m_iSol == -2 && lattice[ix1][iy1].m_ipr != 2 &&
						lattice[ix1][iy1].m_ipr != 3)
					{
						ix2 = ix1 + iPrjx[k];
						iy2 = iy1 + iPrjy[k];
						if (lattice[ix2][iy2].m_iSol==-2&&lattice[ix2][iy2].m_ipr != 2 &&
							lattice[ix2][iy2].m_ipr != 3)//要外推的两都同为内点或外点
						{
							for (n = 0; n < 9; n++)
							{
								lattice[i][j].m_df[n] += 2 * lattice[ix1][iy1].m_df[n] - lattice[ix2][iy2].m_df[n];
							}
							m++;
						}
						else//否则直接用好了
						{
							for (n = 0; n < 9; n++)
							{
								lattice[i][j].m_df[n] += lattice[ix1][iy1].m_df[n];
							}
							m++;
						}
					}
				}//for k
				if (m != 0)
				{
					for (n = 0; n < 9; n++)
					{
						lattice[i][j].m_df[n] /= m;
						lattice[i][j].m_dDen += lattice[i][j].m_df[n];
						lattice[i][j].m_v.m_dx += iPrjx[n] * lattice[i][j].m_df[n];
						lattice[i][j].m_v.m_dy += iPrjy[n] * lattice[i][j].m_df[n];
					}
					lattice[i][j].m_v.m_dx /= lattice[i][j].m_dDen;
					lattice[i][j].m_v.m_dy /= lattice[i][j].m_dDen;
				}
				else
				{
					printf("Porous Insert Fluid error");
				}
			}
		}
	}
	for (i = iBegin_Row; i < iEnd_Row; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (lattice[i][j].m_ipr == 3)
			{
				lattice[i][j].m_ipr = 1;//去激活
			}
		}
	}
}

void PorousCollide()
{
	int i, j, k, ix, iy;
	for (i = 0; i < WIDTH_E; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			if (lattice[i][j].m_ipr == 2)//管外为porous的点
			{
				for (k = 1; k < 9; k++)
				{
					ix = i + iPrjx[k];
					iy = j + iPrjy[k];
					if (ix >= 0 && ix < WIDTH_E&&iy >= 0 && iy < HEIGHT&&lattice[ix][iy].m_iSol == -2)
					{
						lattice[i][j].m_df[k] = lattice[ix][iy].m_df[iRev[k]];
					}
				}
			}
		}
	}
}

void PartialBounceback(int iPosition)
{
	int i,k;
	double dFraction = 0.85;
	if (iClose)
	{
		dFraction = 1;
	}
	for (i = 0; i <= HEIGHT; i++)
	{
		if (lattice[iPosition][i].m_iSol == -1)
		{
			if (lattice[iPosition][i].m_v.m_dx < 0)
			{
				lattice[iPosition][i].m_v = Vector(0, 0);
				for (k = 1; k < 9; k++)
				{
					if (lattice[iPosition + iPrjx[k]][i + iPrjy[k]].m_iSol ==-1)
					{
						lattice[iPosition][i].m_df[k] = dFraction*lattice[iPosition + iPrjx[k]][i + iPrjy[k]].m_df[iRev[k]]
							+ (1 - dFraction)* lattice[iPosition][i].m_df[k];
					}
				}
			}
		}
	}
}

void IncreasPress()
{
	int i, j,k;
	if (iCountP > 1)
	{
		for (i = 0; i < SEGNUM_E; i++)
		{
			LymVessle.m_MemUp.m_segment[i].m_dcCa = LIMITCA;
			LymVessle.m_MemDown.m_segment[i].m_dcCa = LIMITCA;
		}
		dkm = 3 * double(iCountP+1)/iTotal* dkm0;
		if (LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2) - 1].m_v.m_dy*LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2)].m_v.m_dy < 0 &&
			LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2)].m_v.m_dy*LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2) + 1].m_v.m_dy < 0)
		{
			ViscousMem = 0.1;
		}
		else
		{
			ViscousMem = 0;
		}
	}
	else if (iCountP == 1)
	{
		dkm = dkm0;
		for (i = 0; i < SEGNUM_E; i++)
		{
			LymVessle.m_MemUp.m_segment[i].m_dcCa = 0.09;
			LymVessle.m_MemDown.m_segment[i].m_dcCa = 0.09;
		}
		ViscousMem = 0;
	}
	/*if (double(iTotal - iCountP) / iTotal >= double(iMyID) / iNumProcs)//>= 0 && iMyID<8 || iMyID >= iNumProcs / 3 * 2)//if (iMyID == 0)
	{
		if (iCountP > 1)
		{
			for (i = 0; i < SEGNUM_E; i++)
			{
				LymVessle.m_MemUp.m_segment[i].m_dcCa = LIMITCA;
				LymVessle.m_MemDown.m_segment[i].m_dcCa = LIMITCA;
			}
			dkm = 3 * dkm0;
			if (LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2) - 1].m_v.m_dy*LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2)].m_v.m_dy < 0 &&
				LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2)].m_v.m_dy*LymVessle.m_MemUp.m_segment[int((LEFTVALVE + RIGHTVALVE) / 2) + 1].m_v.m_dy < 0)
			{
				ViscousMem = 0.1;
			}
			else
			{
				ViscousMem = 0;
			}
		}
		else if (iCountP == 1)
		{
			dkm = dkm0;
			for (i = 0; i < SEGNUM_E; i++)
			{
				LymVessle.m_MemUp.m_segment[i].m_dcCa = 0.09;
				LymVessle.m_MemDown.m_segment[i].m_dcCa = 0.09;
			}
			ViscousMem = 0;
		}
	}*/
}



	/*if (iMyID == 0)
	{
		for (i = 0; i < WIDTH_E; i++)
		{
			for (j = 0; j < HEIGHT; j++)
			{
				if (lattice[i][j].m_iSol == -2)
				{
					lattice[i][j].m_dDen = dDensity + 0.1;
					for (k = 0; k < 9; k++)
					{
						lattice[i][j].m_df[k] = Lattice::feq(k, lattice[i][j].m_dDen, lattice[i][j].m_v.m_dx, lattice[i][j].m_v.m_dy);
					}

				}
			}
		}
	}
}*/

void Average(int ix1, int ix2)
{
	int i, j, k;
	double df;
	Vector vv;
	if (lattice[ix2][HEIGHT / 2].m_v.m_dx*lattice[ix2 - 1][HEIGHT / 2].m_v.m_dx < 0 ||
		lattice[ix2][H1 + 1].m_v.m_dx*lattice[ix2 - 1][H1 + 1].m_v.m_dx < 0 ||
		lattice[ix2][H2 - 1].m_v.m_dx*lattice[ix2 - 1][H2 - 1].m_v.m_dx < 0)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			for (k = 0; k < 9; k++)
			{
				df = 0;
				for (i = ix1; i <= ix2; i++)
				{
					if (lattice[i][j].m_iSol == -1)
					{
						df += lattice[i][j].m_df[k];
					}
				}
				df /= (ix2 - ix1 + 1);
				for (i = ix1; i <= ix2; i++)
				{
					if (lattice[i][j].m_iSol == -1)
					{
						lattice[i][j].m_df[k] = df;
					}
				}
			}
			vv = 0;
			for (i = ix1; i <= ix2; i++)
			{
				if (lattice[i][j].m_iSol == -1)
				{
					lattice[i][j].m_dDen = 0;
					lattice[i][j].m_v.m_dx = 0;
					lattice[i][j].m_v.m_dy = 0;
					for (k = 0; k < 9; k++)
					{
						lattice[i][j].m_dDen += lattice[i][j].m_df[k];
						lattice[i][j].m_v.m_dx += lattice[i][j].m_df[k]
							* iPrjx[k];
						lattice[i][j].m_v.m_dy += lattice[ix1][j].m_df[k]
							* iPrjy[k];
					}

					lattice[i][j].m_v.m_dx /= lattice[i][j].m_dDen;
					lattice[i][j].m_v.m_dy /= lattice[i][j].m_dDen;
				}
			}
		}
	}
}
