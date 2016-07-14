#include "RelScatter.h"

#include "Particle.h"

#include <cmath>


RelScatter::RelScatter(Particle *pA, Particle *pX, Particle *pY, Particle *pB)
    : Scattering(pA, pX, pY, pB)
{

}

RelScatter::~RelScatter(){ }

double RelScatter::EvaluateY(const double &E, const double &theta, const double &Ex) const
{
    double m1 = X->GetM_MeV(), m2 = A->GetM_MeV(), m3 = Y->GetM_MeV(), m4 = B->GetM_MeV();

    double m4ex = m4 + Ex;

    double s = (m1+m2)*(m1+m2) + 2*m1*E;

    double pcm = sqrt(((s - m1*m1 - m2*m2)*(s - m1*m1 - m2*m2) - 4*m1*m1*m2*m2)/(4*s));

    double chi = log((pcm + sqrt(pcm*pcm + m1*m1))/m1);

    double pcm2 = sqrt(((s - m3*m3 - m4ex*m4ex)*(s - m3*m3 - m4ex*m4ex) - 4*m3*m3*m4ex*m4ex)/(4*s));

    double p3 = sqrt(pcm2*pcm2 + m3*m3)*cos(theta)*sinh(chi)+sqrt(pcm2*pcm2 - pow(m3*sin(theta)*sinh(chi),2))*cosh(chi);
    p3 /= 1 + sin(theta)*sin(theta)*sinh(chi)*sinh(chi);

    double E3 = sqrt(p3*p3 + m3*m3);
    return E3 - m3;
}

adouble RelScatter::EvaluateY(double E, double theta, adouble Ex)
{
    double m1 = X->GetM_MeV(), m2 = A->GetM_MeV(), m3 = Y->GetM_MeV(), m4 = B->GetM_MeV();

    adouble m4ex = m4 + Ex;

    double s = (m1+m2)*(m1+m2) + 2*m1*E;

    double pcm = sqrt(((s - m1*m1 - m2*m2)*(s - m1*m1 - m2*m2) - 4*m1*m1*m2*m2)/(4*s));

    double chi = log((pcm + sqrt(pcm*pcm + m1*m1))/m1);

    adouble pcm2 = sqrt(((s - m3*m3 - m4ex*m4ex)*(s - m3*m3 - m4ex*m4ex) - 4*m3*m3*m4ex*m4ex)/(4*s));

    adouble p3 = sqrt(pcm2*pcm2 + m3*m3)*cos(theta)*sinh(chi)+sqrt(pcm2*pcm2 - pow(m3*sin(theta)*sinh(chi),2))*cosh(chi);
    p3 /= 1 + sin(theta)*sin(theta)*sinh(chi)*sinh(chi);

    adouble E3 = sqrt(p3*p3 + m3*m3);
    return E3 - m3;
}

double RelScatter::FindMaxEx(const double &E, const double &theta) const
{
    double m1 = X->GetM_MeV(), m2 = A->GetM_MeV(), m3 = Y->GetM_MeV(), m4 = B->GetM_MeV();

    double s = (m1+m2)*(m1+m2) + 2*m1*E;

    double pcm = sqrt(((s - m1*m1 - m2*m2)*(s - m1*m1 - m2*m2) - 4*m1*m1*m2*m2)/(4*s));

    double chi = log((pcm + sqrt(pcm*pcm + m1*m1))/m1);

    double Ex = sqrt(s - 2*sqrt(s)*m3*sqrt(1 + pow(sin(theta)*sinh(chi), 2)) + m3*m3) - m4;
    return Ex;
}
