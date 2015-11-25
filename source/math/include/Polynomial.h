#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "Vector.h"

#include <string>

class Polynomial
{
public:
	//! Empty constructor.
	Polynomial();

	//! Constructor, from pointer of coefficients.
	Polynomial(double *coef,	/*!< Pointer containing the coefficients.						*/
			   const int &n		/*!< Number of coefficients (i.e. degree of the polynomial.)	*/);

	//! Constructor, from vector.
	Polynomial(const Vector &vec	/*!< Coeficients contained in a vector.	*/);

	//! Constructor, from string.
	/*! Formating: <n> <a0> <a1> ... <ai> ... <an>.
	 *  n: Number of coefficients.
	 *  ai: ith coeficient.
	 */
	Polynomial(const std::string &cmd	/*!< String to create polynomial from.	*/);

	//! Copy constructor.
	Polynomial(const Polynomial &pol	/*!< Polynomial to copy.	*/);

	//! Destructor.
	~Polynomial();

	//! Function to evaluate the polynomial at a given point.
	/*! \return the value of the polynomial with argument x.
	 */
	double Eval(const double &x	/*!< Argument of the polynomial.	*/) const;

	//! Overload of Eval.
	/*! \return a vector containing the results.
	 */
	Vector Eval(const Vector &vec	/*!< Vector containing the arguments.	*/) const;

	//! Overloaded ().
	/*! \return \ref Eval.
	 */
    double operator()(const double &x 	/*!< Argument of the polynomial.	*/) const { return Eval(x);     }
    Vector operator()(const Vector &vec	/*!< Vector containing arguments.	*/) const { return Eval(vec);   }

	//! Assign operator.
	Polynomial &operator=(const Polynomial &pol		/*!< Polynomial to assign.		*/);

	//! Overloaded +=
	Polynomial &operator+=(const Polynomial &pol 	/*!< Polynomial to add.			*/);

	//! Overloaded -=
	Polynomial &operator-=(const Polynomial &pol 	/*!< Polynomial to subtract.	*/);

private:
	//! Vector to contain the coefficients.
	Vector m_coef;

};


#endif // POLYNOMIAL_H
