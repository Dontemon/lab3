#include "print.h"

QChartView* I_Print_bar::create_Chart(const DataList& data, bool blackAndWhite)
{
    QChartView* chart_View = new QChartView();

    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

   QBarSeries *series = new QBarSeries(chart);

//    QStackedBarSeries *series = new QStackedBarSeries(chart);

        QBarSet *set = new QBarSet("Bar");
        for (const Data &data_Elem : data)
            *set << data_Elem.first.y();

        if (blackAndWhite)
        {
            QLinearGradient gradient(0, 0, 1000, 1000);
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);
            set->setBrush(brush);
        }
        series->append(set);

    chart->addSeries(series);
    chart->createDefaultAxes();

    chart_View->setChart(chart);

           return chart_View;

}

QChartView* I_Print::create_Chart(const DataList& data, bool blackAndWhite)
{

    QChartView* chart_View = new QChartView();
    QChart *chart = new QChart();
        chart->setTitle("Pie chart");

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

            series->setPieSize(1);
            series->setHorizontalPosition(0.5);
            series->setVerticalPosition(0.5);
            chart->addSeries(series);

       chart_View->setChart(chart);

       return chart_View;
}
