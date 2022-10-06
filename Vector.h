// Vector.h: interface for the Vector class.
//
//////////////////////////////////////////////////////////////////////

#include "math.h"

#if !defined(AFX_VECTOR_H__F11B34A9_DEB8_4B4D_9234_8B324CD4614A__INCLUDED_)
#define AFX_VECTOR_H__F11B34A9_DEB8_4B4D_9234_8B324CD4614A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <fstream>
using namespace std;
#include "VectorAnimate.h"

class Vector 
{
public:
	double m_dx;
	double m_dy;
	Vector(double dx=0,double dy=0);
	virtual ~Vector();
	Vector& operator=(const Vector &v);
	Vector& operator=(const VectorAnimate &v);
	Vector operator*(const double &v);
	Vector operator/(const double &v);
	double operator*(const Vector &v);
	Vector operator+(const double &v);
	Vector operator+(const Vector &v);
	Vector& operator+=(const Vector &v);
	Vector operator-();
	Vector operator-(Vector v);
	Vector& operator-=(Vector v);
	Vector& Roll(double v);
	double Mod();
	friend fstream& operator<<(fstream& fs,Vector& v); 
	friend fstream& operator>>(fstream& fs,Vector& v);

};

#endif // !defined(AFX_VECTOR_H__F11B34A9_DEB8_4B4D_9234_8B324CD4614A__INCLUDED_)
