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

#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QPushButton>

#include "IOC.h"
#include "print.h"
#include "reader.h"


QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE

class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Chart;

class ChartWidget: public QWidget
{
    Q_OBJECT
public:
     ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();//деструктор

    void updateData(const QString& filePath);//функия для записи новых данных(при переключении с одной базы данных на другую)

private Q_SLOTS:
    void updateUI();
    void printToPdf();// функция, которая создаёт .pdf файл с диаграммой

private:
    QComboBox *createTypeBox() const;//создаём кнопку выбора диаграммы
    void connectSignals();

    QString getPathToSavePdf();//функция которая возвращает путь до выбранного файла

    QComboBox *typeComboBox;//кнопка выбора диаграммы
    QCheckBox *blackAndWhiteCheckBox;//чекбокс для выбора цвета графика
    QPushButton *printButton;//кнопка для создания .pdf файл с диаграммой

    Chart *chart;
};

class Chart
{
    QChartView view;
    DataList data;

public:
    Chart() {}//конструктор поумолчанию
    void print_Data(bool blackAndWhite = false);//вывод информации на диаграмму 
    void read_Data(const QString& filePath);//считывание информации из документа

    QChartView* getChartView() { return &view; }
};

#endif /* CHARTWIDGET_H */
