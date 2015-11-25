#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <iosfwd>

class Matrix;

/*! A vector class.
 */

class Vector
{
public:
    //! Constructor.
    Vector();

    //! Constructor.
    Vector(const int &length	/*!< Length of the vector.	*/);

    //! Constructor.
    Vector(const int &length,	/*!< Length of the vector.	*/
           const double &v		/*!< Value to initilize.	*/);

    //! Constructor.
    Vector(const double *v,		/*!< Data points.			*/
           const int &length	/*!< Length of array.		*/);

    //! Copy constructor.
    Vector(const Vector &vec	/*!< Vector to copy.		*/);

    //! Destructor.
    ~Vector();

    //! Assign operator.
    Vector &operator=(const Vector &vec 	/*!< Vector to assign.		*/);

    //! Vector addition.
    Vector &operator+=(const Vector &vec	/*!< Vector to add.			*/);

    //! Vector subtraction.
    Vector &operator-=(const Vector &vec	/*!< Vector to subtract.	*/);

    //! Vector multiplication.
    Vector &operator*=(const Vector &vec	/*!< Vector to multiply.	*/);

    //! Get value.
    double &operator[](const int &n) const;

    //! Friend addition operator.
    friend Vector operator+(const Vector &u, const Vector &v);

    //! Friend subtraction operator.
    friend Vector operator-(const Vector &u, const Vector &v);

    //! Friend multiplication operator.
    friend Vector operator*(const Vector &u, const Vector &v);

    //! Vector/Matrix multiplication.
    friend Vector operator*(const Vector &vec, const Matrix &mat);

    //! Matrix/Vector multiplication.
    friend Vector operator*(const Matrix &mat, const Vector &vec);

    friend Vector pow(const Vector &vec, const double &pwr);

    friend std::ostream &operator<<(std::ostream &_os, const Vector &vec);

    //! Get length of vector.
    inline int getLength() const { return len; }

    //! Inner product of the vector.
    double inner() const;

protected:
    //! Type to contain data point.
    typedef std::unique_ptr<double[]> vec_ptr;

    //! Container for vector.
    vec_ptr m_val;

    //! Length of vector.
    int len;

    //! Allocate memory.
    double *allocate(const int &length	/*!< Size of memory to allocate.	*/);

};


#endif // VECTOR_H
