#include "Scattering.h"

#include "Particle.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

Scattering::Scattering(Particle *pA, Particle *pX, Particle *pY, Particle *pB)
    : A( pA )
    , X( pX )
    , Y( pY )
    , B( pB )
{
    // Some checking if reaction is possible. If not, give error message to the user.
    if (A->GetA() + X->GetA() != Y->GetA() + B->GetA()){
        std::cerr << "Missmach in total nucleons. Total number of nucleons before scattering: "
                  << A->GetA() + X->GetA() << ". Total after: " << Y->GetA() + B->GetA() << std::endl;
        std::abort();
    } else if (A->GetZ() + X->GetZ() != Y->GetZ() + B->GetZ()){
        std::cerr << "Missmach in charge. Total charge before scattering: "
                  << A->GetZ() + X->GetZ() << ". Total after: " << Y->GetZ() + B->GetZ() << std::endl;
        std::abort();
    }
}

Scattering::~Scattering(){ }

double Scattering::EvaluateB(const double &EA, const double &EY, const double &thetaY, double &Ex, double &thetaB) const
{
    double m1 = A->GetM_MeV(), m2 = X->GetM_MeV(), m3 = Y->GetM_MeV(), m4 = B->GetM_MeV();
    double E1l = EA + m1;
    double p1l = sqrt(E1l*E1l - m1*m1);
    double betaCM = p1l/(EA + m1 + m2);
    double gammaCM = 1/sqrt(1 - betaCM*betaCM);

    double E1c = gammaCM*(E1l - betaCM*p1l);
    double E2c = gammaCM*m2;

    double Etotc = E1c + E2c;

    double E3l = EY + m3;
    double p3l = sqrt(E3l*E3l - m3*m3);

    double E3c = gammaCM*(E3l - betaCM*p3l*cos(thetaY));
    double p3cx = gammaCM*(p3l*cos(thetaY) - betaCM*E3l);
    double p3cy = p3l*sin(thetaY);

    double E4c = Etotc - E3c;

    double p4lx = gammaCM*(-p3cx + betaCM*E4c);
    double p4ly = -p3cy;

    thetaB = acos(p4lx/sqrt(p4lx*p4lx + p4ly*p4ly));
    Ex = sqrt(E4c*E4c - p3cx*p3cx - p3cy*p3cy) - m4;

    return gammaCM*(E4c - betaCM*p3cx) - m4;
}
