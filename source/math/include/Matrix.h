#ifndef MATRIX_H
#define MATRIX_H


#include <memory>
#include <iosfwd>

class Vector;

/*! Matrix class.
 *  A class for containing a matrix.
 */

class Matrix
{
public:
    //! Constructor
    Matrix();

    //! Constructor.
    Matrix(const int &columns,	/*!< Number of columns.	*/
           const int &rows		/*!< Number of rows.	*/);

    //! Constructor.
    Matrix(const int &columns,	/*!< Number of columns.	*/
           const int &rows,		/*!< Number of rows.	*/
           const double &v		/*!< Value to initilize.*/);

    //! Constructor.
    Matrix(const double **mat,	/*!< Matrix to initilize with.	*/
           const int &columns,	/*!< Number of columns.			*/
           const int &rows		/*!< Number of rows.			*/);

    //! Copy constructor.
    Matrix(const Matrix &mat	/*!< Matrix to copy.			*/);

    //! Destructor.
    ~Matrix();

    //! Assign operator.
    Matrix &operator=(const Matrix &mat		/*!< Matrix to assign.		*/);

    //! Matrix addition.
    Matrix &operator+=(const Matrix &mat	/*!< Matrix to add.			*/);

    //! Matrix subtraction.
    Matrix &operator-=(const Matrix &mat	/*!< Matrix to subtract.	*/);

    //! Matrix multiplication.
    Matrix &operator*=(const Matrix &mat	/*!< Matrix to multiply.	*/);

    //! Get value.
    Vector &operator[](const int &n) const;

    friend Matrix operator+(const Matrix &m, const Matrix &n);

    friend Matrix operator-(const Matrix &m, const Matrix &n);

    friend Matrix operator*(const Matrix &m, const Matrix &n);

    friend Vector operator*(const Matrix &mat, const Vector &vec);

    friend Vector operator*(const Vector &vec, const Matrix &mat);

    friend std::ostream &operator<<(std::ostream &_os, const Matrix &mat);

    //! Get transposed.
    Matrix T();

    //! Get inverse.
    Matrix inv();

    //! Get number of columns.
    inline int getCol() const { return col; }

    //! Get numberf of rows.
    inline int getRow() const { return row; }

protected:
    //! Type to contain the matrix.
    typedef std::unique_ptr<Vector[]> mat_ptr;

    //! Container for matrix.
    mat_ptr m_val;

    //! Number of columns.
    int col;

    //! Number of rows.
    int row;

    //! Allocate memory.
    Vector *allocate(const int &columns,	/*!< Number of columns to allocate.	*/
                     const int &rows		/*!< Number of rows to allocate.	*/);
};

#endif // MATRIX_H
