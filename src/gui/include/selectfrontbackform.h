#ifndef SETFRONTBACKFORM_H
#define SETFRONTBACKFORM_H

#include <QDialog>
#include "types.h"

namespace Ui {
class SelectFrontBackForm;
}

class SelectFrontBackForm : public QDialog
{
    Q_OBJECT

public:
    explicit SelectFrontBackForm(QWidget *parent = 0);
    ~SelectFrontBackForm();

    inline void setExtra(Extra_t *f, Extra_t *b){ front = f; back = b; }

    void Refresh();

signals:
    void DoRefresh();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SelectFrontBackForm *ui;

    Extra_t *front;

    Extra_t *back;
};

#endif // SETFRONTBACKFORM_H
