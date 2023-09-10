
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_turnover.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dark();
    //стартове заповнення валют на випадок, коли файли не втягатимуться, а робитимуться з нуля
    currency cur;
    cur.name="UAH";
    cur.rate=1.0;
    cur.comment="основна валюта";
    currencies.push_back(cur);
    cur.name="USD";
    cur.rate=37.5;
    cur.comment="долар";
    currencies.push_back(cur);
    cur.name="EUR";
    cur.rate=42.1;
    cur.comment="євро";
    currencies.push_back(cur);




    QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    QTime b;
    QDateTime c(a,b);
    ui->dateTimeEdit_start->setDateTime(c);
    ui->dateTimeEdit_start->setCalendarPopup(true);
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_end->setCalendarPopup(true);
    ui->dateTimeEdit_start->setDisplayFormat("dd.MM.yyyy");
    ui->dateTimeEdit_end->setDisplayFormat("dd.MM.yyyy");

    ui->lineEdit_am_cur->setReadOnly(true);
    ui->lineEdit_am_all->setReadOnly(true);
    for(int i=0;i<currencies.size();i++){
        ui->comboBox->addItem(currencies[i].name);
    }


    myModel= new mainwindowtablemodel();
    myModel->account_tablemodel=accounts;
    proxyModel = new MySortFilterProxyModel_main();
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


    ui->tableView->horizontalHeader()->resizeSection(0, 150);
    ui->tableView->horizontalHeader()->resizeSection(1, 120);
    ui->tableView->setSortingEnabled(true);
    //ui->tableView-> sortByColumn ( 0 , Qt :: AscendingOrder );
    ui->tableView->horizontalHeader()->setSelectionMode( QAbstractItemView::NoSelection );
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->sortByColumn(-1, Qt::DescendingOrder);

    // Устанавливаем Контекстное Меню
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_pushButton_oborot_clicked()));
    // Подключаем СЛОТ вызова контекстного меню
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));

    proxyModel->invalidate();
    double all=0.00, only=0.00;
    QMap<QString,double> curr_map;

    for(int i=0;i<currencies.size();i++){
        curr_map[currencies[i].name]=currencies[i].rate;
    }
    for(int i=0;i<myModel->account_tablemodel.size();i++){
        if(myModel->account_tablemodel[i].currency==ui->comboBox->currentText()){
            only+=myModel->account_tablemodel[i].balance_end;
        }
        all+=double(myModel->account_tablemodel[i].balance_end*curr_map[myModel->account_tablemodel[i].currency])/curr_map[ui->comboBox->currentText()];
    }
    ui->lineEdit_am_all->setText(QString::number(all));
    ui->lineEdit_am_cur->setText(QString::number(only));


//Save();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//сортування в таблиці

MySortFilterProxyModel_main::MySortFilterProxyModel_main(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MySortFilterProxyModel_main::filterAcceptsRow(int sourceRow,
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

void MySortFilterProxyModel_main::setFilter()
{

    invalidateFilter();
}





//МЕНЮ ФАЙЛ---------------------------------------------
//новий файл (очистити поточну базу)
void MainWindow::on_action_new_triggered()
{

    currencies.clear();
    accounts.clear();
    incomes.clear();
    expenses.clear();
    transfers.clear();


    //cтартове заповнення валют

    currency cur;
    cur.name="UAH";
    cur.rate=1.0;
    cur.comment="основна валюта";
    currencies.push_back(cur);
    cur.name="USD";
    cur.rate=37.5;
    cur.comment="долар";
    currencies.push_back(cur);
    cur.name="EUR";
    cur.rate=42.1;
    cur.comment="євро";
    currencies.push_back(cur);
    pererah_bal();
    myModel->account_tablemodel=accounts;
    proxyModel->invalidate();
    all_amounts ();
    QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    QTime b;
    QDateTime c(a,b);
    ui->dateTimeEdit_start->setDateTime(c);
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());

}

//імпорт з файла
void MainWindow::on_action_open_triggered()
{
    /*
    currencies.clear();
    accounts.clear();
    incomes.clear();
    expenses.clear();
    transfers.clear();
    */
    Open();
    while(ui->comboBox->count() > 0){
        for(int i = 0; i < ui->comboBox->count(); i++){
            ui->comboBox->removeItem(i);
        }
    }
    for(int i=0;i<currencies.size();i++){
        if(currencies[i].name!="")
            ui->comboBox->addItem(currencies[i].name);
    }
    pererah_bal();
    myModel->account_tablemodel=accounts;
    proxyModel->invalidate();
    all_amounts ();
    QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    QTime b;
    QDateTime c(a,b);
    ui->dateTimeEdit_start->setDateTime(c);
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());

}

//експорт у файл
void MainWindow::on_action_save_triggered()
{
    Save();
}

//НОВІ ВІКНА--------------------------------------------
//має відкритися вікно для редагування валют і курсів
void MainWindow::on_action_val_triggered()
{
    currency_window window (currencies, accounts, incomes, expenses, transfers);

    window.setModal(true);
    window.exec();
    currencies=window.myModel->currencies_tablemodel;
    accounts=window.accounts_for_cur;
    incomes=window.incomes_for_cur;
    expenses=window.expenses_for_cur;
    transfers=window.transfers_for_cur;
    pererah_bal();

    while(ui->comboBox->count() > 0){
        for(int i = 0; i < ui->comboBox->count(); i++){
            ui->comboBox->removeItem(i);
        }
    }
    for(int i=0;i<currencies.size();i++){
        if(currencies[i].name!="")
            ui->comboBox->addItem(currencies[i].name);
    }
    pererah_bal();
    myModel->account_tablemodel=accounts;
    proxyModel->invalidate();
    all_amounts ();
    QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    QTime b;
    QDateTime c(a,b);
    ui->dateTimeEdit_start->setDateTime(c);
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());

}

//має відкритися вікно для перегляду залишків і редагування рахунків
void MainWindow::on_action_rah_triggered()
{
    Accounts_window window (currencies, accounts, incomes, expenses, transfers);

    window.setModal(true);
    window.exec();
    currencies=window.currencies_for_acc;
    accounts=window.myModel->account_tablemodel;
    incomes=window.incomes_for_acc;
    expenses=window.expenses_for_acc;
    transfers=window.transfers_for_acc;
    pererah_bal();
    myModel->account_tablemodel=accounts;
    proxyModel->invalidate();
    all_amounts ();
    QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    QTime b;
    QDateTime c(a,b);
    ui->dateTimeEdit_start->setDateTime(c);
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());


}

//має відкритися вікно для перегляду доходів і додавання/видалення/редагування
void MainWindow::on_action_doh_triggered()
{
    if(accounts.size()>0){
        Income_window window (currencies, accounts, incomes, expenses, transfers);
        window.exec();
        //currencies=window.currencies_for_inc;
        //accounts=window.accounts_for_inc;;
        incomes=window.myModel->income_tablemodel;
        // expenses=window.expenses_for_inc;
        //transfers=window.transfers_for_inc;
        pererah_bal();
        myModel->account_tablemodel=accounts;
        proxyModel->invalidate();
        all_amounts ();
        QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
        QTime b;
        QDateTime c(a,b);
        ui->dateTimeEdit_start->setDateTime(c);
        ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    }
    else{

        switch( QMessageBox::question(
            this,
            tr("Доходи"),
            tr("У Вас немає жодного рахунку на який можна отримати дохід. \nПерейти до створення рахунків?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {
            on_action_rah_triggered();
        }

        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
}

//має відкритися вікно для перегляду витрат і додавання/видалення/редагування
void MainWindow::on_action_vytr_triggered()
{
    if(accounts.size()>0){
        Expense_window window (currencies, accounts, incomes, expenses, transfers);
        window.exec();
        //currencies=window.currencies_for_exp;
        //accounts=window.accounts_for_exp;
        //incomes=window.incomes_for_exp;
        expenses=window.myModel->expense_tablemodel;
        //transfers=window.transfers_for_exp;
        pererah_bal();
        myModel->account_tablemodel=accounts;
        proxyModel->invalidate();
        all_amounts ();
        QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
        QTime b;
        QDateTime c(a,b);
        ui->dateTimeEdit_start->setDateTime(c);
        ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    }
    else{
        switch( QMessageBox::question(
            this,
            tr("Витрати"),
            tr("У Вас немає жодного рахунку для здійснення витрат. \nПерейти до створення рахунків?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {
            on_action_rah_triggered();
        }
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
}


//має відкритися вікно для перегляду переказів між рахунками і додавання/видалення/редагування
void MainWindow::on_action_per_triggered()
{
    if(accounts.size()>1){
        Transfer_window window (currencies, accounts, incomes, expenses, transfers);
        window.exec();
        transfers=window.myModel->transfer_tablemodel;
        pererah_bal();
        myModel->account_tablemodel=accounts;
        proxyModel->invalidate();
        all_amounts ();
        QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
        QTime b;
        QDateTime c(a,b);
        ui->dateTimeEdit_start->setDateTime(c);
        ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    }
    else{
        switch( QMessageBox::question(
            this,
            tr("Витрати"),
            tr("Для здійснення переказу потрібно не менше 2 рахунків. \nПерейти до створення рахунків?"),

            QMessageBox::Yes |
                QMessageBox::Cancel ) )
        {
        case QMessageBox::Yes:
        {
            on_action_rah_triggered();
        }
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
}

//ЗВІТИ-------------------------------------------



//звіт по доходах
void MainWindow::on_action_3_triggered()
{
    QMap<QString,double> curr_map;

    for(int i=0;i<currencies.size();i++){
        curr_map[currencies[i].name]=currencies[i].rate;
    }
    if (incomes.size()>0){
        Chart_incomes window(incomes,curr_map);
        window.exec();
    }
    else QMessageBox::information(this, "Структура доходів", "Відсутні доходи", QMessageBox::Ok);
}

//звіт по витратах
void MainWindow::on_action_10_triggered()
{
    QMap<QString,double> curr_map;

    for(int i=0;i<currencies.size();i++){
        curr_map[currencies[i].name]=currencies[i].rate;
    }
    if (expenses.size()>0){
        Chart_expenses window(expenses,curr_map);
        window.exec();
    }
    else QMessageBox::information(this, "Структура витрат", "Відсутні витрати", QMessageBox::Ok);
}

//звіт по залишкам
void MainWindow::on_action_11_triggered()
{

    if (accounts.size()>0){
        QMap<QString,double> map_rate;
        for(int i=0;i<currencies.size();i++){
            map_rate[currencies[i].name]=currencies[i].rate;
        }

        QList<operation>operations_all;
        operation op, op1;
        for(int i=0;i<incomes.size();i++){
            op.account=incomes[i].account;//рахунок
            op.am_inc=(double)incomes[i].amount/map_rate[incomes[i].currency];//+в грн
            op.am_exp=0.00;//- в грн
            op.date=incomes[i].date;//дата
            op.balance=0.00;//баланс поки не можемо порахувати
            operations_all.push_back(op);
        }

        for(int i=0;i<expenses.size();i++){
            op.account=expenses[i].account;//рахунок
            op.am_exp=(double)expenses[i].amount/map_rate[expenses[i].currency];//- в грн
            op.am_inc=0.00; //+в грн
            op.date=expenses[i].date;
            op.balance=0.00;
            operations_all.push_back(op);

        }
        for(int i=0;i<transfers.size();i++){

            op.account=transfers[i].akk_name1;
            op1.account=transfers[i].akk_name2;
            op.am_exp=(double)transfers[i].amount1/map_rate[transfers[i].curr_name1];//- в грн
            op.am_inc=0.0;
            op1.am_inc=(double)transfers[i].amount2/map_rate[transfers[i].curr_name2];//- в грн
            op1.am_exp=0.0;
            op.date=transfers[i].date;
            op1.date=transfers[i].date;
            op.balance=0.00;
            operations_all.push_back(op);
            operations_all.push_back(op1);
        }
        QMap <QDateTime,int> tymch;
        for(int i=0;i<operations_all.size();i++){
            operations_all[i].forsortnumber=i;

            tymch[operations_all[i].date]=operations_all[i].forsortnumber;
        }
        QList<operation>operations_all_sort;
        if(operations_all.size()>1){
            for(auto it=tymch.cbegin(), end = tymch.cend(); it != end; ++it){
                operations_all_sort.push_back(operations_all[it.value()]);
            }
        }
        operations_all=operations_all_sort;//вже посортоване по даті
        for(int i=0;i<operations_all.size();i++){
        }

        double amount;
        QMap <QDateTime,double>map;

        for(int i=0;i<accounts.size();i++)    {
            amount+=(double) accounts[i].balance/map_rate[incomes[i].currency];//- в грн
        }
        map[operations_all[0].date.addDays(-10)]= amount;
        for(int i=0;i<operations_all.size();i++){
            amount=amount+operations_all[i].am_inc-operations_all[i].am_exp;
            map[operations_all[i].date]= amount;
        }

        QLineSeries *series = new QLineSeries();
        QChart *chart = new QChart();
        QDateTimeAxis *axisX = new QDateTimeAxis;
        QValueAxis *axisY = new QValueAxis;


        for(auto it=map.cbegin(), end = map.cend(); it != end; ++it){
            series->append(it.key().toMSecsSinceEpoch(),it.value());
        }


        chart->addSeries(series);

        axisX->setTickCount(10);
        axisX->setFormat("MMM yyyy");
        axisX->setTitleText("Дата");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        axisY->setLabelFormat("%i");
        axisY->setTitleText("Залишок коштів в екв. UAH");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        chart->legend()->hide();
        chart->setTitle("Динаміка залишків на рахунках");


        chart->setAnimationOptions(QChart::AllAnimations);
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);



        //запуск вікна з графіком
        Chart_accounts window1;
        chartView->setParent(window1.ui->frame);
        window1.exec();

    }
    else QMessageBox::information(this, "Звіт по залишках", "Відсутні рахунки", QMessageBox::Ok);



}


//має відкритися вікно з довідкою про програму
void MainWindow::on_action_about_triggered()
{
    Info window;
    window.exec();
}


//вихід
void MainWindow::on_action_exit_triggered()
{
    QApplication::quit();;
}

//вихід кнопка
void MainWindow::on_pushButton_8_clicked()
{
    on_action_exit_triggered();
}

//кнопка валюти
void MainWindow::on_pushButton_clicked()
{
    on_action_val_triggered();

}

//кнопка рахунки
void MainWindow::on_pushButton_3_clicked()
{
    on_action_rah_triggered();
}

//кнопка доходи
void MainWindow::on_pushButton_4_clicked()
{
    on_action_doh_triggered();
}

//кнопка витрати
void MainWindow::on_pushButton_5_clicked()
{
    on_action_vytr_triggered();
}

void MainWindow::pererah_bal(){
    QMap<QString,double> map;
    for(int i=0;i<accounts.size();i++){
        map[accounts[i].name]=accounts[i].balance;
    }
    for(int i=0;i<incomes.size();i++){
        map[incomes[i].account]+=incomes[i].amount;
    }
    for(int i=0;i<expenses.size();i++){
        map[expenses[i].account]-=expenses[i].amount;
    }
    for(int i=0;i<transfers.size();i++){
        map[transfers[i].akk_name1]-=transfers[i].amount1;
        map[transfers[i].akk_name2]+=transfers[i].amount2;
    }
    for(int i=0;i<accounts.size();i++){
        accounts[i].balance_end=map[accounts[i].name];
    }

}

void MainWindow::on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime newdate=ui->dateTimeEdit_start->dateTime();
    if(newdate>ui->dateTimeEdit_end->dateTime())
        ui->dateTimeEdit_end->setDateTime(newdate);

}

//зайве
void MainWindow::on_dateTimeEdit_end_dateChanged(const QDate &date)
{

}


void MainWindow::on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime newdate=ui->dateTimeEdit_end->dateTime();
    if(newdate<ui->dateTimeEdit_start->dateTime())
        ui->dateTimeEdit_start->setDateTime(newdate);
}

//кнопка перекази
void MainWindow::on_pushButton_6_clicked()
{
    on_action_per_triggered();
}

//звіт по доходах кнопка
void MainWindow::on_pushButton_7_clicked()
{
    on_action_3_triggered();
}

//звіт по витратах кнопка
void MainWindow::on_pushButton_10_clicked()
{
    on_action_10_triggered();
}

//звіт по залишках
void MainWindow::on_pushButton_9_clicked()
{
    on_action_11_triggered();
}

//пошук серед рахунків
void MainWindow::on_pushButton_11_clicked()
{
    proxyModel->search_vn=ui->lineEdit_search->text();
    proxyModel->setFilter();

}

//пошук серед рахунків ентер


//обороти по рахунку кнопка
void MainWindow::on_pushButton_oborot_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(ui->tableView->selectionModel()->hasSelection()){
        QModelIndexList indexes = selectionModel->selectedIndexes();

        int r=proxyModel->mapToSource(indexes[0]).row();

        account acc=myModel->account_tablemodel[r];
        QList<operation>operations;
        QList<operation>operations_all;
        operation op;
        for(int i=0;i<incomes.size();i++){
            if(incomes[i].account==acc.name){
                op.account="";
                op.am_inc=incomes[i].amount;
                op.am_exp=0.00;
                op.comment=incomes[i].comment;
                op.date=incomes[i].date;
                op.type_glob="дохід";
                op.type=incomes[i].type;
                op.balance=0.00;
                operations_all.push_back(op);
            }
        }

        for(int i=0;i<expenses.size();i++){
            if(expenses[i].account==acc.name){
                op.account="";
                op.am_exp=expenses[i].amount;
                op.am_inc=0.00;
                op.comment=expenses[i].comment;
                op.date=expenses[i].date;
                op.type_glob="витрата";
                op.type=expenses[i].type;
                op.balance=0.00;
                operations_all.push_back(op);
            }
        }
        for(int i=0;i<transfers.size();i++){
            if(transfers[i].akk_name1==acc.name){
                op.account=transfers[i].akk_name2;
                op.am_exp=transfers[i].amount1;
                op.am_inc=0.00;
                op.comment=transfers[i].comment;
                op.date=transfers[i].date;
                op.type_glob="переказ";
                op.type="";
                op.balance=0.00;
                operations_all.push_back(op);
            }
            if(transfers[i].akk_name2==acc.name){
                op.account=transfers[i].akk_name1;
                op.am_inc=transfers[i].amount2;
                op.am_exp=0.00;
                op.comment=transfers[i].comment;
                op.date=transfers[i].date;
                op.type_glob="переказ";
                op.type="";
                op.balance=0.00;
                operations_all.push_back(op);
            }
        }



        QMap <QDateTime,int> tymch;
        for(int i=0;i<operations_all.size();i++){
            operations_all[i].forsortnumber=i;

            tymch[operations_all[i].date]=operations_all[i].forsortnumber;
        }

        QList<operation>operations_all_sort;
        if(operations_all.size()>1){
            for(auto it=tymch.cbegin(), end = tymch.cend(); it != end; ++it){
                operations_all_sort.push_back(operations_all[it.value()]);
            }
        }
        operations_all=operations_all_sort;




        if (operations_all.size()>0){
            operations_all[0].balance=acc.balance+operations_all[0].am_inc-operations_all[0].am_exp;
            if (operations_all.size()>1){
                for(int i=1;i<operations_all.size();i++){
                    operations_all[i].balance=operations_all[i-1].balance+operations_all[i].am_inc-operations_all[i].am_exp;
                }
            }
        }

        for(int i=0;i<operations_all.size();i++){
            if(operations_all[i].date>=ui->dateTimeEdit_start->dateTime()&&
                operations_all[i].date<=ui->dateTimeEdit_end->dateTime()){
                operations.push_back(operations_all[i]);
            }
        }
        double bal_end=0.00;
        double inc_am=0.0;
        double exp_am=0.0;
        double bal_start=0.0;
        if(operations.size()>0){
            bal_end=operations[operations.size()-1].balance;
            bal_start=operations[0].balance-operations[0].am_inc+operations[0].am_exp;
        }
        else {
            bal_end=acc.balance_end;
            bal_start=acc.balance;
        }


        for(int i=0;i<operations.size();i++){
            inc_am+=operations[i].am_inc;
            exp_am+=operations[i].am_exp;
        }

        Turnover window(operations);
        window.ui->lineEdit_balance_end->setText(QString::number(bal_end));
        window.ui->lineEdit_balance_start->setText(QString::number(bal_start));
        window.ui->lineEdit_inc_am->setText(QString::number(inc_am));
        window.ui->lineEdit_exp_am->setText(QString::number(exp_am));

        window.ui->dateTimeEdit_start->setDateTime(ui->dateTimeEdit_start->dateTime());
        window.ui->dateTimeEdit_end->setDateTime(ui->dateTimeEdit_end->dateTime());

        window.ui->dateTimeEdit_start->setDisabled(true);
        window.ui->dateTimeEdit_end->setDisabled(true);

        window.ui->lineEdit_acc->setText(acc.name);

        window.ui->lineEdit_acc->setReadOnly(true);
        window.ui->label_cur1->setText(acc.currency);
        window.ui->label_cur2->setText(acc.currency);
        window.ui->label_cur3->setText(acc.currency);
        window.ui->label_cur4->setText(acc.currency);

        window.ui->lineEdit_balance_start->setReadOnly(true);
        window.ui->lineEdit_balance_end->setReadOnly(true);
        window.ui->lineEdit_inc_am->setReadOnly(true);
        window.ui->lineEdit_exp_am->setReadOnly(true);









        window.exec();

    }
    else  QMessageBox::information(
            this,
            tr("Обороти по рахунку"),
            tr("Рядок не вибраний!"));
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * accCur = new QAction(tr("Перейти до рахунків"), this);
    QAction * incCur = new QAction(tr("Перейти до доходів"), this);
    QAction * expCur = new QAction(tr("Перейти до витрат"), this);
    QAction * trCur = new QAction(tr("Перейти до переказів"), this);
    QAction * oborotCur = new QAction(tr("Обороти по рахунку"), this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(accCur, SIGNAL(triggered()), this, SLOT(on_action_rah_triggered()));
    connect(incCur, SIGNAL(triggered()), this, SLOT(on_action_doh_triggered()));
    connect(expCur, SIGNAL(triggered()), this, SLOT(on_action_vytr_triggered()));
    connect(trCur, SIGNAL(triggered()), this, SLOT(on_action_per_triggered()));
    connect(oborotCur, SIGNAL(triggered()), this, SLOT(on_pushButton_oborot_clicked()));
    /* Устанавливаем действия в меню */
    menu->addAction(accCur);
    menu->addAction(incCur);
    menu->addAction(expCur);
    menu->addAction(trCur);
    menu->addAction(oborotCur);
    /* Вызываем контекстное меню */
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

    //connect(ui->lineEdit_search, SIGNAL(Keys.enterPressed()), this, SLOT(on_action_rah_triggered()));
}



//зайве
void MainWindow::on_lineEdit_search_cursorPositionChanged(int arg1, int arg2)
{

}


void MainWindow::on_lineEdit_search_inputRejected()
{
    on_pushButton_11_clicked();
}


void MainWindow::on_lineEdit_search_textChanged(const QString &arg1)
{
    on_pushButton_11_clicked();
}

void MainWindow::all_amounts (){



    double all=0.00, only=0.00;
    QMap<QString,double> curr_map;

    for(int i=0;i<currencies.size();i++){
        curr_map[currencies[i].name]=currencies[i].rate;
    }
    for(int i=0;i<myModel->account_tablemodel.size();i++){
        if(myModel->account_tablemodel[i].currency==ui->comboBox->currentText()){
            only+=myModel->account_tablemodel[i].balance_end;
        }
        all+=double(myModel->account_tablemodel[i].balance_end*curr_map[myModel->account_tablemodel[i].currency])/curr_map[ui->comboBox->currentText()];
    }
    ui->lineEdit_am_all->setText(QString::number(all));
    ui->lineEdit_am_cur->setText(QString::number(only));
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox->hasFocus())
    all_amounts ();
}

void MainWindow::dark()
{
    // Создаём палитру для тёмной темы оформления
    QPalette darkPalette;

    // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    // Устанавливаем данную палитру
    qApp->setPalette(darkPalette);
}

void MainWindow::Open(){

    //читання з файлу
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir d{path};
    if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath()))
    {
        QList <currency> cur_open;
        currency cur;
        QList <account> acc_open;
        account acc;
        QList <income> inc_open;
        income inc;
        QList <expense> exp_open;
        expense exp;
        QList <transfer> tr_open;
        transfer tr;

        //currencies-----------------------------------------------------------------------------------------
        QFile file_cur{"currency.dat"};
        if (!file_cur.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Unable to open file", file_cur.errorString());
            return;
        }
        QDataStream in_cur(&file_cur);
        while(!in_cur.atEnd())
        {
            in_cur >> cur.name;
            in_cur >> cur.rate;
            in_cur >> cur.comment;

            cur_open.push_back(cur);
        }
        file_cur.close();
        if (cur_open.isEmpty()) {
            QMessageBox::information(this, "Нема даних у файлі",
                                     "Файл, який ви намагаєтеся відкрити, не містить валют.");
        } else {
            currencies=cur_open;
        }

        //accounts-----------------------------------------------------------------------------------------
        QFile file_acc{"accounts.dat"};
        if (!file_acc.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Unable to open file", file_acc.errorString());
            return;
        }
        QDataStream in_acc(&file_acc);
        while(!in_acc.atEnd())
        {
            in_acc >> acc.name;
            in_acc >> acc.type;
            in_acc >> acc.overdraft;
            in_acc >> acc.currency;
            in_acc >> acc.balance;
            in_acc >> acc.balance_end;
            acc_open.push_back(acc);
        }
        file_acc.close();
        if (acc_open.isEmpty()) {
            QMessageBox::information(this, "Нема даних у файлі",
                                     "Файл, який ви намагаєтеся відкрити, не містить рахунків.");
        } else {
            accounts=acc_open;
        }

        //incomes-----------------------------------------------------------------------------------------
        QFile file_inc{"incomes.dat"};
        if (!file_inc.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Unable to open file", file_inc.errorString());
            return;
        }
        QDataStream in_inc(&file_inc);
        while(!in_inc.atEnd())
        {
            in_inc >> inc.date;
            in_inc >> inc.amount;
            in_inc >> inc.currency;
            in_inc >> inc.account;
            in_inc >> inc.type;
            in_inc >> inc.comment;

            inc_open.push_back(inc);
        }
        file_inc.close();
        if (inc_open.isEmpty()) {
            QMessageBox::information(this, "Нема даних у файлі",
                                     "Файл, який ви намагаєтеся відкрити, не містить доходів.");
        } else {
            incomes=inc_open;
        }




        //expenses-----------------------------------------------------------------------------------------
        QFile file_exp{"expenses.dat"};
        if (!file_exp.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Unable to open file", file_exp.errorString());
            return;
        }
        QDataStream in_exp(&file_exp);
        while(!in_exp.atEnd())
        {
            in_exp >> exp.date;
            in_exp >> exp.amount;
            in_exp >> exp.currency;
            in_exp >> exp.account;
            in_exp >> exp.type;
            in_exp >> exp.comment;
            exp_open.push_back(exp);
        }
        file_exp.close();
        if (exp_open.isEmpty()) {
            QMessageBox::information(this, "Нема даних у файлі",
                                     "Файл, який ви намагаєтеся відкрити, не містить витрат.");
        } else {
            expenses=exp_open;
        }


        //transfers-----------------------------------------------------------------------------------------
        //QFile file_tr(QStringLiteral(":/new/prefix1/DAT/transfers.dat"));
        QFile file_tr{"transfers.dat"};
        if (!file_tr.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Unable to open file", file_tr.errorString());
            return;
        }
        QDataStream in_tr(&file_tr);
        while(!in_tr.atEnd())
        {
            in_tr >> tr.date;
            in_tr >> tr.akk_name1;
            in_tr >> tr.curr_name1;
            in_tr >> tr.amount1;
            in_tr >> tr.akk_name2;
            in_tr >> tr.curr_name2;
            in_tr >> tr.amount2;
            in_tr >> tr.comment;
            tr_open.push_back(tr);
        }
        file_tr.close();
        if (tr_open.isEmpty()) {
            QMessageBox::information(this, "Нема даних у файлі",
                                     "Файл, який ви намагаєтеся відкрити, не містить витрат.");
        } else {
            transfers=tr_open;
        }
    }
}


void MainWindow::Save(){
    switch( QMessageBox::question(
        this,
        tr("Експортувати дані у файл"),
        tr("Всі попередні дані в файлі будуть\nзамінені на нові, продовжити?"),

        QMessageBox::Yes |
            QMessageBox::Cancel ) )
    {
    case QMessageBox::Yes:
    {
    //запис у файл

    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir d{path};
    if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath()))
    {
        //currencies--------------------------------------------------------

        qDebug() << "currencies in" << QDir::currentPath();
        QFile file_cur{"currency.dat"};
        //QFile file_cur(QStringLiteral(":/new/prefix1/DAT/currency.dat"));
        if (!file_cur.open(QIODevice::WriteOnly))
        {
           QMessageBox::information(this, tr("Не вдалося відкрити файл"), file_cur.errorString());
            return;
        }
        QDataStream out_cur(&file_cur);
        for(const currency &con:currencies)
        {
            out_cur << con.name;
            out_cur << con.rate;
            out_cur << con.comment;
        }
        file_cur.close();

        //transfers-------------------------------------------------------------
        //QFile file_tr(QStringLiteral(":/new/prefix1/DAT/transfers.dat"));
        qDebug() << "transfers in" << QDir::currentPath();
        QFile file_tr{"transfers.dat"};
        if (!file_tr.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Не вдалося відкрити файл"), file_tr.errorString());
            return;
        }
        QDataStream out_tr(&file_tr);
        for(const transfer &con:transfers)
        {
            out_tr << con.date;
            out_tr << con.akk_name1;
            out_tr << con.curr_name1;
            out_tr << con.amount1;
            out_tr << con.akk_name2;
            out_tr << con.curr_name2;
            out_tr << con.amount2;
            out_tr << con.comment;
        }
        file_tr.close();
        //expenses-------------------------
        //QFile file_exp(QStringLiteral(":/new/prefix1/DAT/expenses.dat"));
        qDebug() << "expenses in" << QDir::currentPath();
        QFile file_exp{"expenses.dat"};
        if (!file_exp.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Не вдалося відкрити файл"), file_exp.errorString());
            return;
        }
        QDataStream out_exp(&file_exp);
        for(const expense &con:expenses)
        {
            out_exp << con.date;
            out_exp << con.amount;
            out_exp << con.currency;
            out_exp << con.account;
            out_exp << con.type;
            out_exp << con.comment;
        }
        file_exp.close();
        //incomes-----------------------------------------------------------------
        //QFile file_inc(QStringLiteral(":/new/prefix1/DAT/incomes.dat"));
        qDebug() << "incomes in" << QDir::currentPath();
        QFile file_inc{"incomes.dat"};
        if (!file_inc.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Не вдалося відкрити файл"), file_inc.errorString());
            return;
        }
        QDataStream out_inc(&file_inc);
        for(const income &con:incomes)
        {
            out_inc << con.date;
            out_inc << con.amount;
            out_inc << con.currency;
            out_inc << con.account;
            out_inc << con.type;
            out_inc << con.comment;
        }
        file_inc.close();
        //accounts---------------------------------------
        //QFile file_acc(QStringLiteral(":/new/prefix1/DAT/accounts.dat"));
        qDebug() << "accounts in" << QDir::currentPath();
        QFile file_acc{"accounts.dat"};
        if (!file_acc.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Не вдалося відкрити файл"), file_acc.errorString());
            return;
        }

        QDataStream out_acc(&file_acc);
        for(const account &con:accounts)
        {
            out_acc << con.name;
            out_acc << con.type;
            out_acc << con.overdraft;
            out_acc << con.currency;
            out_acc << con.balance;
            out_acc << con.balance_end;
        }
        file_acc.close();
        QMessageBox::information(this, "Збереження файлів","Всі дані збережено");
    }

    }
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }










}

void MainWindow::on_pushButton_12_clicked()
{
    on_action_open_triggered();
}


void MainWindow::on_pushButton_13_clicked()
{
    on_action_new_triggered();
}


void MainWindow::on_pushButton_2_clicked()
{
    Save();
}

//
void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Закрити програму?");
    //msgBox.setInformativeText("Закрити програму?");
    msgBox.setIcon(QMessageBox::Question);
    QIcon ico(":/new/prefix1/IMG/ico.png");
    msgBox.setWindowIcon(ico);

    QPushButton *save_quit = msgBox.addButton(tr("Зберегти"), QMessageBox::ActionRole);
    QPushButton *quit = msgBox.addButton(tr("Не зберігати"), QMessageBox::ActionRole);
    QPushButton *ignore = msgBox.addButton(tr("Відміна"), QMessageBox::ActionRole);

    //msgBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);
    msgBox.setDefaultButton(ignore);
    msgBox.exec();
    if(msgBox.clickedButton()== save_quit)
    {
        Save();
        event->accept();
    }
    else if(msgBox.clickedButton()== quit)
    {
        event->accept();
    }
    else if(msgBox.clickedButton()== ignore)
    {
        event->ignore();
    }



    /*
    if (QMessageBox::question(this,tr("Вихід"),tr("Вийти з програми?"),QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
    */
    return;

}
bool caseInsensitiveLessThan(const operation &s1, const operation &s2)
{
    return s1.date < s2.date;
}


