#ifndef STADES_H
#define STADES_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>


class Stade {
private:
    int ID_stade;  // Auto-incremented by the database, no need to set it manually
    QString nom;
    QString lieu;
    int capacite;
    int nbr_tickets_vd;
    QDate date_creation;

public:
    // Constructors
    Stade() {}
    Stade(QString, QString, int, int, QDate);  // Removed the ID parameter

    // Getters
    int getID() { return ID_stade; }
    QString getNom() { return nom; }
    QString getLieu() { return lieu; }
    int getCapacite() { return capacite; }
    int getNbrTicketsVd() { return nbr_tickets_vd; }
    QDate getDateCreation() { return date_creation; }

    // Setters
    void setNom(QString name) { nom = name; }
    void setLieu(QString location) { lieu = location; }
    void setCapacite(int capacity) { capacite = capacity; }
    void setNbrTicketsVd(int tickets) { nbr_tickets_vd = tickets; }
    void setDateCreation(QDate date) { date_creation = date; }

    // Basic functionality for the Stade entity
    bool ajouter();
    QSqlQueryModel* afficher();
    bool modifier(int id);
    bool supprimer(int id);


};

#endif // STADES_H

