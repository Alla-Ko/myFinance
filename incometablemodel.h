
#ifndef INCOMETABLEMODEL_H
#define INCOMETABLEMODEL_H
#include <QModelIndex>
#include "structurs.h"



class IncomeTableModel:public QAbstractTableModel
{
private:

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public:
    IncomeTableModel();
    int rowCount(const QModelIndex &parent) const;
    QList <income> income_tablemodel;

};

#endif // INCOMETABLEMODEL_H
