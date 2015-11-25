#ifndef SELECTBEAMFORM_H
#define SELECTBEAMFORM_H

#include <QDialog>

#include "types.h"

namespace Ui {
class SelectBeamForm;
}

class SelectBeamForm : public QDialog
{
    Q_OBJECT

public:
    explicit SelectBeamForm(QWidget *parent = 0);
    inline void setBeam(Beam_t *beam) { thisBeam = beam; }
    ~SelectBeamForm();

    void Refresh();

signals:
    void DoRefresh();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SelectBeamForm *ui;

    Beam_t *thisBeam;
};

#endif // SELECTBEAMFORM_H
