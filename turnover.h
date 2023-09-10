#ifndef TURNOVER_H
#define TURNOVER_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMenu>
#include "structurs.h"
#include "turnovertablemodel.h"


namespace Ui {
class Turnover;
}


class MySortFilterProxyModel_turn : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortFilterProxyModel_turn(QObject *parent = 0);
    void setFilter();




protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:

};







class Turnover : public QDialog
{
    Q_OBJECT

public:
    explicit Turnover(QList <operation> model, QWidget *parent = nullptr);
    ~Turnover();
    Ui::Turnover *ui;
    turnovertablemodel *myModel;


private:
    MySortFilterProxyModel_turn *proxyModel;



};

#endif // TURNOVER_H
