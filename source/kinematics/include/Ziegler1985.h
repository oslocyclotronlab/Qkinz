#ifndef ZIEGLER1985_H
#define ZIEGLER1985_H

#include "StoppingPower.h"

class Material;
class Particle;

class Ziegler1985 : public StoppingPower
{
public:
    Ziegler1985();
	//! Default Constructor.
	Ziegler1985(Material *material, Particle *particle);

    //! Destructor.
    ~Ziegler1985();

    //! Assign operator.
    Ziegler1985 &operator=(const Ziegler1985 &ziegler);

	//! Calculates the stopping power.
    /*! \return Stopping power in [MeV/µm].
	 */
    inline double Evaluate(const double &E /*!< Energy of incident particle in MeV. */) const { return -stop(E)*10; }

    //! Calculates energy loss in the material.
    /*! \return The energy after passing through
     *  the material.
     */
    double Loss(const double &E,        /*!< Initial energy of the incident particle in [MeV].  */
                const double &d,        /*!< Width of target in [µm].                           */
                const int &points=1000  /*!< Number of integration points.                      */) const;

    //! Calculates energy loss in the material.
    /*! \return The energy after passing through
     *  the material.
     */
    double Loss(const double &E,        /*!< Initial energy of the incident particle in [MeV].  */
                const int &points=1000  /*!< Number of integration points.                      */) const;
private:
	//! Fucntion to calculate proton stopping power.
	/*! \return the proton stopping power of protons in the material.
	 */
	double pstop(const double &e /*!< Energy of proton. */) const;

	//! Function to calculate alfa particle stopping power.
	/*! \return the stopping power of alpha particles in the material.
	 */
	double hestop(const double &e /*!< Energy of alfa particle. */) const;

	//! Function to calculate ion stopping power.
	/*! \return the stopping power of ion stopping power in the material.
	 */
	double histop(const double &e /*!< Energy of ion. */) const;

    double stop(const double &ee) const;

	//! Function to calculate the nuclear stopping power.
	/*! \return the nuclear stopping power of particle in the material.
	 */
	double nucstop(const double &e /*!< Energy of particle. */) const; 


};

#endif // ZIEGLER1985_H
