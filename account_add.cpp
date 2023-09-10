#include "account_add.h"
#include "ui_account_add.h"

account_add::account_add(QStringList cur, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::account_add)
{
    ui->setupUi(this);
    for(int i=0;i<cur.size();i++){
        ui->comboBox_cur->addItem(cur[i]);
    }

    ui->lineEdit_name->setFocus();
    QRegularExpression r_name("[\\w,\\s,Є-ЯҐ,а-їґ]{35}");
    QValidator *validator_name=new QRegularExpressionValidator(r_name,this);
    ui->lineEdit_name->setValidator(validator_name);

    QRegularExpression r_plus("(^[0]{1}[.]{1}[0-9]{1,2})|(^[1-9]{1}[0-9]{0,9}[.]{1}[0-9]{1,2})");
    QValidator *validator_plus=new QRegularExpressionValidator(r_plus,this);

    ui->lineEdit_overdraft->setValidator(validator_plus);
    ui->lineEdit_balance->setValidator(validator_plus);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);



}

account_add::~account_add()
{
    delete ui;
}

void account_add::on_comboBox_type_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox_type->currentText()=="Банківська картка"){
        ui->radioButton_yes->setCheckable(true);

    }
    if(ui->comboBox_type->currentText()=="Депозит"){
        ui->radioButton_no->setChecked(true);
        ui->radioButton_yes->setChecked(false);
        ui->radioButton_yes->setCheckable(false);
        ui->lineEdit_overdraft->setReadOnly(true);
        ui->lineEdit_overdraft->setText("0.00");
    }
    if(ui->comboBox_type->currentText()=="Готівка"){

        ui->radioButton_no->setChecked(true);
        ui->radioButton_yes->setChecked(false);
        ui->radioButton_yes->setCheckable(false);
        ui->lineEdit_overdraft->setReadOnly(true);
        ui->lineEdit_overdraft->setText("0.00");

    }
    if(ui->comboBox_type->currentText()=="Інше"){
        ui->radioButton_yes->setCheckable(true);

    }
}

//з кредитним лімітом
void account_add::on_radioButton_yes_clicked(bool checked)
{
    if(ui->radioButton_yes->isChecked()){
    ui->lineEdit_overdraft->setReadOnly(false);
    QRegularExpression r_plus_minus("(^[-]{0,1}[0]{1}[.]{1}[0-9]{1,2})|(^[-]{0,1}[1-9]{1}[0-9]{0,9}[.]{1}[0-9]{1,2})");
    QValidator *validator_plus_minus=new QRegularExpressionValidator(r_plus_minus,this);
    ui->lineEdit_balance->setValidator(validator_plus_minus);
    }
    else{
    ui->lineEdit_overdraft->setReadOnly(true);
    ui->lineEdit_overdraft->setText("0.00");
    QRegularExpression r_plus("(^[0]{1}[.]{1}[0-9]{1,2})|(^[1-9]{1}[0-9]{0,9}[.]{1}[0-9]{1,2})");
    QValidator *validator_plus=new QRegularExpressionValidator(r_plus,this);
    ui->lineEdit_balance->setValidator(validator_plus);
    if(ui->lineEdit_balance->text().toDouble()<0)
        ui->lineEdit_balance->setText("0.00");
    }
}

//без кредитного ліміту
void account_add::on_radioButton_no_clicked(bool checked)
{
    if(ui->radioButton_no->isChecked()){
    ui->lineEdit_overdraft->setReadOnly(true);
    ui->lineEdit_overdraft->setText("0.00");
    QRegularExpression r_plus("(^[0]{1}[.]{1}[0-9]{1,2})|(^[1-9]{1}[0-9]{0,9}[.]{1}[0-9]{1,2})");
    QValidator *validator_plus=new QRegularExpressionValidator(r_plus,this);
    ui->lineEdit_balance->setValidator(validator_plus);
    if(ui->lineEdit_balance->text().toDouble()<0)
        ui->lineEdit_balance->setText("0.00");
    }
}

//зайве
void account_add::on_lineEdit_balance_editingFinished()
{

}


void account_add::on_lineEdit_balance_textChanged(const QString &arg1)
{
    double bal=ui->lineEdit_balance->text().toDouble();
    double over=ui->lineEdit_overdraft->text().toDouble();
    if(bal<0&&-1*bal>over){
        ui->lineEdit_overdraft->setText(QString::number(-1*bal));
    }
    //поміняти доступну суму
    QPalette sample_palette;
    double dost=newacc.balance_end-newacc.balance+bal+over;
    ui->label_avail_am->setText(QString::number(dost));
    if(dost<0){
        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(ui->comboBox_cur->currentText()+"!!!!!");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    }
    else{
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(ui->comboBox_cur->currentText());
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

    }

}


void account_add::on_lineEdit_overdraft_textChanged(const QString &arg1)
{
    double bal=ui->lineEdit_balance->text().toDouble();
    double over=ui->lineEdit_overdraft->text().toDouble();
    if(bal<0&&-1*bal>over){
        ui->lineEdit_balance->setText("0.00");
    }
    //поміняти доступну суму
    QPalette sample_palette;
    double dost=newacc.balance_end-newacc.balance+bal+over;
    ui->label_avail_am->setText(QString::number(dost));
    if(dost<0){
        sample_palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(ui->comboBox_cur->currentText()+"!!!!!");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    }
    else{
        sample_palette.setColor(QPalette::WindowText, Qt::white);
        ui->label_avail_am->setPalette(sample_palette);
        ui->label_avail_cur->setPalette(sample_palette);
        ui->label_avail_cur->setText(ui->comboBox_cur->currentText());
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

    }
}

//зайве
void account_add::on_lineEdit_overdraft_textEdited(const QString &arg1)
{

}

//зайве
void account_add::on_lineEdit_overdraft_editingFinished()
{

}


void account_add::on_buttonBox_accepted()
{
    newacc.name=ui->lineEdit_name->text();
    newacc.balance=ui->lineEdit_balance->text().toDouble();
    newacc.overdraft=ui->lineEdit_overdraft->text().toDouble();
    newacc.currency=ui->comboBox_cur->currentText();
    newacc.type=ui->comboBox_type->currentText();
}

