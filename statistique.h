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
    explicit Statistique(QObject *parent = nullptr);
    ~Statistique();

    QMap<QString, int> obtenirButsParEquipe();
    QChartView* creerGraphiqueButsParEquipe();

private:
         // Database connection or other members if needed
};

#endif // STATISTIQUE_H
