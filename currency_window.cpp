#include "currency_window.h"
#include "ui_currency_add.h"
#include "ui_currency_window.h"
#include <qDebug>

currency_window::currency_window(QList <currency> cur, QList <account> accounts, QList <income> inc, QList <expense> exp, QList <transfer> tr,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::currency_window)
{
    ui->setupUi(this);
    myModel= new CurrencyTableModel();
    accounts_for_cur=accounts;
    incomes_for_cur=inc;
    expenses_for_cur=exp;
    transfers_for_cur=tr;

    myModel->currencies_tablemodel=cur;
    //myModel->accounts_tablemodel=accounts;
    qDebug()<< myModel->currencies_tablemodel.size();
    proxyModel = new MySortFilterProxyModel();
    proxyModel->setSourceModel(myModel);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:QColor(53, 53, 53)}");
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tableView->setSortingEnabled(true);
    //ui->tableView-> sortByColumn ( 0 , Qt :: AscendingOrder );
    ui->tableView->horizontalHeader()->setSelectionMode( QAbstractItemView::NoSelection );
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->sortByColumn(-1, Qt::DescendingOrder);


    // Устанавливаем Контекстное Меню
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_pushButton_edit_clicked()));
    // Подключаем СЛОТ вызова контекстного меню
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));

    //proxyModel->invalidate();
}



currency_window::~currency_window()
{
    delete ui;
}
//для сортування і фільтрування
MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,
                                              const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);


    return (sourceModel()->data(index0).toString().contains(search_vn, Qt::CaseInsensitive)
            || sourceModel()->data(index1).toString().contains(search_vn, Qt::CaseInsensitive)
            ||sourceModel()->data(index2).toString().contains(search_vn, Qt::CaseInsensitive));

}

void MySortFilterProxyModel::setFilter()
{

    invalidateFilter();
}
//кнопка пошуку на формі
void currency_window::on_pushButton_search_clicked()
{
    proxyModel->search_vn=ui->lineEdit->text();
    proxyModel->setFilter();
}

//поле для пошуку
void currency_window::on_lineEdit_inputRejected()
{
    on_pushButton_search_clicked();
}

//видалення валюти
void currency_window::on_pushButton_del_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        switch( QMessageBox::question(
            this,
            tr("Видалити валюту"),
            tr("Ви дійсно хочете видалити?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {

            int r=proxyModel->mapToSource(indexes[0]).row();
            if(myModel->currencies_tablemodel[r].name=="UAH"){
                QMessageBox::information(
                    this,
                    tr("Видалити валюту"),
                    tr("UAH не можливо видалити чи редагувати!"));
            }
            else{
                int lich=0;
                for(int i=0;i<accounts_for_cur.size();i++){
                    if(accounts_for_cur[i].currency==myModel->currencies_tablemodel[r].name){
                        QMessageBox::information(
                            this,
                            tr("Видалити валюту"),
                            tr("Не можливо видалити валюту, якщо є рахунки в цій валюті!"));
                        lich++;
                    }
                }
                if (!lich){
                    auto del_index=myModel->currencies_tablemodel.begin()+r;
                    myModel->currencies_tablemodel.erase(del_index);
                    proxyModel->invalidate();
                    //currencies=myModel->currencies_tablemodel;
                }
            }


        }

        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
    else  QMessageBox::information(
            this,
            tr("Видалити валюту"),
            tr("Валюта не вибрана!"));
}


void currency_window::on_pushButton_add_clicked()
{

    currency_add window;
    auto result=window.exec();
    if(result==1){
        if(window.newcur.name.size()!=3){
            //неправильний код валюти
            QMessageBox::information(
                this,
                tr("Додати валюту"),
                tr("Неправильний код валюти (введіть три латинські літери)"));
        }
        else {
            if(!(window.newcur.rate>0)){
                //неправильний курс
                QMessageBox::information(
                    this,
                    tr("Додати валюту"),
                    tr("Неправильний курс (введіть значення)"));
            }
            else {
                int lich=0;
                for(int i=0;i<myModel->currencies_tablemodel.size();i++){
                    if(myModel->currencies_tablemodel[i].name==window.newcur.name){
                        lich++;
                        //така валюта вже є
                        QMessageBox::information(
                            this,
                            tr("Додати валюту"),
                            tr("Така валюта вже існує"));
                        break;
                    }
                }
                if(lich==0){
                    myModel->currencies_tablemodel.push_back(window.newcur);
                    proxyModel->invalidate();
                    //currencies=myModel->currencies_tablemodel;
                }
            }
        }
    }

}

//редагування валюти
void currency_window::on_pushButton_edit_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        int r=proxyModel->mapToSource(indexes[0]).row();
        if(myModel->currencies_tablemodel[r].name=="UAH"){
            QMessageBox::information(
                this,
                tr("Редагувати валюту"),
                tr("UAH не можливо видалити чи редагувати!"));
        }
        else{
            currency_add window;
            window.ui->lineEdit_name->setText(myModel->currencies_tablemodel[r].name);
            window.ui->lineEdit_rate->setText(QString::number(myModel->currencies_tablemodel[r].rate));
            window.ui->lineEdit_comment->setText(myModel->currencies_tablemodel[r].comment);
            //window.newcur=myModel->currencies_tablemodel[r];
            window.exec();
            if(window.newcur.name!=""&&window.newcur.rate==0){
                QMessageBox::information(
                    this,
                    tr("Редагувати валюту"),
                    tr("Некоректний курс!"));
            }
            if (window.newcur.name!=""&&window.newcur.rate!=0){
                int lich=0;
                for(int i=0;i<myModel->currencies_tablemodel.size();i++){
                    if(i!=r&&myModel->currencies_tablemodel[i].name==window.newcur.name){
                        QMessageBox::information(
                            this,
                            tr("Редагувати валюту"),
                            tr("Некоректна назва, така валюта вже існує"));
                        lich++;
                        break;
                    }
                }
                if(lich==0){
                    currency oldcur=myModel->currencies_tablemodel[r];
                    myModel->currencies_tablemodel[r]=window.newcur;
                    proxyModel->invalidate();
                    //currencies=myModel->currencies_tablemodel;
                    for(int i=0;i<accounts_for_cur.size();i++){
                        if(accounts_for_cur[i].currency==oldcur.name){
                            accounts_for_cur[i].currency=myModel->currencies_tablemodel[r].name;
                        }
                    }
                    for(int i=0;i<incomes_for_cur.size();i++){
                        if(incomes_for_cur[i].currency==oldcur.name){
                            incomes_for_cur[i].currency=myModel->currencies_tablemodel[r].name;
                        }
                    }
                    for(int i=0;i<expenses_for_cur.size();i++){
                        if(expenses_for_cur[i].currency==oldcur.name){
                            expenses_for_cur[i].currency=myModel->currencies_tablemodel[r].name;
                        }
                    }
                    for(int i=0;i<transfers_for_cur.size();i++){
                        if(transfers_for_cur[i].curr_name1==oldcur.name){
                            transfers_for_cur[i].curr_name1=myModel->currencies_tablemodel[r].name;
                        }
                        if(transfers_for_cur[i].curr_name2==oldcur.name){
                            transfers_for_cur[i].curr_name2=myModel->currencies_tablemodel[r].name;
                        }
                    }
                }
            }
        }
    }
    else  QMessageBox::information(
            this,
            tr("Редагувати валюту"),
            tr("Валюта не вибрана!"));
}
void currency_window::slotCustomMenuRequested(QPoint pos)
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
