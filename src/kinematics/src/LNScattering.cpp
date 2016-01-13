#include "LNScattering.h"

#include "Particle.h"

#include <cmath>

LNScattering::LNScattering(Particle *pA, Particle *pX, Particle *pY, Particle *pB)
    : Scattering(pA, pX, pY, pB){ }

LNScattering::~LNScattering(){ }

double LNScattering::EvaluateY(const double &E, const double &theta, const double &Ex) const
{
    double Ma = A->GetM_MeV(), MA = X->GetM_MeV(), Mb = Y->GetM_MeV(), MB = B->GetM_MeV();
    double Q = Ma + MA - Mb - MB - Ex;
    double ET = E + Q;

    //double RaB = Ma*MB*E/(ET*(Ma + MA)*(Mb + MB));
    //double RAb = MA*Mb*(1 + Ma*Q/(MA*ET))/((Ma + MA)*(Mb + MB));
    double Rab = Ma*Mb*E/(ET*(Ma + MA)*(Mb + MB));
    double RAB = MA*MB*(1 + Ma*Q/(MA*ET))/((Ma + MA)*(Mb + MB));

    if (RAB > Rab)
        return Rab*pow(cos(theta) + sqrt(RAB/Rab - sin(theta)*sin(theta)), 2)*ET;
    else
        return Rab*pow(cos(theta) - sqrt(RAB/Rab - sin(theta)*sin(theta)), 2)*ET;
}
