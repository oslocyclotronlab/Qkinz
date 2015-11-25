#include "Ziegler1985.h"

#include "Material.h"
#include "Particle.h"

#include <cmath>
#include <iostream>
#include <cstdlib>

static const double c[6] = { 0.2865, 0.1266, -0.001429, 0.02402, -0.01135, 0.001475 };

#ifndef MAX
#define MAX(X,Y)    (((X)>(Y))?(X):(Y))
#endif

#ifndef MIN
#define MIN(X,Y)    (((X)<(Y))?(X):(Y))
#endif

Ziegler1985::Ziegler1985() : StoppingPower(new Material(), new Particle()){}


Ziegler1985::Ziegler1985(Material *material, Particle *particle)
    : StoppingPower(material, particle) { }

Ziegler1985::~Ziegler1985()
{
    //delete pMaterial;
    //delete pParticle;
}

Ziegler1985 &Ziegler1985::operator=(const Ziegler1985 &ziegler)
{
    this->setMaterial(new Material(*(ziegler.pMaterial)));
    this->setParticle(new Particle(*(ziegler.pParticle)));
    return *this;
}

double Ziegler1985::Loss(const double &E, const double &d, const int &points) const
{
    double dx = d/points;
    double e = E*1e3;
    double R1, R2, R3, R4;
    for (int i = 0 ; i < points ; ++i){
        R1 = dx*Evaluate(e);
        R2 = dx*Evaluate(e + 0.5*R1);
        R3 = dx*Evaluate(e + 0.5*R2);
        R4 = dx*Evaluate(e + R3);
        e += (R1 + 2*(R2 + R3) + R4)/6.0;
        if (e < 0 || e != e){
            e = 0;
            break;
        }
    }
    return e/1e3;
}

double Ziegler1985::Loss(const double &E, const int &points) const
{
    double d = pMaterial->GetWidth(Material::um);
    if ( d <= 0 ){
        //std::cerr << "Width of material not set. Defaulting to 100 µm." << std::endl;
        return Loss(E, 100, points);
    }
    return Loss(E, d, points);
}

double Ziegler1985::pstop(const double &e) const
{
    int z2 = pMaterial->GetZ();
    double *pcoef = pMaterial->Getpcoef_ptr();
    double pe0 = 25.;
    double pe = MAX(pe0, e);
    double sl = pcoef[1]*pow(pe, pcoef[2]) + pcoef[3]*pow(pe, pcoef[4]);
    double sh = pcoef[5]/pow(pe, pcoef[6])*log((pcoef[7]/pe) + pcoef[8]*pe);
    double se = sl*sh/(sl + sh);
    if ( e <= pe0 ){
        double velpwr = 0.45;
        if (z2 <= 6)
            velpwr = 0.25;
        se *= pow(e/pe0, velpwr);
    }
    return se;
}

double Ziegler1985::hestop(const double &e) const
{
    int z1 = pParticle->GetZ();
    int z2 = pMaterial->GetZ();
    double E0 = 1.0;
    double E = MAX(E0, e);
    double lE = log(E);
    double g2He = 0, plE = 1;
    for (int i = 0 ; i < 6 ; ++i, plE *= lE)
        g2He += c[i]*plE;
    g2He = 1 - exp(-MIN(30.0, g2He));

    double tmp1 = 7.6 - MAX(0.0, g2He);
    double tmp2 = 1 + (0.007+0.00005*z2)*exp(-tmp1*tmp1);
    g2He *= tmp2*tmp2;

    double sp = pstop(E);
    double se = sp*g2He*z1*z1;
    if (e <= E0)
        se *= sqrt(e/E0);
    return se;
}

double Ziegler1985::histop(const double &e) const
{
    double vfermi = pMaterial->Getvfermi();
    double lfctr = pParticle->Getlfctr();
    int z1 = pParticle->GetZ(), z2 = pMaterial->GetZ();

    double yrmin = 0.13;
    double vrmin = 1.0;
    double v = sqrt(e/25.)/vfermi;
    double vr;
    if ( v < 1 )
        vr = (3*vfermi/4.0)*(1+(2*v*v/3.0) - pow(v, 4)/15.0);
    else
        vr = v*vfermi*(1+1./(5.*v*v));

    double z13 = pow(z1, 1./3.), z23 = pow(z1, 2./3.);
    double yr = MAX(MAX(yrmin, vr/z23), vrmin/z23);
    double a = -0.803*pow(yr, 0.3) + 1.3167*pow(yr, 0.6) + 0.38157*yr + 0.008983*yr*yr;
    double q = MIN(1.0, MAX(0.0, 1 - exp(-MIN(a, 50.))));
    double b = MIN(0.43, MAX(0.32, 0.12+0.025*z1))/z13;
    double l0 = (0.8 - MIN(1.2, 0.6+z1/30.))/z13;
    double l1;
    if ( q < 0.2 )
        l1 = 0;
    else if ( q  < MAX(0.0, 0.9-0.025*z1) )
        l1 = b*(q-0.2)/fabs(MAX(0.0, 0.9-0.025*z1)-0.2000001);
    else if ( q < MAX(0.0, 1-0.025*MIN(16, z1)) )
        l1 = b;
    else
        l1 = b*(1-q)/(0.025*MIN(16, z1));
    double l = MAX(l1, l0*lfctr);
    double aa = 7.6 - MAX(0.0, log(e));
    double zeta = (q + (1/(2*vfermi*vfermi))*(1-q)*log(1 + pow(4*l*vfermi/1.919,2)))
            *(1+1.0/(z1*z1)*(0.18+0.0015*z2)*exp(-aa*aa));
    double se;
    if ( yr <= MAX(yrmin, vrmin/z23) ){
        double vmin = 0.5*(vrmin + sqrt(MAX(0.0, vrmin*vrmin - 0.8*vfermi*vfermi)));
        double eee = 25*vmin*vmin;
        double sp = pstop(eee);
        double power = 0.5;
        if ( (z2==6) || ((z2==14 || z2 == 32) && (z1 <= 19)) )
            power = 0.375;
        se = sp*pow(zeta*z1, 2)*pow(e/eee, power);
    } else {
        double sp = pstop(e);
        se = sp*pow(zeta*z1, 2);
    }
    return se;
}

double Ziegler1985::stop(const double &ee) const
{
    double se=0, sn=0;

    int z1 = pParticle->GetZ(), z2 = pMaterial->GetZ();
    double m1 = pParticle->GetM_AMU(), m2 = pParticle->GetM_AMU();

    if ( z1 < 1 || z1 > 92 || z2 < 1 || z2 > 92 || m1 <= 0 || m2 <= 0 )
        return -1;
    if ( ee < 1e-10 )
        return 0;

    // Convert to MeV/amu and chec that this is below 100 MeV/amu
    double e = ee/m1;
    if (e > 1.1e5 )
        return 0;

    if ( z1 == 1 )
        se = pstop(e);
    else if ( z1 == 2 )
        se = hestop(e);
    else
        se = histop(e);

    double rm = (m1 + m2)*(pow(z1, 0.23) + pow(z2, 0.23));
    double epsil = 32.53*m2*ee/(z1*z2*rm);
    if (epsil < 30 ){
        double a = 0.01321*pow(epsil, 0.21226) + 0.19593*sqrt(epsil);
        sn = 0.5*log(1 + 1.1383*epsil)/(epsil + a);
    } else {
        sn = 0.5*log(epsil)/epsil;
    }

    se *= pMaterial->Getatrho()*1e-23;
    sn *= pMaterial->Getatrho()*1e-23;
    return se+sn;
}
