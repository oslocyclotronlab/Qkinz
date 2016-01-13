#ifndef RUNDIALOG_H
#define RUNDIALOG_H

#include <QDialog>
#include <QThread>



#include "types.h"

class QMovie;

namespace Ui {
class RunDialog;
}

class RunDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunDialog(QWidget *parent = 0);
    ~RunDialog();

    void RunData(QVector<double> &Ex,
                 QVector<double> &dE,
                 QVector<double> &E,
                 QVector<double> &delta_dE,
                 QVector<double> &delta_E,
                 const double &Angle,
                 const int &fA,
                 const int &fZ);

    void setBeam(Beam_t *beam){ theBeam = beam; }
    void setTarget(Target_t *target){ theTarget = target; }
    void setTelescope(Telescope_t *telescope){ theTelescope = telescope; }


signals:
    void Finished();

public slots:
    void startMovie();

private:
    Ui::RunDialog *ui;

    Beam_t *theBeam;
    Target_t *theTarget;
    Telescope_t *theTelescope;

    QMovie *movie;

    QThread workThread;

};

#endif // RUNDIALOG_H
