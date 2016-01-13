#ifndef ITERATIVE_H
#define ITERATIVE_H

#include "Scattering.h"

class Particle;

/*! This class implements a method for calculating relativistic collisions
 *  using a iterative method. The algorithm is taken from the file kinzlib.cpp
 *  in the Oslosoftware avalible from Magne Guttormsen's personal UiO webpage.
 */
class Iterative : public Scattering
{
public:
    //! Constructor.
    Iterative(Particle *pA, Particle *pX, Particle *pY, Particle *pB)
        : Scattering(pA, pX, pY, pB) { }

    //! Destructor.
    ~Iterative();

    //! Implementation of the scattering algorithm.
    double EvaluateY(const double &E,       /*!< Energy of incident particle (particle A).  */
                     const double &theta,   /*!< Scattering angle (Particle Y).             */
                     const double &Ex       /*!< Excitation energy of particle B.           */) const;
private:
    //! Inline function to calculate relativistic momentum.
    /*! \return Relativistic momentum.
     */
    inline double P2rel(const double &M,    /*!< Mass of the particle in [AMU].         */
                        const double &T     /*!< Kinetic energy of particle in [MeV].   */) const { return 2.*931.5016*M*T + T*T; }
};

#endif // ITERATIVE_H
