#include "transfer_window.h"
#include "ui_transfer_window.h"


Transfer_window::Transfer_window(QList <currency> cur, QList <account> acc, QList <income> inc, QList <expense> exp, QList <transfer> tr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_window)
{
    ui->setupUi(this);
    myModel= new TransferTableModel();
    currencies_for_tr=cur;
    accounts_for_tr=acc;
    incomes_for_tr=inc;
    expenses_for_tr=exp;
    myModel->transfer_tablemodel=tr;

    proxyModel = new MySortFilterProxyModel_tr();
    proxyModel->setSourceModel(myModel);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:QColor(53, 53, 53)}");
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);
    ui->tableView->setSortingEnabled(true);
    //ui->tableView-> sortByColumn ( 0 , Qt :: AscendingOrder );
    ui->tableView->horizontalHeader()->setSelectionMode( QAbstractItemView::NoSelection );
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);


    // Устанавливаем Контекстное Меню
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_pushButton_edit_clicked()));
    // Подключаем СЛОТ вызова контекстного меню
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));

}

Transfer_window::~Transfer_window()
{
    delete ui;
}

MySortFilterProxyModel_tr::MySortFilterProxyModel_tr(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}
bool MySortFilterProxyModel_tr::filterAcceptsRow(int sourceRow,
                                                  const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 1, sourceParent);//рахунок1
    QModelIndex index1 = sourceModel()->index(sourceRow, 4, sourceParent);//рахунок2
    QModelIndex index2 = sourceModel()->index(sourceRow, 3, sourceParent);//валюта1
    QModelIndex index3 = sourceModel()->index(sourceRow, 6, sourceParent);//валюта2
    QModelIndex index4 = sourceModel()->index(sourceRow, 8, sourceParent);//коментар
    return (sourceModel()->data(index0).toString().contains(search_vn, Qt::CaseInsensitive)
            || sourceModel()->data(index1).toString().contains(search_vn, Qt::CaseInsensitive)
            ||sourceModel()->data(index2).toString().contains(search_vn, Qt::CaseInsensitive)
            ||sourceModel()->data(index3).toString().contains(search_vn, Qt::CaseInsensitive)
            ||sourceModel()->data(index4).toString().contains(search_vn, Qt::CaseInsensitive));

}
void MySortFilterProxyModel_tr::setFilter()
{

    invalidateFilter();
}
void Transfer_window::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * editCur = new QAction(tr("Редактувати"), this);
    QAction * deleteCur = new QAction(tr("Видалити"), this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editCur, SIGNAL(triggered()), this, SLOT(on_pushButton_edit_clicked()));     // Обработчик вызова диалога редактирования
    connect(deleteCur, SIGNAL(triggered()), this, SLOT(on_pushButton_del_clicked())); // Обработчик удаления записи
    /* Устанавливаем действия в меню */
    menu->addAction(editCur);
    menu->addAction(deleteCur);
    /* Вызываем контекстное меню */
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

//додати переказ
void Transfer_window::on_pushButton_add_clicked()
{
    Transfer_add window(accounts_for_tr,currencies_for_tr);
    window.ui->label_avail_am1->setVisible(true);
    window.ui->label_dostup_text1->setVisible(true);
    window.ui->label_avail_am2->setVisible(true);
    window.ui->label_dostup_text2->setVisible(true);

    for(int i=0;i<accounts_for_tr.size();i++){
        window.acc_amount_end_map[accounts_for_tr[i].name]=accounts_for_tr[i].balance_end+accounts_for_tr[i].overdraft;
    }
    transfer trtr;
    trtr.amount1=0;
    trtr.amount2=0;
    trtr.date=QDateTime::currentDateTime();

    window.newtr=trtr;

    auto result=window.exec();

    if(result==1){

        transfer a=window.newtr;
        if(!a.amount1||!a.amount2){
            QMessageBox::information(
                this,
                tr("Додати переказ"),
                tr("Сума переказу не внесена!"));
            return;
        }
        myModel->transfer_tablemodel.push_back(a);
        proxyModel->invalidate();

        for(int i=0;i<accounts_for_tr.size();i++){
            if(accounts_for_tr[i].name==a.akk_name1){
                accounts_for_tr[i].balance_end-=a.amount1;
            }
            if(accounts_for_tr[i].name==a.akk_name2){
                accounts_for_tr[i].balance_end+=a.amount2;
            }
        }
    }



}

//редагувати переказ
void Transfer_window::on_pushButton_edit_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();
        int r=proxyModel->mapToSource(indexes[0]).row();
        Transfer_add window(accounts_for_tr,currencies_for_tr);
        window.ui->label_avail_am1->setVisible(true);
        window.ui->label_dostup_text1->setVisible(true);
        window.ui->label_avail_am2->setVisible(true);
        window.ui->label_dostup_text2->setVisible(true);
        window.ui->dateTimeEdit->setDateTime(myModel->transfer_tablemodel[r].date);
        window.ui->comboBox_account1->setCurrentText(myModel->transfer_tablemodel[r].akk_name1);
        window.ui->comboBox_account2->setCurrentText(myModel->transfer_tablemodel[r].akk_name2);
        window.ui->lineEdit_amount1->setText(QString::number(myModel->transfer_tablemodel[r].amount1));
        window.ui->lineEdit_amount2->setText(QString::number(myModel->transfer_tablemodel[r].amount2));
        window.ui->lineEdit_comments->setText(myModel->transfer_tablemodel[r].comment);
        window.ui->label_avail_am1->setVisible(true);
        window.ui->label_dostup_text1->setVisible(true);
        window.ui->label_avail_am2->setVisible(true);
        window.ui->label_dostup_text2->setVisible(true);
        QPalette sample_palette;
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        window.ui->label_avail_am1->setPalette(sample_palette);
        window.ui->label_avail_am2->setPalette(sample_palette);


        for(int i=0;i<accounts_for_tr.size();i++){
            window.acc_amount_end_map[accounts_for_tr[i].name]=accounts_for_tr[i].balance_end+accounts_for_tr[i].overdraft;
        }
        window.acc_amount_end_map[myModel->transfer_tablemodel[r].akk_name1]+=myModel->transfer_tablemodel[r].amount1;
        window.acc_amount_end_map[myModel->transfer_tablemodel[r].akk_name2]-=myModel->transfer_tablemodel[r].amount2;

        window.newtr=myModel->transfer_tablemodel[r];

        //window.ui->label_avail_am->setText(QString::number(window.acc_amount_end_map[myModel->expense_tablemodel[r].account]-myModel->expense_tablemodel[r].amount));
        window.ui->label_avail_am1->setText(QString::number(window.acc_amount_end_map[myModel->transfer_tablemodel[r].akk_name1]-myModel->transfer_tablemodel[r].amount1));
        window.ui->label_avail_am2->setText(QString::number(window.acc_amount_end_map[myModel->transfer_tablemodel[r].akk_name2]+myModel->transfer_tablemodel[r].amount2));
        //window.ui->label_avail_am2->setText("");
        auto result=window.exec();
        if(result==1){
            transfer a=window.newtr;
            if(!a.amount1||!a.amount2){
                QMessageBox::information(
                    this,
                    tr("Редагувати переказ"),
                    tr("Не внесена сума!"));
                return;
            }
            //редагування рахунків
            for(int i=0;i<accounts_for_tr.size();i++){
                if(accounts_for_tr[i].name==a.akk_name1){
                    accounts_for_tr[i].balance_end=accounts_for_tr[i].balance_end-a.amount1;
                }
                if(accounts_for_tr[i].name==a.akk_name2){
                    accounts_for_tr[i].balance_end=accounts_for_tr[i].balance_end+a.amount2;
                }
                if(accounts_for_tr[i].name==myModel->transfer_tablemodel[r].akk_name1){
                    accounts_for_tr[i].balance_end+=myModel->transfer_tablemodel[r].amount1;
                }
                if(accounts_for_tr[i].name==myModel->transfer_tablemodel[r].akk_name2){
                    accounts_for_tr[i].balance_end-=myModel->transfer_tablemodel[r].amount2;
                }
            }

            myModel->transfer_tablemodel[r]=a;
            proxyModel->invalidate();

        }
    }
    else  QMessageBox::information(
            this,
            tr("Редагувати переказ"),
            tr("Рядок не вибраний!"));
}

//видалити переказ
void Transfer_window::on_pushButton_del_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        switch( QMessageBox::question(
            this,
            tr("Видалити переказ"),
            tr("Ви дійсно хочете видалити?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {
            int r=proxyModel->mapToSource(indexes[0]).row();
            transfer a=myModel->transfer_tablemodel[r];
            QMap<QString,double>map;
            for(int i=0;i<accounts_for_tr.size();i++){
                map[accounts_for_tr[i].name]=accounts_for_tr[i].balance_end+accounts_for_tr[i].overdraft;
            }
            if(map[myModel->transfer_tablemodel[r].akk_name2]-myModel->transfer_tablemodel[r].amount2<0){
                QMessageBox::information(
                    this,
                    tr("Видалити переказ"),
                    tr("Залишок доступних коштів на 2му рахунку не може стати від'ємним!"));
                return;
            }
            for(int i=0;i<accounts_for_tr.size();i++){
                if(accounts_for_tr[i].name==myModel->transfer_tablemodel[r].akk_name1){
                    accounts_for_tr[i].balance_end+=myModel->transfer_tablemodel[r].amount1;
                }
                if(accounts_for_tr[i].name==myModel->transfer_tablemodel[r].akk_name2){
                    accounts_for_tr[i].balance_end-=myModel->transfer_tablemodel[r].amount2;
                }
            }
            auto del_index=myModel->transfer_tablemodel.begin()+r;
            myModel->transfer_tablemodel.erase(del_index);
            proxyModel->invalidate();


        }
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
    else  QMessageBox::information(
            this,
            tr("Видалити переказ"),
            tr("Рядок не вибрано!"));

}

//пошук
void Transfer_window::on_pushButton_search_clicked()
{
    proxyModel->search_vn=ui->lineEdit->text();
    proxyModel->setFilter();

}

//ентер в пошуку
void Transfer_window::on_lineEdit_inputRejected()
{
    on_pushButton_search_clicked();
}

