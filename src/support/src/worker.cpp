#include "worker.h"

#include "Particle.h"
#include "Material.h"
#include "global.h"

#include "Scattering.h"
#include "DickNorbury.h"
#include "Iterative.h"
#include "RelScatter.h"

#include "StoppingPower.h"
#include "Ziegler1985.h"
#include "BetheBlock.h"

#include "ziegler1985_table.h"
#include "ame2012_masses.h"
#include "excitation.h"
#include "CustomPower.h"

#include <QVector>
#include <iostream>

#include "Vector.h"
#include "Polyfit.h"

const double PI = acos(-1);
const double ANG_FWD = 47*PI/180.;

static Material::Unit Unit2MatUnit(const Unit_t &unit)
{
    if (unit == mgcm2)
        return Material::mgcm2;
    else if (unit == gcm2)
        return Material::gcm2;
    else if (unit == um)
        return Material::um;
    else
        return Material::um;
}

Worker::Worker(Beam_t *beam, Target_t *target, Extra_t *front, Extra_t *back, Telescope_t *telescope)
    : theBeam( beam )
    , theTarget( target )
    , theFront( front )
    , theBack( back )
    , theTelescope( telescope )
    , haveCpro( false )
    , haveCfrag( false )
{
}

void Worker::setCustomTarget(CustomPower *projectile, CustomPower *fragment)
{
    proCustom.reset(projectile); haveCpro=true;
    fragCustom.reset(fragment); haveCfrag=true;
}

bool Worker::getCoeff(const double &angle, const int &fragA, const int &fragZ, QVector<double> &coeff)
{
    QVector<double> ex, de, e;
    return Curve(ex, de, e, coeff, angle, fragA, fragZ);
}

void Worker::Run(const double &Angle, const bool &p, const bool &d, const bool &t, const bool &h3, const bool &a)
{
    QVector<double> ex, de, d_de, e, d_e, coeff;
    int ntot = 0, nres=0;
    if (p) ntot += 2;
    if (d) ntot += 2;
    if (t) ntot += 2;
    if (h3) ntot += 2;
    if (a) ntot += 2;

    if (p){
        if (Curve(ex, de, e, coeff, Angle, 1, 1)){
            emit ResultCurve(e, de, coeff, Proton);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
        if (Known(ex, de, e, d_de, d_e, Angle, 1, 1)){
            emit ResultScatter(e, d_e, de, d_de, ex, Proton);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
    }
    if (d){
        if (Curve(ex, de, e, coeff, Angle, 2, 1)){
            emit ResultCurve(e, de, coeff, Deutron);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
        if (Known(ex, de, e, d_de, d_e, Angle, 2, 1)){
            emit ResultScatter(e, d_e, de, d_de, ex, Deutron);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
    }
    if (t){
        if (Curve(ex, de, e, coeff, Angle, 3, 1)){
            emit ResultCurve(e, de, coeff, Triton);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
        if (Known(ex, de, e, d_de, d_e, Angle, 3, 1)){
            emit ResultScatter(e, d_e, de, d_de, ex, Triton);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
    }
    if (h3){
        if (Curve(ex, de, e, coeff, Angle, 3, 2)){
            emit ResultCurve(e, de, coeff, Helium3);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
        if (Known(ex, de, e, d_de, d_e, Angle, 3, 2)){
            emit ResultScatter(e, d_e, de, d_de, ex, Helium3);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
    }
    if (a){
        if (Curve(ex, de, e, coeff, Angle, 4, 2)){
            emit ResultCurve(e, de, coeff, Alpha);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
        if (Known(ex, de, e, d_de, d_e, Angle, 4, 2)){
            emit ResultScatter(e, d_e, de, d_de, ex, Alpha);
            ++nres;
            emit curr_prog(100*double(nres)/double(ntot));
        }
    }
    emit FinishedAll();
}

bool Worker::Curve(QVector<double> &Ex, QVector<double> &dE, QVector<double> &E, QVector<double> &coeff, const double &Angle, const int &fA, const int &fZ)
{
        double incAngle;
        if (Angle > PI/2.)
            incAngle = PI - ANG_FWD - Angle;
        else
            incAngle = Angle - ANG_FWD;

        Particle *beam = new Particle(theBeam->Z, theBeam->A);
        Particle *scatIso = new Particle(theTarget->Z, theTarget->A);
        Particle *fragment = new Particle(fZ, fA);
        Particle *residual = new Particle(theBeam->Z+theTarget->Z-fZ, theBeam->A+theTarget->A-fA);
        Material *front = new Material(theFront->Z, theFront->A, theFront->width, Unit2MatUnit(theFront->unit));
        Material *target = new Material(theTarget->Z, theTarget->A, theTarget->width, Unit2MatUnit(theTarget->unit));
        Material *back = new Material(theBack->Z, theBack->A, theBack->width/fabs(cos(Angle)), Unit2MatUnit(theBack->unit));
        Material *abs = new Material(theTelescope->Absorber.Z, Get_mm2(theTelescope->Absorber.Z), theTelescope->Absorber.width/cos(incAngle), Unit2MatUnit(theTelescope->Absorber.unit));
        Material *dEdet = new Material(theTelescope->dEdetector.Z, Get_mm2(theTelescope->dEdetector.Z), theTelescope->dEdetector.width/cos(incAngle), Unit2MatUnit(theTelescope->dEdetector.unit));
        Material *Edet = new Material(theTelescope->Edetector.Z, Get_mm2(theTelescope->Edetector.Z), theTelescope->Edetector.width/cos(incAngle), Unit2MatUnit(theTelescope->Edetector.unit));

        RelScatter *scat = new RelScatter(beam, scatIso, fragment, residual);//new Iterative(beam, scatIso, fragment, residual);

        // Setting up stopping power for the target.
        StoppingPower *stopTargetB;
        StoppingPower *stopTargetF;
        Material::Unit tUnit;
        if (theTarget->Z > 92){
            stopTargetB = new BetheBlock(target, beam);
            stopTargetF = new BetheBlock(target, fragment);
            tUnit = Material::gcm2;
        } else {
            stopTargetB = new Ziegler1985(target, beam);
            stopTargetF = new Ziegler1985(target, fragment);
            tUnit = Material::um;
        }

        // Setting up stopping power for the fronting and backing.
        StoppingPower *stopFrontB;
        StoppingPower *stopFrontF;
        Material::Unit fUnit;
        if (theFront->Z > 92){
            stopFrontB = new BetheBlock(front, beam);
            stopFrontF = new BetheBlock(front, fragment);
            fUnit = Material::gcm2;
        } else {
            stopFrontB = new Ziegler1985(front, beam);
            stopFrontF = new Ziegler1985(front, fragment);
            fUnit = Material::um;
        }
        StoppingPower *stopBack;
        Material::Unit bUnit;
        if (theBack->Z > 92){
            stopBack = new BetheBlock(back, fragment);
            bUnit = Material::gcm2;
        } else {
            stopBack = new Ziegler1985(back, fragment);
            bUnit = Material::um;
        }

        // Setting up stopping power for the absorber.
        StoppingPower *stopAbsor;
        if (theTelescope->Absorber.Z > 92){
            stopAbsor = new BetheBlock(abs, fragment);
        } else {
            stopAbsor = new Ziegler1985(abs, fragment);
        }

        // Setting up stopping power for thin detector.
        StoppingPower *stopDE;
        if (theTelescope->dEdetector.Z > 92){
            stopDE = new BetheBlock(dEdet, fragment);
        } else {
            stopDE = new Ziegler1985(dEdet, fragment);
        }

        // Setting up stopping power for thick detector.
        StoppingPower *stopE;
        if (theTelescope->Edetector.Z > 92){
            stopE = new BetheBlock(Edet, fragment);
        } else {
            stopE = new Ziegler1985(Edet, fragment);
        }
        double E_beam = theBeam->E;
        if (theFront->is_present)
            E_beam = stopFrontB->Loss(E_beam, INTPOINTS);

        double Ehalf, Ewhole;
        if (haveCpro){
            Ehalf = proCustom->Loss(E_beam, target->GetWidth(Material::Unit::mgcm2)/2., INTPOINTS); // The stopping power are in ug/cm^2
            Ewhole = proCustom->Loss(E_beam, target->GetWidth(Material::Unit::mgcm2), INTPOINTS);
        } else {
            Ehalf = stopTargetB->Loss(E_beam, target->GetWidth(tUnit)/2., INTPOINTS);
            Ewhole = stopTargetB->Loss(E_beam, INTPOINTS);
        }
        double dEx = scat->FindMaxEx(Ewhole, Angle)/double(POINTS - 1);

        if ((Ehalf + get_Q_keV(theBeam->A, theBeam->Z, theTarget->A, theTarget->Z, fA, fZ)/1000.)<0){
            // Particles.
            delete beam;
            delete scatIso;
            delete fragment;
            delete residual;
            
            // Materials.
            delete target;
            delete front;
            delete back;
            delete abs;
            delete dEdet;
            delete Edet;
            
            // Scattering calculator.
            delete scat;
            
            // Stopping power calculators.
            delete stopTargetB;
            delete stopTargetF;
            delete stopFrontB;
            delete stopFrontF;
            delete stopBack;
            delete stopAbsor;
            delete stopDE;
            delete stopE;
            return false; // Reaction not possible. Not enough energy :(
        }

        QVector<double> Ex_tmp(POINTS), dE_tmp(POINTS), E_tmp(POINTS), E_err_tmp(POINTS), is_punch(POINTS);
        //adouble Ex_tmp(POINTS), dE_tmp(POINTS), E_tmp(POINTS), E_err_tmp(POINTS), is_punch(POINTS);

        Ex.clear();
        dE.clear();
        E.clear();

        /*double l;
        double m, dm, em;
        double n;

        for (int i = 0 ; i < POINTS ; ++i){
            Ex_tmp[i] = i*dEx;

            l = scat->EvaluateY(E_beam, Angle, Ex_tmp[i]);
            m = scat->EvaluateY(Ehalf, Angle, Ex_tmp[i]);
            n = scat->EvaluateY(Ewhole, Angle, Ex_tmp[i]);

            if (Angle > PI/2.){
                if (haveCfrag)
                    n = fragCustom->Loss(n, target->GetWidth(Material::Unit::mgcm2)/fabs(cos(Angle)), INTPOINTS);
                else
                    n = stopTargetF->Loss(n, target->GetWidth(tUnit)/fabs(cos(Angle)), INTPOINTS);
            } else {
                if (haveCfrag)
                    l = fragCustom->Loss(l, target->GetWidth(Material::Unit::mgcm2)/fabs(cos(Angle)), INTPOINTS);
                else
                    l = stopTargetF->Loss(l, target->GetWidth(tUnit)/fabs(cos(Angle)), INTPOINTS);
            }
            if (haveCfrag)
                m = fragCustom->Loss(m, target->GetWidth(Material::Unit::mgcm2)/fabs(2*cos(Angle)), INTPOINTS);
            else
                m = stopTargetF->Loss(m, target->GetWidth(tUnit)/fabs(2*cos(Angle)), INTPOINTS);

            if (Angle > PI/2. && theFront->is_present){
                l = stopFrontF->Loss(l, front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
                m = stopFrontF->Loss(m, front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
                n = stopFrontF->Loss(n, front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
            } else if (theBack->is_present){
                l = stopBack->Loss(l, INTPOINTS);
                m = stopBack->Loss(m, INTPOINTS);
                n = stopBack->Loss(n, INTPOINTS);
            }

            if (theTelescope->has_absorber){
                l = stopAbsor->Loss(l, INTPOINTS);
                m = stopAbsor->Loss(m, INTPOINTS);
                n = stopAbsor->Loss(n, INTPOINTS);
            }

            E_err_tmp[i] = sqrt(3*l*l + 3*n*n + 4*m*m - 2*n*l -4*m*(l + n))/4.;

            m = (l + 2*m + n)/4.;

            dm = stopDE->Loss(m, INTPOINTS);

            em = stopE->Loss(dm, INTPOINTS);

            dE_tmp[i] = m - dm;
            E_tmp[i] = dm - em;
            is_punch[i] = em;
            if (em != em)
                is_punch[i] = 1000;
        }*/


        adouble l(POINTS);
        adouble m(POINTS), dm(POINTS), em(POINTS);
        adouble n(POINTS);

        for (int i = 0 ; i < POINTS ; ++i){
            Ex_tmp[i] = i*dEx;
        }

        for (int i = 0 ; i < POINTS ; ++i){
            l[i] = scat->EvaluateY(E_beam, Angle, Ex_tmp[i]);
            m[i] = scat->EvaluateY(Ehalf, Angle, Ex_tmp[i]);
            n[i] = scat->EvaluateY(Ewhole, Angle, Ex_tmp[i]);
        }

        if (Angle > PI/2.){
            if (haveCfrag){
                for (int i = 0 ; i < POINTS ; ++i){
                    n[i] = fragCustom->Loss(n[i], target->GetWidth(Material::Unit::mgcm2)/fabs(cos(Angle)), INTPOINTS);
                }
            } else {
                for (int i = 0 ; i < POINTS ; ++i){
                    n[i] = stopTargetF->Loss(n[i], target->GetWidth(tUnit)/fabs(cos(Angle)), INTPOINTS);
                }
            }
        } else {
            if (haveCfrag){
                for (int i = 0 ; i < POINTS ; ++i)
                    l[i] = fragCustom->Loss(l[i], target->GetWidth(Material::Unit::mgcm2)/fabs(cos(Angle)), INTPOINTS);
            } else {
                for (int i = 0 ; i < POINTS ; ++i)
                    l[i] = stopTargetF->Loss(l[i], target->GetWidth(tUnit)/fabs(cos(Angle)), INTPOINTS);
            }
        }
        if (haveCfrag){
            for (int i = 0 ; i < POINTS ; ++i)
                m[i] = fragCustom->Loss(m[i], target->GetWidth(Material::Unit::mgcm2)/fabs(2*cos(Angle)), INTPOINTS);
        } else {
            for (int i = 0 ; i < POINTS ; ++i)
                m[i] = stopTargetF->Loss(m[i], target->GetWidth(tUnit)/fabs(2*cos(Angle)), INTPOINTS);
        }

        if (Angle > PI/2. && theFront->is_present){
            for (int i = 0 ; i < POINTS ; ++i){
                l[i] = stopFrontF->Loss(l[i], front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
                m[i] = stopFrontF->Loss(m[i], front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
                n[i] = stopFrontF->Loss(n[i], front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
            }
        } else if (theBack->is_present){
            for (int i = 0 ; i < POINTS ; ++i){
                l[i] = stopBack->Loss(l[i], INTPOINTS);
                m[i] = stopBack->Loss(m[i], INTPOINTS);
                n[i] = stopBack->Loss(n[i], INTPOINTS);
            }
        }

        if (theTelescope->has_absorber){
            for (int i = 0 ; i < POINTS ; ++i){
                l[i] = stopAbsor->Loss(l[i], INTPOINTS);
                m[i] = stopAbsor->Loss(m[i], INTPOINTS);
                n[i] = stopAbsor->Loss(n[i], INTPOINTS);
            }
        }

        for (int i = 0 ; i < POINTS ; ++i)
            E_err_tmp[i] = sqrt(3*l[i]*l[i] + 3*n[i]*n[i] + 4*m[i]*m[i] - 2*n[i]*l[i] -4*m[i]*(l[i] + n[i]))/4.;

        m = (l + 2*m + n)/4.;
        for (int i = 0 ; i < POINTS ; ++i){
            dm[i] = stopDE->Loss(m[i], INTPOINTS);
            em[i] = stopE->Loss(dm[i], INTPOINTS);

            dE_tmp[i] = m[i] - dm[i];
            E_tmp[i] = dm[i] - em[i];
            is_punch[i] = em[i];
            if (em[i] != em[i])
                is_punch[i] = 1000;
        }

        QVector<double> is_punch2, E_err;
        int not_punch = 0;
        for (int i = 0 ; i < Ex_tmp.size() ; ++i){
            if (E_tmp[i] >= 0.35){
                Ex.push_back(Ex_tmp[i]);
                dE.push_back(dE_tmp[i]);
                E.push_back(E_tmp[i]);
                is_punch2.push_back(is_punch[i]);
                E_err.push_back(E_err_tmp[i]);
                if (is_punch[i] <= 0.05)
                    not_punch += 1;
            }
        }

        if (not_punch >= 3){
            double *x = new double[not_punch];
            double *y = new double[not_punch];
            double *dx = new double[not_punch];
            int j = 0;
            for (int i = 0 ; i < is_punch2.size() ; ++i){
                if (is_punch2[i] <= 0.05 && j < not_punch){
                    x[j] = dE[i] + E[i];
                    y[j] = Ex[i];
                    dx[j] = E_err[i];
                    ++j;
                }
            }
            Polyfit fitting(x, y, not_punch);
            Vector fit = fitting(3);
            coeff = QVector<double>(4);
            coeff[0] = fit[0]; coeff[1] = fit[1]; coeff[2] = fit[2], coeff[3] = 0;

            for (int i = 0 ; i < not_punch ; ++i){
                coeff[3] += (y[i] - coeff[0] - coeff[1]*x[i] - coeff[2]*x[i]*x[i])*(y[i] - coeff[0] - coeff[1]*x[i] - coeff[2]*x[i]*x[i])/(dx[i]*dx[i]);
            }
            coeff[3] /= double(not_punch - 3);

            delete[] x;
            delete[] y;
            delete[] dx;
        }


        // Clearing up memory.

        // Particles.
        delete beam;
        delete scatIso;
        delete fragment;
        delete residual;

        // Materials.
        delete target;
        delete front;
        delete back;
        delete abs;
        delete dEdet;
        delete Edet;

        // Scattering calculator.
        delete scat;

        // Stopping power calculators.
        delete stopTargetB;
        delete stopTargetF;
        delete stopFrontB;
        delete stopFrontF;
        delete stopBack;
        delete stopAbsor;
        delete stopDE;
        delete stopE;

        // Done with the work :)
        return true;
}

bool Worker::Known(QVector<double> &Ex, QVector<double> &dE, QVector<double> &E, QVector<double> &delta_dE, QVector<double> &delta_E,
                   const double &Angle, const int &fA, const int &fZ)
{
    double incAngle;
    if (Angle > PI/2.)
        incAngle = PI - ANG_FWD - Angle;
    else
        incAngle = Angle - ANG_FWD;

    Particle *beam = new Particle(theBeam->Z, theBeam->A);
    Particle *scatIso = new Particle(theTarget->Z, theTarget->A);
    Particle *fragment = new Particle(fZ, fA);
    Particle *residual = new Particle(theBeam->Z+theTarget->Z-fZ, theBeam->A+theTarget->A-fA);

    Material *front = new Material(theFront->Z, theFront->A, theFront->width, Unit2MatUnit(theFront->unit));
    Material *target = new Material(theTarget->Z, theTarget->A, theTarget->width, Unit2MatUnit(theTarget->unit));
    Material *back = new Material(theBack->Z, theBack->A, theBack->width/fabs(cos(Angle)), Unit2MatUnit(theBack->unit));
    Material *abs = new Material(theTelescope->Absorber.Z, Get_mm2(theTelescope->Absorber.Z), theTelescope->Absorber.width/cos(incAngle), Unit2MatUnit(theTelescope->Absorber.unit));
    Material *dEdet = new Material(theTelescope->dEdetector.Z, Get_mm2(theTelescope->dEdetector.Z), theTelescope->dEdetector.width/cos(incAngle), Unit2MatUnit(theTelescope->dEdetector.unit));
    Material *Edet = new Material(theTelescope->Edetector.Z, Get_mm2(theTelescope->Edetector.Z), theTelescope->Edetector.width/cos(incAngle), Unit2MatUnit(theTelescope->Edetector.unit));

    RelScatter *scat = new RelScatter(beam, scatIso, fragment, residual);//new Iterative(beam, scatIso, fragment, residual);

    // Setting up stopping power for the target.
    StoppingPower *stopTargetB;
    StoppingPower *stopTargetF;
    Material::Unit tUnit;
    if (theTarget->Z > 92){
        stopTargetB = new BetheBlock(target, beam);
        stopTargetF = new BetheBlock(target, fragment);
        tUnit = Material::gcm2;
    } else {
        stopTargetB = new Ziegler1985(target, beam);
        stopTargetF = new Ziegler1985(target, fragment);
        tUnit = Material::um;
    }

    // Setting up stopping power for the fronting and backing.
    StoppingPower *stopFrontB;
    StoppingPower *stopFrontF;
    Material::Unit fUnit;
    if (theFront->Z > 92){
        stopFrontB = new BetheBlock(front, beam);
        stopFrontF = new BetheBlock(front, fragment);
        fUnit = Material::gcm2;
    } else {
        stopFrontB = new Ziegler1985(front, beam);
        stopFrontF = new Ziegler1985(front, fragment);
        fUnit = Material::um;
    }
    StoppingPower *stopBack;
    Material::Unit bUnit;
    if (theBack->Z > 92){
        stopBack = new BetheBlock(back, fragment);
        bUnit = Material::gcm2;
    } else {
        stopBack = new Ziegler1985(back, fragment);
        bUnit = Material::um;
    }

    // Setting up stopping power for the absorber.
    StoppingPower *stopAbsor;
    if (theTelescope->Absorber.Z > 92){
        stopAbsor = new BetheBlock(abs, fragment);
    } else {
        stopAbsor = new Ziegler1985(abs, fragment);
    }

    // Setting up stopping power for thin detector.
    StoppingPower *stopDE;
    if (theTelescope->dEdetector.Z > 92){
        stopDE = new BetheBlock(dEdet, fragment);
    } else {
        stopDE = new Ziegler1985(dEdet, fragment);
    }

    // Setting up stopping power for thick detector.
    StoppingPower *stopE;
    if (theTelescope->Edetector.Z > 92){
        stopE = new BetheBlock(Edet, fragment);
    } else {
        stopE = new Ziegler1985(Edet, fragment);
    }

    double E_beam = theBeam->E;
    if (theFront->is_present)
        E_beam = stopFrontB->Loss(E_beam, INTPOINTS);

    double Ehalf = stopTargetB->Loss(E_beam, target->GetWidth(tUnit)/2., INTPOINTS);
    double Ewhole = stopTargetB->Loss(E_beam, INTPOINTS);

    double Exmax = scat->FindMaxEx(Ewhole, Angle);

    if ((Ehalf + get_Q_keV(theBeam->A, theBeam->Z, theTarget->A, theTarget->Z, fA, fZ)/1000.)<0){
        // Particles.
        delete beam;
        delete scatIso;
        delete fragment;
        delete residual;
        
        // Materials.
        delete target;
        delete front;
        delete back;
        delete abs;
        delete dEdet;
        delete Edet;
        
        // Scattering calculator.
        delete scat;
        
        // Stopping power calculators.
        delete stopTargetB;
        delete stopTargetF;
        delete stopFrontB;
        delete stopFrontF;
        delete stopBack;
        delete stopAbsor;
        delete stopDE;
        delete stopE;
        return false; // Reaction not possible. Not enough energy :(
    }

    Excitation ex_data(residual->GetA(), residual->GetZ()); // Class to fetch known energy levels from file.

    QVector<double> tmp = ex_data.asVector();

    if (tmp.empty()){ // If empty, no data excists :(
        // Particles.
        delete beam;
        delete scatIso;
        delete fragment;
        delete residual;
        
        // Materials.
        delete target;
        delete front;
        delete back;
        delete abs;
        delete dEdet;
        delete Edet;
        
        // Scattering calculator.
        delete scat;
        
        // Stopping power calculators.
        delete stopTargetB;
        delete stopTargetF;
        delete stopFrontB;
        delete stopFrontF;
        delete stopBack;
        delete stopAbsor;
        delete stopDE;
        delete stopE;
        return false; // No known energy levels in the residual nucleus :(
    }
    QVector<double> Ex_tmp;

    // Only calculate points that are below the maximum possible excitation energy.
    for (int i = 0 ; i < tmp.size() ; ++i){
        if (tmp[i] <= Exmax){
            Ex_tmp.push_back(tmp[i]);
        }
    }
    if (Ex_tmp.empty()){
        // Particles.
        delete beam;
        delete scatIso;
        delete fragment;
        delete residual;
        
        // Materials.
        delete target;
        delete front;
        delete back;
        delete abs;
        delete dEdet;
        delete Edet;

        // Scattering calculator.
        delete scat;

        // Stopping power calculators.
        delete stopTargetB;
        delete stopTargetF;
        delete stopFrontB;
        delete stopFrontF;
        delete stopBack;
        delete stopAbsor;
        delete stopDE;
        delete stopE;
        return false; // No energy levels that can be used. :(
    }

    tmp.clear(); // We dont need this data anymore. Clearing up memory.

    QVector<double> dE_tmp(Ex_tmp.size());
    QVector<double> E_tmp(Ex_tmp.size());
    QVector<double> delta_dE_tmp(Ex_tmp.size());
    QVector<double> delta_E_tmp(Ex_tmp.size());

    double f, m, b;
    double df, dm, db;
    double ef, em, eb;

    for (int i = 0 ; i < Ex_tmp.size() ; ++i){

        f = scat->EvaluateY(E_beam, Angle, Ex_tmp[i]);
        m = scat->EvaluateY(Ehalf, Angle, Ex_tmp[i]);
        b = scat->EvaluateY(Ewhole, Angle, Ex_tmp[i]);

        if (Angle > PI/2.){
            b = stopTargetF->Loss(b, target->GetWidth(tUnit)/fabs(cos(Angle)), INTPOINTS);
        } else {
            f = stopTargetF->Loss(f, target->GetWidth(tUnit)/fabs(cos(Angle)), INTPOINTS);
        }

        m = stopTargetF->Loss(m, target->GetWidth(tUnit)/fabs(2*cos(Angle)), INTPOINTS);

        if (Angle > PI/2. && theFront->is_present){
            f = stopFrontF->Loss(f, front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
            m = stopFrontF->Loss(m, front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
            b = stopFrontF->Loss(b, front->GetWidth(fUnit)/fabs(cos(Angle)), INTPOINTS);
        } else if (theBack->is_present){
            f = stopBack->Loss(f, INTPOINTS);
            m = stopBack->Loss(m, INTPOINTS);
            b = stopBack->Loss(b, INTPOINTS);
        }

        if (theTelescope->has_absorber){
            f = stopAbsor->Loss(f, INTPOINTS);
            m = stopAbsor->Loss(m, INTPOINTS);
            b = stopAbsor->Loss(b, INTPOINTS);
        }

        df = stopDE->Loss(f, INTPOINTS);
        dm = stopDE->Loss(m, INTPOINTS);
        db = stopDE->Loss(b, INTPOINTS);

        ef = stopE->Loss(df, INTPOINTS);
        em = stopE->Loss(dm, INTPOINTS);
        eb = stopE->Loss(db, INTPOINTS);

        dE_tmp[i] = m - dm;
        delta_dE_tmp[i] = sqrt(0.5*((f-df - dE_tmp[i])*(f-df - dE_tmp[i]) + (b - db - dE_tmp[i])*(b - db - dE_tmp[i])));

        E_tmp[i] = dm - em;
        delta_E_tmp[i] = sqrt(0.5*((df - ef - E_tmp[i])*(df - ef - E_tmp[i]) + (db - eb - E_tmp[i])*(db - eb - E_tmp[i])));
    }

    // Making sure that there are no data left in the arrays.
    Ex.clear();
    dE.clear();
    E.clear();
    delta_dE.clear();
    delta_E.clear();

    // Keeping only the points that are useless to uncertain due to limitations in the way we calculate stopping power.
    for (int i = 0 ; i < Ex_tmp.size() ; ++i){
        if (E_tmp[i] > 0.35){
            Ex.push_back(Ex_tmp[i]);
            dE.push_back(dE_tmp[i]);
            E.push_back(E_tmp[i]);
            delta_dE.push_back(delta_dE_tmp[i]);
            delta_E.push_back(delta_E_tmp[i]);
        }
    }

    // Clearing up memory.
    Ex_tmp.clear(); dE_tmp.clear(); E_tmp.clear(); delta_dE_tmp.clear(); delta_E_tmp.clear();

    // Deleting stuff we dont need.

    // Particles.
    delete beam;
    delete scatIso;
    delete fragment;
    delete residual;

    // Materials.
    delete target;
    delete front;
    delete back;
    delete abs;
    delete dEdet;
    delete Edet;

    // Scattering calculator.
    delete scat;

    // Stopping power calculators.
    delete stopTargetB;
    delete stopTargetF;
    delete stopFrontB;
    delete stopFrontF;
    delete stopBack;
    delete stopAbsor;
    delete stopDE;
    delete stopE;

    return true; // Calculations successful :D

}
