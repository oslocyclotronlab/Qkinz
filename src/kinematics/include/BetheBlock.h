#ifndef BETHEBLOCK_H
#define BETHEBLOCK_H

#include "StoppingPower.h"

class BetheBlock : public StoppingPower
{
public:
	struct DensityCorr {
		double C0;
		double a;
		double m;
		double X1;
		double X0;
	};
    //! Empty constructor.
    BetheBlock();

	//! Constructor.
	BetheBlock(Material *material, Particle *particle);

	~BetheBlock();

    //! Assign operator.
    BetheBlock &operator=(const BetheBlock &bb);

	//! Calculate stopping power.
	/*! \return the stopping power for a
	 *  particle of a given type in the given material.
	 */
	double Evaluate(const double &E	/*!< Energy of incident particle.	*/) const;

	double Loss(const double &E,		/*!< Initial energy of the incident particle in [MeV].	*/
                const int &points=1000	/*!< Number of integration points.						*/) const;

    double Loss(const double &E,
                const double &width,
                const int &points=1000) const;

	//! Set density correction parameters (optional).
	void setDensityCorr(const DensityCorr &densC)
	{
		densCorr = densC;
		densCorrSet = true;
	}

private:
	DensityCorr densCorr;
	bool densCorrSet;

	double calcDensityCorrection(const double &X) const;
};


#endif // BETHEBLOCK_H
