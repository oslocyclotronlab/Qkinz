#include "selecttelescopeform.h"
#include "ui_selecttelescopeform.h"

SelectTelescopeForm::SelectTelescopeForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTelescopeForm)
{
    ui->setupUi(this);
    QDialog::setWindowFlags(Qt::Sheet);
}

SelectTelescopeForm::~SelectTelescopeForm()
{
    delete ui;
}

void SelectTelescopeForm::on_buttonBox_accepted()
{
    theTelescope->dEdetector.Z = ui->dEelement->value();
    theTelescope->dEdetector.width = ui->dEwidth->value();

    theTelescope->Edetector.Z = ui->Eelement->value();
    theTelescope->Edetector.width = ui->Ewidth->value();

    theTelescope->Absorber.Z = ui->AbsElement->value();
    theTelescope->Absorber.width = ui->AbsWidth->value();
    emit DoRefresh();
}

void SelectTelescopeForm::Refresh()
{
    ui->dEelement->setValue(theTelescope->dEdetector.Z);
    ui->dEwidth->setValue(theTelescope->dEdetector.width);
    ui->Eelement->setValue(theTelescope->Edetector.Z);
    ui->Ewidth->setValue(theTelescope->Edetector.width);
    ui->AbsElement->setValue(theTelescope->Absorber.Z);
    ui->AbsWidth->setValue(theTelescope->Absorber.width);
}
