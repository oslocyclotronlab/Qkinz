#include "BetheBlockComp.h"

#include "BetheBlock.h"
#include "Particle.h"
#include "Material.h"

#ifndef BETHEBLOCKCONST
#define BETHEBLOCKCONST 0.1535 // MeVcm^2/g
#endif

#ifndef MASSELECTRON
#define MASSELECTRON 0.511 // MeV/c^2
#endif

BetheBlockComp::BetheBlockComp(){}


BetheBlockComp::BetheBlockComp(Material *material, double *weights, const int &n_mat, Particle *beam)
{
    elements.reset(new BetheBlock[n_mat]);
    weight.reset(new double[n_mat]);
    projectile.reset(new Particle(*beam));
    nbr_el = n_mat;
    Zeff = 0;
    Aeff = 0;
    Ieff = 0;
    for (int i = 0 ; i < n_mat ; ++i){
        Zeff += material[i].GetZ()*weights[i];
        Aeff += material[i].GetA()*weights[i];
        Ieff += log(material[i].GetMeanEx())*weights[i]*material[i].GetZ();
        weight[i] = weights[i];
        elements[i] = BetheBlock(new Material(*(material+i)), new Particle(*beam));
    }
    Ieff /= Zeff;
    Ieff = exp(Ieff);
    densCorrSet = false;
}

BetheBlockComp::~BetheBlockComp()
{
    if (elements)
        elements.reset();
    if (weight)
        weight.reset();
    if (projectile)
        projectile.reset();
}

BetheBlockComp &BetheBlockComp::operator=(const BetheBlockComp &bbc)
{
    this->elements.reset(new BetheBlock[bbc.nbr_el]);
    this->weight.reset(new double[bbc.nbr_el]);
    this->nbr_el = bbc.nbr_el;
    this->projectile.reset(new Particle(*(bbc.projectile)));
    for (int i = 0 ; i < this->nbr_el ; ++i){
        this->elements[i] = bbc.elements[i];
        this->weight[i] = bbc.weight[i];
    }
    return *this;
}

double BetheBlockComp::Evaluate(const double &E) const
{
    double Erel = E + projectile->GetM_MeV();
    double prel = sqrt(pow(Erel, 2) - pow(projectile->GetM_MeV(), 2));
    double beta = prel/Erel;
    double gamma = 1./sqrt(1 - beta*beta);

    double result = 2*log(2*MASSELECTRON*pow(beta*gamma, 2)/Ieff);
    result -= log(pow(MASSELECTRON/projectile->GetM_MeV(), 2) + 2*gamma*MASSELECTRON/projectile->GetM_MeV() + 1);
    result -= 2*beta*beta;
    if (densCorrSet)
        result -= CalcDensCorr(log10(beta*gamma));
    result *= BETHEBLOCKCONST*Zeff*pow(projectile->GetZ(),2)/(Aeff*pow(beta,2));
    return -result;
}

/*
double BetheBlockComp::Evaluate(const double &E) const
{
	double result = 0;
	for (int i = 0 ; i < nbr_el ; ++i){
		result += weight[i]*elements[i](E);
	}
	return result;
}
*/
double BetheBlockComp::Loss(const double &E, const double &width, const int &points)
{
    double dx = width/(points - 1);
	double e = E;
	double R1, R2, R3, R4;
	for (int i = 0 ; i < points ; ++i){
		R1 = dx*Evaluate(e);
		R2 = dx*Evaluate(e + 0.5*R1);
		R3 = dx*Evaluate(e + 0.5*R2);
		R4 = dx*Evaluate(e + R3);
		e += (R1 + 2*(R2*R3) + R4)/6.;
		if (e < 0) return 0;
	}
	return e;
}
void BetheBlockComp::setDensityCorrections(BetheBlock::DensityCorr densCor){ densCorr = densCor; densCorrSet = true; }

double BetheBlockComp::CalcDensCorr(double X) const
{
    if (!densCorrSet)
        return 0;
    else if (X < densCorr.X0)
        return 0;
    else if (X < densCorr.X1)
        return 4.6052*X + densCorr.C0 + densCorr.a*pow(densCorr.X1 - X, densCorr.m);
    else
        return 4.6052*X + densCorr.C0;
}
/*
void BetheBlockComp::setDensityCorrections(BetheBlock::DensityCorr *densCorr)
{
    BetheBlock::DensityCorr tmp;
    for (int i = 0 ; i < nbr_el ; ++i){
        tmp = densCorr[i];
        elements[i].setDensityCorr(tmp);
    }
}
*/
