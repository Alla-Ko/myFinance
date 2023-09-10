#include "accounts_window.h"
#include "ui_accounts_window.h"
#include "account_add.h"

Accounts_window::Accounts_window(QList <currency> cur, QList <account> accounts, QList <income> inc, QList <expense> exp, QList <transfer> tr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Accounts_window)
{
    ui->setupUi(this);
    myModel= new accounttablemodel();
    currencies_for_acc=cur;
    incomes_for_acc=inc;
    expenses_for_acc=exp;
    transfers_for_acc=tr;
    myModel->account_tablemodel=accounts;

    proxyModel = new MySortFilterProxyModel_acc();
    proxyModel->setSourceModel(myModel);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:QColor(53, 53, 53)}");
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
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

Accounts_window::~Accounts_window()
{
    delete ui;
}


MySortFilterProxyModel_acc::MySortFilterProxyModel_acc(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MySortFilterProxyModel_acc::filterAcceptsRow(int sourceRow,
                                              const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
   // QModelIndex index3 = sourceModel()->index(sourceRow, 3, sourceParent);
   // QModelIndex index4 = sourceModel()->index(sourceRow, 4, sourceParent);
  //  QModelIndex index5 = sourceModel()->index(sourceRow, 5, sourceParent);

    return (sourceModel()->data(index0).toString().contains(search_vn, Qt::CaseInsensitive)
            || sourceModel()->data(index1).toString().contains(search_vn, Qt::CaseInsensitive)
            ||sourceModel()->data(index2).toString().contains(search_vn, Qt::CaseInsensitive));

}

void MySortFilterProxyModel_acc::setFilter()
{

    invalidateFilter();
}






//нажата кнопка пошуку
void Accounts_window::on_pushButton_search_clicked()
{
    proxyModel->search_vn=ui->lineEdit->text();
    proxyModel->setFilter();
}

//додати рахунок
void Accounts_window::on_pushButton_add_clicked()
{
    QStringList currencies;
    for(int i=0;i<currencies_for_acc.size();i++){
        currencies.push_back(currencies_for_acc[i].name);
    }

    account_add window(currencies);

    window.ui->lineEdit_overdraft->setText("0.00");
    window.ui->lineEdit_balance->setText("0.00");
    window.ui->lineEdit_overdraft->setReadOnly(true);
    window.ui->radioButton_no->setChecked(true);
    window.ui->radioButton_yes->setCheckable(false);
    window.ui->label_avail_am->setVisible(false);
    window.ui->label_dostup_text->setVisible(false);
    window.ui->label_avail_cur->setVisible(false);
    window.ui->label_avail_am->setVisible(false);
    window.ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    window.newacc.balance=0.00;
    window.newacc.balance_end=0.00;

    auto result=window.exec();

    if(result==1){
        account a=window.newacc;
        a.balance_end=a.balance;
        if(a.name==""){
            QMessageBox::information(
                this,
                tr("Додати рахунок"),
                tr("Відсутня назва рахунку!"));
        }
        else{
            for(int i=0;i<myModel->account_tablemodel.size();i++){
                if(myModel->account_tablemodel[i].name.toUpper()==a.name.toUpper()){
                    QMessageBox::information(
                        this,
                        tr("Додати рахунок"),
                        tr("Рахунок з такою назвою вже існує!"));
                    return;
                }
            }
            myModel->account_tablemodel.push_back(a);
            proxyModel->invalidate();

        }
    }




}

//редагувати рахунок
void Accounts_window::on_pushButton_edit_clicked()
{



    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        int r=proxyModel->mapToSource(indexes[0]).row();
        QStringList currencies;
        for(int i=0;i<currencies_for_acc.size();i++){
            currencies.push_back(currencies_for_acc[i].name);
        }
        account acc=myModel->account_tablemodel[r];
        account_add window(currencies);
        window.ui->lineEdit_name->setText(acc.name);
        window.ui->lineEdit_balance->setText(QString::number(acc.balance));
        window.ui->lineEdit_overdraft->setText(QString::number(acc.overdraft));
        window.ui->label_avail_am->setVisible(false);

        if(acc.overdraft>0){
            window.ui->radioButton_yes->setChecked(true);
            window.ui->radioButton_no->setChecked(false);
            window.ui->lineEdit_overdraft->setReadOnly(false);
            QRegularExpression r_plus_minus("(^[-]{0,1}[0]{1}[.]{1}[0-9]{1,2})|(^[-]{0,1}[1-9]{1}[0-9]{0,9}[.]{1}[0-9]{1,2})");
            QValidator *validator_plus_minus=new QRegularExpressionValidator(r_plus_minus,this);
            window.ui->lineEdit_balance->setValidator(validator_plus_minus);


        }
        else{
            window.ui->radioButton_yes->setChecked(false);
            window.ui->radioButton_no->setChecked(true);
            window.ui->lineEdit_overdraft->setReadOnly(true);

        }
        if(acc.type=="Банківська картка"){
            window.ui->radioButton_yes->setCheckable(true);


        }
        if(acc.type=="Депозит"){
            window.ui->radioButton_no->setChecked(true);
            window.ui->radioButton_yes->setChecked(false);
            window.ui->radioButton_yes->setCheckable(false);
            window.ui->lineEdit_overdraft->setReadOnly(true);
            window.ui->lineEdit_overdraft->setText("0.00");
        }
        if(acc.type=="Готівка"){

            window.ui->radioButton_no->setChecked(true);
            window.ui->radioButton_yes->setChecked(false);
            window.ui->radioButton_yes->setCheckable(false);
            window.ui->lineEdit_overdraft->setReadOnly(true);
            window.ui->lineEdit_overdraft->setText("0.00");

        }
        if(acc.type=="Інше"){
            window.ui->radioButton_yes->setCheckable(true);

        }
        window.ui->comboBox_cur->setCurrentText(acc.currency);
        window.ui->comboBox_type->setCurrentText(acc.type);

        window.ui->label_dostup_text->setVisible(true);
        window.ui->label_avail_cur->setVisible(true);
        window.ui->label_avail_am->setVisible(true);
        window.ui->label_avail_cur->setText(acc.currency);
        window.ui->label_avail_am->setText(QString::number(acc.balance_end+acc.overdraft));
        window.newacc=acc;
        auto result=window.exec();
        if(result){
            account a=window.newacc;
            if(a.name==""){
                QMessageBox::information(
                    this,
                    tr("Редагувати рахунок"),
                    tr("Відсутня назва рахунку!"));
                return;
            }
            for(int i=0;i<myModel->account_tablemodel.size();i++){
                if(myModel->account_tablemodel[i].name.toUpper()==a.name.toUpper()&&i!=r){
                    QMessageBox::information(
                        this,
                        tr("Редагувати рахунок"),
                        tr("Рахунок з такою назвою вже існує!"));
                    return;
                }
            }
            account old=myModel->account_tablemodel[r];

            a.balance_end=old.balance_end-old.balance+a.balance;
            myModel->account_tablemodel[r]=a;
            proxyModel->invalidate();
            for(int i=0;i<incomes_for_acc.size();i++){
                if(incomes_for_acc[i].account==old.name){
                    incomes_for_acc[i].account=a.name;
                    incomes_for_acc[i].currency=a.currency;
                }
            }
            for(int i=0;i<expenses_for_acc.size();i++){
                if(expenses_for_acc[i].account==old.name){
                    expenses_for_acc[i].account=a.name;
                    expenses_for_acc[i].currency=a.currency;
                }
            }
            for(int i=0;i<transfers_for_acc.size();i++){
                if(transfers_for_acc[i].akk_name1==old.name){
                    transfers_for_acc[i].akk_name1=a.name;
                    transfers_for_acc[i].curr_name1=a.currency;
                }
                if(transfers_for_acc[i].akk_name2==old.name){
                    transfers_for_acc[i].akk_name2=a.name;
                    transfers_for_acc[i].curr_name2=a.currency;
                }
            }
        }
    }
    else  QMessageBox::information(
            this,
            tr("Редагувати рахунок"),
            tr("Рахунок не вибраний!"));


}

//видалити рахунок
void Accounts_window::on_pushButton_del_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        switch( QMessageBox::question(
            this,
            tr("Видалити рахунок"),
            tr("Ви дійсно хочете видалити?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {

            int r=proxyModel->mapToSource(indexes[0]).row();
            int lich=0;
            for(int i=0;i<incomes_for_acc.size();i++){
                if(incomes_for_acc[i].account==myModel->account_tablemodel[r].name){
                    QMessageBox::information(
                        this,
                        tr("Видалити рахунок"),
                        tr("Не можливо видалити рахунок, по якому є операції!"));
                    lich++;
                    break;
                }
            }
            if(!lich){
                for(int i=0;i<expenses_for_acc.size();i++){
                    if(expenses_for_acc[i].account==myModel->account_tablemodel[r].name){
                        QMessageBox::information(
                            this,
                            tr("Видалити рахунок"),
                            tr("Не можливо видалити рахунок, по якому є операції!"));
                        lich++;
                        break;
                    }
                }
            }
            if(!lich){
                for(int i=0;i<transfers_for_acc.size();i++){
                    if(transfers_for_acc[i].akk_name1==myModel->account_tablemodel[r].name||transfers_for_acc[i].akk_name2==myModel->account_tablemodel[r].name){
                        QMessageBox::information(
                            this,
                            tr("Видалити рахунок"),
                            tr("Не можливо видалити рахунок, по якому є операції!"));
                        lich++;
                        break;
                    }
                }
            }
            if(!lich){
                auto del_index=myModel->account_tablemodel.begin()+r;
                myModel->account_tablemodel.erase(del_index);
                proxyModel->invalidate();
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
            tr("Видалити рахунок"),
            tr("Рахунок не вибрано!"));
}



void Accounts_window::on_lineEdit_inputRejected()
{
    on_pushButton_search_clicked();
}

void Accounts_window::slotCustomMenuRequested(QPoint pos)
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
