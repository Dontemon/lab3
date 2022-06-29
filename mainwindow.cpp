#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include "IOC.h"

#include <QtGlobal>

int IOC::IOCContainer::s_typeId = 121;

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    buttonChangeFolder = new QPushButton("Change folder");//кнопка выбора другой папки для таблицы

    chartWidget = new ChartWidget();
    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1500, 500);
    this->setStatusBar(new QStatusBar(this));
    this->statusBar()->showMessage("Choosen Path: ");
    QString homePath = QDir::homePath();
    // Определим  файловой системы:

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);//устанавливаем фильтр для файлов

    fileModel->setRootPath(homePath);//устанавливаем начальное положение папки

    QSplitter *splitter = new QSplitter();//создаём разделитель
    tableView = new QTableView;//создаём представление таблицы
    tableView->setModel(fileModel);
    splitter->addWidget(buttonChangeFolder);//добавляем кнопку
    splitter->addWidget(tableView);//добавляем в разделённое окно таблицу с файлами


    splitter->addWidget(chartWidget);//добавляем диаграмму 
    setCentralWidget(splitter);

    tableView->verticalHeader()->resizeSection(0, 2000);
    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TableView
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    connect(buttonChangeFolder, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));
}


//Слот для обработки выбора элемента в TableView
//выбор осуществляется с помощью курсора
void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
    Q_UNUSED(deselected);
    QModelIndex index = tableView->selectionModel()->currentIndex();
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {//для отслеживания пути к нужному файлу
        QModelIndex ix =  indexs.constFirst();
        filePath = fileModel->filePath(ix);
        this->statusBar()->showMessage("Выбранный путь : " + fileModel->filePath(indexs.constFirst()));
    }

    QString typeFile = filePath;//создаём строку содержащую путь до нужного файла
    typeFile.remove(0, typeFile.indexOf('.'));//удаляем всё кроме формат файла
    if (typeFile == ".sqlite")//если формат sqlite
    {
        IOC::IOCContainer::instance().RegisterFactory<I_Reader, Reader_SQL_lite>();
        chartWidget->updateData(filePath);//передаём новый файл для диаграммы
    }
    if (typeFile == ".json")//если формат json
    {
        IOC::IOCContainer::instance().RegisterFactory<I_Reader, Reader_JSON>();
        chartWidget->updateData(filePath);//передаём новый файл для диаграммы
    }

}

void MainWindow::openFileDialogWindow()//диалоговое окно для выбора новой папки
{
    QFileDialog fileDialog(this);//создаём диалоговое окно

    fileDialog.setFileMode(QFileDialog::Directory);//устанавливаем режим работы с файлами
    fileDialog.setDirectory(QDir::homePath());//устанавилваем начальное положение

    QString filePath;
    if(fileDialog.exec())
    {
        filePath = fileDialog.selectedFiles().first();

        tableView->setRootIndex(fileModel->setRootPath(filePath));
    }
}

MainWindow::~MainWindow()
{

}

