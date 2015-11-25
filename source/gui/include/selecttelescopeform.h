#ifndef SELECTTELESCOPEFORM_H
#define SELECTTELESCOPEFORM_H

#include <QDialog>

#include "types.h"

namespace Ui {
class SelectTelescopeForm;
}

class SelectTelescopeForm : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTelescopeForm(QWidget *parent = 0);
    ~SelectTelescopeForm();

    inline void setTelescope(Telescope_t *telescope){ theTelescope = telescope; }

    void Refresh();

signals:
    void DoRefresh();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SelectTelescopeForm *ui;

    Telescope_t *theTelescope;
};

#endif // SELECTTELESCOPEFORM_H
