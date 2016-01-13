#include "ZieglerComp.h"


#include "Ziegler1985.h"
#include "Material.h"
#include "Particle.h"

ZieglerComp::ZieglerComp(Material *material, double *weight, const int &n, Particle *particle, const double &dens)
	: Ziegler( new Ziegler1985[n] )
	, Weights( new double[n] )
	, projectile( new Particle(*particle) )
	, n_mat( n )
	, density( dens )
{
	for (int i = 0 ; i < n_mat ; ++i){
        Ziegler[i] = Ziegler1985(new Material(*(material + i)), new Particle(*projectile));
		Weights[i] = weight[i];
	}
}

double ZieglerComp::Evaluate(const double &E) const
{
	double result = 0;
	for (int i = 0 ; i < n_mat ; ++i){
		result += Weights[i]*Ziegler[i](E);
	}
	return result;
}

double ZieglerComp::Loss(const double &E, const double &width, const int &points) const
{
	double dx = 10*width/((points-1)*density);
	double e = E;
	double R1, R2, R3, R4;
	for (int i = 0 ; i < points ; ++i){
		R1 = dx*Evaluate(e);
		R2 = dx*Evaluate(e + 0.5*R1);
		R3 = dx*Evaluate(e + 0.5*R2);
		R4 = dx*Evaluate(e + R3);
		e += (R1 + 2*(R2 + R3) + R4)/6.0;
		if (e < 0)
			return 0;
	}
	return e;
}
