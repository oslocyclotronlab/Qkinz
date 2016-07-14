#ifndef RELSCATTER_H
#define RELSCATTER_H

#include "types.h"

class Particle;
#include "Scattering.h"

class RelScatter : public Scattering
{
public:
    //! Constuctor.
    RelScatter(Particle *pA, /*!< Incident particle.            */
               Particle *pX, /*!< Target particle.              */
               Particle *pY, /*!< Fragement particle.           */
               Particle *pB  /*!< Residual particle.            */);

    //! Destructor.
    ~RelScatter();

    //! Evaluate energy of residual particle.
    /*! \return Energy of particle Y after scattering.
     */
    double EvaluateY(const double &E,       /*!< Incident energy.               */
                     const double &theta,   /*!< Scattering angle of particle Y */
                     const double &Ex       /*!< Excitation of particle B.      */) const;

    adouble EvaluateY(double E, double theta, adouble Ex);


    //! Calculate the maximum excitation energy of particle B at given scattering angle and residual energy.
    double FindMaxEx(const double &E,       /*!< Incident energy.   */
                     const double &theta    /*!< Scattering angle.  */) const;

};

#endif // RELSCATTER_H
