#pragma once
#include "ValveSegment.h"
#define ALFA0 (12*PI/180)
#define ALFAL (9*PI/180)
class MemValve
{
public:
	MemValve();
	~MemValve();
	friend MPI_File& operator<<(MPI_File& fs, MemValve& v);
	friend MPI_File& operator>>(MPI_File& fs, MemValve& v);
public:
	ValveSegment *m_Seg[VALSEGNUM];
	ValveSegment m_SegData[VALSEGNUM - 1];
	int m_iType;
public:
	static double m_dDeltax;
	void InitMem();
	void Init(int bFromFile = 0);
	double y0(int i, int iType,double dDeltay);
	double yLimit(int i, int iType);
	int OnMem(int i, int j);
	int SamePoint(int i, int j, Vector v);
	void Angle(ValveSegment** v);
	void Angle();
	void Force();
	void HydroAcc();
	void ElastAcc(int j,int n1,int n2);
	void VescoAcc(int j);
	void Move(int j);//Valve 夹在两膜之间，新点插值交给膜做
	void Update();
	void LimitFirstPoint();
};

