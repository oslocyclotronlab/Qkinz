#ifndef ZIEGLERCOMP_H
#define ZIEGLERCOMP_H

#include <memory>

class Ziegler1985;

class Material;
class Particle;

class ZieglerComp
{
public:
	ZieglerComp(Material *material, double *weight, const int &n, Particle *particle, const double &dens);

	double Evaluate(const double &E) const;

	double Loss(const double &E, const double &width /* In mg/cm2 */, const int &points=1000) const;


private:
	std::unique_ptr<Ziegler1985[]> Ziegler;

	std::unique_ptr<double[]> Weights;

	std::unique_ptr<Particle> projectile;

	int n_mat;

	double density;
};


#endif // ZIEGLERCOMP_H