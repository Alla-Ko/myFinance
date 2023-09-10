#ifndef CHART_ACCOUNTS_H
#define CHART_ACCOUNTS_H

#include <QDialog>
#include <QtCharts>
#include<QChartView>
#include<QLineSeries>
#include "structurs.h"

namespace Ui {
class Chart_accounts;
}

class Chart_accounts : public QDialog
{
    Q_OBJECT

public:
    explicit Chart_accounts(QWidget *parent = nullptr);
    ~Chart_accounts();
    Ui::Chart_accounts *ui;

private:



};

#endif // CHART_ACCOUNTS_H
