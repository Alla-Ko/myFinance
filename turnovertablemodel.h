
#ifndef TURNOVERTABLEMODEL_H
#define TURNOVERTABLEMODEL_H
#include <QModelIndex>
#include "structurs.h"



class turnovertablemodel:public QAbstractTableModel
{
private:

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public:
    turnovertablemodel();
    int rowCount(const QModelIndex &parent) const;
    QList <operation> operation_tablemodel;
};

#endif // TURNOVERTABLEMODEL_H
