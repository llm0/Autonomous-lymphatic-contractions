// PermeatLine.h: interface for the PermeatLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERMEATLINE_H__4CCBBECE_9311_4C48_9ADA_ED6F266951A8__INCLUDED_)
#define AFX_PERMEATLINE_H__4CCBBECE_9311_4C48_9ADA_ED6F266951A8__INCLUDED_
#include "Line.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "lymph_h.h"
#include "PermeatAnimate.h"

class PermeatLine  
{
public:
	void ReadForAnimate(fstream* ff);
	void SaveForMap(fstream* ff);
	PermeatLine& PermeatLine::operator=(const PermeatAnimate& v);

	void init();
	
	Line m_LineV;
	Line m_LineDown;
	Line m_LineUp;
	PermeatLine();
	virtual ~PermeatLine();

};

#endif // !defined(AFX_PERMEATLINE_H__4CCBBECE_9311_4C48_9ADA_ED6F266951A8__INCLUDED_)
