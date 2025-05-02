#include "statisticscontainer.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

// Constructor
StatisticsContainer::StatisticsContainer(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Add generated charts by calling static methods directly
    layout->addWidget(StatisticsContainer::generateHighPoolDeGainDisparityChart());  // Using the class name to call the static method
    layout->addWidget(StatisticsContainer::generatePercentageOfGainsChart());  // Using the class name to call the static method
}

// Static function to generate the Pool de Gains disparity chart
QChartView* StatisticsContainer::generateHighPoolDeGainDisparityChart() {
    QSqlQuery query("SELECT NOM, POOL_GAINS FROM CHAMPIONNATS");

    QBarSet *set = new QBarSet("Pool de Gains");
    QStringList categories;

    while (query.next()) {
        QString nom = query.value(0).toString();
        double poolGains = query.value(1).toDouble();
        *set << poolGains;
        categories << nom;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Disparity of Pool de Gains between Championnats");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

// Static function to generate the percentage of gains chart
QChartView* StatisticsContainer::generatePercentageOfGainsChart() {
    QSqlQuery query("SELECT NOM, POOL_GAINS, NBR_EQUIPE FROM CHAMPIONNATS");

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Percentage of Gains");

    // Store championship names dynamically
    QStringList championshipNames;
//hrllo
    while (query.next()) {
        QString nom = query.value(0).toString();
        double poolGains = query.value(1).toDouble();
        int nbrEquipes = query.value(2).toInt();

        // Debug output to check the values of poolGains and nbrEquipes
        qDebug() << "Championship: " << nom << " | Pool Gains: " << poolGains << " | Nbr Equipes: " << nbrEquipes;

        if (nbrEquipes == 0) {
            qDebug() << "Skipping " << nom << " because NBR_EQUIPE is 0";
            continue;  // Avoid division by zero
        }

        double bf_percentage = (poolGains / nbrEquipes);
        double percent = (bf_percentage/poolGains)*100;

        // Debug output to check the percentage calculation
        qDebug() << "Calculated Percentage for " << nom << ": " << bf_percentage;

        set->append(percent);
        championshipNames.append(nom);  // Store championship names for X-axis
    }

    series->append(set);
//hello
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Percentage of Gains per Team");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set up the X-axis dynamically with the championship names
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(championshipNames);  // Use dynamic names from the query
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Set up the Y-axis with an appropriate range for percentage
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);  // Percentage ranges from 0% to 100%
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}
