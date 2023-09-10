#include "expence_add.h"
#include "ui_expence_add.h"

Expence_add::Expence_add(QList <account> acc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Expence_add)
{
    ui->setupUi(this);
    for(int i=0;i<acc.size();i++){
        ui->comboBox_account->addItem(acc[i].name);
        acc_map[acc[i].name]=acc[i].currency;
    }
    ui->dateTimeEdit->setFocus();
    setTabOrder(ui->dateTimeEdit,ui->lineEdit_amount);
    setTabOrder(ui->lineEdit_amount,ui->comboBox_type);
    setTabOrder(ui->comboBox_type,ui->comboBox_account);
    setTabOrder(ui->comboBox_account, ui->lineEdit_comments);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit->setCalendarPopup(true);
    ui->lineEdit_currency->setReadOnly(true);
    ui->lineEdit_currency->setText(acc_map[ui->comboBox_account->currentText()]);
    ui->label_avail_cur->setText(acc_map[ui->comboBox_account->currentText()]);



    QRegularExpression r_plus("(^[0]{1}[.]{1}[0-9]{1,2})|(^[1-9]{1}[0-9]{0,9}[.]{1}[0-9]{1,2})");
    QValidator *validator_plus=new QRegularExpressionValidator(r_plus,this);

    ui->lineEdit_amount->setValidator(validator_plus);
    ui->lineEdit_amount->setText("0.00");

}

Expence_add::~Expence_add()
{
    delete ui;
}

//зміна рахунку - поміняти валюту
void Expence_add::on_comboBox_account_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_currency->setText(acc_map[ui->comboBox_account->currentText()]);
    ui->label_avail_am->setText(QString::number(acc_amount_end_map[ui->comboBox_account->currentText()]-ui->lineEdit_amount->text().toDouble()));
    ui->label_avail_cur->setText(acc_map[ui->comboBox_account->currentText()]);


    //ніби і суму поміняли
    QPalette sample_palette;
    ui->label_avail_am->setText(QString::number(acc_amount_end_map[ui->comboBox_account->currentText()]-ui->lineEdit_amount->text().toDouble()));
    if (acc_amount_end_map[ui->comboBox_account->currentText()]-ui->lineEdit_amount->text().toDouble()<0){

        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(acc_map[ui->comboBox_account->currentText()]+"!!!!!");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else{
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(acc_map[ui->comboBox_account->currentText()]);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}


void Expence_add::on_buttonBox_accepted()
{
    newexp.date=ui->dateTimeEdit->dateTime();
    newexp.account=ui->comboBox_account->currentText();
    newexp.amount=ui->lineEdit_amount->text().toDouble();
    newexp.type=ui->comboBox_type->currentText();
    newexp.comment=ui->lineEdit_comments->text();
    newexp.currency=ui->lineEdit_currency->text();

}


void Expence_add::on_lineEdit_amount_textChanged(const QString &arg1)
{
    QPalette sample_palette;
    ui->label_avail_am->setText(QString::number(acc_amount_end_map[ui->comboBox_account->currentText()]-ui->lineEdit_amount->text().toDouble()));
    if (acc_amount_end_map[ui->comboBox_account->currentText()]-ui->lineEdit_amount->text().toDouble()<0){

        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(acc_map[ui->comboBox_account->currentText()]+"!!!!!");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else{
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(acc_map[ui->comboBox_account->currentText()]);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}

