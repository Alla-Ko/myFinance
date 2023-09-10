#include "transfer_add.h"
#include "ui_transfer_add.h"

Transfer_add::Transfer_add(QList <account> acc, QList <currency> cur, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transfer_add)
{
    ui->setupUi(this);
    for(int i=0;i<acc.size();i++){
        ui->comboBox_account1->addItem(acc[i].name);
        //ui->comboBox_account2->addItem(acc[i].name);
        acc_map[acc[i].name]=acc[i].currency;
        acc_amount_end_map[acc[i].name]=acc[i].balance_end+acc[i].overdraft;
    }
    for(int i=0;i<cur.size();i++){
        cur_rate_map[cur[i].name]=cur[i].rate;
    }

    ui->dateTimeEdit->setFocus();
    setTabOrder(ui->dateTimeEdit,ui->comboBox_account1);
    setTabOrder(ui->comboBox_account1,ui->lineEdit_amount1);
    setTabOrder(ui->lineEdit_amount1,ui->comboBox_account2);
    setTabOrder(ui->comboBox_account2, ui->lineEdit_amount2);
    setTabOrder(ui->lineEdit_amount2, ui->lineEdit_rate);
    setTabOrder(ui->lineEdit_rate, ui->lineEdit_comments);

    if(newtr.date.isNull())ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    else ui->dateTimeEdit->setDateTime(newtr.date);
    QDateTime::currentDateTime();
    ui->dateTimeEdit->setCalendarPopup(true);



    ui->lineEdit_currency1->setReadOnly(true);
    ui->lineEdit_currency1->setText(acc_map[ui->comboBox_account1->currentText()]);
    //ui->label_avail_am1->setText(QString::number(acc_amount_end_map[ui->comboBox_account1->currentText()])+" "+acc_map[ui->comboBox_account1->currentText()]);


    ui->lineEdit_currency2->setReadOnly(true);

    //всі крім першого рахунку
    for(int i=0;i<acc.size();i++){
        if(acc[i].name!=ui->comboBox_account1->currentText()){
            ui->comboBox_account2->addItem(acc[i].name);
        }
    }



    ui->lineEdit_currency2->setReadOnly(true);
    ui->lineEdit_currency2->setText(acc_map[ui->comboBox_account2->currentText()]);


    QRegularExpression r_plus("(^[0]{1}[.]{1}[0-9]{1,2})|(^[1-9]{1}[0-9]{0,9}[.]{1}[0-9]{1,2})");
    QValidator *validator_plus=new QRegularExpressionValidator(r_plus,this);

    ui->lineEdit_amount1->setValidator(validator_plus);
    ui->lineEdit_amount1->setText("0.00");
    ui->lineEdit_amount2->setValidator(validator_plus);
    ui->lineEdit_amount2->setText("0.00");

    QRegularExpression r_rate("(^[0]{1}[.]{1}[0-9]{1,3})|(^[1-9]{1}[0-9]{0,5}[.]{1}[0-9]{1,3})");
    QValidator *validator_rate=new QRegularExpressionValidator(r_rate,this);
    ui->lineEdit_rate->setValidator(validator_rate);

    ui->lineEdit_rate->setText(QString::number((double)cur_rate_map[ui->lineEdit_currency2->text()]/cur_rate_map[ui->lineEdit_currency1->text()]));
    available();


}

Transfer_add::~Transfer_add()
{
    delete ui;
}

void Transfer_add::on_comboBox_account1_currentTextChanged(const QString &arg1)
{
    QString old_acc2=ui->comboBox_account2->currentText();
    while(ui->comboBox_account2->count() > 0){
        for(int i = 0; i < ui->comboBox_account2->count(); i++){
            ui->comboBox_account2->removeItem(i);
        }
    }

    for(auto i=acc_map.cbegin(), end = acc_map.cend(); i != end; ++i){
        if(i.key()!=ui->comboBox_account1->currentText()&&i.key()!=""){
            ui->comboBox_account2->addItem(i.key());
        }
    }
    if(ui->comboBox_account1->currentText()!=old_acc2){
        ui->comboBox_account2->setCurrentText(old_acc2);
    }

    ui->lineEdit_currency2->setText(acc_map[ui->comboBox_account2->currentText()]);

    ui->lineEdit_currency1->setText(acc_map[ui->comboBox_account1->currentText()]);



    //оновити курс
    ui->lineEdit_rate->setText(QString::number((double)cur_rate_map[ui->lineEdit_currency2->text()]/cur_rate_map[ui->lineEdit_currency1->text()]));
    double a=(double)ui->lineEdit_amount1->text().toDouble()/ui->lineEdit_rate->text().toDouble()*100;
    //оновити другу суму
    a=(double)qFloor(a)/100;
    if(a>=0)ui->lineEdit_amount2->setText(QString::number(a));
    available();

}


void Transfer_add::on_comboBox_account2_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_currency2->setText(acc_map[ui->comboBox_account2->currentText()]);


    //оновити курс
    ui->lineEdit_rate->setText(QString::number((double)cur_rate_map[ui->lineEdit_currency2->text()]/cur_rate_map[ui->lineEdit_currency1->text()]));
    //оновити другу суму
    double a=(double)ui->lineEdit_amount1->text().toDouble()/ui->lineEdit_rate->text().toDouble()*100;

    a=(double)qFloor(a)/100;
    if(a>=0)ui->lineEdit_amount2->setText(QString::number(a));
    available();
}


void Transfer_add::on_lineEdit_amount1_textChanged(const QString &arg1)
{
    double a=(double)ui->lineEdit_amount1->text().toDouble()/ui->lineEdit_rate->text().toDouble()*100;

    a=(double)qFloor(a)/100;
    if(a>=0)ui->lineEdit_amount2->setText(QString::number(a));
    available();
}


void Transfer_add::on_lineEdit_amount2_textChanged(const QString &arg1)
{
    if(ui->lineEdit_amount2->hasFocus()){
        double a=(double)ui->lineEdit_amount1->text().toDouble()/ui->lineEdit_amount2->text().toDouble()*1000;
        a=(double)qFloor(a)/1000;
        ui->lineEdit_rate->setText(QString::number(a));
    }
    available();

}


void Transfer_add::on_lineEdit_rate_textChanged(const QString &arg1)
{
    if(ui->lineEdit_rate->hasFocus()){
        double a=(double)ui->lineEdit_amount1->text().toDouble()/ui->lineEdit_rate->text().toDouble()*100;
        a=(double)qFloor(a)/100;
        if(a>=0)ui->lineEdit_amount2->setText(QString::number(a));
    }
    available();
}


void Transfer_add::on_buttonBox_accepted()
{
   // qDebug()<<"!!!!!!!!!";
    newtr.date=ui->dateTimeEdit->dateTime();
    newtr.akk_name1=ui->comboBox_account1->currentText();
    newtr.akk_name2=ui->comboBox_account2->currentText();
    newtr.curr_name1=ui->lineEdit_currency1->text();
    newtr.curr_name2=ui->lineEdit_currency2->text();
    newtr.amount1=ui->lineEdit_amount1->text().toDouble();
    newtr.amount2=ui->lineEdit_amount2->text().toDouble();
    newtr.comment=ui->lineEdit_comments->text();
    close();
}

void Transfer_add::available()
{
    //оновити доступну суму
    double av1=acc_amount_end_map[ui->comboBox_account1->currentText()]-ui->lineEdit_amount1->text().toDouble();
    double av2=acc_amount_end_map[ui->comboBox_account2->currentText()]+ui->lineEdit_amount2->text().toDouble();
    //double av1=acc_amount_end_map[ui->comboBox_account1->currentText()]+newtr.amount1-ui->lineEdit_amount1->text().toDouble();
    //double av2=acc_amount_end_map[ui->comboBox_account2->currentText()]-newtr.amount2+ui->lineEdit_amount2->text().toDouble();

    if(av1>-0.01&&av1<0.01)av1=0.00;
    if(av2>-0.01&&av2<0.01)av2=0.00;
    //оновлення доступності
    ui->label_avail_am1->setText(QString::number(av1)+" "+acc_map[ui->comboBox_account1->currentText()]);
    ui->label_avail_am2->setText(QString::number(av2)+" "+acc_map[ui->comboBox_account2->currentText()]);
    QPalette sample_palette;
    if(av1<0){
        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_avail_am1->setPalette(sample_palette);

        ui->label_avail_am1->setText(QString::number(av1)+" "+acc_map[ui->comboBox_account1->currentText()]+"!!!");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    }
    else{
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        ui->label_avail_am1->setPalette(sample_palette);
        ui->label_avail_am1->setText(QString::number(av1)+" "+acc_map[ui->comboBox_account1->currentText()]);

    }
    if(av2<0){
        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_avail_am2->setPalette(sample_palette);

        ui->label_avail_am2->setText(QString::number(av2)+" "+acc_map[ui->comboBox_account2->currentText()]+"!!!");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    }
    else{
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        ui->label_avail_am2->setPalette(sample_palette);
        ui->label_avail_am2->setText(QString::number(av2)+" "+acc_map[ui->comboBox_account2->currentText()]);

    }
    if(av1>=0&&av2>=0){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}


