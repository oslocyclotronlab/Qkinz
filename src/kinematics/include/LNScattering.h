#ifndef LNSCATTERING_H
#define LNSCATTERING_H

#include "Scattering.h"

class LNScattering : public Scattering
{
public:
	//! Constructor.
	LNScattering(Particle *pA,	/*!< Incident particle.		*/
                     Particle *pX,	/*!< Target particle.		*/
                     Particle *pY,	/*!< Light product.		*/
                     Particle *pB	/*!< Heavy product.		*/);

	//! Destructor.
	~LNScattering();

	//! Calculate energy of light product.
        double EvaluateY(const double &E,	/*!< Incident energy.				*/
                         const double &theta,	/*!< Scattering angle of light product.		*/
                         const double &Ex	/*!< Excitation energy of the heavy product.	*/) const;
};

#endif // LNSCATTERING_H
