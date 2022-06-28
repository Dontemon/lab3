#include <QtSql>
#include <QtGlobal>
#include "reader.h"

DataList Reader_SQL_lite::read_Data(const QString& filePath)
{
    DataList data_List;
    static QSqlDatabase sql_Data_Base = QSqlDatabase::addDatabase("QSQLITE");
    sql_Data_Base.setConnectOptions("QSQLITE_OPEN_READONLY=1");
    sql_Data_Base.setDatabaseName(filePath);
    if (sql_Data_Base.open())
    {
        QString data_Base_Name = filePath.mid(filePath.lastIndexOf('/')+1,filePath.indexOf('.') - filePath.lastIndexOf('/') - 1);
        QSqlQuery query("SELECT VALUE, TIME FROM " + data_Base_Name, sql_Data_Base);

        for (int i = 0;query.next() && i < 10; i++)
        {
            QPointF value(i, query.value(0).toDouble());

            data_List << Data(value, query.value(1).toString());
        }

    }

    return data_List;
}

DataList Reader_JSON::read_Data(const QString& filePath)
{
    DataList data_List;
    QString val;
    QFile file;
    file.setFileName(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();

        QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

        if (document.isArray())
        {       
            QJsonArray array = document.array();

            int index = 0;
            foreach (const QJsonValue & value, array)
            {
                if (value.isObject())
                {
                    QJsonObject obj = value.toObject();
                    double val = obj["Value"].toDouble();
                    QString time = obj["Time"].toString();

                    QPointF point(index, val);
                    data_List << Data(point, time);

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
