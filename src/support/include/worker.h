#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

#include "types.h"

//! Worker class.
//! This class handles all of the actual calls to other object that performs the calculations.
class Worker : public QObject
{
    Q_OBJECT
public:

    //! Constructor.
    Worker(Beam_t *beam,            /*!< Current beam settings.             */
           Target_t *target,        /*!< Current target settings.           */
           Extra_t *front,          /*!< Current target fronting settings.  */
           Extra_t *back,           /*!< Current target backing settings.   */
           Telescope_t *telescope   /*!< Current telescope settings.        */);

public slots:

    //! Slot to indicate that the class have to perform the calculations.
    void Run(const double &Angle,   /*!< Scattering angle.                  */
             const bool &p,         /*!< Calculate for proton fragment.     */
             const bool &d,         /*!< Calculate for deutron fragment.    */
             const bool &t,         /*!< Calculate for triton fragment.     */
             const bool &h3,        /*!< Calculate for Helium-3 fragment.   */
             const bool &a          /*!< Calculate for alpha fragment.      */);

signals:

    //! Emitting the result of calculations from \see Curve.
    void ResultCurve(const QVector<double> &x,      /*!< The x-value result.    */
                     const QVector<double> &y,      /*!< The y-value result.    */
                     const QVector<double> &coeff,  /*!< Calculated fit.        */
                     const Fragment_t &what         /*!< For what fragment.     */);

    //! Emitting the result of calculations for discrete points, \see Known.
    void ResultScatter(const QVector<double> &x,    /*!< The x-value result.        */
                       const QVector<double> &dx,   /*!< Uncertainty in x-value.    */
                       const QVector<double> &y,    /*!< The y-value result.        */
                       const QVector<double> &dy,   /*!< Unvertainty in y-value.    */
                       const QVector<double> &ex,   /*!< Excitation energy.         */
                       const Fragment_t &what       /*!< Indicating the fragment.   */);

    //! Signal emitted when all calculations are complete.
    void FinishedAll();

private:
    //! The current beam.
    Beam_t *theBeam;

    //! The current target.
    Target_t *theTarget;

    //! Fronting of the target.
    Extra_t *theFront;

    //! Backing of the target.
    Extra_t *theBack;

    //! The current telescope.
    Telescope_t *theTelescope;

    //! Function to calculate using continious excitation energy.
    /*! \return true if reaction possible. false otherwise.
     */
    bool Curve(QVector<double> &Ex,     /*!< Excitation energy.                     */
               QVector<double> &dE,     /*!< Energy deposited in thin detector.     */
               QVector<double> &E,      /*!< Energy deposited in thick detector.    */
               QVector<double> &coeff,  /*!< Fit of the data.                       */
               const double &Angle,     /*!< Scattering angle.                      */
               const int &fA,           /*!< Mass number of the fragment.           */
               const int &fZ            /*!< Element number of the framgent.        */);

    //! Function to calculate using known states in the residual nucleus.
    /*! \return true if reaction possible. false otherwise.
     */
    bool Known(QVector<double> &Ex,         /*!< Excitation energy.             */
               QVector<double> &dE,         /*!< Energy in thin detector.       */
               QVector<double> &E,          /*!< Energy in thick detector.      */
               QVector<double> &delta_dE,   /*!< Uncertainty in thin detector.  */
               QVector<double> &delta_E,    /*!< Uncertainty in thick detector. */
               const double &Angle,         /*!< Scattering angle.              */
               const int &fA,               /*!< Mass number of fragment.       */
               const int &fZ                /*!< Element number of fragment.    */);

};


#endif // WORKER_H
