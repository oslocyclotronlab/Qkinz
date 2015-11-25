#ifndef SELECTTARGETFORM_H
#define SELECTTARGETFORM_H

#include <QDialog>

#include "types.h"

namespace Ui {
class SelectTargetForm;
}

class SelectTargetForm : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTargetForm(QWidget *parent = 0);
    ~SelectTargetForm();

    inline void setTarget(Target_t *target){ theTarget = target; }

    void Refresh();

signals:
    void DoRefresh();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SelectTargetForm *ui;

    Target_t *theTarget;
};

#endif // SELECTTARGETFORM_H
