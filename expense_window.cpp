#include "expense_window.h"
#include "ui_expense_window.h"
#include "account_add.h"

Expense_window::Expense_window(QList <currency> cur, QList <account> acc, QList <income> inc, QList <expense> exp, QList <transfer> tr,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Expense_window)
{
    ui->setupUi(this);
    myModel= new ExpenseTableModel();
    myModel->expense_tablemodel=exp;
    currencies_for_exp=cur;
    accounts_for_exp=acc;
    incomes_for_exp=inc;
    transfers_for_exp=tr;

    proxyModel = new MySortFilterProxyModel_exp();
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


    for(int i=0;i<currencies_for_exp.size();i++){
        ui->comboBox_currency->addItem(currencies_for_exp[i].name);
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

Expense_window::~Expense_window()
{
    delete ui;
}


MySortFilterProxyModel_exp::MySortFilterProxyModel_exp(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}
bool MySortFilterProxyModel_exp::filterAcceptsRow(int sourceRow,
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
void MySortFilterProxyModel_exp::setFilter()
{

    invalidateFilter();
}

void Expense_window::all_amounts (QString currency){
    double all=0.00, only=0.00;
    QMap<QString,double> curr_map;

    for(int i=0;i<currencies_for_exp.size();i++){
        curr_map[currencies_for_exp[i].name]=currencies_for_exp[i].rate;
    }
    for(int i=0;i<myModel->expense_tablemodel.size();i++){
        if(myModel->expense_tablemodel[i].currency==currency){
            only+=myModel->expense_tablemodel[i].amount;
        }
        all+=double(myModel->expense_tablemodel[i].amount*curr_map[myModel->expense_tablemodel[i].currency])/curr_map[currency];
    }
    ui->lineEdit_am_all->setText(QString::number(all));
    ui->lineEdit_am_cur->setText(QString::number(only));

}


void Expense_window::slotCustomMenuRequested(QPoint pos)
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




//пошук
void Expense_window::on_pushButton_search_clicked()
{
    proxyModel->search_vn=ui->lineEdit->text();
    proxyModel->setFilter();

}

//пошук ентер
void Expense_window::on_lineEdit_returnPressed()
{
    on_pushButton_search_clicked();
}

//додати витрату
void Expense_window::on_pushButton_add_clicked()
{
    Expence_add window(accounts_for_exp);
    for(int i=0;i<accounts_for_exp.size();i++){
        window.acc_amount_end_map[accounts_for_exp[i].name]=accounts_for_exp[i].balance_end+accounts_for_exp[i].overdraft;
    }
    //window.acc_amount_end_map[myModel->expense_tablemodel[r].account]-=myModel->income_tablemodel[r].amount;
    window.ui->label_avail_am->setText(QString::number(window.acc_amount_end_map[window.ui->comboBox_account->currentText()]));



    auto result=window.exec();
    if(result==1){
        expense a=window.newexp;
        if(!a.amount){
            QMessageBox::information(
                this,
                tr("Додати витрату"),
                tr("Не внесена сума!"));
            return;
        }
        myModel->expense_tablemodel.push_back(a);
        proxyModel->invalidate();
        all_amounts (ui->comboBox_currency->currentText());
        for(int i=0;i<accounts_for_exp.size();i++){
            if(accounts_for_exp[i].name==a.account){
                accounts_for_exp[i].balance_end-=a.amount;
                break;
            }
        }
    }

}

//редагувати витрату
void Expense_window::on_pushButton_edit_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();
        int r=proxyModel->mapToSource(indexes[0]).row();
        Expence_add window(accounts_for_exp);
        window.ui->dateTimeEdit->setDateTime(myModel->expense_tablemodel[r].date);
        window.ui->lineEdit_amount->setText(QString::number(myModel->expense_tablemodel[r].amount));
        window.ui->lineEdit_currency->setText(myModel->expense_tablemodel[r].currency);
        window.ui->lineEdit_comments->setText(myModel->expense_tablemodel[r].comment);
        window.ui->comboBox_account->setCurrentText(myModel->expense_tablemodel[r].account);
        window.ui->comboBox_type->setCurrentText(myModel->expense_tablemodel[r].type);
        QPalette sample_palette;
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        window.ui->label_avail_am->setPalette(sample_palette);
        window.ui->label_avail_cur->setPalette(sample_palette);
        window.ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        window.ui->label_avail_cur->setText(window.acc_map[window.ui->comboBox_account->currentText()]);

        for(int i=0;i<accounts_for_exp.size();i++){
            window.acc_amount_end_map[accounts_for_exp[i].name]=accounts_for_exp[i].balance_end+accounts_for_exp[i].overdraft;
        }
        window.acc_amount_end_map[myModel->expense_tablemodel[r].account]+=myModel->expense_tablemodel[r].amount;
        window.ui->label_avail_am->setText(QString::number(window.acc_amount_end_map[myModel->expense_tablemodel[r].account]-myModel->expense_tablemodel[r].amount));




        auto result=window.exec();
        if(result==1){
            expense a=window.newexp;
            if(!a.amount){
                QMessageBox::information(
                    this,
                    tr("Редагувати витрату"),
                    tr("Не внесена сума!"));
                return;
            }
            for(int i=0;i<accounts_for_exp.size();i++){
                if(accounts_for_exp[i].name==myModel->expense_tablemodel[r].account){
                    accounts_for_exp[i].balance_end+= myModel->expense_tablemodel[r].amount;
                }
                if(accounts_for_exp[i].name==a.account){
                    accounts_for_exp[i].balance_end-= a.amount;
                }
            }

            myModel->expense_tablemodel[r]=a;
            proxyModel->invalidate();
            all_amounts (ui->comboBox_currency->currentText());
        }
    }
    else  QMessageBox::information(
            this,
            tr("Редагувати витрату"),
            tr("Рядок не вибраний!"));

}

//видалити витрату
void Expense_window::on_pushButton_del_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        switch( QMessageBox::question(
            this,
            tr("Видалити витрату"),
            tr("Ви дійсно хочете видалити?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {

            int r=proxyModel->mapToSource(indexes[0]).row();
            expense a=myModel->expense_tablemodel[r];
            auto del_index=myModel->expense_tablemodel.begin()+r;
            myModel->expense_tablemodel.erase(del_index);
            proxyModel->invalidate();
            all_amounts (ui->comboBox_currency->currentText());
            for(int i=0;i<accounts_for_exp.size();i++){
                if(accounts_for_exp[i].name==a.account){
                    accounts_for_exp[i].balance_end+=a.amount;
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
            tr("Видалити витрату"),
            tr("Рядок не вибрано!"));

}


void Expense_window::on_comboBox_currency_currentTextChanged(const QString &arg1)
{
    all_amounts (ui->comboBox_currency->currentText());
}

