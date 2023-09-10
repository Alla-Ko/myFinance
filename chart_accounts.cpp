#include "chart_accounts.h"
#include "ui_chart_accounts.h"

Chart_accounts::Chart_accounts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chart_accounts)
{
    ui->setupUi(this);
}

Chart_accounts::~Chart_accounts()
{
    delete ui;
}
