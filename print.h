#ifndef PRINT_H
#define PRINT_H

#include "reader.h"
#include "QtCharts"

enum Types_of_Charts
{
    pie,
    bar,
};


class I_Print//фабрика для вывода разных диаграмм
{   
public:
    virtual void create_Chart(QChartView& chartView, const DataList& data, bool blackAndWhite = false) = 0;
    I_Print(){}
};

class Print_Bar: public I_Print// вывод диаграммы bar
{
public:
    virtual void create_Chart(QChartView& chartView, const DataList& data, bool blackAndWhite = false);
    Print_Bar(){}
};

class Print_Pie: public I_Print// вывод диаграммы pie
{
public:
    virtual void create_Chart(QChartView& chartView, const DataList& data, bool blackAndWhite = false);
    Print_Pie(){}
};

#endif // PRINT_H
