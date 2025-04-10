#ifndef STADES_H
#define STADES_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPainter>
#include <QPdfWriter>
#include <QFileDialog>

class Stade {
private:
    int ID_stade;
    QString nom;
    QString lieu;
    int capacite;
    int nbr_tickets_vd;
    QDate date_creation;

public:
    Stade() {}
    Stade(QString, QString, int, int, QDate);

    int getID() { return ID_stade; }
    QString getNom() { return nom; }
    QString getLieu() { return lieu; }
    int getCapacite() { return capacite; }
    int getNbrTicketsVd() { return nbr_tickets_vd; }
    QDate getDateCreation() { return date_creation; }

    void setNom(QString name) { nom = name; }
    void setLieu(QString location) { lieu = location; }
    void setCapacite(int capacity) { capacite = capacity; }
    void setNbrTicketsVd(int tickets) { nbr_tickets_vd = tickets; }
    void setDateCreation(QDate date) { date_creation = date; }

    bool ajouter();
    QSqlQueryModel* afficher();
    bool modifierParNom(QString oldName);
    bool supprimer(QString name);
    bool idExisteParNom(QString name);
    Stade getStadeByName(QString name);
    QSqlQueryModel* rechercherParNom(QString nomRecherche);
    QSqlQueryModel* rechercherParCapacite(int capaciteMin, int capaciteMax);
    QSqlQueryModel* trier(QString critere, QString ordre);

    bool getCoordinatesFromLieu(double &latitude, double &longitude) const;
};

#endif // STADES_H
