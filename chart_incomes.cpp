#include "chart_incomes.h"
#include "ui_chart_incomes.h"

Chart_incomes::Chart_incomes(QList<income> inc, QMap<QString,double> map, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chart_incomes)
{
    ui->setupUi(this);

    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    QDate a(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    QTime b;
    QDateTime c(a,b);
    ui->dateTimeEdit_start->setDateTime(c);
    ui->dateTimeEdit_start->setCalendarPopup(true);
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_end->setCalendarPopup(true);
    ui->dateTimeEdit_start->setDisplayFormat("dd.MM.yyyy");
    ui->dateTimeEdit_end->setDisplayFormat("dd.MM.yyyy");

    if(inc.size()){

        income ink_new;
        for(int i=0;i<inc.size();i++){

            ink_new.date=inc[i].date;
            ink_new.type=inc[i].type;
            ink_new.amount=inc[i].amount*map[inc[i].currency];

            inc_list.push_back(ink_new);
        }
    // Define slices and percentage of whole they take up
    QPieSeries *series = new QPieSeries();

    for(int i=0;i<inc_list.size();i++){
        chartmap[inc_list[i].type]+=inc_list[i].amount;
    }
    double summ=0.0;
    for(auto it=chartmap.cbegin(), end = chartmap.cend(); it != end; ++it){
        series->append(it.key(),it.value());
        summ+=it.value();
    }
    ui->label->setVisible(true);
    ui->label->setText("Всього доходів за період: "+QString::number(summ)+" UAH");
    //Сума доходів

    /*
    series->append("Vegetables",.40);
    series->append("Beans",.20);
    series->append("Fruits",.15);
    series->append("Seeds/Nuts",.10);
    series->append("Whole Grains",.15);
    */



    for(int i=0;i<chartmap.size();i++){
        series->slices().at(i)->setLabelVisible();
        series->slices().at(i)->setLabel(series->slices().at(i)->label()+",\n"+QString("%1%").arg(100* series->slices().at(i)->percentage(), 0, 'f', 1));
    }
    series->slices().at(chartmap.size()-1)->setExploded();
    /*
    // Add label to 1st slice
    QPieSlice *slice0 = series->slices().at(0);
    slice0->setLabelVisible();
    */




    /*
    // Add label, explode and define brush for 2nd slice
    QPieSlice *slice1 = series->slices().at(1);
    slice1->setExploded();
    slice1->setLabelVisible();
    slice1->setPen(QPen(Qt::darkGreen, 2));
    slice1->setBrush(Qt::green);
    */



    QChart *chart = new QChart();
    chartView = new QChartView(chart);
    chart->addSeries(series);
    //chart->setTitle("Структура доходів за період з "+ui->dateTimeEdit_start->date().toString()+" по "+ui->dateTimeEdit_end->date().toString());
    chart->legend()->show();
    chart->legend()->setAlignment(Qt::AlignRight);

    chart->setAnimationOptions(QChart::AllAnimations);



    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);
    chartView->setRubberBand(QChartView::RectangleRubberBand);

    //QPalette pal = qApp->palette();
    //pal.theme
    //pal.setColor(QPalette::Window, QRgb(0x3d3d3d));
    //pal.setColor(QPalette::WindowText, QRgb(0xffffff));
    //chartView->setAutoFillBackground(true);
    //auto theme=chart->theme();

    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    //chartView->chart()->setTheme(QChart::ChartThemeBlueIcy);
    //chartView->chart()->setBackgroundVisible(false);


    //chartView->setBackgroundRole(QPalette::NoRole);
    //chartView->chart()->setPlotAreaBackgroundVisible(false);
    chartView->setParent(ui->frame);



    }
    renew();
}


Chart_incomes::~Chart_incomes()
{
    delete ui;
}
//зайве
void Chart_incomes::on_dateTimeEdit_start_dateChanged(const QDate &date)
{

}


void Chart_incomes::on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime newdate=ui->dateTimeEdit_start->dateTime();
    if(newdate>ui->dateTimeEdit_end->dateTime())
        ui->dateTimeEdit_end->setDateTime(newdate);
}


void Chart_incomes::on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime newdate=ui->dateTimeEdit_end->dateTime();
    if(newdate<ui->dateTimeEdit_start->dateTime())
        ui->dateTimeEdit_start->setDateTime(newdate);
}


void Chart_incomes::on_pushButton_clicked()
{
    renew();
}
void Chart_incomes::renew(){

    QPieSeries* series = new QPieSeries();
    chartmap.clear();
    for(int i=0;i<inc_list.size();i++){
        if(inc_list[i].date.date()>=ui->dateTimeEdit_start->date()&&inc_list[i].date.date()<=ui->dateTimeEdit_end->date()){
        chartmap[inc_list[i].type]+=inc_list[i].amount;

        }
    }
    int ind=0;
    int min_ind=0;
    double min_value=0.0;
    double summ=0.0;
    for(auto it=chartmap.cbegin(), end = chartmap.cend(); it != end; ++it){
        series->append(it.key(),it.value());
        summ+=it.value();
        if (ind==0)min_value=it.value();
        if (ind>0&&it.value()<min_value)min_value=it.value();
        ind++;
    }
    ui->label->setVisible(true);
    ui->label->setText("Всього доходів за період: "+QString::number(summ)+" UAH");

            ind=0;
        for(auto it=chartmap.cbegin(), end = chartmap.cend(); it != end; ++it){
            if(it.value()==min_value){
                qDebug()<<it.key();
                break;
            }
            ind++;
        }
        for(int i=0;i<chartmap.size();i++){
            series->slices().at(i)->setLabelVisible();
            series->slices().at(i)->setLabel(series->slices().at(i)->label()+
                                             ",\n"+QString("%1%").arg(100* series->
                                                                              slices().at(i)->percentage(), 0, 'f', 1));
        }
        if(series->slices().size()>ind)
        series->slices().at(ind)->setExploded();
        QChart* chart=new QChart;

        chart->addSeries(series);
        //chart->setTitle("Структура доходів за період з "+ui->dateTimeEdit_start->date().toString()+" по "+ui->dateTimeEdit_end->date().toString());

        chart->legend()->show();
        chart->legend()->setAlignment(Qt::AlignRight);

        chart->setAnimationOptions(QChart::AllAnimations);
        chartView->setChart(chart);

        chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
        chartView->repaint();
}
