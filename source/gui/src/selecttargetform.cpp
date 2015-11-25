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
    emit DoRefresh();
}

void SelectTargetForm::Refresh()
{
    ui->MassNumb->setValue(theTarget->A);
    ui->ElemNumb->setValue(theTarget->Z);
    ui->Width->setValue(theTarget->width);
}
