#include "MemValve.h"
#include "Lattice.h"
#pragma once
class SolftValve
{
public:
	SolftValve();
	~SolftValve();
	friend MPI_File& operator<<(MPI_File& fs, SolftValve& v);
	friend MPI_File& operator>>(MPI_File& fs, SolftValve& v);
public:
	MemValve m_MemDL;
	MemValve m_MemUL;
	MemValve m_MemDR;
	MemValve m_MemUR;
	int m_ixn;
	int m_iyn;
	int m_ixm;
	int m_iym;
	Link m_link[LINKNUM];
	int m_il0;
	int m_il;
	int m_lActive;
	int m_rActive;
public:
	Segment** m_SegDL0;
	Segment** m_SegUL0;
	Segment** m_SegDR0;
	Segment** m_SegUR0;
	static Lattice(*m_lattice)[HEIGHT];
	static int* m_iPrjx;
	static int* m_iPrjy;
	static int(*m_iPrjxy)[9];
	static int* m_iRev;
	static double* m_dCoe;
	static double m_dTau;
	static double m_dDensity;
	static double m_kb;
	static double m_dDeltax;
	void InitMem();
	void Init(int bFromFile=0);
	double y0(int i, int iType);
	double yLimit(int i, int iType);
	void DrawValve();
	void FirstDrawValve();
	void Remark();
	int IsInner(int ix, int iy);
	int Between(int ix, int *i, int *j);
	void FindLink();
	void FindLink(ValveSegment** su, ValveSegment**sd);
	int OnMem(int i, int j);
	int SamePoint(int i, int j, Vector v);
	void Angle(ValveSegment** v);
	void Angle();
	void Collide();
	void Force();
	void HydroAcc();
	void ElastAcc();
	void VescoAcc();
	void Update();
	void Move(int j);//Valve 夹在两膜之间，新点插值交给膜做
	void Contact();
	void LimitFirstPoint();
	void Pack(char* buffer, int size, int* position);
	void Unpack(char* buffer, int size, int* position);
	//void Draw(CDC* pDC, CRect rect, double k);
	//void Draw(MemValve * mem, CDC* pDC, CRect rect, double k);
};

