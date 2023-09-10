
#include "incometablemodel.h"

IncomeTableModel::IncomeTableModel()
{

}

int IncomeTableModel::rowCount(const QModelIndex &parent) const
{
    return income_tablemodel.size();
}
int IncomeTableModel::columnCount(const QModelIndex &parent) const
{
    return 6; // количество колонок сделаем также фиксированным
}

QVariant IncomeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= income_tablemodel.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        QString unswer;
        QDateTime unswer_t;
        double unswer_d;
        if(index.column()==0)
        {
            unswer_t=income_tablemodel[index.row()].date;
            return QVariant(unswer_t);
        }
        if(index.column()==1)
        {
            unswer_d=income_tablemodel[index.row()].amount;
            return QVariant(unswer_d);
        }
        if(index.column()==2)
        {
            unswer=income_tablemodel[index.row()].currency;
            return QVariant(unswer);
        }
        if(index.column()==3)
        {
            unswer=income_tablemodel[index.row()].type;
            return QVariant(unswer);
        }
        if(index.column()==4)
        {
            unswer=income_tablemodel[index.row()].account;
            return QVariant(unswer);
        }
        if(index.column()==5)
        {
            unswer=income_tablemodel[index.row()].comment;
            return QVariant(unswer);
        }
    }
    return QVariant();
}
QVariant IncomeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Дата");
        case 1:
            return tr("Сума");
        case 2:
            return tr("Валюта");
        case 3:
            return tr("Категорія");
        case 4:
            return tr("Рахунок");
        case 5:
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
