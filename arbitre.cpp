#include "arbitre.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>

// Default constructor
Arbitre::Arbitre() : id_arbitre(0), age(0), experience(0) {}

// Constructor without ID since it's auto-incremented
Arbitre::Arbitre(QString nom, QString prenom, int age, int experience, QString sexe, QString email)
{
    this->nom = nom;
    this->prenom = prenom;
    this->age = age;
    this->experience = experience;
    this->sexe = sexe;
    this->email = email;
}

// ✅ Add a referee (ID is auto-generated)
bool Arbitre::ajouter() {
    QSqlQuery query;

    query.prepare("INSERT INTO \"C##HEDI\".\"ARBITRE\" (NOM, PRENOM, AGE, EXPERIENCE, SEXE, EMAIL) "
                  "VALUES (:nom, :prenom, :age, :experience, :sexe, :email)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":experience", experience);
    query.bindValue(":sexe", sexe);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "❌ SQL Insert Error:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Arbitre added successfully!";
    return true;
}

// ✅ Delete a referee
bool Arbitre::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM \"C##HEDI\".\"ARBITRE\" WHERE ID_ARBITRE = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ SQL Delete Error:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Arbitre deleted successfully!";
    return true;
}

// ✅ Modify a referee
bool Arbitre::modifier(int id, QString nom, QString prenom, int age, int experience, QString sexe, QString email) {
    QSqlQuery query;
    query.prepare("UPDATE \"C##HEDI\".\"ARBITRE\" SET NOM = :nom, PRENOM = :prenom, AGE = :age, "
                  "EXPERIENCE = :experience, SEXE = :sexe, EMAIL = :email WHERE ID_ARBITRE = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":experience", experience);
    query.bindValue(":sexe", sexe);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "❌ SQL Update Error:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Arbitre modified successfully!";
    return true;
}


// ✅ Display referees in QTableWidget
void Arbitre::afficher(QTableWidget *tableWidget) {
    QSqlQuery query;
    query.prepare("SELECT * FROM \"C##HEDI\".\"ARBITRE\"");

    if (!query.exec()) {
        qDebug() << "❌ SQL Select Error:" << query.lastError().text();
        return;
    }

    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Age", "Expérience", "Sexe", "Email"});

    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        for (int col = 0; col < 7; col++) {
            tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    qDebug() << "✅ Data loaded into tableWidget successfully!";
}
