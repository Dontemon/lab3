/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


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
#include <QFileDialog>
#include "chartwidget.h"


ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent),
    typeComboBox(createTypeBox()),//создаём кнопку для выбора графика
    blackAndWhiteCheckBox(new QCheckBox("Черно-белый график")),//создаём кнопку-галочку для чёрно-белого графика
    printButton(new QPushButton("Печать графика"))//кнопка для печати графика
{
    connectSignals();
    QVBoxLayout *baseLayout = new QVBoxLayout();//создаём два слоя
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel("Выберите тип диаграммы:"));//добавляем кнопки на слой
    settingsLayout->addWidget(typeComboBox);
    settingsLayout->addWidget(blackAndWhiteCheckBox);
    settingsLayout->addWidget(printButton);
    settingsLayout->addStretch();//добавляем разделитель на слой
    baseLayout->addLayout(settingsLayout);//помещаем слой настроек на базовый

    chart = new Chart();// создаём диаграмму

    baseLayout->addWidget(chart->getChartView());//добавляем диаграмму на слой

    setLayout(baseLayout);//устанавливаем базовый слой

    blackAndWhiteCheckBox->setChecked(false);//устанавливаем поумолчанию цветной график
    updateUI();// обновляем график
}

ChartWidget::~ChartWidget() {}//деструктор

void ChartWidget::connectSignals()// соединяем сигналы
{
    //соединяем изменения типа диаграммы и обовление диаграммы
    connect(typeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ChartWidget::updateUI);
    connect(blackAndWhiteCheckBox, &QCheckBox::toggled, this, &ChartWidget::updateUI); //соединяем изменения цвета диаграммы и обовление диаграммы
    connect(printButton, SIGNAL(clicked()), this, SLOT(printToPdf()));  //соединяем нажатие на кнопку печати со слотом печати pdf файла
}

QString ChartWidget::getPathToSavePdf()
{
    QFileDialog *fileDialog = new QFileDialog(this);
             // определить заголовок файла
    //fileDialog-> setWindowTitle (tr ("Открыть изображение"));
             // Установить путь к файлу по умолчанию
    fileDialog->setDirectory(".");
             // Установить фильтр файлов
    fileDialog->setNameFilter(tr("pdf(*.pdf)"));
             // Настройка позволяет выбрать несколько файлов, по умолчанию используется только один файл QFileDialog :: ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
             // Установить режим просмотра
    fileDialog->setViewMode(QFileDialog::Detail);
             // выводим путь ко всем выбранным файлам
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }
    return fileNames.first();
}

void ChartWidget::printToPdf()
{
    QString fileName = getPathToSavePdf();//находим путь

    QPdfWriter writer(fileName + ".pdf");//устанавливаем формат файла пдф

    writer.setPageSize(QPagedPaintDevice::A4);//Устанавливаем размер страницы

    QPainter painter(&writer);

    chart->getChartView()->render(&painter);//загружаем полученный пдф
    painter.end();
}



QComboBox *ChartWidget::createTypeBox() const//метод для создания переключателя между диаграммами
{
    // type layout
    QComboBox *themeComboBox = new QComboBox();//создаём сам переключатель
    themeComboBox->addItem("Bar", Types_of_Charts::bar);//добавляем вариант диаграммы Bar
    themeComboBox->addItem("Pie", Types_of_Charts::pie);//добавляем вариант диаграммы Pie
    return themeComboBox;
}

void ChartWidget::updateData(const QString& filePath)
{
    chart->read_Data(filePath);//считываем из файла данные
    updateUI();//выводим данные на диаграмму
}

void ChartWidget::updateUI()//меняем диаграмму в зависимости от выбранного переключателя
{
    Types_of_Charts typeChart = static_cast<Types_of_Charts>( //получаем выбранный тип диаграммы
                typeComboBox->itemData(typeComboBox->currentIndex()).toInt());

    switch (typeChart)
    {
    case Types_of_Charts::bar ://если выбран bar, то помещаем в контейнер принтер bar
        IOC::IOCContainer::instance().RegisterFactory<I_Print, Print_Bar>();
        chart->print_Data(blackAndWhiteCheckBox->isChecked());//и перерисовывваем диаграмму
        break;
    case Types_of_Charts::pie ://если выбран pie, то помещаем в контейнер принтер pie
        IOC::IOCContainer::instance().RegisterFactory<I_Print, Print_Pie>();
        chart->print_Data(blackAndWhiteCheckBox->isChecked());//и перерисовывваем диаграмму
        break;
    }

}

void Chart::print_Data(bool blackAndWhite)//отрисовываем диаграмму в зависимости чёрно-белая она или нет
{
    IOC::IOCContainer::instance().getObject<I_Print>()->create_Chart(view, data, blackAndWhite);// получаем из контейнера нужный принтер и перерисовываем диаграмму
}

void Chart::read_Data(const QString& filePath)//считываем информацию из файла
{
    data = IOC::IOCContainer::instance().getObject<I_Reader>()->read_Data(filePath);// получаем из контейнера нужный ридер и обновляем данные
}
