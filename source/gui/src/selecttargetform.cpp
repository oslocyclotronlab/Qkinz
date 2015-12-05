#include "selecttargetform.h"
#include "ui_selecttargetform.h"

#include <QMessageBox>

SelectTargetForm::SelectTargetForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTargetForm)
{
    ui->setupUi(this);
    QDialog::setWindowFlags(Qt::Sheet);
}

SelectTargetForm::~SelectTargetForm()
{
    delete ui;
}

void SelectTargetForm::on_buttonBox_accepted()
{
    theTarget->A = ui->MassNumb->value();
    theTarget->Z = ui->ElemNumb->value();
    theTarget->width = ui->Width->value();

    if (ui->mgcm2Button->isChecked())
        theTarget->unit = mgcm2;
    else if (ui->gcm2Button->isChecked())
        theTarget->unit = gcm2;
    else if (ui->umButton->isChecked())
        theTarget->unit = um;
    emit DoRefresh();
}

void SelectTargetForm::Refresh()
{
    ui->MassNumb->setValue(theTarget->A);
    ui->ElemNumb->setValue(theTarget->Z);
    ui->Width->setValue(theTarget->width);
    if (theTarget->unit == mgcm2){
        ui->mgcm2Button->setChecked(true);
        ui->gcm2Button->setChecked(false);
        ui->umButton->setChecked(false);
    } else if (theTarget->unit == gcm2){
        ui->mgcm2Button->setChecked(false);
        ui->gcm2Button->setChecked(true);
        ui->umButton->setChecked(false);
    } else if (theTarget->unit == um){
        ui->mgcm2Button->setChecked(false);
        ui->gcm2Button->setChecked(false);
        ui->umButton->setChecked(true);
    }
}
