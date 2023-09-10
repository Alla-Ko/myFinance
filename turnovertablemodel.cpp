
#include "turnovertablemodel.h"
#include <QVariant>

turnovertablemodel::turnovertablemodel()
{

}
int turnovertablemodel::rowCount(const QModelIndex &parent) const
{
    return operation_tablemodel.size();
}
int turnovertablemodel::columnCount(const QModelIndex &parent) const
{
    return 8; // количество колонок сделаем также фиксированным
}

QVariant turnovertablemodel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= operation_tablemodel.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        if(index.column()==0)
        {
            return QVariant(operation_tablemodel[index.row()].date);
        }
        if(index.column()==1)
        {
            if(operation_tablemodel[index.row()].am_inc>0)
                return QVariant(operation_tablemodel[index.row()].am_inc);
            else return QVariant("");
        }
        if(index.column()==2)
        {
            if(operation_tablemodel[index.row()].am_exp>0)
                 return QVariant(operation_tablemodel[index.row()].am_exp);
            else return QVariant("");
        }
        if(index.column()==3)
        {
            return QVariant(operation_tablemodel[index.row()].type_glob);
        }
        if(index.column()==4)
        {
            return QVariant(operation_tablemodel[index.row()].type);
        }
        if(index.column()==5)
        {
            return QVariant(operation_tablemodel[index.row()].account);
        }
        if(index.column()==6)
        {
            return QVariant(operation_tablemodel[index.row()].balance);
        }
        if(index.column()==7)
        {
            return QVariant(operation_tablemodel[index.row()].comment);
        }


        //unswer = QString("row = ") + QString::number(index.row()) + "  col = " + QString::number(index.column());
        // строкой выше мы формируем ответ. QString::number преобразует число в текст
        //return QVariant(unswer);
    }
    if (role == Qt::ForegroundRole) {
        //выделение цветом
        if(index.column()==6)
            return QBrush(((operation_tablemodel[index.row()].balance<0)) ? QColorConstants::Svg::red : QColorConstants::Svg::skyblue);
        if(index.column()==1||index.column()==2)
            return QBrush(QColorConstants::Svg::skyblue);

        //if(index.column()==0||index.column()==1||index.column()==2)return QBrush(Qt::white);
    }
    return QVariant();
}
QVariant turnovertablemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Дата");
        case 1:
            return tr("Сума надходження");
        case 2:
            return tr("Сума списання");
        case 3:
            return tr("Тип операції");
        case 4:
            return tr("Категорія");
        case 5:
            return tr("Рахунок");
        case 6:
            return tr("Баланс");
        case 7:
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

