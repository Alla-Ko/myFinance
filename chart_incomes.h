#ifndef CHART_INCOMES_H
#define CHART_INCOMES_H

#include <QDialog>
#include <QtCharts>
#include<QChartView>
#include<QPieSeries>
#include<QLineSeries>
#include "structurs.h"

namespace Ui {
class Chart_incomes;
}

class Chart_incomes : public QDialog
{
    Q_OBJECT

public:
    explicit Chart_incomes(QList<income> inc, QMap<QString,double> map, QWidget *parent = nullptr);
    ~Chart_incomes();

private slots:
    void on_dateTimeEdit_start_dateChanged(const QDate &date);

    void on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime);

    void on_pushButton_clicked();

private:
    Ui::Chart_incomes *ui;
    QList<income>inc_list;
    QMap <QString, double>chartmap ;
    QChartView *chartView;
    void renew();

};

#endif // CHART_INCOMES_H
