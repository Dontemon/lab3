#include "print.h"

void Print_Bar::create_Chart(QChartView& chart_View, const DataList& data, bool blackAndWhite)
{
    QChart *chart = new QChart();//создаем диаграмму
    chart->setTitle("Bar chart");//называем диаграмму

    QBarSeries *series = new QBarSeries(chart);

//    QStackedBarSeries *series = new QStackedBarSeries(chart);

    QBarSet *set = new QBarSet("Bar");
        for (const Data &data_Elem : data)
            *set << data_Elem.first.y();

    if (blackAndWhite)//если нужно сделать график черно белым
    {
        QLinearGradient gradient(0, 0, 1000, 1000);
        gradient.setColorAt(0, Qt::black);
        gradient.setColorAt(1, Qt::white);

        QBrush brush(gradient);
        set->setBrush(brush);
    }
    series->append(set);

    chart->addSeries(series);//добавляем в диаграмму столбцы
    chart->createDefaultAxes();//добавляем оси

    chart_View.setChart(chart);
}

void Print_Pie::create_Chart(QChartView& chart_View, const DataList& data, bool blackAndWhite)
{
    QChart *chart = new QChart();//создаем диаграмму
    chart->setTitle("Pie chart");//называем диаграмму

    QPieSeries *series = new QPieSeries(chart);
        for (const Data &data_Elem : data)
        {
            QPieSlice *slice = series->append(data_Elem.second, data_Elem.first.y());

            if (blackAndWhite)
            {
                QLinearGradient gradient(0, 0, 1000, 1000);
                gradient.setColorAt(0, Qt::black);
                gradient.setColorAt(1, Qt::white);

                QBrush brush(gradient);
                slice->setBrush(brush);
            }
        }

    chart->addSeries(series);//добавляем данные на диаграмму

    chart_View.setChart(chart);
}
