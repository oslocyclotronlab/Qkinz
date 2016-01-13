#include "selecttelescopeform.h"
#include "ui_selecttelescopeform.h"

static Unit_t Text2Unit(const QString &str)
{
    if (str == "um")
        return um;
    else if (str == "g/cm2")
        return gcm2;
    else if (str == "mg/cm2")
        return mgcm2;
    else
        return um;
}

static int Unit2Int(const Unit_t &unit)
{
    if (unit == um)
        return 0;
    else if (unit == gcm2)
        return 1;
    else if (unit == mgcm2)
        return 2;
    else
        return 0;
}

SelectTelescopeForm::SelectTelescopeForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTelescopeForm)
{
    ui->setupUi(this);
    QDialog::setWindowFlags(Qt::Sheet);

    connect(ui->absPres, SIGNAL(toggled(bool)), ui->AbsElement, SLOT(setEnabled(bool)));
    connect(ui->absPres, SIGNAL(toggled(bool)), ui->AbsWidth, SLOT(setEnabled(bool)));
    connect(ui->absPres, SIGNAL(toggled(bool)), ui->AbsUnit, SLOT(setEnabled(bool)));
}

SelectTelescopeForm::~SelectTelescopeForm()
{
    delete ui;
}

void SelectTelescopeForm::on_buttonBox_accepted()
{
    theTelescope->dEdetector.Z = ui->dEelement->value();
    theTelescope->dEdetector.width = ui->dEwidth->value();
    theTelescope->dEdetector.unit = Text2Unit(ui->dEUnit->currentText());

    theTelescope->Edetector.Z = ui->Eelement->value();
    theTelescope->Edetector.width = ui->Ewidth->value();
    theTelescope->Edetector.unit = Text2Unit(ui->EUnit->currentText());

    theTelescope->has_absorber = ui->absPres->isChecked();
    theTelescope->Absorber.Z = ui->AbsElement->value();
    theTelescope->Absorber.width = ui->AbsWidth->value();
    theTelescope->Absorber.unit = Text2Unit(ui->AbsUnit->currentText());

    emit DoRefresh();
}

void SelectTelescopeForm::Refresh()
{
    ui->dEelement->setValue(theTelescope->dEdetector.Z);
    ui->dEwidth->setValue(theTelescope->dEdetector.width);
    ui->dEUnit->setCurrentIndex(Unit2Int(theTelescope->dEdetector.unit));
    ui->Eelement->setValue(theTelescope->Edetector.Z);
    ui->Ewidth->setValue(theTelescope->Edetector.width);
    ui->EUnit->setCurrentIndex(Unit2Int(theTelescope->Edetector.unit));

    ui->absPres->setChecked(theTelescope->has_absorber);
    emit ui->absPres->toggled(theTelescope->has_absorber);
    ui->AbsElement->setValue(theTelescope->Absorber.Z);
    ui->AbsWidth->setValue(theTelescope->Absorber.width);
    ui->AbsUnit->setCurrentIndex(Unit2Int(theTelescope->Absorber.unit));
}
