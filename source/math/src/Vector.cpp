#include "Vector.h"

#include "Matrix.h"

#include <iostream>
#include <cmath>

Vector::Vector()
    : len( 0 ){}

Vector::Vector(const int &length)
    : m_val( allocate(length) )
    , len( length ){}

Vector::Vector(const int &length, const double &v)
    : m_val( allocate(length) )
    , len( length )
{
    for (int i = 0 ; i < length ; ++i)
        m_val[i] = v;
}


Vector::Vector(const double *v, const int &length)
    : m_val( allocate(length) )
    , len( length )
{
    for (int i = 0 ; i < length ; ++i)
        m_val[i] = v[i];
}

Vector::Vector(const Vector &vec)
    : m_val( allocate(vec.len) )
    , len( vec.len )
{
    for (int i = 0 ; i < len ; ++i)
        m_val[i] = vec[i];
}

Vector::~Vector()
{
    if (m_val) m_val.reset();
}

Vector &Vector::operator=(const Vector &vec)
{
    this->m_val.reset(this->allocate(vec.len));
    this->len = vec.len;

    for (int i = 0 ; i < vec.len ; ++i)
        this->m_val[i] = vec[i];
    return *this;
}

Vector &Vector::operator+=(const Vector &vec)
{
    if (this->len > vec.len){
        for (int i = 0 ; i < vec.len ; ++i)
            this->m_val[i] += vec[i];
        return *this;
    } else if (this->len < vec.len){
        double *tmp = allocate(vec.len);
        for (int i = 0 ; i < this->len ; ++i)
            tmp[i] = this->m_val[i] + vec[i];
        for (int i = this->len ; i < vec.len ; ++i)
            tmp[i] = vec[i];
        this->len = vec.len;
        this->m_val.reset(tmp);
        return *this;
    } else {
        for (int i = 0 ; i < this->len ; ++i)
            this->m_val[i] += vec[i];
        return *this;
    }
}

Vector &Vector::operator-=(const Vector &vec)
{
    if (this->len > vec.len){
        for (int i = 0 ; i < vec.len ; ++i)
            this->m_val[i] -= vec[i];
        return *this;
    } else if (this->len < vec.len){
        double *tmp = allocate(vec.len);
        for (int i = 0 ; i < this->len ; ++i)
            tmp[i] = this->m_val[i] - vec[i];
        for (int i = this->len ; i < vec.len ; ++i)
            tmp[i] = vec[i];
        this->len = vec.len;
        this->m_val.reset(tmp);
        return *this;
    } else {
        for (int i = 0 ; i < this->len ; ++i)
            this->m_val[i] -= vec[i];
        return *this;
    }
}

Vector &Vector::operator*=(const Vector &vec)
{
    if (this->len > vec.len){
        for (int i = 0 ; i < vec.len ; ++i)
            this->m_val[i] *= vec[i];
        return *this;
    } else if (this->len < vec.len){
        double *tmp = allocate(vec.len);
        for (int i = 0 ; i < this->len ; ++i)
            tmp[i] = this->m_val[i] * vec[i];
        for (int i = this->len ; i < vec.len ; ++i)
            tmp[i] = 0;
        this->len = vec.len;
        this->m_val.reset(tmp);
        return *this;
    } else {
        for (int i = 0 ; i < this->len ; ++i)
            this->m_val[i] *= vec[i];
        return *this;
    }
}

double *Vector::allocate(const int &length)
{
    return new double[length];
}

double Vector::inner() const
{
    double result = 0;
    for (int i = 0 ; i < len ; ++i)
        result += m_val[i]*m_val[i];
    return result;
}

double &Vector::operator[](const int &n) const
{
    if (n >= 0 && n < len && m_val)
        return m_val[n];
    else {
        std::cerr << "Vector::operator[](const int&): Error, index out of bound. Returning first element." << std::endl;
        return m_val[0];
    }
}

Vector operator+(const Vector &u, const Vector &v)
{
    Vector result(u);
    result += v;
    return result;
}

Vector operator-(const Vector &u, const Vector &v)
{
    Vector result(u);
    result -= v;
    return result;
}

Vector operator*(const Vector &u, const Vector &v)
{
    Vector result(u);
    result *= v;
    return result;
}

Vector operator*(const Vector &vec, const Matrix &mat)
{
    if (vec.len != mat.col){
        std::cerr << "Operator*(const Vector&, const Matrix &mat): Size error. Returning Vector&." << std::endl;
        return Vector(vec);
    }
    Vector result(mat.row);
    for (int i = 0 ; i < mat.row ; ++i){
        result[i] = 0;
        for (int j = 0 ; j < mat.col ; ++j)
            result[i] += vec[j]*mat[j][i];
    }
    return result;
}

Vector operator*(const Matrix &mat, const Vector &vec)
{
    if (mat.row != vec.len){
        std::cerr << "operator*(const Matrix&, const Vector&): Size error. Returning Vector&." << std::endl;
        return Vector(vec);
    }
    Vector result(mat.col, 0);
    for (int i = 0 ; i < mat.col ; ++i){
        for (int j = 0 ; j < mat.row ; ++j)
            result[i] += mat[i][j]*vec[j];
    }
    return result;
}

Vector pow(const Vector &vec, const double &pwr)
{
    Vector result(vec);
    for (int i = 0 ; i < vec.getLength() ; ++i)
        result[i] = pow(result[i], pwr);
    return result;
}

std::ostream &operator<<(std::ostream &_os, const Vector &vec)
{
    for (int i = 0 ; i < vec.getLength() ; ++i)
        _os << vec[i] << std::endl;
    return _os;
}
