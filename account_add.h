#ifndef ACCOUNT_ADD_H
#define ACCOUNT_ADD_H

#include <QDialog>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include "structurs.h"
namespace Ui {
class account_add;
}

class account_add : public QDialog
{
    Q_OBJECT

public:
    explicit account_add(QStringList cur, QWidget *parent = nullptr);
    ~account_add();
    account newacc;
    Ui::account_add *ui;

private slots:
    void on_comboBox_type_currentTextChanged(const QString &arg1);

    void on_radioButton_yes_clicked(bool checked);

    void on_radioButton_no_clicked(bool checked);

    void on_lineEdit_balance_editingFinished();

    void on_lineEdit_balance_textChanged(const QString &arg1);

    void on_lineEdit_overdraft_textChanged(const QString &arg1);

    void on_lineEdit_overdraft_textEdited(const QString &arg1);

    void on_lineEdit_overdraft_editingFinished();

    void on_buttonBox_accepted();

private:

};

#endif // ACCOUNT_ADD_H
