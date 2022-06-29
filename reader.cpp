#include <QtSql>
#include <QtGlobal>
#include "reader.h"

DataList Reader_SQL_lite::read_Data(const QString& filePath)
{
    DataList data_List;//создаём список данных для бд
    static QSqlDatabase sql_Data_Base = QSqlDatabase::addDatabase("QSQLITE");
    sql_Data_Base.setConnectOptions("QSQLITE_OPEN_READONLY=1");//чтобы открывался файл только формата sqlite 
    sql_Data_Base.setDatabaseName(filePath);//указываем адрес файла
    if (sql_Data_Base.open())//если бд открывается
    {
        QString data_Base_Name = filePath.mid(filePath.lastIndexOf('/')+1,filePath.indexOf('.') - filePath.lastIndexOf('/') - 1);//получаем название базы данных
        QSqlQuery query("SELECT VALUE, TIME FROM " + data_Base_Name, sql_Data_Base);//создаём запрос

        for (int i = 0;query.next() && i < 10; i++)
        {
            QPointF value(i, query.value(0).toDouble());

            data_List << Data(value, query.value(1).toString());//заполняем данными из бд
        }

    }

    return data_List;
}

DataList Reader_JSON::read_Data(const QString& filePath)
{
    DataList data_List;//создаём список данных для бд
    QString val;//создаём строку
    QFile file;//создаём файл
    file.setFileName(filePath);//записываем путь до нужного файла
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))//если файл открывается
    {
        val = file.readAll();//записываем в val всё из файла

        QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());//создаём документ для считывания с json формата

        if (document.isArray())
        {       
            QJsonArray array = document.array();//получаем json массив 

            int index = 0;
            foreach (const QJsonValue & value, array)//цикл считывания данных
            {
                if (value.isObject())
                {
                    QJsonObject obj = value.toObject();
                    double val = obj["Value"].toDouble();//получаем данные из элемента массива
                    QString time = obj["Time"].toString();

                    QPointF point(index, val);
                    data_List << Data(point, time);//заполняем данными список

                    index++;
                }
                if (index >= 10)
                    break;
            }
        }
    }

    file.close();
    return data_List;
}
