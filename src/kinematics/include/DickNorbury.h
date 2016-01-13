#ifndef DICKNORBURY_H
#define DICKNORBURY_H

#include "Scattering.h"

/*! Scattering calculator for inelastic relativistic scattering.
 *  This class is based on F. Dick and J. W. Norbury, Eur. J. Phys. 30 (2009) 403-416.
 *  The implementation depends on equations 48, 50-53.
 */
class DickNorbury : public Scattering
{
public:
    //! Constructor.
    DickNorbury(Particle *pA, /*!< The incident particle.                            */
                Particle *pX, /*!< The particle that are scattered of.               */
                Particle *pY, /*!< Outgoing particle to calculate energy/angle of.   */
                Particle *pB  /*!< Secondary product particle.                       */);

    //! Destructor.
    ~DickNorbury();

    //! Calculate energy of particle Y after scattering.
    /*! \return Energy of particle Y after scattering.
     */
    double EvaluateY(const double &E,       /*!< Energy of incident particle (particle A).  */
                     const double &theta,   /*!< Scattering angle (Particle Y).             */
                     const double &Ex       /*!< Excitation energy of particle B.           */) const;

    //! Calculate maximum excitation energy of the B particle after scattering.
    /*! \return Maximum posible excitation energy after scattering.
     */
    double FindMaxEx(const double &E,       /*!< Energy of incident particle A.     */
                     const double &theta    /*!< Outgoing angle of the fragment.    */) const;

};

#endif // DICKNORBURY_H
