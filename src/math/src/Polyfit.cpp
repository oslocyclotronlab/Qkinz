#include "Polyfit.h"


#include "Matrix.h"
#include "Vector.h"

#include <cmath>
#include <iostream>

Polyfit::Polyfit(double *x, double *y, int nvals)
    : xv( x )
    , yv( y )
    , nv( nvals ){ }

Vector Polyfit::operator()(const int &n)
{
    Matrix X(nv, n);
    for (int i = 0 ; i < nv ; ++i){
        for (int j = 0 ; j < n ; ++j){
            X[i][j] = pow(xv[i], j);
        }
    }
    Matrix XT = X.T();
    Matrix XTX = XT*X;
    Matrix XTXinv = XTX.inv();
    Vector result = XTXinv*XT*Vector(yv, nv);
    return result;
}
