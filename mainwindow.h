#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QBoxLayout>
#include "chartwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);//слот который вызывается когда в таблице выбирается файл
    void openFileDialogWindow();//функция открывающая диалоговое окно для выбора файла

public:
    MainWindow(QWidget *parent = 0);
private:
    QFileSystemModel *fileModel;
    QTableView  *tableView;//таблица файлов в окне
    ChartWidget *chartWidget;//диаграмма в окне
    QVBoxLayout *tableLayout;//слой для таблицы

    QPushButton *buttonChangeFolder;// кнопка выбора файла для диаграммы
};

#endif // MAINWINDOW_H

