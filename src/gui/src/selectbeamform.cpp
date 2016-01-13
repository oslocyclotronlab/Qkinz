#include "selectbeamform.h"
#include "ui_selectbeamform.h"

SelectBeamForm::SelectBeamForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectBeamForm)
{
    ui->setupUi(this);
    QDialog::setWindowFlags(Qt::Sheet);
}

SelectBeamForm::~SelectBeamForm()
{
    delete ui;
}

void SelectBeamForm::on_buttonBox_accepted()
{
    thisBeam->A = ui->massNumberBox->value();
    thisBeam->Z = ui->elementNumberBox->value();
    thisBeam->E = ui->beamEnergyBox->value();
    emit DoRefresh();

}

void SelectBeamForm::Refresh()
{
    ui->massNumberBox->setValue(thisBeam->A);
    ui->elementNumberBox->setValue(thisBeam->Z);
    ui->beamEnergyBox->setValue(thisBeam->E);
}
