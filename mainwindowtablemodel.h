
#ifndef MAINWINDOWTABLEMODEL_H
#define MAINWINDOWTABLEMODEL_H
#include <QModelIndex>
#include "structurs.h"



class mainwindowtablemodel:public QAbstractTableModel
{
private:

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public:
    mainwindowtablemodel();
    int rowCount(const QModelIndex &parent) const;
    QList <account> account_tablemodel;
};

#endif // MAINWINDOWTABLEMODEL_H
