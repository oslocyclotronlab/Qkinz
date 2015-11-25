#include "rundialog.h"
#include "ui_rundialog.h"

#include "QMovie"

#include "Particle.h"
#include "Material.h"
#include "runsystem.h"

#include "ziegler1985_table.h"

#include "worker.h"

RunDialog::RunDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunDialog)
{
    QDialog::setWindowFlags(Qt::Sheet);
    ui->setupUi(this);
    connect(this, SIGNAL(Finished()), this, SLOT(close()));

    ui->label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    movie = new QMovie(":/media/giphy.gif");
    ui->label->setMovie(movie);
    ui->label->setScaledContents(true);
    connect(movie, SIGNAL(finished()), this, SLOT(startMovie()));
    movie->start();

}

RunDialog::~RunDialog()
{
    delete ui;
    delete movie;
}

void RunDialog::startMovie()
{
    movie->start();
}

void RunDialog::RunData(QVector<double> &Ex, QVector<double> &dE, QVector<double> &E,
                        QVector<double> &delta_dE, QVector<double> &delta_E, const double &Angle,
                        const int &fA, const int &fZ)
{
    show();
    movie->start();
    Particle *beam = new Particle(theBeam->Z, theBeam->A);
    Particle *scatIso = new Particle(theTarget->Z, theTarget->A);
    Material *target = new Material(theTarget->Z, theTarget->A, theTarget->width, Material::mgcm2);
    Particle *frag = new Particle(fZ, fA);
    
    Material *abs = new Material(theTelescope->Absorber.Z, Get_mm2(theTelescope->Absorber.Z), theTelescope->Absorber.width, Material::um);
    Material *dEdet = new Material(theTelescope->dEdetector.Z, Get_mm2(theTelescope->dEdetector.Z), theTelescope->dEdetector.width, Material::um);
    Material *Edet = new Material(theTelescope->Edetector.Z, Get_mm2(theTelescope->Edetector.Z), theTelescope->Edetector.width, Material::um);

    RunSystem sys(beam, scatIso, target, frag, abs, dEdet, Edet);
    int points = sys.Run(theBeam->E, Angle);

    double *ex = sys.getEx();
    double *de = sys.getdE_dep();
    double *delta_de = sys.getUdE_dep();
    double *e = sys.getE_dep();
    double *delta_e = sys.getUE_dep();

    Ex = QVector<double>(points);
    dE = QVector<double>(points);
    E = QVector<double>(points);
    delta_dE = QVector<double>(points);
    delta_E = QVector<double>(points);

    for (int i = 0 ; i < points ; ++i){
        Ex[i] = ex[i];
        dE[i] = de[i];
        E[i] = e[i];
        delta_dE[i] = delta_de[i];
        delta_E[i] = delta_e[i];
    }

    delete[] ex;
    delete[] de;
    delete[] e;
    delete[] delta_de;
    delete[] delta_e;
}
