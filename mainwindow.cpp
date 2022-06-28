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
    : //QWidget(parent)
      QMainWindow(parent)
{
    buttonChangeFolder = new QPushButton("Change folder");

    chartWidget = new ChartWidget();
    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1500, 500);
    this->setStatusBar(new QStatusBar(this));
    this->statusBar()->showMessage("Choosen Path: ");
    QString homePath = QDir::homePath();
    // Определим  файловой системы:

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    fileModel->setRootPath(homePath);

    QSplitter *splitter = new QSplitter();
//    QVBoxLayout *fileTableLayout = new QVBoxLayout();
    tableView = new QTableView;
    tableView->setModel(fileModel);
//    fileTableLayout->addWidget(tableView);
//    fileTableLayout->addWidget(buttonChangeFolder);
    splitter->addWidget(buttonChangeFolder);
    splitter->addWidget(tableView);


    splitter->addWidget(chartWidget);
    setCentralWidget(splitter);

    tableView->verticalHeader()->resizeSection(0, 2000);
    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TreeView
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    connect(buttonChangeFolder, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));
}
//Слот для обработки выбора элемента в TreeView
//выбор осуществляется с помощью курсора


void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    //Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QModelIndex index = tableView->selectionModel()->currentIndex();
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = fileModel->filePath(ix);
        this->statusBar()->showMessage("Выбранный путь : " + fileModel->filePath(indexs.constFirst()));
    }

    QString typeFile = filePath;
    typeFile.remove(0, typeFile.indexOf('.'));
    if (typeFile == ".sqlite")
    {
        IOC::IOCContainer::instance().RegisterFactory<I_Reader, Reader_SQL_lite>();
        chartWidget->updateData(filePath);
    }
    if (typeFile == ".json")
    {
        IOC::IOCContainer::instance().RegisterFactory<I_Reader, Reader_JSON>();
        chartWidget->updateData(filePath);
    }

}

void MainWindow::openFileDialogWindow()
{
    QFileDialog fileDialog(this);

    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setDirectory(QDir::homePath());

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

