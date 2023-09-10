
#include "expensetablemodel.h"

ExpenseTableModel::ExpenseTableModel()
{

}

int ExpenseTableModel::rowCount(const QModelIndex &parent) const
{
    return expense_tablemodel.size();
}
int ExpenseTableModel::columnCount(const QModelIndex &parent) const
{
    return 6; // количество колонок сделаем также фиксированным
}
QVariant ExpenseTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= expense_tablemodel.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        QString unswer;
        QDateTime unswer_t;
        double unswer_d;
        if(index.column()==0)
        {
            unswer_t=expense_tablemodel[index.row()].date;
            return QVariant(unswer_t);
        }
        if(index.column()==1)
        {
            unswer_d=expense_tablemodel[index.row()].amount;
            return QVariant(unswer_d);
        }
        if(index.column()==2)
        {
            unswer=expense_tablemodel[index.row()].currency;
            return QVariant(unswer);
        }
        if(index.column()==3)
        {
            unswer=expense_tablemodel[index.row()].type;
            return QVariant(unswer);
        }
        if(index.column()==4)
        {
            unswer=expense_tablemodel[index.row()].account;
            return QVariant(unswer);
        }
        if(index.column()==5)
        {
            unswer=expense_tablemodel[index.row()].comment;
            return QVariant(unswer);
        }
    }
    return QVariant();
}
QVariant ExpenseTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
