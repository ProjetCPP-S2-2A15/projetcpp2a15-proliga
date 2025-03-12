#include "stades.h"

Stade::Stade(QString name, QString location, int capacity, int tickets, QDate date) {
    this->nom = name;
    this->lieu = location;
    this->capacite = capacity;
    this->nbr_tickets_vd = tickets;
    this->date_creation = date;
}

// Add a new stadium
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

// Display all stadiums
QSqlQueryModel* Stade::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Stades");

    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Creation"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("capacite"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("tickets vendus"));

    return model;
}
// Update an existing stadium
bool Stade::modifier(int id)
{
    QSqlQuery query;

    // Préparer la requête SQL pour mettre à jour le stade correspondant à l'ID donné
    query.prepare("UPDATE Stades SET nom = :nom, lieu = :lieu, capacite = :capacite, "
                  "nbr_tickets_vd = :nbr_tickets_vd, date_creation = :date_creation "
                  "WHERE ID_stade = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":lieu", lieu);
    query.bindValue(":capacite", capacite);
    query.bindValue(":nbr_tickets_vd", nbr_tickets_vd);
    query.bindValue(":date_creation", date_creation);
    query.bindValue(":id", id);

    // Exécuter la requête et retourner le résultat
    return query.exec();
}

// Delete a stadium by ID
bool Stade::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Stades WHERE ID_stade = :id");
    query.bindValue(":id", id);

    return query.exec();
}

