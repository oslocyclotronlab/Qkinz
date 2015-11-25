#ifndef STOPPINGPOWER_H
#define STOPPINGPOWER_H

#include "AbstractFunction.h"

#include <memory>

class Material;
class Particle;

class StoppingPower : public AbstractFunction
{
public:
	//! Constructor.
    StoppingPower(Material *material, Particle *particle);

	//! Virtual destructor.
    virtual ~StoppingPower() {}

	//! Calculate stopping power.
	/*! \return the stopping power for a
	 *  particle of a given type in the material.
	 *  This 
	 */
    virtual double Evaluate(const double &E /*!< Energy of incident particle. */) const=0;

    //! Calculates energy loss in the material.
    /*! \return The energy after passing through
     *  the material.
     */
    virtual double Loss(const double &E,        /*!< Initial energy of the incident particle in [MeV].  */
                        const int &points=1000  /*!< Number of integration points.                      */) const=0;

    //! Calculates energy loss in the material.
    /*! \return The energy after passing through
     *  the material.
     */
    virtual double Loss(const double &E,        /*!< Initial energy of the incident particle in [MeV].      */
                        const double &width,    /*!< Width of the target. Units depends on implementation.  */
                        const int &points=1000  /*!< Number of integration points.                          */) const=0;

	//! Overloaded () operator.
	/*! \return \ref Evaluate.
	 */
	inline double operator()(const double &E) const { return Evaluate(E); }

	//! Set the material.
	/*! \return true if possible, false otherwise.
	 */
	void setMaterial(Material *material /*!< Material to set. */);

	//! Set the incident particle.
	/*! \return true if possible, false otherwise.
	 */
	void setParticle(Particle *particle /*!< Incident particle to set. */);

protected:
	//! Variable to contain the material.
    Material *pMaterial;

	//! Variable to contain incident particle.
    Particle *pParticle;
};

#endif // STOPPINGPOWER_H
