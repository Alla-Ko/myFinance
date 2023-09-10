
#ifndef EXPENSETABLEMODEL_H
#define EXPENSETABLEMODEL_H
#include <QModelIndex>
#include "structurs.h"




class ExpenseTableModel:public QAbstractTableModel
{
private:

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public:
    ExpenseTableModel();
    int rowCount(const QModelIndex &parent) const;
    QList <expense> expense_tablemodel;

};

#endif // EXPENSETABLEMODEL_H
