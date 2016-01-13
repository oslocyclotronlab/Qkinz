#include "DickNorbury.h"

#include "Particle.h"

#include <cmath>

DickNorbury::DickNorbury(Particle *pA, Particle *pX, Particle *pY, Particle *pB)
    : Scattering(pA, pX, pY, pB){ }

DickNorbury::~DickNorbury(){ }

double DickNorbury::EvaluateY(const double &E, const double &theta, const double &Ex) const
{
    if (E + A->GetM_MeV() + X->GetM_MeV() < Y->GetM_MeV() + B->GetM_MeV() + Ex)
        return 0;
    double m1 = A->GetM_MeV(), m2 = X->GetM_MeV(), m3 = Y->GetM_MeV(), m4 = B->GetM_MeV() + Ex;
    double c3l = cos(theta);
    double E1l = E + m1;
    double s = m1*m1 + m2*m2 + 2*E1l*m2;
    double a = 0.5*(s + m3*m3 - m4*m4);
    double b = E1l + m2;
    double x2 = (E1l*E1l - m1*m1)*c3l*c3l;
    double x = sqrt(x2);
    double D = b*b - x2;
    double y = x*sqrt(a*a - m3*m3*D);
    double k3la = (a*b + y)/D - m3;
    double k3lb = (a*b - y)/D - m3;

    if (k3la < 0 && k3lb < 0)
        return 0;
    else if (k3la > k3lb)
        return k3la;
    else
        return k3lb;

}

double DickNorbury::FindMaxEx(const double &E, const double &theta) const
{
    double Ex = 0.0;
    double EaS = EvaluateY(E, theta, Ex);
    while (EaS == EaS){
        Ex += 0.01;
        EaS = EvaluateY(E, theta, Ex);
    }
    return Ex - 0.01;
}
