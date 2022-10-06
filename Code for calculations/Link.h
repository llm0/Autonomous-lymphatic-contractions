// Link.h: interface for the Link class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINK_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
#define AFX_LINK_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <fstream>
#include "Vector.h"
using namespace std;

class Link  
{
public:
	Link(int Solx=0,int Soly=0,int Flux=0,int Fluy=0,double Boux=0,double Bouy=0,int Dir=0,double Delta=0);
	~Link();
public://define members
	int m_iSegf;
	int m_iSegs;
	int m_iType;
	int m_iSolx;
	int m_iSoly;
	int m_iFlux;
	int m_iFluy;
	int m_iDir;
	Vector m_bf;//Ò»Á´´©Ë«Ä¤
	Vector m_bs;
	double m_dDlf;
	double m_dDls;
	int m_bdb;//Ë«Ä¤±êÖ¾
public://define operators
	Link& operator = (const Link &v);
	Link operator -();
	friend fstream& operator<<(fstream& fs, Link& v);
};

#endif // !defined(AFX_LINK_H__01398C4E_0E8B_11D6_845F_00E04C392A24__INCLUDED_)
