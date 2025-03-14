#ifndef ARBITRE_H
#define ARBITRE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QTableWidget> // Include QTableWidget

class Arbitre {
public:
    // Default Constructor
    Arbitre();

    // Constructor for adding a referee (ID is auto-incremented, so it's not included)
    Arbitre(QString nom, QString prenom, int age, int experience, QString sexe, QString email);

    // Constructor with ID (for modifying or deleting an existing referee)
    Arbitre(int id, QString nom, QString prenom, int age, int experience, QString sexe, QString email);

    // CRUD Operations
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id, QString nom, QString prenom, int age, int experience, QString sexe, QString email);
    void afficher(QTableWidget *tableWidget);

private:
    int id_arbitre; // Auto-incremented by Oracle DB
    QString nom, prenom, sexe, email;
    int age, experience;
};

#endif // ARBITRE_H
