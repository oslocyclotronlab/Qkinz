#include "Polynomial.h"

#include <sstream>
#include <cmath>

Polynomial::Polynomial(){ }

Polynomial::Polynomial(double *coef, const int &n)
	: m_coef( Vector(coef, n) ) { }

Polynomial::Polynomial(const Vector &vec)
	: m_coef( vec ) { }

Polynomial::Polynomial(const std::string &cmd)
{
	std::stringstream icmd(cmd.c_str());

	int n;
	icmd >> n;
	m_coef = Vector(n);
	double coef;
	for (int i = 0 ; i < n ; ++i){
		icmd >> coef;
		m_coef[i] = coef;
	}
}

Polynomial::Polynomial(const Polynomial &pol)
	: m_coef( pol.m_coef ) { }

Polynomial::~Polynomial(){ }

double Polynomial::Eval(const double &x) const
{
	double result = 0;
    for (int i = 0 ; i < m_coef.getLength() ; ++i)
		result += m_coef[i]*pow(x, i);
	return result;
}

Vector Polynomial::Eval(const Vector &vec) const
{
    Vector result(vec.getLength(), 0);
    for (int i = 0 ; i < m_coef.getLength() ; ++i){
		result += m_coef[i]*pow(vec, i);
	}
	return result;
}

Polynomial &Polynomial::operator=(const Polynomial &pol)
{
    this->m_coef = pol.m_coef;
    return *this;
}

Polynomial &Polynomial::operator+=(const Polynomial &pol)
{
    this->m_coef += pol.m_coef;
    return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &pol)
{
    this->m_coef -= pol.m_coef;
    return *this;
}
