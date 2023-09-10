
#include "transfertablemodel.h"

TransferTableModel::TransferTableModel()
{

}

int TransferTableModel::rowCount(const QModelIndex &parent) const
{
    return transfer_tablemodel.size();
}
int TransferTableModel::columnCount(const QModelIndex &parent) const
{
    return 9; // количество колонок сделаем также фиксированным
}

QVariant TransferTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= transfer_tablemodel.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        QString unswer;
        QDateTime unswer_t;
        double unswer_d;
        if(index.column()==0)
        {
            unswer_t=transfer_tablemodel[index.row()].date;
            return QVariant(unswer_t);
        }
        if(index.column()==1)
        {
            unswer=transfer_tablemodel[index.row()].akk_name1;
            return QVariant(unswer);
        }
        if(index.column()==2)
        {
            unswer_d=transfer_tablemodel[index.row()].amount1;
            return QVariant(unswer_d);
        }
        if(index.column()==3)
        {
            unswer=transfer_tablemodel[index.row()].curr_name1;
            return QVariant(unswer);
        }
        if(index.column()==4)
        {
            unswer=transfer_tablemodel[index.row()].akk_name2;
            return QVariant(unswer);
        }
        if(index.column()==5)
        {
            unswer_d=transfer_tablemodel[index.row()].amount2;
            return QVariant(unswer_d);
        }
        if(index.column()==6)
        {
            unswer=transfer_tablemodel[index.row()].curr_name2;
            return QVariant(unswer);
        }
        if(index.column()==7)
        {
            unswer_d=(double)transfer_tablemodel[index.row()].amount2/transfer_tablemodel[index.row()].amount1;
            return QVariant(unswer_d);
        }
        if(index.column()==8)
        {
            unswer=transfer_tablemodel[index.row()].comment;
            return QVariant(unswer);
        }


    }
    return QVariant();
}
QVariant TransferTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Дата");
        case 1:
            return tr("З рахунку");
        case 2:
            return tr("Сума");
        case 3:
            return tr("Валюта");
        case 4:
            return tr("На рахунок");
        case 5:
            return tr("Сума");
        case 6:
            return tr("Валюта");
        case 7:
            return tr("Фактичний курс");
        case 8:
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

