#ifndef CURRENCY_WINDOW_H
#define CURRENCY_WINDOW_H

#include <QDialog>
#include "currencytablemodel.h"
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMenu>
#include "structurs.h"





namespace Ui {
class currency_window;
}
class MySortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortFilterProxyModel(QObject *parent = 0);
    QString search_vn;
    void setFilter();



protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:

};


class currency_window : public QDialog
{
    Q_OBJECT

public:
    explicit currency_window(QList <currency> cur, QList <account> accounts, QList <income> inc, QList <expense> exp, QList <transfer> tr, QWidget *parent = nullptr);
    ~currency_window();
    //QList <currency> currencies;
    QList <account> accounts_for_cur;
    CurrencyTableModel *myModel;
    QList <income> incomes_for_cur;
    QList <expense> expenses_for_cur;
    QList <transfer> transfers_for_cur;

private slots:
    void on_pushButton_search_clicked();

    void on_lineEdit_inputRejected();

    void on_pushButton_del_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();
    //доданий рядок
    void slotCustomMenuRequested(QPoint pos);

private:
    Ui::currency_window *ui;

    MySortFilterProxyModel *proxyModel;

};

#endif // CURRENCY_WINDOW_H
