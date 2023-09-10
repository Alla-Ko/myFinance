#ifndef INCOME_ADD_H
#define INCOME_ADD_H

#include <QDialog>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include "structurs.h"
//#include <QMap>

namespace Ui {
class Income_add;
}

class Income_add : public QDialog
{
    Q_OBJECT

public:
    explicit Income_add(QList <account> acc, QWidget *parent = nullptr);
    ~Income_add();
    income newinc;
    QMap <QString,QString> acc_map;
    Ui::Income_add *ui;
    QMap <QString,double> acc_amount_end_map;

private slots:
    void on_comboBox_account_currentTextChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_lineEdit_amount_textChanged(const QString &arg1);

private:

};

#endif // INCOME_ADD_H
