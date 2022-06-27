#ifndef PRINT_H
#define PRINT_H

#include "IOC.h"
#include "QtCharts"

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;

enum Types_of_Charts
{
    Pie,
    Bar,
};


class I_Print
{   
public:
    virtual QChartView create_Chart(const DataList& dataList, bool blackAndWhite = false) = 0;
    I_Print(){}
};

class I_Print_bar: public I_Print
{
public:
    virtual QChartView create_Chart( const DataList& Data, bool blackAndWhite = false);
    I_Print_bar(){}
};

class I_Print_Pie: public I_Print
{
public:
    virtual QChartView create_Chart(const DataList& Data, bool blackAndWhite = false);
    I_Print_Pie(){}
};

#endif // PRINT_H
