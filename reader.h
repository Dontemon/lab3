#ifndef READER_H
#define READER_H

#include "QtCharts"

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;


class I_Reader//фабрика для считывание информации из разных файлов
{
public:
    virtual DataList read_Data(const QString& filePath) = 0;
    I_Reader(){}
};

class Reader_SQL_lite: public I_Reader//считывание из файла sqlite
{
public:
    virtual DataList read_Data(const QString& filePath);
    Reader_SQL_lite(){}
};

class Reader_JSON: public I_Reader//добавляем из файла json формата
{
public:
    virtual DataList read_Data(const QString& filePath);
    Reader_JSON(){}
};

#endif // READER_H
