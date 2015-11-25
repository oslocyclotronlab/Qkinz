#ifndef BETHEBLOCKCOMP_H
#define BETHEBLOCKCOMP_H

class Particle;
class Material;

#include "BetheBlock.h"

#include <memory>

class BetheBlockComp
{
public:
    //! Constructor.
    BetheBlockComp();

    //! Constructor.
    BetheBlockComp(Material *material, double *weights, const int &n_mat, Particle *beam);

	//! Destructor.
    ~BetheBlockComp();

    //! Assign operator.
    BetheBlockComp &operator=(const BetheBlockComp &bbc);

	//! Calculate stopping power.
	double Evaluate(const double &E) const;

	//! Calculate loss.
	double Loss(const double &E,		/*!< Energy of incident particle [MeV].	*/
				const double &width,	/*!< Width of compound in [g/cm^3].		*/
				const int &points=1000	/*!< Number of integration points.		*/);

    //! Set target materials.
    void setDensityCorrections(BetheBlock::DensityCorr densCorr);

private:
	//! Pointer to contain the Bethe-Block's of the components.
	std::unique_ptr<BetheBlock[]> elements;

	//! Pointer to contain the weighting of the components.
	std::unique_ptr<double[]> weight;

    //! Pointer to contain the beam variable.
    std::unique_ptr<Particle> projectile;

	//! Number of elements in the composit material.
	int nbr_el;

    //! Trying effective.
    double Zeff;
    double Aeff;
    double Ieff;

    BetheBlock::DensityCorr densCorr;
    bool densCorrSet;

    double CalcDensCorr(double X) const;

	//! Variable to contain the density of the material.
    //double density; // Currently unused.

};

#endif // BETHEBLOCK
