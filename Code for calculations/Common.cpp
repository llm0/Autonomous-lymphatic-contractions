#include "Common.h"
#include "math.h"
#include <fstream>

int iClose = 0;
int GPUID = 0;
int GPUst = 0;
double h = 0.03;// 0.025;//0.027;// 0.03;// 0.025;// 0.04;// 0.025;// 0.03;// 0.03;//0.05;// // 0.018;// 0.015; // 0.03;//0.022;//  0.3;// 0.1;// 0.03;// 1;// 0.06;// 0.04;// 0.02;// 0.015;//0.01;// 0.03;// 0.04s;// 0.02S;//0.02;// 0.025;// 0.06;//10;//1;//10;//(1/T)//0.65
double dValveDensity = 1.0;
double dkNO = 20;//  200;// 300L;// 200;//400;//300;// 100s;// 1500;//1000;//原来 X v=(2t-1)/6=0.083
double dDeltaR=HEIGHT/5;
double Ca0 = 0.0999;//0.0349999999; //0.0351;//  0.014999999; //0.016; 
double NO0 = 0;// 0.05;
double dPorous = 0.05;// 0.1;
double dStregth = 0.3;
double dResistance = 0;// 0.5;//0.2; //bounce back ratio
double dGravity;
double dGravityAngle;
double dEpsilon;
double dNau;
double dDensity;
double dDenTal;
double dMass;
double dBending;
double dTotal;
double dhPI;
double dcCa[100];
double dcNO[100];
double dDiameter;
double dDelta = PI / 100;

//并行用变量
int GCardNum;
int bCanCuda;
AnimateData anidata;
int InitFromFile = 0;

//MPI
char chrName[100];
int iRight, iLeft, iTag[4] = { 1, 2, 3, 4 };
MPI_Request Request[4];
MPI_Status Status[4];
int iMyID, iNumProcs, iLen;
int iBegin_Row, iEnd_Row, iBegin_Mem, iEnd_Mem;
MPI_Datatype Lattice_data_type0;
MPI_Datatype Lattice_data_type;
MPI_Datatype Mem_data_type;
MPI_Datatype One_Mem_data_type;
MPI_Datatype One_Valve_Mem_data_type;
int iBufferSize;
char* SendBuffer;
data_str * pDataSize = NULL;

//GPU
parameter pa;

double Shrink;//收缩弹性
double Bending;//拉伸弹性
double Confine;//限制弹性
double DCa;//限制半径
double Viscous;//粘滞系数
double ViscousMem;
double ViscousValve;
double CaDecay;//Ca衰减率
double CaThreshold;//Ca临界值
double CaPeak;//Ca最大值
double CaNODecay;//Ca,NO的联合衰减率
double CaProduct;//Ca产生率
double dR0;//淋巴管的基准直径
double WanR0;//范德瓦尔兹起作用的最初距离
double dMemDensity;
double dValveMemDensity=1;
double dSpCa;
double dCaBuffUp[SEGNUM_E];
double dCaBuffDown[SEGNUM_E];

int iCycle;
int iStep;

Valve_str valve;
                 // 0, 1, 2,   3,    4,    5,     6,   7,      8 九速模型的速度集
int iPrjx[] = { 0, 1, -1, 0, 0, 1, -1, 1, -1};
int iPrjy[] = { 0, 0, 0, 1, -1, -1, 1, 1, -1};//9
double dCoe[] = { W0, W1, W1, W1, W1, W2, W2, W2, W2 };
double dTau;//19
double NOSpread;//NO扩散系数20
double NODecay;//NO的衰减率21
int bPulse = 0;


int iPrjxy[2][9]= { 0, 1,-1,   0,    0,    1,    -1,   1,     -1,
			        0, 0, 0,   1,   -1,   -1,     1,   1,     -1};
int iRev[] = { 0, 2, 1, 4, 3, 6, 5, 8, 7 };//18
int iySym[]=	  { 0 ,1, 2,   4,    3,    7,     8,   5,      6};
double dAngle[]=  { 0, 0,PI,PI/2,-PI/2,-PI/4,3*PI/4,PI/4,-3*PI/4};
//double Ca0 = 0.09, NO0 = 0;// 0.05;
long lStep=100000;
long lNowStep;
long lStartStep=1;
long lSave=0;
int iFile=0;
int iNewton = 100;//100;//60;
long lStopLeak=-1;
int iDelay=-1;
double dx0,dy0;
double dOutletDensity = (1.0 + 0);// 0.015);// +0.1);// = 0.001);//- 0.001//(1.0 +0.000085);// - 0.00001);// 0.00013);//0.000085);// 0.00009946949601);//-0.000058023872679);//- 0.00001);//+ 0.00006631299734748);// -0.00003315649867374);//- 0.00008289124668);/////- 0.000049734748);//+ 0.0001326);// 0.0002);//- 0.00001);//- 0.0001);// 0.0003);//- 0.00001);// +0.0005);//- 0.001);// +0.0008);// 低：0,中：0.001(能振荡）,高：0.05
double dInletDensity = 1.0;

LymphaticVessle LymVessle;
PermeatLine PerLine,li1;
SolftValve SolValve;

char szFile [BUFFER_LENGTH];
char szFileTitle [BUFFER_LENGTH];
char strDir[70];

int bDrawNow;
int bShear;

Lattice lattice[WIDTH_E][HEIGHT]; //lattice[WIDTH][HEIGHT];
double latbuf[WIDTH_E][HEIGHT][8];
double NOBuf[WIDTH_E][HEIGHT];


fstream filep,filea,FileRead;

double Test;
//int iCountP =100000;
int iCountP = 500000;
int iTotal;
double dkm0 = 0.05;
double dkm;
int argc;
char **argv;

int GetInt(double data)
{
	if(data>=0)
	{
		return int(data+0.1);
	}
	else
	{
		return int(data-0.1);
	}
}

int GetModx(int Data)
{
	return (Data+WIDTH_E)%WIDTH_E;
}

double GetModx(double Data)
{
	return fmod(Data+WIDTH_E,WIDTH_E);
}

int GetMody(int Data)
{
	return (Data+HEIGHT)%HEIGHT;
}

double GetMody(double Data)
{
	return fmod(Data+HEIGHT,HEIGHT);
}

int MaxInt(double x)
{
	if(x==int(x))
	{
		return int(x);
	}
	else
	{
		return int(x)+1;
	}
}

double ErrorCorrect(double x,double x1,double x2)//已知x位于x1和x2之间，计算x如果超出这个范围可能导致致命结果，必须修正
{
	if(x1<=x2)
	{
		if(x<=x1)
		{
			return x1;
		}
		else if(x>=x2)
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
		if(x<=x2)
		{
			return x2;
		}
		else if(x>=x1)
		{
			return x1;
		}
		else
		{
			return x;
		}
	}
}

int InRegionX(double x)
{
	if (iMyID > 0 && iMyID < iNumProcs - 1 && x >= 0 && x < WIDTH_E)
	{
		return 1;
	}
	else if (iMyID == 0 && x >= iBegin_Row &&  x < WIDTH_E)
	{
		return 1;
	}
	else if (iMyID == iNumProcs - 1 && x >= 0 && x < iEnd_Row)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}