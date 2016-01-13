#ifndef POLYFIT_H
#define POLYFIT_H

class Vector;

//! Class to fit data to polynomial.
class Polyfit
{
public:
    Polyfit(double *x,  /*!< x-values.                      */
            double *y,  /*!< y-values.                      */
            int nvals   /*!< Number of values.              */);

    //! Get calculated coefficients.
    /*! \return the calculated coefficients.
     */
    Vector operator()(const int &n /*!< Degree to fit the data. */);

private:
    double *xv;
    double *yv;
    int nv;
};

#endif // POLYFIT_H
