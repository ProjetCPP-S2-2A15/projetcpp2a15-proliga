#include "stades.h"
Stade::Stade(QString name, QString location, int capacity, int tickets, QDate date) {
    this->nom = name;
    this->lieu = location;
    this->capacite = capacity;
    this->nbr_tickets_vd = tickets;
    this->date_creation = date;
}

bool Stade::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO Stades (nom, lieu, capacite, nbr_tickets_vd, date_creation) "
                  "VALUES (:nom, :lieu, :capacite, :nbr_tickets_vd, :date_creation)");
    query.bindValue(":nom", nom);
    query.bindValue(":lieu", lieu);
    query.bindValue(":capacite", capacite);
    query.bindValue(":nbr_tickets_vd", nbr_tickets_vd);
    query.bindValue(":date_creation", date_creation);

    if (query.exec()) {
        ID_stade = query.lastInsertId().toInt();
        return true;
    }
    return false;
}

QSqlQueryModel* Stade::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tickets vendus"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date de création"));

    return model;
}

bool Stade::modifierParNom(QString oldName) {
    QSqlQuery query;
    query.prepare("UPDATE Stades SET nom = :nom, lieu = :lieu, capacite = :capacite, "
                  "nbr_tickets_vd = :nbr_tickets_vd, date_creation = :date_creation "
                  "WHERE nom = :oldName");
    query.bindValue(":nom", nom);
    query.bindValue(":lieu", lieu);
    query.bindValue(":capacite", capacite);
    query.bindValue(":nbr_tickets_vd", nbr_tickets_vd);
    query.bindValue(":date_creation", date_creation);
    query.bindValue(":oldName", oldName);

    return query.exec();
}

bool Stade::supprimer(QString name) {
    QSqlQuery query;
    query.prepare("DELETE FROM Stades WHERE nom = :nom");
    query.bindValue(":nom", name);

    return query.exec();
}
bool Stade::idExisteParNom(QString name) {
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Stades WHERE nom = :nom");
    query.bindValue(":nom", name);
    return query.exec() && query.next();
}

Stade Stade::getStadeByName(QString name) {
    QSqlQuery query;
    query.prepare("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades WHERE nom = :nom");
    query.bindValue(":nom", name);

    Stade stade;

    if (query.exec() && query.next()) {
        stade.nom = query.value("nom").toString();
        stade.lieu = query.value("lieu").toString();
        stade.capacite = query.value("capacite").toInt();
        stade.nbr_tickets_vd = query.value("nbr_tickets_vd").toInt();
        stade.date_creation = query.value("date_creation").toDate();
    }
    return stade;
}
QSqlQueryModel* Stade::rechercherParNom(QString nomRecherche) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades WHERE LOWER(nom) LIKE LOWER(:nom)");
    query.bindValue(":nom", "%" + nomRecherche.toLower() + "%");
    query.exec();

    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tickets vendus"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date de création"));

    return model;
}
QSqlQueryModel* Stade::rechercherParCapacite(int capaciteMin, int capaciteMax) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades WHERE capacite BETWEEN :min AND :max");
    query.bindValue(":min", capaciteMin);
    query.bindValue(":max", capaciteMax);
    query.exec();

    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tickets vendus"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date de création"));

    return model;
}
QSqlQueryModel* Stade::trier(QString critere, QString ordre) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades ORDER BY " + critere + " " + ordre);
    query.exec();

    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tickets vendus"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date de création"));

    return model;
}

