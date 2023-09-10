
#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //без цього виходить жах, можна ще так
    //a.setStyle(QStyleFactory::create("Fusion"));
    qApp->setStyle(QStyleFactory::create("Fusion"));

    a.setOrganizationDomain("linkedin.com/in/alla-kokhaniuk-aa0833272");
    a.setApplicationName("myFinance.2023");
    MainWindow w;
    w.show();
    return a.exec();
}
