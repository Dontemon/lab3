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
        QString data_Base_Name = filePath.mid(filePath.lastIndexOf('/') + 1, filePath.indexOf('.') - filePath.lastIndexOf('/') - 1);//получаем название базы данных
        QSqlQuery query("SELECT VALUE, TIME FROM " + data_Base_Name, sql_Data_Base);//создаём запрос

        for (int i = 0;query.next() && i < 10; i++)
        {
            QPointF value(i, query.value(0).toDouble());

            data_List << Data(value, query.value(1).toString());//заполняем данными из бд
        }

        if (data_List.isEmpty())//если файл пустой выдаём ошибку
        {
            QMessageBox messageBox;
            messageBox.setText("Данные не найдены, график пустой: " + filePath);
            messageBox.exec();
        }

    }
    else
    {
        QMessageBox messageBox;//если файл не открывается, то выдаём ошибку
        messageBox.setText("Ошибка открытия " + filePath);
        messageBox.exec();
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

            bool partDataNotFound = false;

            for (int index = 0; index < 10; index++)//цикл считывания данных
            {
                if (array[index].isObject())
                {
                    QJsonObject obj = array[index].toObject();

                    if (obj.contains("Value") && obj.contains("Time"))
                    {
                        double val = obj["Value"].toDouble();//получаем данные из элемента массива
                        QString time = obj["Time"].toString();

                        QPointF point(index, val);
                        data_List << Data(point, time);//заполняем данными список
                    }
                    else//если не найдена Value или Time
                    {
                        QPointF point(index, 0);
                        data_List << Data(point, "not found");

                        partDataNotFound = true;
                    }


                }
            }

            if (partDataNotFound)//если часть данных отсутствует, то выдаём ошибку
            {
                QMessageBox messageBox;
                messageBox.setText("Часть данных не найдена: " + filePath);
                messageBox.exec();
            }

            if (data_List.isEmpty())//если файл пустой, то выдаём ошибку
            {
                QMessageBox messageBox;
                messageBox.setText("Данные не найдены, график пустой: " + filePath);
                messageBox.exec();
            }


        }
        else//если файл json сформирован не как массив, то выдаём ошибку
        {
            QMessageBox messageBox;
            messageBox.setText("Не найден json-массив в: " + filePath);
            messageBox.exec();
        }
        file.close();
    }
    else//если файл не открывается, то выдаём ошибку
    {
        QMessageBox messageBox;
        messageBox.setText("Ошибка открытия " + filePath);
        messageBox.exec();
    }



    return data_List;
}
