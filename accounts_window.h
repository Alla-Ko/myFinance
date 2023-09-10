#ifndef ACCOUNTS_WINDOW_H
#define ACCOUNTS_WINDOW_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMenu>
#include "structurs.h"
#include "accounttablemodel.h"
#include "ui_account_add.h"

namespace Ui {
class Accounts_window;
}

class MySortFilterProxyModel_acc : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortFilterProxyModel_acc(QObject *parent = 0);
    QString search_vn;
    void setFilter();



protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:

};






class Accounts_window : public QDialog
{
    Q_OBJECT

public:
    explicit Accounts_window(QList <currency> cur, QList <account> accounts, QList <income> inc, QList <expense> exp, QList <transfer> tr, QWidget *parent = nullptr);
    ~Accounts_window();
    QList <currency> currencies_for_acc;
    accounttablemodel *myModel;

    QList <income> incomes_for_acc;
    QList <expense> expenses_for_acc;
    QList <transfer> transfers_for_acc;


private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_del_clicked();

    void on_lineEdit_inputRejected();

    //доданий рядок
    void slotCustomMenuRequested(QPoint pos);

private:
    Ui::Accounts_window *ui;
    MySortFilterProxyModel_acc *proxyModel;
};

#endif // ACCOUNTS_WINDOW_H
