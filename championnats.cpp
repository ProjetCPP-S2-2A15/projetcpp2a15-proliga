#include "championnats.h"

Championnats::Championnats(int id, int teams, QString name, QString champType, QString organizer, double gains) {
    this->ID_champ = id;
    this->nbr_equipe = teams;
    this->nom = name;
    this->type = champType;
    this->organizateur = organizer;
    this->pool_gains = gains;
}

QSqlQueryModel* Championnats::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT nom, type FROM Championnats");
    query.exec();

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));

    return model;
}
