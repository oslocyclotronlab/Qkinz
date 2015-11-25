#include "BetheBlock.h"

#include "Material.h"
#include "Particle.h"

#ifndef BETHEBLOCKCONST
#define BETHEBLOCKCONST 0.1535 // MeVcm^2/g
#endif

#ifndef MASSELECTRON
#define MASSELECTRON 0.511 // MeV/c^2
#endif

BetheBlock::BetheBlock()
    : StoppingPower(new Material(), new Particle())
    , densCorrSet( false ) {}

BetheBlock::BetheBlock(Material *material, Particle *particle)
	: StoppingPower(material, particle)
	, densCorrSet( false )
{
}

BetheBlock::~BetheBlock()
{
    //delete pMaterial;
    //delete pParticle;
}

BetheBlock &BetheBlock::operator=(const BetheBlock &bb)
{
    this->pMaterial = new Material(*(bb.pMaterial));
    this->pParticle = new Particle(*(bb.pParticle));
    this->densCorrSet = bb.densCorrSet;
    this->densCorr = bb.densCorr;
    return *this;
}

double BetheBlock::Evaluate(const double &E) const
{
	double Erel = E + pParticle->GetM_MeV();
	double prel = sqrt(Erel*Erel - pParticle->GetM_MeV()*pParticle->GetM_MeV());
	double beta = prel/Erel;
	double gamma = 1./sqrt(1 - beta*beta);

    double result = 2*log(2*MASSELECTRON*pow(beta*gamma, 2)/pMaterial->GetMeanEx());
    result -= log(pow(MASSELECTRON/pParticle->GetM_MeV(), 2) + 2*gamma*MASSELECTRON/pParticle->GetM_MeV() + 1);
	result -= 2*beta*beta;
	if (densCorrSet)
		result -= calcDensityCorrection(log10(beta*gamma));
    result *= BETHEBLOCKCONST*pMaterial->GetZ()*pow(pParticle->GetZ(), 2)/(pMaterial->GetA()*pow(beta,2));
    return -fabs(result);
}

double BetheBlock::Loss(const double &E, const int &points) const
{
    double dx = pMaterial->GetWidth(Material::gcm2)/(points - 1);
    double e = E;
    double R1, R2, R3, R4;
    for (int i = 0 ; i < points ; ++i){
        R1 = dx*Evaluate(e);
        R2 = dx*Evaluate(e + 0.5*R1);
        R3 = dx*Evaluate(e + 0.5*R2);
        R4 = dx*Evaluate(e + R3);
        e += (R1 + 2*(R2 + R3) + R4)/6.;
        if (e < 0.1 || e != e)
            return 0;
    }
    return e;
}

double BetheBlock::Loss(const double &E, const double &width, const int &points) const
{
    double dx = width/(points - 1);
    double e = E;
    double R1, R2, R3, R4;
    for (int i = 0 ; i < points ; ++i){
        R1 = dx*Evaluate(e);
        R2 = dx*Evaluate(e + 0.5*R1);
        R3 = dx*Evaluate(e + 0.5*R2);
        R4 = dx*Evaluate(e + R3);
        e += (R1 + 2*(R2 + R3) + R4)/6.;
        if (e < 0.1 || e != e)
            return 0;
    }
    return e;
}

double BetheBlock::calcDensityCorrection(const double &X) const
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

#ifdef BETHEBLOCKCONST
#undef BETHEBLOCKCONST
#endif
#ifdef MASSELECTRON
#undef MASSELECTRON
#endif

