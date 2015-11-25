#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H


//! Class to build functions upon.

class AbstractFunction
{
public:
    //! Virtual destructor.
    virtual ~AbstractFunction();

    //! Virtual function to evaluate the function in a point x.
    /*! \return value of the function in point x.
     */
    virtual double Evaluate(const double &x) const = 0;

    //! Overloaded () function.
    /*! \return \ref Evaluate
     */
    inline double operator()(const double &x) const { return Evaluate(x); }
};

#endif // ABSTRACTFUNCTION_H
