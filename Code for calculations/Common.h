//#include "Resource.h"
#include "mpi.h"
#include  "cudastr.h"
#include "Lattice.h"
#include <fstream>
#include "lbmc.h"
#include "math.h"
#include "lymph_h.h"
#include "LymphaticVessle.h"
#include "PermeatLine.h"
#include "Line.h"
#include "Boundary.h"
#include "SolftValve.h"
#include "AnimateData.h"
#include "SolftValve.h"
#define ZERO 1.0e-13
using namespace std;
#pragma once

extern int iClose;
extern int GPUID;
extern int GPUst;
extern double dDeltaR;
extern int iPrjx[];
extern int iPrjy[];
extern int iPrjxy[2][9];
extern int iySym[];
extern int iRev[];
extern double dAngle[];
extern double dCoe[];
extern double dEpsilon;
extern double dTau;
extern double dNau;
extern double dDensity;
extern double dMass;
extern double dBending;
extern double dTotal;
extern int iCycle;
extern double Ca0,NO0;
extern double dcCa[100];//画Ca增长的
extern double dcNO[100];
extern double dPorous;
extern double dGravity;
extern double dGravityAngle;
extern double dDiameter;
extern double dOutletDensity;
extern double dInletDensity;
extern double dSpCa;
extern double dCaBuffUp[SEGNUM_E];
extern double dCaBuffDown[SEGNUM_E];
extern double dDelta;
extern int iTotal;
extern double dkm0;
extern double dkm;

extern Valve_str valve;
extern double Shrink;//收缩弹性
extern double Bending;//拉伸弹性
extern double DCa;//限制弹性
extern double Viscous;//粘滞系数
extern double ViscousMem;
extern double ViscousValve;
extern double CaDecay;//Ca衰减率
extern double CaThreshold;//Ca极限系数
extern double CaPeak;//Ca最大值
extern double CaNODecay;//Ca,NO的联合衰减率
extern double CaProduct;//Ca产生率
extern double NOSpread;//NO扩散系数
extern double NODecay;//NO的衰减率
extern double dR0;//淋巴管的基准半径
extern double WanR0;//范德瓦尔兹起作用的最初距离
extern double dMemDensity;
extern double dValveMemDensity;
extern LymphaticVessle LymVessle;
extern PermeatLine PerLine,li1;
extern SolftValve SolValve;
//extern Valve ValveDual;

extern long lStep;
extern long lNowStep;
extern long lStartStep;
extern long lSave;
extern int iFile;
extern int iNewton;
extern long lStopLeak;
extern int iDelay;
extern double dx0,dy0;
extern double dhPI;
extern double dStregth;
extern double dResistance;

extern int GCardNum;
extern int bCanCuda;
extern AnimateData anidata;

//MPI
extern char chrName[100];
extern int InitFromFile;
extern int iRight, iLeft, iTag[4];
extern MPI_Request Request[4];
extern MPI_Status Status[4];
extern int iMyID, iNumProcs, iLen;
extern int iBegin_Row, iEnd_Row, iBegin_Mem, iEnd_Mem;
extern MPI_Datatype Lattice_data_type0;
extern MPI_Datatype Lattice_data_type;
extern MPI_Datatype Mem_data_type;
extern MPI_Datatype One_Mem_data_type;
extern MPI_Datatype One_Valve_Mem_data_type;
extern int iBufferSize;
extern char* SendBuffer;
extern data_str * pDataSize;

//GPU
extern parameter pa;

extern char szFile [BUFFER_LENGTH];
extern char szFileTitle [BUFFER_LENGTH];
extern char strDir[70];

extern int bDrawNow;
extern int bShear;

extern Lattice lattice[WIDTH_E][HEIGHT];
extern double latbuf[WIDTH_E][HEIGHT][8];
extern double NOBuf[WIDTH_E][HEIGHT];
extern int bPulse;
extern double dkNO;

extern int iStep;
extern int iBouNum;
extern double h;
extern double dValveDensity;

extern fstream filep,filea,FileRead;

extern int argc;
extern char **argv;

extern double Test;
extern int iCountP;


double DotMet(int,double,double);
//double feq(int Order,double Den,double Velx,double Vely);

int GetInt(double data);
int MaxInt(double x);
int GetModx(int Data);
double GetModx(double Data);
int GetMody(int Data);
double GetMody(double Data);

int BetweenValve(int j);
double ErrorCorrect(double x,double x1,double x2);
int InRegionX(double x);

# pragma once
#pragma pack(8)
class testcc
{
public:
	double m_dx;
	double m_dy;
	testcc(double dx = 0, double dy = 0);
	~testcc();
	testcc& operator=(const testcc &v);
	testcc operator*(const double &v);
	/*testcc& operator*=(const double &v);
	testcc operator/(const double &v);
	testcc& operator/=(const double &v);
	double operator*(const testcc &v);*/
	/*double operator^(const testcc &v);
	testcc operator+(const double &v);
	testcc operator+(const testcc &v);
	testcc& operator+=(const testcc &v);
	testcc operator-();
	testcc operator-(testcc v);
	testcc& operator-=(testcc v);
	testcc& Roll(double v);
	double Mod();
	friend fstream& operator<<(fstream& fs, testcc& v);
	friend fstream& operator>>(fstream& fs, testcc& v);*/
};
#pragma pack()