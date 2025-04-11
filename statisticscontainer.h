#ifndef STATISTICSCONTAINER_H
#define STATISTICSCONTAINER_H

#include <QWidget>
#include <QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPieSeries>

QT_BEGIN_NAMESPACE
class QSqlQuery;
QT_END_NAMESPACE

class StatisticsContainer : public QWidget
{
    Q_OBJECT

public:
    StatisticsContainer(QWidget *parent = nullptr);

    // Static function declarations
    static QChartView* generateHighPoolDeGainDisparityChart();
    static QChartView* generatePercentageOfGainsChart();
};

#endif // STATISTICSCONTAINER_H
