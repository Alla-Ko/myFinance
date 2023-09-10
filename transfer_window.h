#ifndef TRANSFER_WINDOW_H
#define TRANSFER_WINDOW_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMenu>
#include "structurs.h"
#include "transfertablemodel.h"
#include "transfer_add.h"
#include "ui_transfer_add.h"


namespace Ui {
class Transfer_window;
}

class MySortFilterProxyModel_tr : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortFilterProxyModel_tr(QObject *parent = 0);
    QString search_vn;
    void setFilter();


protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:

};





class Transfer_window : public QDialog
{
    Q_OBJECT

public:
    explicit Transfer_window(QList <currency> cur, QList <account> acc, QList <income> inc, QList <expense> exp, QList <transfer> tr,  QWidget *parent = nullptr);
    ~Transfer_window();
    TransferTableModel *myModel;
    QList <currency> currencies_for_tr;
    QList <account> accounts_for_tr;
    QList <expense> expenses_for_tr;
    QList <income> incomes_for_tr;

private slots:
    //доданий рядок
    void slotCustomMenuRequested(QPoint pos);

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_search_clicked();

    void on_lineEdit_inputRejected();

private:
    Ui::Transfer_window *ui;
    MySortFilterProxyModel_tr *proxyModel;


};

#endif // TRANSFER_WINDOW_H
