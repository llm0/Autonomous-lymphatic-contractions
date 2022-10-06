// PermeatLine.h: interface for the PermeatLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERMEATLINE_H__4CCBBECE_9311_4C48_9ADA_ED6F266951A8__INCLUDED_)
#define AFX_PERMEATLINE_H__4CCBBECE_9311_4C48_9ADA_ED6F266951A8__INCLUDED_
#include "Line.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Lattice.h"
#include "lymph_h.h"
#include "Boundary.h"

class PermeatLine  
{
public:
	void SaveForAnimate(fstream* ff);


	void init(int bf);
	
	Line m_LineV;
	Line m_LineDown;
	Line m_LineUp;
	double m_dk;//dk大于100时用m_dk_re;
	static Lattice (*m_lattice)[HEIGHT];
	static int* m_iPrjx;
	static int* m_iPrjy;
	static int (*m_iPrjxy)[9];
	static int* m_iRev;
	static int m_iBouNum;
	static Boundary * m_pBoud;
	void Collide();
	void FindLink();
	void DrawPermeatArea();
	double Curvebound(int x,int y,int Order,double delta);
	void Oneway();
	void Pack(char* buffer, int  size, int* position);
	void Unpack(char* buffer, int  size, int* position);
	friend MPI_File& operator<<(MPI_File& fs, PermeatLine& v);
	friend MPI_File& operator>>(MPI_File& fs, PermeatLine& v);
	PermeatLine();
	virtual ~PermeatLine();

};

#endif // !defined(AFX_PERMEATLINE_H__4CCBBECE_9311_4C48_9ADA_ED6F266951A8__INCLUDED_)
