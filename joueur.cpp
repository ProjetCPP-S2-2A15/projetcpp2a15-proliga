#include "joueur.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>

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

    query.prepare("INSERT INTO joueur1 (Nom, Prenom, Date_de_naissance, Pays_origine, Position) "
                  "VALUES (:Nom, :Prenom, :Date_de_naissance, :Pays_origine, :Position)");

    query.bindValue(":Nom", this->Nom);
    query.bindValue(":Prenom", this->Prenom);
    query.bindValue(":Date_de_naissance", this->Date_de_naissance);
    query.bindValue(":Pays_origine", this->Pays_origine);
    query.bindValue(":Position", this->Position);

    if (!query.exec()) {
        qDebug() << "Failed to add Joueur: " << query.lastError().text();
    } else {
        qDebug() << "Joueur added successfully!";
    }
}


void Joueur::readJoueur(QTableWidget *tableWidget) {
    if (!tableWidget) return;

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

    QSqlQuery query("SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE FROM joueur1");

    tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        tableWidget->insertRow(row);

        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("Nom").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Prenom").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("Position").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Date_de_naissance").toDate().toString("yyyy-MM-dd")));
        tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("Pays_origine").toString()));

        row++;
    }

    if (query.lastError().isValid()) {
        qDebug() << "Error reading players:" << query.lastError().text();
    } else {
        qDebug() << "Players loaded successfully!";
    }
}

void Joueur::deleteJoueur(const QString &nom) {
    if (nom.isEmpty()) {
        qDebug() << "Player name is empty!";
        return;
    }

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
    query.prepare("DELETE FROM joueur1 WHERE Nom = :Nom");
    query.bindValue(":Nom", nom);

    if (!query.exec()) {
        qDebug() << "Failed to delete Joueur:" << query.lastError().text();
    } else {
        qDebug() << "Joueur deleted successfully!";
    }
}

bool Joueur::updateJoueur(const QString &nom, const QString &prenom, const QDate &date, const QString &position, const QString &paysOrigine) {
    Connection conn;

    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return false;
    }

    QSqlDatabase db = conn.getDatabase();

    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE joueur1 SET Prenom = :prenom, Date_de_naissance = TO_DATE(:date, 'YYYY-MM-DD'), Position = :position, Pays_origine = :paysOrigine WHERE Nom = :nom");

    // Bind values
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":position", position);
    query.bindValue(":paysOrigine", paysOrigine);

    if (!query.exec()) {
        qDebug() << "Error updating player: " << query.lastError().text();
        return false;
    } else {
        qDebug() << "Player updated successfully!";
        return true;
    }
}


void Joueur::rechercheJoueur(QTableWidget *tableWidget, const QString &nom) {
    if (!tableWidget) return;

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

    QString trimmedNom = nom.trimmed();
    QString lowerCaseNom = trimmedNom.toLower();

    QSqlQuery query(db);
    QString queryString = QString("SELECT Nom, Prenom, Pays_origine, Position, Date_de_naissance "
                                  "FROM joueur1 WHERE LOWER(Nom) = LOWER('%1')").arg(lowerCaseNom);
    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("Nom").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Prenom").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("Position").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Date_de_naissance").toDate().toString("yyyy-MM-dd")));
        tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("Pays_origine").toString()));

        row++;
    }

    if (row == 0) {
        qDebug() << "No players found for the given name:" << nom;
    } else {
        qDebug() << "Players loaded successfully!";
    }
}

