#ifndef INCOME_WINDOW_H
#define INCOME_WINDOW_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMenu>
#include "structurs.h"
#include "incometablemodel.h"
#include "income_add.h"
#include "ui_income_add.h"

namespace Ui {
class Income_window;
}
class MySortFilterProxyModel_inc : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortFilterProxyModel_inc(QObject *parent = 0);
    QString search_vn;
    void setFilter();


protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:

};








class Income_window : public QDialog
{
    Q_OBJECT

public:
    explicit Income_window(QList <currency> cur, QList <account> acc, QList <income> inc, QList <expense> exp, QList <transfer> tr, QWidget *parent = nullptr);
    ~Income_window();

    IncomeTableModel *myModel;
    QList <currency> currencies_for_inc;
    QList <account> accounts_for_inc;
    QList <expense> expenses_for_inc;
    QList <transfer> transfers_for_inc;

private slots:
    //доданий рядок
    void slotCustomMenuRequested(QPoint pos);


    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_search_clicked();

    void on_lineEdit_inputRejected();

    void on_comboBox_currency_currentTextChanged(const QString &arg1);

private:
    Ui::Income_window *ui;
    MySortFilterProxyModel_inc *proxyModel;
    void all_amounts (QString currency);

};

#endif // INCOME_WINDOW_H
