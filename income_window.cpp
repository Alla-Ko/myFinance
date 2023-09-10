#include "income_window.h"
#include "ui_income_window.h"
#include "account_add.h"

Income_window::Income_window(QList <currency> cur, QList <account> acc, QList <income> inc, QList <expense> exp, QList <transfer> tr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Income_window)
{
    ui->setupUi(this);

    myModel= new IncomeTableModel();
    currencies_for_inc=cur;
    accounts_for_inc=acc;
    expenses_for_inc=exp;
    transfers_for_inc=tr;
    myModel->income_tablemodel=inc;



    proxyModel = new MySortFilterProxyModel_inc();
    proxyModel->setSourceModel(myModel);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:QColor(53, 53, 53)}");
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->tableView->setSortingEnabled(true);
    //ui->tableView-> sortByColumn ( 0 , Qt :: AscendingOrder );
    ui->tableView->horizontalHeader()->setSelectionMode( QAbstractItemView::NoSelection );
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);


    for(int i=0;i<currencies_for_inc.size();i++){
        ui->comboBox_currency->addItem(currencies_for_inc[i].name);
        ui->comboBox_currency->setCurrentText("UAH");
        ui->lineEdit_am_all->setReadOnly(true);
        ui->lineEdit_am_cur->setReadOnly(true);
        all_amounts ("UAH");
    }



    // Устанавливаем Контекстное Меню
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_pushButton_edit_clicked()));
    // Подключаем СЛОТ вызова контекстного меню
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));


}

Income_window::~Income_window()
{
    delete ui;
}

MySortFilterProxyModel_inc::MySortFilterProxyModel_inc(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}
bool MySortFilterProxyModel_inc::filterAcceptsRow(int sourceRow,
                                                  const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 5, sourceParent);//коментар
    QModelIndex index1 = sourceModel()->index(sourceRow, 3, sourceParent);//категорія
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);//валюта
    QModelIndex index3 = sourceModel()->index(sourceRow, 4, sourceParent);//рахунок
    return (sourceModel()->data(index0).toString().contains(search_vn, Qt::CaseInsensitive)
            || sourceModel()->data(index1).toString().contains(search_vn, Qt::CaseInsensitive)
            ||sourceModel()->data(index2).toString().contains(search_vn, Qt::CaseInsensitive)
            ||sourceModel()->data(index3).toString().contains(search_vn, Qt::CaseInsensitive));

}
void MySortFilterProxyModel_inc::setFilter()
{

    invalidateFilter();
}
void Income_window::all_amounts (QString currency){
    double all=0.00, only=0.00;
    QMap<QString,double> curr_map;

    for(int i=0;i<currencies_for_inc.size();i++){
        curr_map[currencies_for_inc[i].name]=currencies_for_inc[i].rate;
    }


    for(int i=0;i<myModel->income_tablemodel.size();i++){
        if(myModel->income_tablemodel[i].currency==currency){
            only+=myModel->income_tablemodel[i].amount;
        }
        all+=double(myModel->income_tablemodel[i].amount*curr_map[myModel->income_tablemodel[i].currency])/curr_map[currency];
    }
    ui->lineEdit_am_all->setText(QString::number(all));
    ui->lineEdit_am_cur->setText(QString::number(only));
}



void Income_window::slotCustomMenuRequested(QPoint pos)
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

//додати дохід
void Income_window::on_pushButton_add_clicked()
{
    Income_add window(accounts_for_inc);

    window.ui->label_avail_am->setVisible(false);
    window.ui->label_dostup_text->setVisible(false);
    window.ui->label_avail_cur->setVisible(false);

    for(int i=0;i<accounts_for_inc.size();i++){
        window.acc_amount_end_map[accounts_for_inc[i].name]=accounts_for_inc[i].balance_end+accounts_for_inc[i].overdraft;
    }

    auto result=window.exec();
    if(result==1){
        income a=window.newinc;
        if(!a.amount){
            QMessageBox::information(
                this,
                tr("Додати дохід"),
                tr("Не внесена сума доходу!"));
            return;
        }
        myModel->income_tablemodel.push_back(a);
        proxyModel->invalidate();
        all_amounts (ui->comboBox_currency->currentText());
        for(int i=0;i<accounts_for_inc.size();i++){
            if(accounts_for_inc[i].name==a.account){
                accounts_for_inc[i].balance_end+=a.amount;
                break;
            }
        }
    }

}

//відредагувати дохід
void Income_window::on_pushButton_edit_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();
        int r=proxyModel->mapToSource(indexes[0]).row();
        Income_add window(accounts_for_inc);
        window.ui->dateTimeEdit->setDateTime(myModel->income_tablemodel[r].date);
        window.ui->lineEdit_amount->setText(QString::number(myModel->income_tablemodel[r].amount));
        window.ui->lineEdit_currency->setText(myModel->income_tablemodel[r].currency);
        window.ui->lineEdit_comments->setText(myModel->income_tablemodel[r].comment);
        window.ui->comboBox_account->setCurrentText(myModel->income_tablemodel[r].account);
        window.ui->comboBox_type->setCurrentText(myModel->income_tablemodel[r].type);
        window.ui->label_avail_am->setVisible(true);
        window.ui->label_dostup_text->setVisible(true);
        window.ui->label_avail_cur->setVisible(true);

        for(int i=0;i<accounts_for_inc.size();i++){
            window.acc_amount_end_map[accounts_for_inc[i].name]=accounts_for_inc[i].balance_end+accounts_for_inc[i].overdraft;

        }

        window.acc_amount_end_map[myModel->income_tablemodel[r].account]-=myModel->income_tablemodel[r].amount;
        window.ui->label_avail_am->setText(QString::number(window.acc_amount_end_map[myModel->income_tablemodel[r].account]+myModel->income_tablemodel[r].amount));

        auto result=window.exec();
        if(result==1){
            income a=window.newinc;
            if(!a.amount){
                QMessageBox::information(
                    this,
                    tr("Редагувати дохід"),
                    tr("Не внесена сума доходу!"));
                return;
            }

            for(int i=0;i<accounts_for_inc.size();i++){
                if(accounts_for_inc[i].name==myModel->income_tablemodel[r].account){
                    accounts_for_inc[i].balance_end-= myModel->income_tablemodel[r].amount;
                }
                if(accounts_for_inc[i].name==a.account){
                    accounts_for_inc[i].balance_end+= a.amount;
                }
            }
            myModel->income_tablemodel[r]=a;
            proxyModel->invalidate();
            all_amounts (ui->comboBox_currency->currentText());


        }
    }
    else  QMessageBox::information(
            this,
            tr("Редагувати дохід"),
            tr("Рядок не вибраний!"));

}

//видалити дохід
void Income_window::on_pushButton_del_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        switch( QMessageBox::question(
            this,
            tr("Видалити дохід"),
            tr("Ви дійсно хочете видалити?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {
            int r=proxyModel->mapToSource(indexes[0]).row();
            income a=myModel->income_tablemodel[r];
            QMap<QString,double>map;
            for(int i=0;i<accounts_for_inc.size();i++){
                map[accounts_for_inc[i].name]=accounts_for_inc[i].balance_end+accounts_for_inc[i].overdraft;
            }
            if(map[myModel->income_tablemodel[r].account]-myModel->income_tablemodel[r].amount<0){
                QMessageBox::information(
                    this,
                    tr("Видалити дохід"),
                    tr("Залишок доступних коштів на рахунку не може бути менше нуля!"));
                return;
            }

            auto del_index=myModel->income_tablemodel.begin()+r;
            myModel->income_tablemodel.erase(del_index);
            proxyModel->invalidate();
            all_amounts (ui->comboBox_currency->currentText());
            for(int i=0;i<accounts_for_inc.size();i++){
                if(accounts_for_inc[i].name==a.account){
                    accounts_for_inc[i].balance_end-=a.amount;
                    break;
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
            tr("Видалити дохід"),
            tr("Рядок не вибрано!"));

}

//пошук
void Income_window::on_pushButton_search_clicked()
{
    proxyModel->search_vn=ui->lineEdit->text();
    proxyModel->setFilter();

}

//пошук ентер
void Income_window::on_lineEdit_inputRejected()
{
    on_pushButton_search_clicked();
}

//зміна валюти відображення
void Income_window::on_comboBox_currency_currentTextChanged(const QString &arg1)
{
    all_amounts (ui->comboBox_currency->currentText());
}

