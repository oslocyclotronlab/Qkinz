#include "Iterative.h"

#include "Particle.h"
#include "ame2012_masses.h"

#include <cmath>

Iterative::~Iterative(){ }

double Iterative::EvaluateY(const double &E, const double &theta, const double &Ex) const
{
    double m1 = A->GetM_AMU(), m3 = Y->GetM_AMU(), m4 = B->GetM_AMU();
    double Q = get_Q_keV(X->GetA(), X->GetZ(), A->GetA(), A->GetZ(), Y->GetA(), Y->GetZ())/1e3;

    double t2f = 0.6*(E + Q);
    double delta = -0.2*(E + Q);

    while ( fabs(delta) > 1e-20 ){
        double t1f = t2f;
        t2f += delta;

        double t1r = Q + E - t1f;
        double t2r = Q + E - t2f;
        double p21r = P2rel(m4, t1r);
        double p22r = P2rel(m4, t2r);
        double p2i = P2rel(m1, E);
        double p21f = P2rel(m3, t1f);
        double p22f = P2rel(m3, t2f);

        double f1 = p21r - p2i - p21f + 2*cos(theta)*sqrt(p2i*p21f);
        double f2 = p22r - p2i - p22f + 2*cos(theta)*sqrt(p2i*p22f);

        if ( fabs(f1 -f2) < 1e-10 )
            break;

        delta *= -1./(1- f1/f2);
    }
    return t2f - Ex;
}
