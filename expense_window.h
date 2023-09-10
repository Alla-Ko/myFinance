#ifndef EXPENSE_WINDOW_H
#define EXPENSE_WINDOW_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMenu>
#include "structurs.h"
#include "expensetablemodel.h"
#include "expence_add.h"
#include "ui_expence_add.h"


namespace Ui {
class Expense_window;
}

class MySortFilterProxyModel_exp : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortFilterProxyModel_exp(QObject *parent = 0);
    QString search_vn;
    void setFilter();


protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:

};


class Expense_window : public QDialog
{
    Q_OBJECT

public:
    explicit Expense_window(QList <currency> cur, QList <account> acc, QList <income> inc, QList <expense> exp, QList <transfer> tr,  QWidget *parent = nullptr);
    ~Expense_window();
    ExpenseTableModel *myModel;
    QList <currency> currencies_for_exp;
    QList <account> accounts_for_exp;
    QList <income> incomes_for_exp;
    QList <transfer> transfers_for_exp;

private slots:
    //доданий рядок
    void slotCustomMenuRequested(QPoint pos);


    void on_pushButton_search_clicked();

    void on_lineEdit_returnPressed();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_del_clicked();

    void on_comboBox_currency_currentTextChanged(const QString &arg1);

private:
    Ui::Expense_window *ui;
    MySortFilterProxyModel_exp *proxyModel;
    void all_amounts (QString currency);
};

#endif // EXPENSE_WINDOW_H
