#include "mainwindow.h"
#include <QApplication>


QTextStream cout(stdout);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
/*
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("BLOOD_SUGAR.sqlite");
    if (sdb.open())
    {
        qDebug() << "BD open\n";
        QSqlQuery query("SELECT * FROM BLOOD_SUGAR", sdb);
        while (query.next())
        {
             QString temp = query.value(0).toString();
             qDebug() << "*";
             qDebug() << temp;
        }
    }
    else
    {
        qDebug() << "BD not open\n";
    }
*/

    MainWindow w;
    w.show();

	return a.exec();
}
