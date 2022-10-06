// Vector.cpp: implementation of the Vector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vector::Vector(double dx,double dy)
{
	m_dx=dx;
	m_dy=dy;
}

Vector::~Vector()
{

}

Vector& Vector:: operator =(const Vector &v)
{
	m_dx=v.m_dx;
	m_dy=v.m_dy;
	return *this;
}

Vector& Vector:: operator =(const VectorAnimate &v)
{
	m_dx = v.m_fx;
	m_dy = v.m_fy;
	return *this;
}


Vector Vector:: operator *(const double &v)
{
	Vector R;
	R.m_dx=m_dx*v;
	R.m_dy=m_dy*v;
	return R;
}

Vector Vector:: operator /(const double &v)
{
	Vector R;
	R.m_dx=m_dx/v;
	R.m_dy=m_dy/v;
	return R;
}

double Vector::operator*(const Vector &v)
{
	double dR;
	dR=m_dx*v.m_dx+m_dy*v.m_dy;
	return dR;
}

Vector Vector:: operator +(const double &v)
{
	Vector R;
	R.m_dx=m_dx+v;
	R.m_dy=m_dy+v;
	return R;
}

Vector Vector:: operator +(const Vector &v)
{
	Vector R;
	R.m_dx=m_dx+v.m_dx;
	R.m_dy=m_dy+v.m_dy;
	return R;
}

Vector& Vector:: operator +=(const Vector &v)
{
	m_dx+=v.m_dx;
	m_dy+=v.m_dy;
	return *this;
}

Vector Vector:: operator-()
{
	Vector R;
	R.m_dx=-m_dx;
	R.m_dy=-m_dy;
	return R;
}

Vector Vector:: operator-(Vector v)
{
	Vector R;
	R.m_dx=m_dx-v.m_dx;
	R.m_dy=m_dy-v.m_dy;
	return R;
}

Vector& Vector:: operator-=(Vector v)
{
	m_dx-=v.m_dx;
	m_dy-=v.m_dy;
	return* this;
}

fstream& operator<<(fstream& fs,Vector& v)
{
	fs<< v.m_dx<<" "<<v.m_dy;
	return fs;
}

fstream& operator>>(fstream& fs,Vector& v)
{
	fs>>v.m_dx>>v.m_dy;
	return fs;
}


Vector& Vector::Roll(double v)
{
	double x,y;
	x=m_dx*cos(v)-m_dy*sin(v);
	y=m_dx*sin(v)+cos(v)*m_dy;
	m_dx=x;
	m_dy=y;
	return *this;
}

double Vector::Mod()
{
	return sqrt(m_dx*m_dx+m_dy*m_dy);
}