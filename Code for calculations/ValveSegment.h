#pragma once
#include "Segment.h"
#define DELTA 0.5
class ValveSegment :
	public Segment
{
public:
	double m_y0;
	double m_ym;
	void Init();
	void ElastAcc(int n1,int n2);
	void HydroAcc();
	void VescoAcc();
	double kchange(double k1, double k2, double x);
public:
	static double m_dk;
	static double m_dm;

public:
	ValveSegment();
	~ValveSegment();
};

