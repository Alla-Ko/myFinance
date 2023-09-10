
#ifndef STRUCTURS_H
#define STRUCTURS_H
#include <QDialog>
#include <QDate>

struct bal_by_date
{
    QString account;
    double balance;
    QDateTime date;
};




struct currency
{
    QString name;
    double rate;
    QString comment;
};

struct account
{
    QString name;
    QString type;
    double overdraft;
    QString currency;
    double balance;
    double balance_end;
};

struct income
{
    QDateTime date;
    double amount;
    QString currency;
    QString account;
    QString type;
    QString comment;
};
struct expense
{
    QDateTime date;
    double amount;
    QString currency;
    QString account;
    QString type;
    QString comment;
};


struct transfer
{
    QDateTime date;
    QString akk_name1;
    QString curr_name1;
    double amount1;
    QString akk_name2;
    QString curr_name2;
    double amount2;
    QString comment;
};


struct operation
{
    QDateTime date;
    double am_inc;
    double am_exp;
    QString type_glob;
    QString type;
    QString account;
    double balance;
    QString comment;
    QString forsort;
    int forsortnumber;
    


};

#endif // STRUCTURS_H
