// Link.cpp: implementation of the Link class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Link.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Link::Link(int Solx,int Soly,int Flux,int Fluy,double Boux,double Bouy,int Dir,double Delta)
{
	m_iSolx=Solx;
	m_iSoly=Soly;
	m_iFlux=Flux;
	m_iFluy=Fluy;
	m_bf.m_dx=Boux;
	m_bf.m_dy=Bouy;
	m_iDir=Dir;
	m_dDlf=Delta;
	m_bdb = 0;
}

Link::~Link()
{

}

Link& Link::operator=(const Link &v)
{
	m_iSolx = v.m_iSolx;
	m_iSoly = v.m_iSoly;
	m_iFlux = v.m_iFlux;
	m_iFluy = v.m_iFluy;
	m_bdb = v.m_bdb;
	m_bf = v.m_bf;
	m_dDlf=v.m_dDlf;
	m_iDir=v.m_iDir;
	m_iSegf = v.m_iSegf;
	m_iType = v.m_iType;
	return *this;
}

Link Link::operator -()
{
	Link link;
	link.m_iSolx=m_iFlux;
	link.m_iSoly=m_iFluy;
	link.m_iFlux=m_iSolx;
	link.m_iFluy=m_iSoly;
	link.m_iDir=iRev[m_iDir];
	link.m_bdb = m_bdb;
	if (!m_bdb)
	{
		link.m_bf = m_bf;
		link.m_dDlf = 1 - m_dDlf;
		link.m_iSegf = m_iSegf;
		link.m_iType = m_iType;
	}
	else
	{
		link.m_bf = m_bs;
		link.m_bs = m_bf;
		link.m_dDlf = m_dDls;
		link.m_dDls = m_dDlf;
		link.m_iType = -m_iType;
		link.m_iSegf = m_iSegs;
		link.m_iSegs = m_iSegf; 
	}
	return link;
}

fstream& operator<<(fstream& fs, Link& v)
{
	fs<<v.m_iSolx<<" ";
	fs << v.m_iSoly << " ";
	fs << v.m_iFlux << " ";
	fs << v.m_iFluy << " ";
	fs << v.m_bf.m_dx << " ";
	fs << v.m_bf.m_dy << " ";
	fs << v.m_dDlf << " ";
	fs << v.m_iDir << endl;
	return fs;
}