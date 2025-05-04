#include "statistique.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QToolTip>
#include <QCursor>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>

Statistique::Statistique(QObject *parent) : QObject(parent)
{
}

Statistique::~Statistique()
{
}

QMap<QString, int> Statistique::obtenirButsParEquipe()
{
    QMap<QString, int> butsParEquipe;
    QSqlQuery query("SELECT NOMEQUIPE, NOMBREBUTSMARQUES FROM equipe");

    while (query.next()) {
        QString nomEquipe = query.value(0).toString();
        int buts = query.value(1).toInt();
        butsParEquipe[nomEquipe] = buts;
    }

    if (query.lastError().isValid()) {
        qDebug() << "Erreur lors de l'exécution de la requête SQL :" << query.lastError().text();
    }

    return butsParEquipe;
}

QChartView* Statistique::creerGraphiqueButsParEquipe()
{
    QMap<QString, int> butsParEquipe = this->obtenirButsParEquipe();

    QPieSeries *series = new QPieSeries();

    // Définir une palette de couleurs vertes similaires à l'exemple
    QVector<QColor> greenShades = {
        QColor(0, 100, 0), QColor(34, 139, 34), QColor(50, 205, 50), QColor(60, 179, 113),
        QColor(46, 139, 87), QColor(107, 142, 35), QColor(154, 205, 50), QColor(85, 107, 47),
        QColor(144, 238, 144), QColor(173, 255, 47), QColor(0, 255, 127), QColor(0, 250, 154)
    };

    int colorIndex = 0;
    int totalButs = 0;

    // Calculer le total des buts
    for (int buts : butsParEquipe.values()) {
        totalButs += buts;
    }

    // Créer les tranches pour chaque équipe
    QMapIterator<QString, int> it(butsParEquipe);
    while (it.hasNext()) {
        it.next();
        QString nomEquipe = it.key();
        int buts = it.value();

        if (buts > 0) { // Ne pas afficher les équipes avec 0 but
            double pourcentage = (totalButs > 0) ? (buts * 100.0 / totalButs) : 0;
            QString label = QString("%1 (%2%)").arg(nomEquipe).arg(QString::number(pourcentage, 'f', 1));

            QPieSlice *slice = series->append(label, buts);
            QColor sliceColor = greenShades[colorIndex % greenShades.size()];
            slice->setBrush(sliceColor);
            colorIndex++;

            // Configurer l'affichage des labels
            slice->setLabelVisible();
            slice->setLabelColor(Qt::white);
            slice->setLabelPosition(QPieSlice::LabelOutside);

            // Ajouter un effet de survol
            connect(slice, &QPieSlice::hovered, [slice, nomEquipe, buts, totalButs](bool hovered) {
                if (hovered) {
                    double pourcentage = (buts * 100.0 / totalButs);
                    slice->setExploded(true);
                    QToolTip::showText(QCursor::pos(),
                                       QString("Équipe: %1\nButs marqués: %2\nPourcentage: %3%")
                                           .arg(nomEquipe)
                                           .arg(buts)
                                           .arg(QString::number(pourcentage, 'f', 1)));
                } else {
                    slice->setExploded(false);
                }
            });
        }
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des buts marqués" "\n ""par équipe");
    chart->legend()->hide(); // On cache la légende par défaut

    // Personnaliser le titre
    QFont titleFont;
    titleFont.setPointSize(10);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(Qt::black));

    // Créer la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Ajouter un effet d'opacité avec animation
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(chartView);
    chartView->setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(0);

    QTimer::singleShot(0, [opacityEffect]() {
        QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
        animation->setDuration(5000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // Créer une légende personnalisée
    QGraphicsScene *scene = chartView->scene();
    QGridLayout *legendLayout = new QGridLayout();
    legendLayout->setSpacing(5);

    QWidget *legendWidget = new QWidget();
    legendWidget->setLayout(legendLayout);
    legendWidget->setStyleSheet("background-color: transparent;");

    scene->addWidget(legendWidget);

    // Ajouter les éléments de légende
    colorIndex = 0;
    int columns = 4;
    int itemIndex = 0;

    for (const QString &nomEquipe : butsParEquipe.keys()) {
        int buts = butsParEquipe[nomEquipe];
        if (buts > 0) {
            int row = itemIndex / columns;
            int col = itemIndex % columns;

            QHBoxLayout *rowLayout = new QHBoxLayout();

            QFrame *colorBox = new QFrame();
            colorBox->setFrameShape(QFrame::Box);
            colorBox->setFixedSize(10, 10);
            colorBox->setStyleSheet(QString("background-color: %1").arg(greenShades[colorIndex % greenShades.size()].name()));

            double pourcentage = (buts * 100.0 / totalButs);
            QLabel *textItem = new QLabel(QString("%1 (%2%)").arg(nomEquipe).arg(QString::number(pourcentage, 'f', 1)));
            textItem->setStyleSheet("font-size: 8px;");

            rowLayout->addWidget(colorBox);
            rowLayout->addWidget(textItem);
            legendLayout->addLayout(rowLayout, row, col);

            colorIndex++;
            itemIndex++;
        }
    }

    // Positionner la légende
    legendWidget->setGeometry(20, chart->size().height() - 100, 300, 100);

    return chartView;
}
