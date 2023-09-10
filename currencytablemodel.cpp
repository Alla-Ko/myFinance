
#include "currencytablemodel.h"
#include <QVariant>

CurrencyTableModel::CurrencyTableModel()
{

}
int CurrencyTableModel::rowCount(const QModelIndex &parent) const
{
    //return 5; // сделаем фиксированно 5 строк в таблице
    return currencies_tablemodel.size();
}
int CurrencyTableModel::columnCount(const QModelIndex &parent) const
{
    return 3; // количество колонок сделаем также фиксированным
}

QVariant CurrencyTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= currencies_tablemodel.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        QString unswer;
        double unswer_d;
        if(index.column()==0)
        {
            unswer=currencies_tablemodel[index.row()].name;
            return QVariant(unswer);
        }
        if(index.column()==1)
        {
            unswer_d=currencies_tablemodel[index.row()].rate;
            return QVariant(unswer_d);
        }
        if(index.column()==2)
        {
            unswer=currencies_tablemodel[index.row()].comment;
            return QVariant(unswer);
        }

        //unswer = QString("row = ") + QString::number(index.row()) + "  col = " + QString::number(index.column());
        // строкой выше мы формируем ответ. QString::number преобразует число в текст

    }
    return QVariant();
}
QVariant CurrencyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Назва валюти");
        case 1:
            return tr("Курс до гривні");
        case 2:
            return tr("Коментар");
        default:
            break;
        }
    }
    if (orientation == Qt::Vertical) {

        return QString::number(section+1);
    }
    return QVariant();
}
