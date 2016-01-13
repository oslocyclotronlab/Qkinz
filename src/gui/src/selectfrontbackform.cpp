#include "selectfrontbackform.h"
#include "ui_selectfrontbackform.h"

static Unit_t Text2Unit(const QString &str)
{
    if (str == "mg/cm2")
        return mgcm2;
    else if (str == "g/cm2")
        return gcm2;
    else if (str == "um")
        return um;
    else
        return mgcm2;
}

static int Unit2Int(const Unit_t &unit)
{
    if (unit == mgcm2)
        return 0;
    else if (unit == gcm2)
        return 1;
    else if (unit == um)
        return 2;
    else
        return 0;
}

SelectFrontBackForm::SelectFrontBackForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFrontBackForm)
{
    ui->setupUi(this);
    QDialog::setWindowFlags(Qt::Sheet);
    connect(ui->frontPres, SIGNAL(toggled(bool)), ui->frontA, SLOT(setEnabled(bool)));
    connect(ui->frontPres, SIGNAL(toggled(bool)), ui->frontZ, SLOT(setEnabled(bool)));
    connect(ui->frontPres, SIGNAL(toggled(bool)), ui->frontW, SLOT(setEnabled(bool)));
    connect(ui->frontPres, SIGNAL(toggled(bool)), ui->frontUnit, SLOT(setEnabled(bool)));

    connect(ui->backPres, SIGNAL(toggled(bool)), ui->backA, SLOT(setEnabled(bool)));
    connect(ui->backPres, SIGNAL(toggled(bool)), ui->backZ, SLOT(setEnabled(bool)));
    connect(ui->backPres, SIGNAL(toggled(bool)), ui->backW, SLOT(setEnabled(bool)));
    connect(ui->backPres, SIGNAL(toggled(bool)), ui->backUnit, SLOT(setEnabled(bool)));
}

SelectFrontBackForm::~SelectFrontBackForm()
{
    delete ui;
}

void SelectFrontBackForm::Refresh()
{
    ui->frontPres->setChecked(front->is_present);
    emit ui->frontPres->toggled(front->is_present);
    ui->frontA->setValue(front->A);
    ui->frontZ->setValue(front->Z);
    ui->frontW->setValue(front->width);
    ui->frontUnit->setCurrentIndex(Unit2Int(front->unit));

    ui->backPres->setChecked(back->is_present);
    emit ui->backPres->toggled(back->is_present);
    ui->backA->setValue(back->A);
    ui->backZ->setValue(back->Z);
    ui->backW->setValue(back->width);
    ui->backUnit->setCurrentIndex(Unit2Int(back->unit));
}

void SelectFrontBackForm::on_buttonBox_accepted()
{
    front->is_present = ui->frontPres->isChecked();
    front->A = ui->frontA->value();
    front->Z = ui->frontZ->value();
    front->width = ui->frontW->value();
    front->unit = Text2Unit(ui->frontUnit->currentText());

    back->is_present = ui->backPres->isChecked();
    back->A = ui->backA->value();
    back->Z = ui->backZ->value();
    back->width = ui->backW->value();
    back->unit = Text2Unit(ui->backUnit->currentText());

    emit DoRefresh();
}
