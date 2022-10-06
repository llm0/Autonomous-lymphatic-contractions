// LymphaticVessle.h: interface for the LymphaticVessle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LYMPHATICVESSLE_H__E4FAC06A_F41E_4EB1_952A_58E7D4A8B797__INCLUDED_)
#define AFX_LYMPHATICVESSLE_H__E4FAC06A_F41E_4EB1_952A_58E7D4A8B797__INCLUDED_
#include <fstream>
#include "Membrane.h"
#include "LymphAnimate.h"
#include "SolftValve.h"
#include "mpi.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LymphaticVessle  
{
public:
	int m_ixn;
	int m_iyn;
	int m_ixm;
	int m_iym;
	static Lattice (*m_lattice)[HEIGHT];
	Link m_link[LINKNUM];
	int m_il;
	Membrane m_MemUp;
	Membrane m_MemDown;
	static int* m_iPrjx;
	static int* m_iPrjy;
	static int (*m_iPrjxy)[9];
	static int* m_iRev;
	static double* m_dAngleE;
	static double* m_dCoe;
	static double m_dTau;
	static double m_dDensity;
	void ForAnimate(LymphAnimate* anid);
	void SaveForAnimate(fstream *ff);
	void AverageStress_L();
	void AverageCaForce_M();
	void Activate_M();
	void SaveCa();
	void CaReaction();
	void CaDiffusion_M();
	void BackupPosition_M();
	void CalciumForce_M();
	void VescoAcc_M(int j);
	void Angle_M();
	void SavePosition(fstream *output);
	void ReMark_L();
	void Insert_L();
	int OnSameSide(int y1, int y2);
	friend MPI_File& operator<<(MPI_File& fs, LymphaticVessle& v);
	friend MPI_File & operator>>(MPI_File & fs, LymphaticVessle& v);
	void ElastAcc_M(int j);
	void Move_M(int);
	void HydroAcc_M();
	void Force_M();
	void Collide_L();
	void FindLink_L();
	LymphaticVessle();
	void init_LM(int bFromFile);
	void Pack(char* buffer, int size, int * position);
	void Unpack(char* buffer, int size, int * position);
	int IsInner(int ix, int iy);
	void DrawLymph_L();
	int OnMem(int i, int j);
	int SamePoint(int i, int j, Vector v);
	void CaAverage_M();
	void CorrectX();
	virtual ~LymphaticVessle();

};

#endif // !defined(AFX_LYMPHATICVESSLE_H__E4FAC06A_F41E_4EB1_952A_58E7D4A8B797__INCLUDED_)
