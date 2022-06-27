#include "print.h"

I_Print::create_Chart(QChartView &chart_View, const DataList& Data, bool notColored = false)
{
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    for (int i(0); i < m_dataTable.count(); i++)
    {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));
        for (const Data &data_Elem : Data)
            *set << data_Elem.first.y();

        if (notColored)
        {
            QLinearGradient gradient(0, 0, this->width(), this->height());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);
            set->setBrush(brush);
        }
        series->append(set);
    }
    chart->addSeries(series);
    chart->createDefaultAxes();

    chart_View.setChart(chart);

}

QChartView I_Print::create_Chart(const DataList& Data, bool notColored = false)
{
    //QChart *ThemeWidget::createPieChart(bool notColored) const
        QChart *chart = new QChart();
        chart->setTitle("Pie chart");
        qreal pieSize = 1.0 / m_dataTable.count();

            for (const Data &data_Elem : Data)
            {
                QPieSlice *slice = series->append(data_Elem.second, data_Elem.first.y());

                if (notColored)
                {
                    QLinearGradient gradient(0, 0, chartView->height(), chartView->width());
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

       chart_View.setChart(chart);
}
