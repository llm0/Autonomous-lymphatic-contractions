// olorMap.cpp: implementation of the ColorMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "animate.h"
#include "ColorMap.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ColorMap::ColorMap()
{

}

ColorMap::~ColorMap()
{

}


void ColorMap::Map(double x,char cType)//0-255
{
	double slope;
	double nslope;
	double Max;
	double dScale;
	dScale=255.0/255.0;
	Max=255;
	switch (cType)
	{
	case 'n':
		x=Max-x;
		slope=Max/60.0;
		nslope=-Max/60.0;
		if( x < 60 )
		{
			m_R = Max;
			m_G = 255/60*x;//slope*x;
			m_B = 0;
		}
		else if ( x < 120 )
		{
			m_R= nslope*x+2*Max;
			m_G=Max;
			m_B=0;
		}
		else if ( x < 180  )
		{
			m_R = 0;
			m_G=Max;
			m_B= slope*x-2*Max;
		}
		else if ( x < 240  )
		{
			m_R = 0;
			m_G = nslope*x+4*Max;
			m_B = Max;
		}
		else if ( x < 300  )
		{
			m_R = slope*x-4*Max;
			m_G = 0;
			m_B = Max;
		}
		else
		{
			m_R = Max;
			m_G = 0;
			m_B = 0;//nslope*x+6*Max;
		}
		break;
	case 'h' :
		if(x<=85)
		{
			m_R = 42+(255.0-42)/85*x;
			m_G = 0;
			m_B = 0;
		}
		else if(x<=187)
		{
			m_R = 255;
			m_G = 42+(255.0-42)/(187-86)*(x-86);
			m_B = 0;
		}
		else if(x<=255)
		{
			m_R = 255;
			m_G = 255;
			m_B = 64+(255.0-42)/(255-188)*(x-188);
		}
		else
		{
			m_R = 255;
			m_G = 255;
			m_B = 255;
		}
	default:;
	}
	m_R*=dScale;
	m_G*=dScale;
	m_B*=dScale;
}