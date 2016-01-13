#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ame2012_masses.h"
#include "tablemakerhtml.h"

#include <iostream>
#include <cmath>

#include <QWebFrame>
#include <QFile>

const double PI = acos(-1);

static QString Unit2String(const Unit_t &unit)
{
    if (unit == mgcm2)
        return "mg/cm^2";
    else if (unit == gcm2)
        return "g/cm2";
    else
        return "um";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , setBeam_form(new SelectBeamForm(this))
    , setTarget_form(new SelectTargetForm(this))
    , setFrontBack_form(new SelectFrontBackForm(this))
    , setTelescope_form(new SelectTelescopeForm(this))
    , runDialog(new RunDialog(this))
{
    ui->setupUi(this);

    worker = new Worker(&theBeam, &theTarget, &theFront, &theBack, &theTelescope);
    worker->moveToThread(&workThread);
    qRegisterMetaType<QVector<double> >("QVector<double>");
    qRegisterMetaType<Fragment_t>("Fragment_t");
    connect(&workThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::operate, worker, &Worker::Run);
    connect(worker, &Worker::ResultCurve, this, &MainWindow::CurveData);
    connect(worker, &Worker::ResultScatter, this, &MainWindow::ScatterData);
    connect(worker, &Worker::FinishedAll, this, &MainWindow::WorkFinished);
    workThread.start();

    ui->plotTab->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                 QCP::iSelectLegend | QCP::iSelectPlottables);

    ui->plotTab->xAxis->setLabel("E [MeV]");
    ui->plotTab->yAxis->setLabel("dE [MeV]");
    ui->plotTab->axisRect()->setupFullAxesBox();
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->plotTab->legend->setVisible(true);
    ui->plotTab->legend->setFont(legendFont);
    ui->plotTab->legend->setSelectedFont(legendFont);
    ui->plotTab->legend->setSelectableParts(QCPLegend::spItems);

    connect(ui->plotTab, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(ui->plotTab, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->plotTab, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    connect(ui->plotTab->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotTab->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotTab->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotTab->yAxis2, SLOT(setRange(QCPRange)));


    connect(ui->ChangeBeamButton, SIGNAL(clicked()), this, SLOT(showBeam()));
    connect(ui->ChangeTargetButton, SIGNAL(clicked()), this, SLOT(showTarget()));
    connect(ui->ChangeFBButton, SIGNAL(clicked()), this, SLOT(showFB()));
    connect(ui->ChangeTelescopeButton, SIGNAL(clicked()), this, SLOT(showTelescope()));

    connect(ui->RunButton, SIGNAL(clicked()), this, SLOT(run()));
    connect(ui->ResetButton, SIGNAL(clicked()), this, SLOT(Reset_All()));

    connect(setBeam_form, SIGNAL(DoRefresh()), this, SLOT(Refresh()));
    connect(setTarget_form, SIGNAL(DoRefresh()), this, SLOT(Refresh()));
    connect(setFrontBack_form, SIGNAL(DoRefresh()), this, SLOT(Refresh()));
    connect(setTelescope_form, SIGNAL(DoRefresh()), this, SLOT(Refresh()));

    connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(close()));

    // Setting default values. Should be done from saved file?
    theBeam.A = 1;
    theBeam.Z = 1;
    theBeam.E = 16.0;

    theTarget.A = 28;
    theTarget.Z = 14;
    theTarget.width = 4.0;
    theTarget.unit = mgcm2;

    theFront.is_present = false;
    theFront.A = 27;
    theFront.Z = 13;
    theFront.width = 0.5;
    theFront.unit = mgcm2;

    theBack.is_present = false;
    theBack.A = 27;
    theBack.Z = 13;
    theBack.width = 0.5;
    theBack.unit = mgcm2;

    theTelescope.dEdetector.Z = 14;
    theTelescope.dEdetector.width = 130.0;
    theTelescope.dEdetector.unit = um;
    theTelescope.Edetector.Z = 14;
    theTelescope.Edetector.width = 1550.0;
    theTelescope.Edetector.unit = um;
    theTelescope.has_absorber = true;
    theTelescope.Absorber.Z = 13;
    theTelescope.Absorber.width = 10.5;
    theTelescope.Absorber.unit = um;

    Refresh();
}

void MainWindow::Reset_All()
{
    theBeam.A = 1;
    theBeam.Z = 1;
    theBeam.E = 16.0;

    theTarget.A = 28;
    theTarget.Z = 14;
    theTarget.width = 4.0;
    theTarget.unit = mgcm2;

    theFront.is_present = false;
    theFront.A = 27;
    theFront.Z = 13;
    theFront.width = 0.5;
    theFront.unit = mgcm2;

    theBack.is_present = false;
    theBack.A = 27;
    theBack.Z = 13;
    theBack.width = 0.5;
    theBack.unit = mgcm2;

    theTelescope.dEdetector.Z = 14;
    theTelescope.dEdetector.width = 130.0;
    theTelescope.dEdetector.unit = um;
    theTelescope.Edetector.Z = 14;
    theTelescope.Edetector.width = 1550.0;
    theTelescope.Edetector.unit = um;
    theTelescope.has_absorber = true;
    theTelescope.Absorber.Z = 13;
    theTelescope.Absorber.width = 10.5;
    theTelescope.Absorber.unit = um;

    RemoveAllGraphs();
    Refresh();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete setBeam_form;
    delete setTarget_form;
    delete setTelescope_form;
    workThread.quit();
    workThread.wait();
}


void MainWindow::Refresh()
{
    std::string bName;
    if (theBeam.Z == 1){
        if (theBeam.A == 1)
            bName = "p";
        else if (theBeam.A == 2)
            bName = "d";
        else if (theBeam.A == 3)
            bName = "t";
        else
            bName = "?";
    } else if (theBeam.Z == 2 && theBeam.A == 2){
        bName = "\u03B1";
    } else {
        bName = std::string(get_element_name(theBeam.Z)) + "-" + std::to_string(theBeam.A);
    }
    std::string bEnergy = std::to_string(theBeam.E);
    ui->CurrentBeam->setText(bName.c_str());
    ui->CurrentEnergy->setText(bEnergy.c_str());

    std::string tName = std::to_string(theTarget.A) + std::string(get_element_name(theTarget.Z));
    std::string tWidth = std::to_string(theTarget.width);
    ui->CurrentTarget->setText(tName.c_str());
    ui->CurrentThickness->setText(tWidth.c_str());
    ui->TargetUnitLabel->setText(Unit2String(theTarget.unit));


    if (theFront.is_present){
        ui->CurrentFront->setText(std::string(std::to_string(theFront.A) + std::string(get_element_name(theFront.Z))).c_str());
        ui->FrontWidth->setText(std::to_string(theFront.width).c_str());
        ui->FrontUnitLabel->setText(Unit2String(theFront.unit));
    } else {
        ui->CurrentFront->setText("-");
        ui->FrontWidth->setText("-");
    }
    if (theBack.is_present){
        ui->CurrentBack->setText(std::string(std::to_string(theBack.A) + std::string(get_element_name(theBack.Z))).c_str());
        ui->BackWidth->setText(std::to_string(theBack.width).c_str());
        ui->BackUnitLabel->setText(Unit2String(theBack.unit));
    } else {
        ui->CurrentBack->setText("-");
        ui->BackWidth->setText("-");
    }

    ui->dEMatSym->setText(get_element_name(theTelescope.dEdetector.Z));
    ui->CurrentDE->setText(std::to_string(theTelescope.dEdetector.width).c_str());
    ui->dEdetUnitLabel->setText(Unit2String(theTelescope.dEdetector.unit));
    ui->EMatSym->setText(get_element_name(theTelescope.Edetector.Z));
    ui->CurrentE->setText(std::to_string(theTelescope.Edetector.width).c_str());
    ui->EdetUnitLabel->setText(Unit2String(theTelescope.Edetector.unit));
    if (theTelescope.has_absorber){
        ui->AMatSym->setText(get_element_name(theTelescope.Absorber.Z));
        ui->CurrentA->setText(std::to_string(theTelescope.Absorber.width).c_str());
        ui->AbsUnitLabel->setText(Unit2String(theTelescope.Absorber.unit));
    } else {
        ui->AMatSym->setText("-");
        ui->CurrentA->setText("-");
    }

}

void MainWindow::CurveData(const QVector<double> &x, const QVector<double> &y, const QVector<double> &coeff, const Fragment_t &what)
{
    QString legend = QString("%1(%2,").arg(ui->CurrentTarget->text()).arg(ui->CurrentBeam->text());
    if (what == Proton){
        int Zres = theBeam.Z + theTarget.Z - 1;
        int Ares = theBeam.A + theTarget.A - 1;
        QString outP = std::string(std::to_string(Ares) + get_element_name(Zres)).c_str();
        if (theBeam.A == 1 && theBeam.Z == 1){
            legend = QString("%1%2)%3").arg(legend).arg("p'").arg(outP);
        } else {
            legend = QString("%1%2)%3").arg(legend).arg("p").arg(outP);
        }
        ui->plotTab->addPlottable(makeCurve(x, y, QPen(QColor(255,0,0)), legend));
        table.setCoeff(coeff, TableMakerHTML::Proton);
    } else if (what == Deutron){
        int Zres = theBeam.Z + theTarget.Z - 1;
        int Ares = theBeam.A + theTarget.A - 2;
        QString outP = std::string(std::to_string(Ares) + get_element_name(Zres)).c_str();
        if (theBeam.A == 2 && theBeam.Z == 1){
            legend = QString("%1%2)%3").arg(legend).arg("d'").arg(outP);
        } else {
            legend = QString("%1%2)%3").arg(legend).arg("d").arg(outP);
        }
        ui->plotTab->addPlottable(makeCurve(x, y, QPen(QColor(0,255,0)), legend));
        table.setCoeff(coeff, TableMakerHTML::Deutron);
    } else if (what == Triton){
        int Zres = theBeam.Z + theTarget.Z - 1;
        int Ares = theBeam.A + theTarget.A - 3;
        QString outP = std::string(std::to_string(Ares) + get_element_name(Zres)).c_str();
        if (theBeam.A == 3 && theBeam.Z == 1){
            legend = QString("%1%2)%3").arg(legend).arg("t'").arg(outP);
        } else {
            legend = QString("%1%2)%3").arg(legend).arg("t").arg(outP);
        }
        ui->plotTab->addPlottable(makeCurve(x, y, QPen(QColor(128,128,128)), legend));
        table.setCoeff(coeff, TableMakerHTML::Triton);
    } else if (what == Helium3){
        int Zres = theBeam.Z + theTarget.Z - 2;
        int Ares = theBeam.A + theTarget.A - 3;
        QString outP = std::string(std::to_string(Ares) + get_element_name(Zres)).c_str();
        if (theBeam.A == 3 && theBeam.Z == 2){
            legend = QString("%1%2)%3").arg(legend).arg("He-3'").arg(outP);
        } else {
            legend = QString("%1%2)%3").arg(legend).arg("He-3").arg(outP);
        }
        ui->plotTab->addPlottable(makeCurve(x, y, QPen(QColor(0,0,255)), legend));
        table.setCoeff(coeff, TableMakerHTML::Helium3);
    } else if (what == Alpha){
        int Zres = theBeam.Z + theTarget.Z - 2;
        int Ares = theBeam.A + theTarget.A - 4;
        QString outP = std::string(std::to_string(Ares) + get_element_name(Zres)).c_str();
        if (theBeam.A == 4 && theBeam.Z == 2){
            legend = QString("%1%2)%3").arg(legend).arg("α'").arg(outP);
        } else {
            legend = QString("%1%2)%3").arg(legend).arg("α").arg(outP);
        }
        ui->plotTab->addPlottable(makeCurve(x, y, QPen(QColor(0,0,0)), legend));
        table.setCoeff(coeff, TableMakerHTML::Alpha);
    }
    ui->plotTab->replot();
    ui->plotTab->rescaleAxes();
    ui->plotTab->show();
}

void MainWindow::ScatterData(const QVector<double> &x, const QVector<double> &dx, const QVector<double> &y,
                             const QVector<double> &dy, const QVector<double> &ex, const Fragment_t &what)
{
    if (what == Proton){
        ui->plotTab->addPlottable(makeGraph(x, dx, y, dy, QPen(QColor(255,0,0))));
        ui->plotTab->plottable()->removeFromLegend();
        table.setData(ex, y, dy, x, dx, TableMakerHTML::Proton);
    } else if (what == Deutron){
        ui->plotTab->addPlottable(makeGraph(x, dx, y, dy, QPen(QColor(0,255,0))));
        ui->plotTab->plottable()->removeFromLegend();
        table.setData(ex, y, dy, x, dx, TableMakerHTML::Deutron);
    } else if (what == Triton){
        ui->plotTab->addPlottable(makeGraph(x, dx, y, dy, QPen(QColor(128,128,128))));
        ui->plotTab->plottable()->removeFromLegend();
        table.setData(ex, y, dy, x, dx, TableMakerHTML::Triton);
    } else if (what == Helium3){
        ui->plotTab->addPlottable(makeGraph(x, dx, y, dy, QPen(QColor(0,0,255))));
        ui->plotTab->plottable()->removeFromLegend();
        table.setData(ex, y, dy, x, dx, TableMakerHTML::Helium3);
    } else if (what == Alpha){
        ui->plotTab->addPlottable(makeGraph(x, dx, y, dy, QPen(QColor(0,0,0))));
        ui->plotTab->plottable()->removeFromLegend();
        table.setData(ex, y, dy, x, dx, TableMakerHTML::Alpha);
    }
    ui->plotTab->replot();
    ui->plotTab->rescaleAxes();
    ui->plotTab->show();
}

void MainWindow::run()
{
    runDialog->show();

    RemoveAllGraphs();
    table.Reset();

    double angle = (ui->StripNumbr->value()*2 + 40)*PI/180;
    if (ui->BwdAngRButton){
        angle = PI - angle;
    }

    emit operate(angle, ui->protons->isChecked(), ui->deutrons->isChecked(), ui->tritons->isChecked(), ui->He3s->isChecked(), ui->alphas->isChecked());

    /*
    if (ui->protons->isChecked()){
        if (worker->Run(Ex, dE, E, coeff, angle, 1, 1)){
            QVector<double> x, y, ex;
            for (int i = 0 ; i < dE.size() ; ++i){
                if (E[i] >= 0.05){
                    ex.push_back(Ex[i]);
                    x.push_back(E[i]);
                    y.push_back(dE[i]);
                }
            }
            table.setCoeff(coeff, TableMakerHTML::Proton);
            QCPCurve *nCurve = makeCurve(x, y, QPen(QColor(255,0,0)), QString("%1%2)").arg(legend).arg("p"));
            ui->plotTab->addPlottable(nCurve);
        }
        if (worker->Known(Ex, dE, E, d_dE, d_E, angle, 1, 1)){
            table.setData(Ex, dE, d_dE, E, d_E, TableMakerHTML::Proton);
            QCPGraph *nGraph = makeGraph(E, d_E, dE, d_dE, QPen(QColor(255,0,0)));
            ui->plotTab->addPlottable(nGraph);
            ui->plotTab->plottable()->removeFromLegend();
        }
        ui->plotTab->replot();
        ui->plotTab->show();

    }
    if (ui->deutrons->isChecked()){
        if (worker->Run(Ex, dE, E, coeff, angle, 2, 1)){
            QVector<double> x, y, ex;
            for (int i = 0 ; i < dE.size() ; ++i){
                if (E[i] >= 0.05){
                    ex.push_back(Ex[i]);
                    x.push_back(E[i]);
                    y.push_back(dE[i]);
                }
            }
            table.setCoeff(coeff, TableMakerHTML::Deutron);
            QCPCurve *nCurve = makeCurve(x, y, QPen(QColor(0,255,0)), QString("%1%2)").arg(legend).arg("d"));
            ui->plotTab->addPlottable(nCurve);
        }
        if (worker->Known(Ex, dE, E, d_dE, d_E, angle, 2, 1)){
            table.setData(Ex, dE, d_dE, E, d_E, TableMakerHTML::Deutron);
            QCPGraph *nGraph = makeGraph(E, d_E, dE, d_dE, QPen(QColor(0,255,0)));
            ui->plotTab->addPlottable(nGraph);
            ui->plotTab->plottable()->removeFromLegend();
        }
        ui->plotTab->replot();
        ui->plotTab->show();

    }
    if (ui->tritons->isChecked()){
        if (worker->Run(Ex, dE, E, coeff, angle, 3, 1)){
            QVector<double> x, y, ex;
            for (int i = 0 ; i < dE.size() ; ++i){
                if (E[i] >= 0.05){
                    ex.push_back(Ex[i]);
                    x.push_back(E[i]);
                    y.push_back(dE[i]);
                }
            }
            table.setCoeff(coeff, TableMakerHTML::Triton);
            QCPCurve *nCurve = makeCurve(x, y, QPen(QColor(128,128,128)), QString("%1%2)").arg(legend).arg("t"));
            ui->plotTab->addPlottable(nCurve);
        }
        if (worker->Known(Ex, dE, E, d_dE, d_E, angle, 3, 1)){
            table.setData(Ex, dE, d_dE, E, d_E, TableMakerHTML::Triton);
            QCPGraph *nGraph = makeGraph(E, d_E, dE, d_dE, QPen(QColor(128,128,128)));
            ui->plotTab->addPlottable(nGraph);
            ui->plotTab->plottable()->removeFromLegend();
        }
        ui->plotTab->replot();
        ui->plotTab->show();

    }
    if (ui->He3s->isChecked()){
        if (worker->Run(Ex, dE, E, coeff, angle, 3, 2)){
            QVector<double> x, y, ex;
            for (int i = 0 ; i < dE.size() ; ++i){
                if (E[i] >= 0.05){
                    ex.push_back(Ex[i]);
                    x.push_back(E[i]);
                    y.push_back(dE[i]);
                }
            }
            table.setCoeff(coeff, TableMakerHTML::Helium3);
            QCPCurve *nCurve = makeCurve(x, y, QPen(QColor(0,0,255)), QString("%1%2)").arg(legend).arg("He3"));
            ui->plotTab->addPlottable(nCurve);
        }
        if (worker->Known(Ex, dE, E, d_dE, d_E, angle, 3, 2)){
            table.setData(Ex, dE, d_dE, E, d_E, TableMakerHTML::Helium3);
            QCPGraph *nGraph = makeGraph(E, d_E, dE, d_dE, QPen(QColor(0,0,255)));
            ui->plotTab->addPlottable(nGraph);
            ui->plotTab->plottable()->removeFromLegend();
        }

        ui->plotTab->replot();
        ui->plotTab->show();
    }
    if (ui->alphas->isChecked()){
        if (worker->Run(Ex, dE, E, coeff, angle, 4, 2)){
            QVector<double> x, y, ex;
            for (int i = 0 ; i < dE.size() ; ++i){
                if (E[i] >= 0.05){
                    ex.push_back(Ex[i]);
                    x.push_back(E[i]);
                    y.push_back(dE[i]);
                }
            }
            table.setCoeff(coeff, TableMakerHTML::Alpha);
            QCPCurve *nCurve = makeCurve(x, y, QPen(QColor(0,0,0)), QString("%1%2)").arg(legend).arg("α"));
            ui->plotTab->addPlottable(nCurve);
        }
        if (worker->Known(Ex, dE, E, d_dE, d_E, angle, 4, 2)){
            table.setData(Ex, dE, d_dE, E, d_E, TableMakerHTML::Alpha);
            QCPGraph *nGraph = makeGraph(E, d_E, dE, d_dE, QPen(QColor(0,0,0)));
            ui->plotTab->addPlottable(nGraph);
            ui->plotTab->plottable()->removeFromLegend();
        }
        ui->plotTab->replot();
        ui->plotTab->show();
    }
    ui->webView->setHtml(table.getHTMLCode());
    ui->plotTab->rescaleAxes();
    emit runDialog->Finished();
    */
}

void MainWindow::WorkFinished()
{
    ui->webView->setHtml(table.getHTMLCode());
    emit runDialog->Finished();
}

void MainWindow::selectionChanged()
{
    if (ui->plotTab->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->plotTab->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        ui->plotTab->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->plotTab->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels)){
        ui->plotTab->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->plotTab->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    if (ui->plotTab->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->plotTab->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        ui->plotTab->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->plotTab->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels)){
        ui->plotTab->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->plotTab->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
}

void MainWindow::mousePress()
{
    if (ui->plotTab->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->plotTab->axisRect()->setRangeDrag(ui->plotTab->xAxis->orientation());
    else if (ui->plotTab->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->plotTab->axisRect()->setRangeDrag(ui->plotTab->yAxis->orientation());
    else
        ui->plotTab->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
    if (ui->plotTab->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
       ui->plotTab->axisRect()->setRangeZoom(ui->plotTab->xAxis->orientation());
     else if (ui->plotTab->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
       ui->plotTab->axisRect()->setRangeZoom(ui->plotTab->yAxis->orientation());
     else
       ui->plotTab->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::RemoveAllGraphs()
{
    ui->plotTab->clearPlottables();
    ui->plotTab->replot();
}

QCPCurve *MainWindow::makeCurve(QVector<double> x, QVector<double> y, QPen pen, QString label)
{
    QCPCurve *nCurve = new QCPCurve(ui->plotTab->xAxis, ui->plotTab->yAxis);

    nCurve->setName(label);
    nCurve->setPen(pen);
    nCurve->setData(x, y);
    nCurve->setLineStyle(QCPCurve::lsLine);

    return nCurve;
}

QCPGraph *MainWindow::makeGraph(QVector<double> x, QVector<double> dx, QVector<double> y, QVector<double> dy, QPen pen)
{
    QCPGraph *nGraph = new QCPGraph(ui->plotTab->xAxis, ui->plotTab->yAxis);

    nGraph->removeFromLegend();
    nGraph->setDataBothError(x, y, dx, dy);
    nGraph->setPen(pen);
    nGraph->setLineStyle(QCPGraph::lsNone);
    nGraph->setScatterStyle(QCPScatterStyle::ssDisc);
    nGraph->setErrorType(QCPGraph::etBoth);

    return nGraph;
}

void MainWindow::on_actionExport_plot_triggered()
{
    QString Filters("PDF (*.pdf);;Image (*.png);;Image (*.jpg);;Bitmap image (*.bmp)");
    QString DefaultFilter("PDF (*.pdf)");
    QFileDialog *SaveFigDialog = new QFileDialog(this);

    QString FilePath = SaveFigDialog->getSaveFileName(this, "Save figure", QDir::homePath(), Filters, &DefaultFilter);
    if (FilePath.endsWith(".pdf", Qt::CaseInsensitive))
        ui->plotTab->savePdf(FilePath);
    else if (FilePath.endsWith(".png", Qt::CaseInsensitive))
        ui->plotTab->savePng(FilePath);
    else if (FilePath.endsWith(".jpg", Qt::CaseInsensitive))
        ui->plotTab->saveJpg(FilePath);
    else if (FilePath.endsWith(".bmp", Qt::CaseInsensitive))
        ui->plotTab->saveBmp(FilePath);
    delete SaveFigDialog;
}

void MainWindow::on_actionExport_table_triggered()
{
    QString Filters("PDF (*.pdf);;HTML (*.html);;Text file (*.txt)");
    QString DefaultFilter("PDF (*.pdf)");
    QFileDialog *SaveTabDialog = new QFileDialog(this);

    QString FilePath = SaveTabDialog->getSaveFileName(this, "Save table", QDir::homePath(), Filters, &DefaultFilter);
    if (FilePath.endsWith("pdf", Qt::CaseInsensitive)){
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFileName(FilePath);
        ui->webView->print(&printer);
    } else if (FilePath.endsWith("html", Qt::CaseInsensitive)){
        QString htmlCode = ui->webView->page()->currentFrame()->toHtml();
        QFile file(FilePath);
        if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream out(&file);
            out << htmlCode;
        }
        file.close();
    } else if (FilePath.endsWith("txt", Qt::CaseInsensitive)){
        QString txt = table.getTXT();
        QFile file(FilePath);
        if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream out(&file);
            out << txt;
        }
        file.close();
    }

    delete SaveTabDialog;
}

void MainWindow::on_actionSave_triggered()
{
    QString Filters("Qkinz file (.qkz)");
    QFileDialog *SaveSettingDialog = new QFileDialog(this);

    QString FilePath = SaveSettingDialog->getSaveFileName(this, "Save setup", QDir::homePath(), Filters, &Filters);
    if (!FilePath.isEmpty()){
        if (!FilePath.endsWith(".qkz"))
            FilePath += ".qkz";
        FILE *outFile = std::fopen(FilePath.toStdString().c_str(), "wb");
        fwrite(&theBeam, sizeof(Beam_t), 1, outFile);
        fwrite(&theTarget, sizeof(Target_t), 1, outFile);
        fwrite(&theTelescope, sizeof(Telescope_t), 1, outFile);
        std::fclose(outFile);
    }

    delete SaveSettingDialog;
}

void MainWindow::on_actionOpen_triggered()
{
    QString Filters("Qkinz file (.qkz)");
    QFileDialog *OpenSettingDialog = new QFileDialog(this);

    QString FilePath = OpenSettingDialog->getOpenFileName(this, "Open setup", QDir::homePath());//, Filters, &Filters);
    if (!FilePath.isEmpty()){
        FILE *inFile = std::fopen(FilePath.toStdString().c_str(), "rb");
        fread(&theBeam, sizeof(Beam_t), 1, inFile);
        fread(&theTarget, sizeof(Target_t), 1, inFile);
        fread(&theTelescope, sizeof(Telescope_t), 1, inFile);
        std::fclose(inFile);
    }

    Refresh();
    delete OpenSettingDialog;
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionAbout_triggered()
{
    QFile file(":/text/about.html");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream ts(&file);
    ts.setCodec("UTF-8");

    QMessageBox::about(this, tr("About Qkinz"), ts.readAll());
    file.close();

}

void MainWindow::on_actionAbout_QCustomPlot_triggered()
{
    QFile file(":/text/about_qcustomplot.html");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream ts(&file);
    ts.setCodec("UTF-8");
    QMessageBox::about(this, tr("About QCustomPlot 1.30"), ts.readAll());
    file.close();
}
