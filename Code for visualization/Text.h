// Text.h: interface for the Text class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>

#if !defined(AFX_TEXT_H__C79BB512_046A_473A_968D_45AC09E3D4C6__INCLUDED_)
#define AFX_TEXT_H__C79BB512_046A_473A_968D_45AC09E3D4C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;
class Text  
{
public:
	char m_Text[256];
	double m_y;
	double m_x;
	Text();
	friend fstream& operator<<(fstream& fs,Text& v); 
	friend fstream& operator>>(fstream& fs,Text& v);
	virtual ~Text();

};

#endif // !defined(AFX_TEXT_H__C79BB512_046A_473A_968D_45AC09E3D4C6__INCLUDED_)
