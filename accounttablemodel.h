
#ifndef ACCOUNTTABLEMODEL_H
#define ACCOUNTTABLEMODEL_H
#include <QModelIndex>
//#include "account_add.h"
#include "structurs.h"




class accounttablemodel:public QAbstractTableModel
{
private:

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public:
    accounttablemodel();
    int rowCount(const QModelIndex &parent) const;
    QList <account> account_tablemodel;

};

#endif // ACCOUNTTABLEMODEL_H
