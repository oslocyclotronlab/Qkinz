#include "PolyD2.h"

#include <sstream>
#include <iostream>

PolyD2::PolyD2()
	: a( 0 )
	, b( 0 )
	, c( 0 ) { }

PolyD2::PolyD2(const double &c0, const double &c1, const double &c2)
	: a( c0 )
	, b( c1 )
	, c( c2 ) { }

PolyD2::PolyD2(std::istream icmd)
{
	icmd >> a;
	icmd >> b;
	icmd >> c;
}

PolyD2::PolyD2(const PolyD2 &poly)
	: a( poly.a )
	, b( poly.b )
	, c( poly.c ) { }

PolyD2::~PolyD2(){}

PolyD2 &PolyD2::operator=(const PolyD2 &poly)
{
	this->a = poly.a;
	this->b = poly.b;
	this->c = poly.c;
	return *this;
}

PolyD2 &PolyD2::operator+=(const PolyD2 &poly)
{
	this->a += poly.a;
	this->b += poly.b;
	this->c += poly.c;
	return *this;
}

PolyD2 operator+(const PolyD2 &poly1, const PolyD2 &poly2)
{
	PolyD2 result(poly1);
	result += poly2;
	return result;
}

PolyD2 &PolyD2::operator-=(const PolyD2 &poly)
{
	this->a -= poly.a;
	this->b -= poly.b;
	this->c -= poly.c;
	return *this;
}

PolyD2 operator-(const PolyD2 &poly1, const PolyD2 &poly2)
{
	PolyD2 result(poly1);
	result -= poly2;
	return result;
}

double PolyD2::Evaluate(const double &x) const
{
	double result = a;
	result += b*x;
	result += c*x*x;
	return result;
}

void PolyD2::setCoeff(const int &deg, const double &value)
{
	if (deg == 0)
		a = value;
	else if (deg == 1)
		b = value;
	else if (deg == 2)
		c = value;
	else
		std::cerr << "Degree error." << std::endl;		
}

#ifdef TEST

int main()
{
	PolyD2 poly1(1.2, -2.1, 0.6);
	PolyD2 poly2(-1.2, 2.1, -0.6);

	std::cout << poly1(1.5) << std::endl;
	std::cout << poly2(1.5) << std::endl;
	PolyD2 poly3 = poly1 + poly2 - poly2;
	std::cout << poly3(1.5) << std::endl;
	return 0;
}

#endif // TEST