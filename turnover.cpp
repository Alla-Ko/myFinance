#include "turnover.h"
#include "ui_turnover.h"
#include "turnovertablemodel.h"

Turnover::Turnover(QList <operation> model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Turnover)
{
    ui->setupUi(this);
    myModel= new turnovertablemodel();
    myModel->operation_tablemodel=model;
    proxyModel = new MySortFilterProxyModel_turn();

    proxyModel->setSourceModel(myModel);


    ui->tableView->setModel(proxyModel);

    ui->tableView->setSelectionMode (QAbstractItemView::NoSelection);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:QColor(53, 53, 53)}");
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->tableView->setSortingEnabled(false);
    //ui->tableView-> sortByColumn ( -1 , Qt :: AscendingOrder );
    //ui->tableView->horizontalHeader()->setSelectionMode( QAbstractItemView::NoSelection );
    ui->tableView->horizontalHeader()->setSelectionMode( QAbstractItemView::NoSelection );
    ui->tableView->verticalHeader()->setVisible(true);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->resizeSection(1, 120);

}

Turnover::~Turnover()
{
    delete ui;
}


MySortFilterProxyModel_turn::MySortFilterProxyModel_turn(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MySortFilterProxyModel_turn::filterAcceptsRow(int sourceRow,
                                                  const QModelIndex &sourceParent) const
{

    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
    // QModelIndex index3 = sourceModel()->index(sourceRow, 3, sourceParent);
    // QModelIndex index4 = sourceModel()->index(sourceRow, 4, sourceParent);
    //  QModelIndex index5 = sourceModel()->index(sourceRow, 5, sourceParent);

    return (sourceModel()->data(index0).toString().contains("", Qt::CaseInsensitive)
            || sourceModel()->data(index1).toString().contains("", Qt::CaseInsensitive)
            ||sourceModel()->data(index2).toString().contains("", Qt::CaseInsensitive));


}

void MySortFilterProxyModel_turn::setFilter()
{

    invalidateFilter();
}
