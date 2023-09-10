#ifndef EXPENCE_ADD_H
#define EXPENCE_ADD_H

#include <QDialog>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include "structurs.h"


namespace Ui {
class Expence_add;
}

class Expence_add : public QDialog
{
    Q_OBJECT

public:
    explicit Expence_add(QList <account> acc, QWidget *parent = nullptr);
    ~Expence_add();
    expense newexp;
    QMap <QString,QString> acc_map;
    Ui::Expence_add *ui;
    QMap <QString,double> acc_amount_end_map;

private slots:
    void on_comboBox_account_currentTextChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_lineEdit_amount_textChanged(const QString &arg1);

private:

};

#endif // EXPENCE_ADD_H
