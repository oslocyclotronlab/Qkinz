#ifndef POLYD2_H
#define POLYD2_H

//#define TEST 1

#include <iosfwd>

//! Class to contain polynomials of second degree.
class PolyD2
{
public:
	//! Constructor.
	PolyD2();

	//! Constructor. Assuming p(x) = a + bx + bx^2
	PolyD2(const double &a,	/*!< First coefficient.		*/
		   const double &b,	/*!< Second coefficient.	*/
		   const double &c	/*!< Third coefficient.		*/);

	//! Copy constructor.
	PolyD2(const PolyD2 &poly	/*!< Polynomial to copy.	*/);

	//! Construct from string.
	PolyD2(std::istream icmd);

	//! Destructor.
	~PolyD2();

	//! Assign operator.
	PolyD2 &operator=(const PolyD2 &poly	/*!< Polynomial to assign.	*/);

	//! Addition operator.
	PolyD2 &operator+=(const PolyD2 &poly	/*!< Polynomial to add.		*/);

	//! Add two polynomials.
	friend PolyD2 operator+(const PolyD2 &poly1,	/*!< Firt polynomial.	*/
						    const PolyD2 &poly2		/*!< Second polynomial.	*/);

	//! Subtract operator.
	PolyD2 &operator-=(const PolyD2 &poly	/*!< Polynomial to subtract.	*/);

	//! Subtract two polynomials.
	friend PolyD2 operator-(const PolyD2 &poly1,	/*!< Firt polynomial.	*/
						    const PolyD2 &poly2		/*!< Second polynomial.	*/);

	//! Evaluate the polynomial.
	/*! \return the value at the given argument.
	 */
	double Evaluate(const double &x	/*!< Argument to evaluate.	*/) const;

	//! Overload () operator.
	/*! \return \ref Evaluate
	 */
	inline double operator()(const double &x) const { return Evaluate(x); }

	//! Change coefficients.
	void setCoeff(const int &deg,		/*!< What degree to change.	*/
				  const double &value	/*!< New coefficient.		*/);

private:
	//! First coefficient.
	double a;

	//! Second coefficient.
	double b;

	//! Third coefficient.
	double c;
};

#endif // POLYD2_H