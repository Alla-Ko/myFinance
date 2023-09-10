#ifndef CURRENCY_ADD_H
#define CURRENCY_ADD_H

#include <QDialog>
#include <QRegularExpressionValidator>
#include "structurs.h"

namespace Ui {
class currency_add;
}


class currency_add : public QDialog
{
    Q_OBJECT

public:
    explicit currency_add(QWidget *parent = nullptr);
    ~currency_add();
    currency newcur;
    Ui::currency_add *ui;

private slots:
    void on_buttonBox_accepted();

private:

};

#endif // CURRENCY_ADD_H
