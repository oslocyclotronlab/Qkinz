#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "qcustomplot.h"

#include "selectbeamform.h"
#include "selecttargetform.h"
#include "selecttelescopeform.h"
#include "selectfrontbackform.h"
#include "rundialog.h"

#include "types.h"
#include "worker.h"

#include "tablemakerhtml.h"

namespace Ui {
class MainWindow;
}
//! Class: MainWindow
/*! This is the main window of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Constructor.
    explicit MainWindow(QWidget *parent = 0 /*!< Parent widget. */);

    //! Destructor.
    ~MainWindow();

signals:
    void operate(const double &Angle, const bool &p, const bool &d, const bool &t, const bool &h3, const bool &a);

public slots:
    //! Slot for reciving curve data from the worker. It will plot the data for the
    //! recived data.
    void CurveData(const QVector<double> &x,       /*!< x-values.                                                      */
                   const QVector<double> &y,       /*!< y-values.                                                      */
                   const QVector<double> &coeff,   /*!< Coefficients for the fit Ex(x+y) = a0 + a1(x+y) + a2(x+y)^2    */
                   const Fragment_t &what          /*!< What fragment.                                                 */);

    //! Slot for reciving scatter graph data from the worker.
    void ScatterData(const QVector<double> &x,     /*!< x-values.                                      */
                     const QVector<double> &dx,    /*!< Error in x-values.                             */
                     const QVector<double> &y,     /*!< y-values.                                      */
                     const QVector<double> &dy,    /*!< Error in y-values.                             */
                     const QVector<double> &ex,    /*!< The excitation energy at each x and y point.   */
                     const Fragment_t &what        /*!< Indication what type of fragment.              */);

    //! Slot to indicate that the worker is finished with calculations.
    void WorkFinished();

private slots:
    //! Slot for showing the change beam dialog.
    inline void showBeam()
    {
        setBeam_form->setBeam(&theBeam);
        setBeam_form->Refresh();
        setBeam_form->show();
    }

    //! Slot for shoiwing the change target dialog.
    inline void showTarget()
    {
        setTarget_form->setTarget(&theTarget);
        setTarget_form->Refresh();
        setTarget_form->show();
    }

    //! Slot for showing the change front/back dialog.
    inline void showFB()
    {
        setFrontBack_form->setExtra(&theFront, &theBack);
        setFrontBack_form->Refresh();
        setFrontBack_form->show();
    }

    //! Slot for showing the change telescope dialog.
    inline void showTelescope()
    {
        setTelescope_form->setTelescope(&theTelescope);
        setTelescope_form->Refresh();
        setTelescope_form->show();
    }

    //! Perform the calculations with the parameters given by the user.
    void run();

    //! Refresh the view. Sets all the labels to the correct values.
    void Refresh();

    //! Resets all parameters and resets the labels.
    void Reset_All();

    //! For the plot system.
    void selectionChanged();

    //! For the plot system.
    void mousePress();

    //! For the plot system.
    void mouseWheel();

    //! Export the current plot view.
    void on_actionExport_plot_triggered();

    //! Export the current table.
    void on_actionExport_table_triggered();

    //! Save the current setup.
    void on_actionSave_triggered();

    //! Open a saved setup.
    void on_actionOpen_triggered();

    //! Show the about Qt dialog.
    void on_actionAbout_Qt_triggered();

    //! Show the about Qkinz dialog.
    void on_actionAbout_triggered();

    //! Show the about QCustomPlot dialog.
    void on_actionAbout_QCustomPlot_triggered();

private:
    //! The ui of the window.
    Ui::MainWindow *ui;

    //! Select Beam Form instance.
    SelectBeamForm *setBeam_form;

    //! Select Target Form instance.
    SelectTargetForm *setTarget_form;

    //! Select optional fronting - backing of the target.
    SelectFrontBackForm *setFrontBack_form;

    //! Select Telescope Form instance.
    SelectTelescopeForm *setTelescope_form;

    //! Dialog indicating that the system is running.
    RunDialog *runDialog;

    //! Class doing all the calculations.
    Worker *worker;

    //! Struct containing information about the beam.
    Beam_t theBeam;

    //! Structure containing information about the target.
    Target_t theTarget;

    //! Structure containing element in front of the target.
    Extra_t theFront;

    //! Structure containing element behind the target.
    Extra_t theBack;

    //! Structure containing information about the telescope.
    Telescope_t theTelescope;

    //! Thread to run the calculations.
    QThread workThread;

    //! Class making the tables.
    TableMakerHTML table;

    //! Function to remove all graphs from the plot.
    void RemoveAllGraphs();

    //! Create a QCPCurve from data points.
    /*! \return the pointer to the created curve.
     */
    QCPCurve *makeCurve(QVector<double> x,  /*!< x-values of the curve. */
                        QVector<double> y,  /*!< y-values of the curve. */
                        QPen pen,           /*!< Color of the plot.     */
                        QString label       /*!< Label of the curve.    */);
    //! Create a QCPGraph from data points.
    /*! \return a scatter graph with errorbars.
     */
    QCPGraph *makeGraph(QVector<double> x,  /*!< x-values.              */
                        QVector<double> dx, /*!< Error of the x-values. */
                        QVector<double> y,  /*!< y-values.              */
                        QVector<double> dy, /*!< Error of the y-values. */
                        QPen pen            /*!< Color of the points.   */);
};

#endif // MAINWINDOW_H
