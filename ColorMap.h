// olorMap.h: interface for the ColorMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLORMAP_H__E8C02F81_625D_4CB2_A4A9_264399DEAC6B__INCLUDED_)
#define AFX_OLORMAP_H__E8C02F81_625D_4CB2_A4A9_264399DEAC6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ColorMap  
{
public:
	ColorMap();
	virtual ~ColorMap();

public: 
	unsigned char  m_R;
	unsigned char  m_B;
	unsigned char  m_G;
	void Map(double,char cType);
};

#endif // !defined(AFX_OLORMAP_H__E8C02F81_625D_4CB2_A4A9_264399DEAC6B__INCLUDED_)
