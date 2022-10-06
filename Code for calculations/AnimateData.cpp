//#include "stdafx.h"
#include "AnimateData.h"
#include "Common.h"

Bytef*  AnimateData::m_bf = NULL;
uLongf AnimateData::m_dl;
uLongf AnimateData::m_bfl;
uLongf AnimateData::m_bfl0;



int AnimateData::zip()
{
	return compress(m_bf, & m_bfl, (Bytef*) this, m_dl);
}
int AnimateData::unzip()
{
	return uncompress((Bytef*) this, &m_dl, m_bf ,m_bfl);
}

void AnimateData::StoreV()
{
	int i;
	for (i = 0; i<HEIGHT; i++)
	{
		m_v[i][0] = float(lattice[WIDTH_E / 2][i].m_v.m_dx);
		m_v[i][1] = 0;// float(lattice[WIDTH_E / 2][i].m_Stress);
		m_v[i][2] = float(lattice[int(WIDTH_E*9.0 / 10)][i].m_v.m_dx);
	}
}

AnimateData::AnimateData()
{
	m_bfl = sizeof(m_bf);
	m_dl = sizeof(*this);
	m_bfl0 = compressBound(anidata.m_dl);
	anidata.m_bf = new Bytef[m_bfl0];
}


AnimateData::~AnimateData()
{
	if (anidata.m_bf != NULL)
	{
		delete anidata.m_bf;
		anidata.m_bf = NULL;
		anidata.m_bfl0 = 0;
	}
}
