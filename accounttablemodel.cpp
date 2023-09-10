
#include "accounttablemodel.h"
#include <QVariant>

accounttablemodel::accounttablemodel()
{

}

int accounttablemodel::rowCount(const QModelIndex &parent) const
{
    return account_tablemodel.size();
}
int accounttablemodel::columnCount(const QModelIndex &parent) const
{
    return 5; // количество колонок сделаем также фиксированным
}

QVariant accounttablemodel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= account_tablemodel.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        QString unswer;
        double unswer_d;
        if(index.column()==0||index.column()==1||index.column()==2){
            if(index.column()==0)
            {
                unswer=account_tablemodel[index.row()].name;
            }
            if(index.column()==1)
            {
                unswer=account_tablemodel[index.row()].type;
            }
            if(index.column()==2)
            {
                unswer=account_tablemodel[index.row()].currency;
            }
            return QVariant(unswer);
        }
        if(index.column()==3)
        {
            //unswer=QString::number(account_tablemodel[index.row()].overdraft);
            unswer_d=account_tablemodel[index.row()].overdraft;
            return QVariant(unswer_d);
        }

        if(index.column()==4)
        {
            //unswer_d=QString::number(account_tablemodel[index.row()].balance);
            unswer_d=account_tablemodel[index.row()].balance;
            return QVariant(unswer_d);
        }



        //unswer = QString("row = ") + QString::number(index.row()) + "  col = " + QString::number(index.column());
        // строкой выше мы формируем ответ. QString::number преобразует число в текст
        //return QVariant(unswer);
    }
    if (role == Qt::ForegroundRole) {
        //выделение цветом
        return QBrush(((account_tablemodel[index.row()].balance<0&&index.column()==4)||(account_tablemodel[index.row()].balance+account_tablemodel[index.row()].overdraft<0&&index.column()==5)) ? Qt::red : Qt::white);
    }
    return QVariant();
}
QVariant accounttablemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Назва рахунку");
        case 1:
            return tr("Тип рахунку");
        case 2:
            return tr("Валюта");
        case 3:
            return tr("Кредитний ліміт");
        case 4:
            return tr("Стартовий баланс");
        default:
            break;
        }
    }
    if (orientation == Qt::Vertical) {

        return QString::number(section+1);
    }
    return QVariant();
}
