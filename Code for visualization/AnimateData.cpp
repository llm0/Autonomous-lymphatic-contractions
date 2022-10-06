#include "stdafx.h"
#include "AnimateData.h"
#include "Common.h"

Bytef* AnimateData::m_bf = NULL;
uLongf AnimateData::m_dl;
uLongf AnimateData::m_bfl;



int AnimateData::zip()
{
	return compress(m_bf, & m_bfl, (Bytef*) this, m_dl);
}
int AnimateData::unzip()
{
	return uncompress((Bytef*) this, &m_dl, m_bf ,m_bfl);
}

void AnimateData::DistributeData()
{
	int i,j;
	lNowStep=m_lNowStep;
	PerLine=m_Permeat;
	ValveDual=m_Valve;
	for (i = 0; i < WIDTH; i++)
	{
		for (j = 0; j < HEIGHT; j++)
		{
			//lattice[i][j] = m_Lattice[i][j];
		}
	}
	LymVessle=m_lymani;
	for(i=0;i<HEIGHT;i++)
	{
		Velocity1[i] = m_v[i][0];
		Stress[i] = m_v[i][1];
		Velocity2[i] = m_v[i][2];
	}
}

AnimateData::AnimateData()
{
	m_lNowStep=-1;
	if (m_bf == NULL)
	{
		m_bfl = sizeof(m_bf);
		m_dl = sizeof(*this);
		m_bfl = compressBound(m_dl);
		m_bf = new Bytef[m_bfl];
	}
}


AnimateData::~AnimateData()
{
	if (m_bf != NULL)
	{
		delete m_bf; 
		m_bf = NULL;
		m_bfl = 0;
	}
}
