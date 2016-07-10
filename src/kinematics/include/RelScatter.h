#ifndef RELSCATTER_H
#define RELSCATTER_H


class Particle;
#include "Scattering.h"

class RelScatter : public Scattering
{
public:
    //! Constuctor.
    RelScatter(Particle *pA, /*!< Incident particle.             */
               Particle *pX, /*!< Target particle.               */
               Particle *pY, /*!< Residual particle.             */
               Particle *pB  /*!< Fragement particle.            */);

    //! Destructor.
    ~RelScatter();

    //! Evaluate energy of residual particle.
    /*! \return Energy of particle Y after scattering.
     */
    double EvaluateY(const double &E,       /*!< Incident energy.               */
                     const double &theta,   /*!< Scattering angle of particle Y */
                     const double &Ex       /*!< Excitation of particle B.      */) const;


    //! Calculate the maximum excitation energy of particle B at given scattering angle and residual energy.
    double FindMaxEx(const double &E,       /*!< Incident energy.   */
                     const double &theta    /*!< Scattering angle.  */) const;

};

#endif // RELSCATTER_H
