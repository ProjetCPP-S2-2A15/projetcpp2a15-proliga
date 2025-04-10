#ifndef STATISTIQUE_H
#define STATISTIQUE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QPair>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>

class Statistique : public QObject
{
    Q_OBJECT
public:
    Statistique(QObject *parent = nullptr);
    ~Statistique();

    int obtenirNombreJoueurs();
    int obtenirNombreButsMarques();
    QChartView* creerGraphiqueJoueursEtButs();

private:
         // Database connection or other members if needed
};

#endif // STATISTIQUE_H
