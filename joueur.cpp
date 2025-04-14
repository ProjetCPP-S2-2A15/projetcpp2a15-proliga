#include "joueur.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QSqlError>
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

    //QSqlQuery query("SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE FROM joueur1");
    QString queryString;
    if (filter == 1) {
        // Filter by Name (Order by NOM)
        queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE FROM joueur1 ORDER BY NOM";
    } else if (filter == 2) {
        // Filter by Nationality (Order by PAYS_ORIGINE)
        queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE FROM joueur1 ORDER BY PAYS_ORIGINE";
    } else {
        // Filter by Position (Order by POSITION)
        queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE FROM joueur1 ORDER BY POSITION";
    }

    // Create the query using the constructed query string
    //QSqlQuery query(queryString);
    QSqlQuery query(queryString, db);

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


QVector<Joueur> Joueur::getJoueurs() {
    QVector<Joueur> joueurs;

    Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return joueurs;
    }

    QSqlDatabase db = conn.getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return joueurs;
    }

    QString queryString;
    queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE FROM joueur1";

    QSqlQuery query(queryString, db);

    while (query.next()) {
        Joueur j;
        j.setNom(query.value("NOM").toString());
        j.setPrenom(query.value("PRENOM").toString());
        j.setPaysOrigine(query.value("PAYS_ORIGINE").toString());
        j.setPosition(query.value("POSITION").toString());
        j.setDateDeNaissance(query.value("DATE_DE_NAISSANCE").toDate());

        joueurs.append(j);
    }

    if (query.lastError().isValid()) {
        qDebug() << "Error reading players:" << query.lastError().text();
    } else {
        qDebug() << "Players loaded successfully!";
    }

    return joueurs;
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

void Joueur::rechercheJoueurFilter(QTableWidget *tableWidget, const QString &arg1) {
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

    QSqlQuery query(db);
    QString queryString = "SELECT Nom, Prenom, Pays_origine, Position, Date_de_naissance FROM joueur1";

    // Modify query based on selected filter option
    if (arg1 == "Nom") {
        queryString += " ORDER BY Nom";
    } else if (arg1 == "Nationalite") {
        queryString += " ORDER BY Pays_origine";
    } else if (arg1 == "Position") {
        queryString += " ORDER BY Position";
    }

    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    tableWidget->clearContents();  // Clear previous rows but keep the table structure (headers)

    int row = 0;
    bool playersFound = false;

    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("Nom").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Prenom").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("Position").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Date_de_naissance").toDate().toString("yyyy-MM-dd")));
        tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("Pays_origine").toString()));

        row++;
        playersFound = true;
    }

    if (!playersFound) {
        qDebug() << "No players found for the selected filter: " << arg1;
    } else {
        qDebug() << "Players loaded successfully!";
    }
}
