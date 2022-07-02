#ifndef READER_H
#define READER_H

#include "QtCharts"

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;


class I_Reader//Абстрактный класс для считывания информации из разных файлов
{
public:
    virtual DataList read_Data(const QString& filePath) = 0;//виртуальная функция для считывания данных
    I_Reader(){}
};

class Reader_SQL_lite: public I_Reader//класс для считывания из файла sqlite
{
public:
    virtual DataList read_Data(const QString& filePath);
    Reader_SQL_lite(){}
};

class Reader_JSON: public I_Reader//класс для считывания из файла json формата
{
public:
    virtual DataList read_Data(const QString& filePath);
    Reader_JSON(){}
};

#endif // READER_H
