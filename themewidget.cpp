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
#include "themewidget.h"


ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(1),
    m_valueMax(10),
    m_valueCount(10),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
    m_typeComboBox(createTypeBox()),
    m_notColoredCheckBox(new QCheckBox("Черно-белый график")),
    m_printButton(new QPushButton("Печать графика"))
{
    connectSignals();
    QVBoxLayout *baseLayout = new QVBoxLayout();
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel("Выберите тип диаграммы:"));
    settingsLayout->addWidget(m_typeComboBox);
    settingsLayout->addWidget(m_notColoredCheckBox);
    settingsLayout->addWidget(m_printButton);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout);

    //create chart

    chartView = new QChartView();

    baseLayout->addWidget(chartView);

    setLayout(baseLayout);

    // Set defaults
    m_notColoredCheckBox->setChecked(true);
    updateUI();
}

ThemeWidget::~ThemeWidget()
{
}

void ThemeWidget::connectSignals()
{
    connect(m_typeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);
    connect(m_notColoredCheckBox, &QCheckBox::toggled, this, &ThemeWidget::updateUI);
    connect(m_printButton, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));
}

void ThemeWidget::openFileDialogWindow()
{
    QFileDialog *fileDialog = new QFileDialog(this);
             // определить заголовок файла
    fileDialog-> setWindowTitle (tr ("Открыть изображение"));
             // Установить путь к файлу по умолчанию
    fileDialog->setDirectory(".");
             // Установить фильтр файлов
    fileDialog->setNameFilter(tr("Images(*.sqlite *.json)"));
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
}


DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}

QComboBox *ThemeWidget::createTypeBox() const
{
    // type layout
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Bar", Types_of_Charts::Bar);
    themeComboBox->addItem("Pie", Types_of_Charts::pie);
    return themeComboBox;
}

void ThemeWidget::updateUI()
{
//    TypeThemeWidget typeChart = static_cast<TypeThemeWidget>(
//                m_typeComboBox->itemData(m_typeComboBox->currentIndex()).toInt());

//    switch (typeChart)
//    {
//    case TypeThemeWidget::Area :
//        chartView->setChart(createAreaChart(m_notColoredCheckBox->isChecked()));
//        break;
//    case TypeThemeWidget::Bar :
//        chartView->setChart(createBarChart(m_notColoredCheckBox->isChecked()));
//        break;
//    case TypeThemeWidget::Line :
//        chartView->setChart(createLineChart(m_notColoredCheckBox->isChecked()));
//        break;
//    case TypeThemeWidget::Pie :
//        chartView->setChart(createPieChart(m_notColoredCheckBox->isChecked()));
//        break;
//    case TypeThemeWidget::Spline :
//        chartView->setChart(createSplineChart(m_notColoredCheckBox->isChecked()));
//        break;
//    case TypeThemeWidget::Scatter :
//        chartView->setChart(createScatterChart(m_notColoredCheckBox->isChecked()));
//        break;
//    }
}

void Chart::print_Data(bool blackAndWhite)
{
//    *view = gContainer.getObject<I_Print>()->create_Chart(data, notColored);

}

void Chart::read_Data(const QString& filePath)
{
//    data = gContainer.getObject<I_Reader>()->read_Data(filePath);
}
