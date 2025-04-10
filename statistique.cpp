#include "statistique.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>

Statistique::Statistique(QObject *parent) : QObject(parent)
{
}

Statistique::~Statistique()
{
}

int Statistique::obtenirNombreJoueurs()
{
    QSqlQuery query;
    query.prepare("SELECT SUM(NOMBREJOUEURS) FROM equipe");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête SQL :" << query.lastError().text();
        return 0;
    }
}

int Statistique::obtenirNombreButsMarques()
{
    QSqlQuery query;
    query.prepare("SELECT SUM(NOMBREBUTSMARQUES) FROM equipe");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête SQL :" << query.lastError().text();
        return 0;
    }
}

QChartView* Statistique::creerGraphiqueJoueursEtButs()
{
    int nombreJoueurs = this->obtenirNombreJoueurs();
    int nombreButs = this->obtenirNombreButsMarques();

    QPieSeries *series = new QPieSeries();
    QPieSlice *joueursSlice = series->append("Joueurs", nombreJoueurs);
    QPieSlice *butsSlice = series->append("Buts", nombreButs);

    // Customize slices
    joueursSlice->setColor(QColor(32, 159, 223));  // Blue
    butsSlice->setColor(QColor(255, 126, 24));    // Orange
    joueursSlice->setLabelVisible();
    butsSlice->setLabelVisible();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques Joueurs vs Buts");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
