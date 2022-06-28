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
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void openFileDialogWindow();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QFileSystemModel *fileModel;
    QTableView *tableView;
    ChartWidget *chartWidget;
    QVBoxLayout *tableLayout;
    QHBoxLayout *tableButtonLayout;

    QPushButton *buttonChangeFolder;
};

#endif // MAINWINDOW_H

