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
    m_listCount(3),
    m_valueMax(10),
    m_valueCount(5),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
    m_typeComboBox(createTypeBox()),
    m_notColoredCheckBox(new QCheckBox("Черно-белый график")),
    m_printButton(new QPushButton("Печать графика"))
{
    connectSignals();
    // create layout
    QVBoxLayout *baseLayout = new QVBoxLayout();
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel("Выберите тип диаграммы:"));
    settingsLayout->addWidget(m_typeComboBox);
    settingsLayout->addWidget(m_notColoredCheckBox);
    settingsLayout->addWidget(m_printButton);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout);

    //create chart

    chartView = new QChartView(createAreaChart());

    baseLayout->addWidget(chartView);

    setLayout(baseLayout);

    // Set defaults
    m_notColoredCheckBox->setChecked(true);
    updateUI();
}

ThemeWidget::~ThemeWidget()
{
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
    //for(auto tmp:fileNames)
        //qDebug()<<tmp<<endl;
}


DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // set seed for random stuff
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(rand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    /*
    DataTable dataTable;

    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("BLOOD_SUGAR.sqlite");
    if (sdb.open())
    {
        qDebug() << "BD open\n";
        QSqlQuery query("SELECT VALUE FROM BLOOD_SUGAR", sdb);
        int index = 0;
        DataList dataList;
        while (query.next() && (index < 10))
        {
            QString temp = query.value(0).toString();
            QPointF value(temp, index);

            dataList << Data(value, "BLOOD_SUGAR");

            index++;
        }
        dataTable << dataList;
    }
    else
    {
        qDebug() << "BD not open\n";
    }

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
*/

    return dataTable;
}

QComboBox *ThemeWidget::createTypeBox() const
{
    // type layout
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Bar", TypeThemeWidget::Bar);
    themeComboBox->addItem("Pie", TypeThemeWidget::Pie);
    return themeComboBox;
}





void ThemeWidget::updateUI()
{
    TypeThemeWidget typeChart = static_cast<TypeThemeWidget>(
                m_typeComboBox->itemData(m_typeComboBox->currentIndex()).toInt());

//    QBrush brush;
//    if (m_notColoredCheckBox->isChecked()) {
//        QRadialGradient gradient(50, 50, 50, 50, 50);
//        gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
//        gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));

//        brush = QBrush(gradient);
//    }
//    else
//    {
//        QRadialGradient gradient(50, 50, 50, 50, 50);
//        gradient.setColorAt(0, QColor::fromRgbF(0, 0, 1, 1));
//        gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));

//        brush = QBrush(gradient);
//    }


    //chartView->chart()->Series["Series 0"];

}

