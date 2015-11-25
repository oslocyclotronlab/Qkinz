#include "Matrix.h"

#include "Vector.h"

#include <iostream>
#include <cmath>

#ifndef ZERO
#define ZERO 1.0E-10
#endif

static void ludcmp(Matrix &a, int n, int *indx, double *d)
{
   int      i, imax, j, k;
   double   big, dum, sum, temp, *vv;

  vv = new(std::nothrow) double [n];
  if(!vv) {
    printf("\n\nError in function ludcm():");
    printf("\nNot enough memory for vv[%d]\n",n);
    exit(1);
  }

   *d = 1.0;                              // no row interchange yet
   for(i = 0; i < n; i++) {     // loop over rows to get scaling information
      big = ZERO;
      for(j = 0; j < n; j++) {
         if((temp = fabs(a[i][j])) > big) big = temp;
      }
      if(big == ZERO) {
         printf("\n\nSingular matrix in routine ludcmp()\n");
         exit(1);
      }
      vv[i] = 1.0/big;                 // save scaling */
   } // end i-loop */

   for(j = 0; j < n; j++) {     // loop over columns of Crout's method
      for(i = 0; i< j; i++) {   // not i = j
         sum = a[i][j];
     for(k = 0; k < i; k++) sum -= a[i][k]*a[k][j];
     a[i][j] = sum;
      }
      big = ZERO;   // initialization for search for largest pivot element
      for(i = j; i< n; i++) {
         sum = a[i][j];
     for(k = 0; k < j; k++) sum -= a[i][k]*a[k][j];
     a[i][j] = sum;
     if((dum = vv[i]*fabs(sum)) >= big) {
        big = dum;
        imax = i;
     }
      } // end i-loop
      if(j != imax) {    // do we need to interchange rows ?
         for(k = 0;k< n; k++) {       // yes
        dum        = a[imax][k];
        a[imax][k] = a[j][k];
        a[j][k]    = dum;
     }
     (*d)    *= -1;            // and change the parit of d
     vv[imax] = vv[j];         // also interchange scaling factor
      }
      indx[j] = imax;
      if(fabs(a[j][j]) < ZERO)  a[j][j] = ZERO;

        /*
        ** if the pivot element is zero the matrix is singular
        ** (at least to the precision of the algorithm). For
        ** some application of singular matrices, it is desirable
        ** to substitute ZERO for zero,
        */

      if(j < (n - 1)) {                   // divide by pivot element
         dum = 1.0/a[j][j];
     for(i=j+1;i < n; i++) a[i][j] *= dum;
      }
   } // end j-loop over columns

   delete [] vv;   // release local memory

}  // End: function ludcmp()

static void lubksb(Matrix &a, int n, int *indx, double *b)
{
   int        i, ii = -1, ip, j;
   double     sum;

   for(i = 0; i< n; i++) {
      ip    = indx[i];
      sum   = b[ip];
      b[ip] = b[i];
      if(ii > -1)   for(j = ii; j < i; j++) sum -= a[i][j] * b[j];
      else if(sum) ii = i;
      b[i] = sum;
   }
   for(i = n - 1; i >= 0; i--) {
      sum = b[i];
      for(j = i+1; j < n; j++) sum -= a[i][j] * b[j];
      b[i] = sum/a[i][i];
   }
} // End: function lubksb()

#ifdef ZERO
#undef ZERO
#endif

Matrix::Matrix()
    : col( 0 )
    , row( 0 ){ }

Matrix::Matrix(const int &columns, const int &rows)
    : m_val( allocate(columns, rows) )
    , col( columns )
    , row( rows ) { }

Matrix::Matrix(const int &columns, const int &rows, const double &v)
    : m_val( allocate(columns, rows) )
    , col( columns )
    , row( rows )
{
    for (int i = 0 ; i < col ; ++i){
        for (int j = 0 ; j < row ; ++j){
            m_val[i][j] = v;
        }
    }
}

Matrix::Matrix(const double **mat, const int &columns, const int &rows)
    : m_val( allocate(columns, rows) )
    , col( columns )
    , row( rows )
{
    for (int i = 0 ; i < col ; ++i){
        for (int j = 0 ; j < row ; ++j){
            m_val[i][j] = mat[i][j];
        }
    }
}

Matrix::Matrix(const Matrix &mat)
    : m_val( allocate(mat.col, mat.row) )
    , col( mat.col )
    , row( mat.row )
{
    for (int i = 0 ; i < col ; ++i){
        m_val[i] = mat[i];
    }
}

Matrix::~Matrix()
{
    if (m_val) m_val.reset();
}

Vector *Matrix::allocate(const int &columns, const int &rows)
{
    Vector *mat = new Vector[columns];
    for (int i = 0 ; i < columns ; ++i)
        mat[i] = Vector(rows);
    return mat;
}

Matrix &Matrix::operator=(const Matrix &mat)
{
    if (this->col == mat.col && this->row == mat.row){
        for (int i = 0 ; i < this->col ; ++i)
            this->m_val[i] = mat[i];
    } else {
        m_val.reset(allocate(mat.col, mat.row));
        this->col = mat.col;
        this->row = mat.row;
        for (int i = 0 ; i < mat.col ; ++i)
            this->m_val[i] = mat[i];
    }
    return *this;
}

Matrix &Matrix::operator+=(const Matrix &mat)
{
    if (this->col != mat.col && this->row != mat.row){
        std::cerr << "Matrix::operator+=: Size error. Returning *this." << std::endl;
        return *this;
    }
    for (int i = 0 ; i < this->col ; ++i)
        this->m_val[i] += mat[i];
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &mat)
{
    if (this->col != mat.col && this->row != mat.row){
        std::cerr << "Matrix::operator-=: Size error. Returning *this." << std::endl;
        return *this;
    }
    for (int i = 0 ; i < this->col ; ++i)
        this->m_val[i] -= mat[i];
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &mat)
{
    if ( this->row != mat.col ){
        std::cerr << "Matrix::operator*=: Size error. Returning *this." << std::endl;
        return *this;
    }
    Vector *tmp = allocate(this->col, mat.row);
    for (int i = 0 ; i < this->col ; ++i){
        for (int j = 0 ; j < mat.row ; ++j){
            tmp[i][j] = 0;
            for (int k = 0 ; k < this->row ; ++k)
                tmp[i][j] += this->m_val[i][k]*mat[k][j];
        }
    }
    this->row = mat.row;
    this->m_val.reset(tmp);
    return *this;
}

Matrix Matrix::T()
{
    Matrix result(row, col);
    for (int i = 0 ; i < col ; ++i){
        for (int j = 0 ; j < row ; ++j)
            result[j][i] = m_val[i][j];
    }
    return result;
}

Matrix Matrix::inv()
{
    if (col != row){
        std::cerr << "Matrix::inv(): Error, not a NxN - matrix. Returning *this." << std::endl;
        return *this;
    }

    int i, j, *indx;
    double d, *c;
    Matrix result(col, col);
    Matrix tmp(col,col);
    for (i = 0 ; i < col ; ++i){
        for (j = 0 ; j < col ; ++j)
            tmp[i][j] = m_val[i][j];
    }

    indx = new int[col];
    c = new double[col];
    ludcmp(tmp, col, indx, &d);
    for (j = 0 ; j < col ; ++j){
        for (i = 0 ; i < col ; ++i) c[i] = 0.0;
        c[j] = 1.0;
        lubksb(tmp, col, indx, c);
        for (i = 0 ; i < col ; ++i) result[i][j] = c[i];
    }
    delete[] indx;
    delete[] c;
    return result;
}

Vector &Matrix::operator[](const int &n) const
{
    if (n >= 0 && n < col && m_val)
        return m_val[n];
    else {
        std::cerr << "Matrix::operator[](const int&): Error, index out of bounds. Returning first element." << std::endl;
        return m_val[0];
    }

}

Matrix operator+(const Matrix &m, const Matrix &n)
{
    Matrix result(m);
    result += n;
    return result;
}

Matrix operator-(const Matrix &m, const Matrix &n)
{
    Matrix result(m);
    result -= n;
    return result;
}

Matrix operator*(const Matrix &m, const Matrix &n)
{
    Matrix result(m);
    result *= n;
    return result;
}

std::ostream &operator<<(std::ostream& _os, const Matrix &mat)
{
    for (int i = 0 ; i < mat.getCol() ; ++i){
        for (int j = 0 ; j < mat.getRow() - 1 ; ++j)
            _os << mat[i][j] << " ";
        _os << mat[i][mat.getRow()-1] << std::endl;
    }
    return _os;
}
