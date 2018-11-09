#include "runsystem.h"

#include "global.h"

#include "Particle.h"
#include "Material.h"

#include "Scattering.h"
#include "DickNorbury.h"
#include "RelScatter.h"
#include "StoppingPower.h"
#include "Ziegler1985.h"
#include "BetheBlock.h"
#include "ame2012_masses.h"

#include <cmath>
#include <iostream>

RunSystem::RunSystem(Particle *pbeam,
                     Particle *pscatIso,
                     Material *ptarget,
                     Particle *pfragment,
                     Material *pabsorber,
                     Material *pdEdet,
                     Material *pEdet)
    : beam( pbeam )
    , scatIso( pscatIso )
    , target( ptarget )
    , fragment( pfragment )
    , absorber( pabsorber )
    , dEmaterial( pdEdet )
    , Ematerial( pEdet )
    , Ex( new double[POINTS] )
    , dE( new double[POINTS] )
    , delta_dE( new double[POINTS] )
    , E( new double[POINTS] )
    , delta_E( new double[POINTS] )
{
    residual = new Particle(beam->GetZ() + scatIso->GetZ() - fragment->GetZ(), beam->GetA() + scatIso->GetA() - fragment->GetA());
}

RunSystem::~RunSystem()
{
    delete residual;
    Ex.reset();
    dE.reset();
    delta_dE.reset();
    E.reset();
    delta_E.reset();
}

int RunSystem::Run(const double &Energy, const double &Angle) const
{
    RelScatter  *scat = new RelScatter(beam, scatIso, fragment, residual);
    StoppingPower *stopTargetB;
    StoppingPower *stopTargetF;
    StoppingPower *stopAbsor;
    StoppingPower *stopDE;
    StoppingPower *stopE;

    if ( target->GetZ() <= 92 ){
        stopTargetB = new Ziegler1985(target, beam);
        stopTargetF = new Ziegler1985(target, fragment);
    } else {
        stopTargetB = new BetheBlock(target, beam);
        stopTargetF = new BetheBlock(target, fragment);
        std::cout << "Warning: Target Z= " << target->GetZ();
        std::cout << ", Ziegler stopping-power only supports elements up to Z=92. Using Bethe-Block formula for the stopping power.";
        std::cout << std::endl;
    }

    if ( absorber->GetZ() <= 92 ){
        stopAbsor = new Ziegler1985(absorber, fragment);
    } else {
        stopAbsor = new BetheBlock(absorber, fragment);
        std::cout << "Warning: Absorber Z= " << absorber->GetZ();
        std::cout << ", Ziegler stopping-power only supports elements up to Z=92. Using Bethe-Block formula for the stopping power.";
        std::cout << std::endl;
    }

    if ( dEmaterial->GetZ() <= 92 ){
        stopDE = new Ziegler1985(dEmaterial, fragment);
    } else {
        stopDE = new BetheBlock(dEmaterial, fragment);
        std::cout << "Warning: dE detector Z= " << dEmaterial->GetZ();
        std::cout << ", Ziegler stopping-power only supports elements up to Z=92. Using Bethe-Block formula for the stopping power.";
        std::cout << std::endl;

    }

    if ( Ematerial->GetZ() <= 92 ){
        stopE = new Ziegler1985(Ematerial, fragment);
    } else {
        stopE = new BetheBlock(Ematerial, fragment);
        std::cout << "Warning: E detector Z= " << Ematerial->GetZ();
        std::cout << ", Ziegler stopping-power only supports elements up to Z=92. Using Bethe-Block formula for the stopping power.";
        std::cout << std::endl;
    }

    double Ehalf = stopTargetB->Loss(Energy, target->GetWidth(), INTPOINTS);
    double Ehole = stopTargetB->Loss(Energy, INTPOINTS);
    double Exmax = scat->FindMaxEx(Ehalf, Angle);

    //Exmax += Energy;
    double dEX = Exmax/double(POINTS - 1);
    double f, m, b;
    double df, dm, db;
    double ef, em, eb;

    for (int i = 0 ; i < POINTS ; ++i){
        Ex[i] = i*dEX;

        f = scat->EvaluateY(Energy, Angle, Ex[i]);
        m = scat->EvaluateY(Ehalf, Angle, Ex[i]);
        b = scat->EvaluateY(Ehole, Angle, Ex[i]);

        f = stopTargetF->Loss(f, target->GetWidth()/fabs(cos(Angle)), INTPOINTS);
        m = stopTargetF->Loss(m, target->GetWidth()/fabs(2*cos(Angle)), INTPOINTS);

        f = stopAbsor->Loss(f, INTPOINTS);
        m = stopAbsor->Loss(m, INTPOINTS);
        b = stopAbsor->Loss(b, INTPOINTS);

        df = stopDE->Loss(f, INTPOINTS);
        dm = stopDE->Loss(m, INTPOINTS);
        db = stopDE->Loss(b, INTPOINTS);

        if (m - dm == m - dm){
            dE[i] = m-dm;
            delta_dE[i] = (fabs(f - df - dE[i]) + fabs(b - db - dE[i]))/2.;
            ef = stopE->Loss(df, INTPOINTS);
            em = stopE->Loss(dm, INTPOINTS);
            eb = stopE->Loss(db, INTPOINTS);

            if (dm - em == dm - em){
                E[i] = dm - em;
                delta_E[i] = (fabs(df - ef - E[i]) + fabs(db - eb - E[i]))/2.;
            } else {
                dE[i] = 0.;
                delta_dE[i] = 0.;
                E[i] = 0.;
                delta_E[i] = 0.;
            }

        } else {
            dE[i] = 0.;
            delta_dE[i] = 0.;
            E[i] = 0.;
            delta_E[i] = 0.;
        }
    }
    return POINTS;
}
