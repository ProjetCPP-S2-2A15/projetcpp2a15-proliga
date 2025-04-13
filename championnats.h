#ifndef CHAMPIONNATS_H
#define CHAMPIONNATS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Championnats {
private:
    int ID_champ;
    int nbr_equipe;
    QString nom;
    QString type;
    QString organizateur;
    double pool_gains;

public:
    Championnats() {}
    Championnats(int, int, QString, QString, QString, double);

    int getID() { return ID_champ; }
    int getNbrEquipe() { return nbr_equipe; }
    QString getNom() { return nom; }
    QString getType() { return type; }
    QString getOrganizateur() { return organizateur; }
    double getPoolGains() { return pool_gains; }

    void setNbrEquipe(int teams) { nbr_equipe = teams; }
    void setNom(QString name) { nom = name; }
    void setType(QString champType) { type = champType; }
    void setOrganizateur(QString organizer) { organizateur = organizer; }
    void setPoolGains(double gains) { pool_gains = gains; }

    QSqlQueryModel* afficher();

};

#endif // CHAMPIONNATS_H
