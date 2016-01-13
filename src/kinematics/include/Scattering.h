#ifndef SCATTERING_H
#define SCATTERING_H

class Particle;

//! Base class for calculating scattering of
//! two nuclei. The actual calculations needed
//! are implemented in derived classes.
class Scattering
{
public:
    //! Constructor.
    Scattering(Particle *pA, /*!< The incident particle.                            */
               Particle *pX, /*!< The particle that are scattered of.               */
               Particle *pY, /*!< Outgoing particle to calculate energy/angle of.   */
               Particle *pB  /*!< Secondary product particle.                       */);

    //! Destructor.
    virtual ~Scattering();

    //! Function for calculating energy of particle Y.
    /*! \return Energy of particle X after scattering.
     */
    virtual double EvaluateY(const double &E,       /*!< Energy of incident particle (particle A).  */
                             const double &theta,   /*!< Scattering angle (Particle Y).             */
                             const double &Ex       /*!< Excitation energy of particle B.           */) const=0;

    //! Function for calculating energy of particle B.
    /*! \return Energy of particle B after scattering.
     */
    double EvaluateB(const double &EA,      /*!< Energy of incident particle A.             */
                     const double &EY,      /*!< Energy of particle Y after scattering.     */
                     const double &thetaY,  /*!< Scattering angle of particle Y.            */
                     double &Ex,            /*!< Calculated excitation energy of particle B.*/
                     double &thetaB         /*!< Calculated scattering angle of particle B. */) const;

    //! Function for calculating maximum excitation energy of particle B.
    /*! \return The highest posible positive excitation energy for particle B.
     */
    //virtual double FindMaxEx(const double &E,       /*!< Energy of incident particle A.     */
    //                         const double &theta    /*!< Outgoing angle of the fragment.    */) const=0;

    // Functions to set particles.
    //! Set particle A.
    void setA(Particle *pA /*!< Incomming incident particle.     */);

    //! Set particle X.
    void setX(Particle *pX /*!< Scattering particle.             */);

    //! Set particle Y.
    void setY(Particle *pY /*!< Outgoing particle.               */);

    //! Set particle B.
    void setB(Particle *pB /*!< Product particle.                */);

protected:
    //! Pointer to incomming particle.
    Particle *A;

    //! Pointer to scattering particle.
    Particle *X;

    //! Pointer to outgoing particle.
    Particle *Y;

    //! Pointer to product particle.
    Particle *B;
};

inline void Scattering::setA(Particle *pA){ A = pA; }
inline void Scattering::setX(Particle *pX){ X = pX; }
inline void Scattering::setY(Particle *pY){ Y = pY; }
inline void Scattering::setB(Particle *pB){ B = pB; }

#endif // SCATTERING_H
