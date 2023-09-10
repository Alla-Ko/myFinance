
#ifndef TRANSFERTABLEMODEL_H
#define TRANSFERTABLEMODEL_H
#include <QModelIndex>
#include "structurs.h"




class TransferTableModel:public QAbstractTableModel
{
private:

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public:
    TransferTableModel();
    int rowCount(const QModelIndex &parent) const;
    QList <transfer> transfer_tablemodel;

};

#endif // TRANSFERTABLEMODEL_H
