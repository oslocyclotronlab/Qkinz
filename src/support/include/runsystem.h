#ifndef RUNSYSTEM_H
#define RUNSYSTEM_H

#include <memory>

class Particle;
class Material;

class RunSystem
{
public:
    //! Constructor.
    RunSystem(Particle *beam,       /*!< Incomming beam.        */
              Particle *scatIso,    /*!< Isotope scattered of.  */
              Material *target,     /*!< Target material.       */
              Particle *fragment,   /*!< Outgoing fragment.     */
              Material *absorber,   /*!< Absorber material.     */
              Material *dEdet,      /*!< dE detector material.  */
              Material *Edet        /*!< E detector material.   */);

    ~RunSystem();

    //! Run the model.
    /*! \return number of points created.
     */
    int Run(const double &Energy,   /*!< Current energy of the incomming beam.  */
            const double &Angle     /*!< Current angle of outgoing fragment.    */) const;

    //! Get excitation energies.
    /*! \return Excitation energy used to create the curve.
     */
    inline double *getEx(){ return Ex.release(); }

    //! Get deposited energy for the dE detector.
    /*! \return Energy deposited in the dE detector.
     */
    inline double *getdE_dep(){ return dE.release(); }

    //! Get uncertainty in energy deposited in the dE detector.
    /*! \return Uncertainty in energy deposited in dE detector.
     */
    inline double *getUdE_dep(){ return delta_dE.release(); }

    //! Get deposited energy for the dE detector.
    /*! \return Energy deposited in the E detector.
     */
    inline double *getE_dep(){ return E.release(); }

    //! Get uncertainty in energy deposited in E detector.
    /*! \return Uncertainty in energy deposited in E detector.
     */
    inline double *getUE_dep(){ return delta_E.release(); }

private:
    //! Variable to store the beam.
    Particle *beam;

    //! Variable to store the isotope scattered of.
    Particle *scatIso;

    //! Variable to store the target material.
    Material *target;

    //! Variable to store the fragment.
    Particle *fragment;

    //! Variable to store the residual isotope.
    Particle *residual;

    //! Variable to store the absorber material.
    Material *absorber;

    //! Variable to store the dE detector material.
    Material *dEmaterial;

    //! Variable to store the E detector material.
    Material *Ematerial;

    std::unique_ptr<double[]> Ex, ExK;
    std::unique_ptr<double[]> dE, dEK;
    std::unique_ptr<double[]> delta_dE;
    std::unique_ptr<double[]> E, EK;
    std::unique_ptr<double[]> delta_E;
};

#endif // RUNSYSTEM_H
