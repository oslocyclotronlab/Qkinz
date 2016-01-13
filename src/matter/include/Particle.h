#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
public:
    //! Empty constructor.
    Particle();

	//! Constructor
	Particle(const int &_Z, /*!< Element number of particle. */
			 const int &_A  /*!< Mass number of particle. 	 */);

    //! Copy constructor.
    Particle(const Particle &particle);

	//! Destructor
	~Particle();

    //! Assign operator.
    Particle &operator=(const Particle &particle);

	//! Function to set/change particle.
	void setParticle(const int &_Z, /*!< Element number of particle. */
			 		 const int &_A  /*!< Mass number of particle. 	 */);

	//! Get element number.
	/*! \return the element number of the particle. */
	int GetZ() const;

	//! Get mass number.
	/*! \return the mass number of the particle. */
	int GetA() const;

	//! Get the mass of the particle.
	/*! \return mass of particle in [AMU]. */
	double GetM_AMU() const;

	//! Get the mass of the particle.
	/*! \return mass of particle in [MeV]. */
	double GetM_MeV() const;

	//! Get lfctr.
	/*! \return lfctr of the particle. */
	double Getlfctr() const;

private:
	//! Variable to store element number.
	int Z;

	//! Variable to store mass number.
	int A;

	//! Variable to store mass of particle [AMU].
	double mAMU;

	//! Variable to store mass of particle [MeV].
	double mMeV;

	//! Variable to store lfctr.
	double lfctr;

	//! Boolean to store if all variables needed for Ziegler excists.
	bool Ziegler;

	//! Function to retrive variables.
	/*! \return true if successful, false otherwise.
	 */
	bool retriveVariables();

};

inline int Particle::GetZ() const { return Z; }
inline int Particle::GetA() const { return A; }
inline double Particle::GetM_AMU() const { return mAMU; }
inline double Particle::GetM_MeV() const { return mMeV; }
inline double Particle::Getlfctr() const { return lfctr; }


#endif // PARTICLE_H
