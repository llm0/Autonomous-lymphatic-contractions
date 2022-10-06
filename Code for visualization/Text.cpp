// Text.cpp: implementation of the Text class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "animate.h"
#include "Text.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Text::Text()
{
	sprintf(m_Text,"%s","");
}

Text::~Text()
{

}

fstream& operator<<(fstream& fs,Text& v)
{
	fs<<v.m_x<<" "<<v.m_y;
	return fs;
}

fstream& operator>>(fstream& fs,Text& v)
{
	CString strText;
	char chSpace;
	fs>>v.m_x>>v.m_y;
	chSpace=fs.peek();
	if(chSpace!='\n'&&chSpace!=32&&chSpace!='	')
	{
		fs>>v.m_Text;
	}
	chSpace=fs.peek();
	if(chSpace!='\n')
	{
		strText=v.m_Text;
	}
	while(chSpace!='\n')
	{
		chSpace=fs.peek();
		if(chSpace!='\n')
		{
			fs.read(&chSpace,sizeof(chSpace));
			strText=strText+chSpace;
			chSpace=fs.peek();
			if(chSpace!='\n'&&chSpace!=32&&chSpace!='	')
			{
				fs>>v.m_Text;
			}
			strText=strText+v.m_Text;
		}
		else
		{
			sprintf(v.m_Text,"%s",(LPCTSTR)strText);
			break;
		}
	}
	return fs;
}
