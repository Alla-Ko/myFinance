
#ifndef CURRENCYTABLEMODEL_H
#define CURRENCYTABLEMODEL_H
#include <QModelIndex>
#include "currency_add.h"
#include "structurs.h"

class CurrencyTableModel:public QAbstractTableModel
{
private:


    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public:
    CurrencyTableModel();
    int rowCount(const QModelIndex &parent) const;
    QList <currency> currencies_tablemodel;
};

#endif // CURRENCYTABLEMODEL_H
