#include "runsystem.h"

#include "global.h"

#include "Particle.h"
#include "Material.h"

#include "Scattering.h"
#include "DickNorbury.h"
#include "StoppingPower.h"
#include "Ziegler1985.h"
#include "ame2012_masses.h"

#include <cmath>

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
    DickNorbury  *scat = new DickNorbury(beam, scatIso, fragment, residual);
    Ziegler1985 *stopTargetB = new Ziegler1985(target, beam);
    Ziegler1985 *stopTargetF = new Ziegler1985(target, fragment);
    Ziegler1985 *stopAbsor = new Ziegler1985(absorber, fragment);
    Ziegler1985 *stopDE = new Ziegler1985(dEmaterial, fragment);
    Ziegler1985 *stopE = new Ziegler1985(Ematerial, fragment);

    double Ehalf = stopTargetB->Loss(Energy, target->GetWidth(), INTPOINTS);
    double Ehole = stopTargetB->Loss(Energy, INTPOINTS);
    double Exmax = get_Q_keV(beam->GetA(), beam->GetZ(), scatIso->GetA(), scatIso->GetZ(), fragment->GetA(), fragment->GetZ())/1000;

    Exmax += Energy;
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
