#ifndef TRANSFER_ADD_H
#define TRANSFER_ADD_H

#include <QDialog>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include "structurs.h"
#include <QtCore/qmath.h>


namespace Ui {
class Transfer_add;
}

class Transfer_add : public QDialog
{
    Q_OBJECT

public:
    explicit Transfer_add(QList <account> acc, QList <currency> cur, QWidget *parent = nullptr);
    ~Transfer_add();
    Ui::Transfer_add *ui;
    QMap <QString,double> acc_amount_end_map;
    QMap <QString,double> cur_rate_map;
    transfer newtr;
    QMap <QString,QString> acc_map;

private slots:
    void on_comboBox_account1_currentTextChanged(const QString &arg1);

    void on_comboBox_account2_currentTextChanged(const QString &arg1);

    void on_lineEdit_amount1_textChanged(const QString &arg1);

    void on_lineEdit_amount2_textChanged(const QString &arg1);

    void on_lineEdit_rate_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    void available();
};

#endif // TRANSFER_ADD_H
