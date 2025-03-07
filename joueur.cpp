#include "joueur.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

Joueur::Joueur() {}

void Joueur::addJoueur() {
    Connection conn;

    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();

    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    query.prepare("INSERT INTO joueurs (Nom, Prenom, Date_de_naissance, Pays_origine) "
                  "VALUES (:Nom, :Prenom, :Date_de_naissance, :Pays_origine)");

    query.bindValue(":Nom", this->Nom);
    query.bindValue(":Prenom", this->Prenom);
    query.bindValue(":Date_de_naissance", this->Date_de_naissance);
    query.bindValue(":Pays_origine", this->Pays_origine);

    if (!query.exec()) {
        qDebug() << "Failed to add Joueur: " << query.lastError().text();
    } else {
        qDebug() << "Joueur added successfully!";
    }
}
