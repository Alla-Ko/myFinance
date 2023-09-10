#ifndef CHART_EXPENSES_H
#define CHART_EXPENSES_H

#include <QDialog>
#include <QtCharts>
#include<QChartView>
#include<QPieSeries>
#include "structurs.h"

namespace Ui {
class Chart_expenses;
}

class Chart_expenses : public QDialog
{
    Q_OBJECT

public:
    explicit Chart_expenses(QList<expense> exp, QMap<QString,double> map, QWidget *parent = nullptr);
    ~Chart_expenses();

private slots:
    void on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime);

    void on_pushButton_clicked();

private:
    Ui::Chart_expenses *ui;
    QList<expense> exp_list;
    QMap <QString, double>chartmap ;
    QChartView *chartView;
    void renew();
};

#endif // CHART_EXPENSES_H
