
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QTimer>
#include "currency_window.h"
#include "accounts_window.h"
#include "structurs.h"
#include "income_window.h"
#include "expense_window.h"
#include "transfer_window.h"
#include "mainwindowtablemodel.h"
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QCloseEvent>
#include "turnover.h"
#include "chart_incomes.h"
#include "chart_expenses.h"
#include "chart_accounts.h"
#include "ui_chart_accounts.h"
#include "info.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MySortFilterProxyModel_main : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortFilterProxyModel_main(QObject *parent = 0);
    QString search_vn;
    void setFilter();




protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:

};





class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    inline static QString fileName =
        QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).value(0)
        + QStringLiteral("/text1.dat");

    //inline static QString fileName =QFileDialog::getSaveFileName();







private slots:
    //доданий рядок
    void slotCustomMenuRequested(QPoint pos);

    void on_action_exit_triggered();

    void on_pushButton_8_clicked();

    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_action_val_triggered();

    void on_action_rah_triggered();

    void on_action_doh_triggered();

    void on_action_vytr_triggered();

    void on_action_about_triggered();

    void on_action_per_triggered();

    void on_action_3_triggered();

    void on_action_10_triggered();

    void on_action_11_triggered();

    void on_action_new_triggered();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_end_dateChanged(const QDate &date);

    void on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();



    void on_pushButton_oborot_clicked();



    void on_lineEdit_search_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_search_inputRejected();

    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_2_clicked();

    void on_MainWindow_destroyed();

private:
    Ui::MainWindow *ui;
    QList <currency> currencies;
    QList <account> accounts;
    QList <income> incomes;
    QList <expense> expenses;
    QList <transfer> transfers;
    mainwindowtablemodel *myModel;
    MySortFilterProxyModel_main *proxyModel;
    void closeEvent(QCloseEvent *event);
    void Open();
    void Save();

    void pererah_bal();
    void all_amounts ();
    void dark();
    bool caseInsensitiveLessThan(const operation &s1, const operation &s2);

};

#endif // MAINWINDOW_H
