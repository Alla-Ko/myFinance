#include "currency_add.h"
#include "ui_currency_add.h"

currency_add::currency_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::currency_add)
{
    ui->setupUi(this);
    ui->lineEdit_name->setFocus();
    setTabOrder(ui->lineEdit_name,ui->lineEdit_rate);
    setTabOrder(ui->lineEdit_rate,ui->lineEdit_comment);
    QRegularExpression r_name("[a-z,A-Z]{3}");
    QValidator *validator_name=new QRegularExpressionValidator(r_name,this);
    ui->lineEdit_name->setValidator(validator_name);
    //
    QRegularExpression r_rate("(^[0]{1}[.]{1}[0-9]{1,3})|(^[1-9]{1}[0-9]{0,5}[.]{1}[0-9]{1,3})");
    QValidator *validator_rate=new QRegularExpressionValidator(r_rate,this);
    ui->lineEdit_rate->setValidator(validator_rate);
}

currency_add::~currency_add()
{
    delete ui;
}

void currency_add::on_buttonBox_accepted()
{

    newcur.name=ui->lineEdit_name->text().toUpper();
    newcur.rate=ui->lineEdit_rate->text().toDouble();
    newcur.comment=ui->lineEdit_comment->text();
}

