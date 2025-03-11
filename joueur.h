#ifndef JOUEUR_H
#define JOUEUR_H

#include <QString>
#include <QDate>

class Joueur
{
public:
    Joueur();
    void addJoueur();
    void deleteJoueur();
    void updateJoueur();
    void readJoueur();

    // Setters
    void setNom(const QString &nom) { Nom = nom; }
    void setPrenom(const QString &prenom) { Prenom = prenom; }
    void setDateDeNaissance(const QDate &date) { Date_de_naissance = date; }
    void setPaysOrigine(const QString &pays) { Pays_origine = pays; }
    void setPosition(const QString &position) { Position = position; }
    void setSalaire(float s) { salaire = s; }

    // Getters
    QString getNom() const { return Nom; }
    QString getPrenom() const { return Prenom; }
    QDate getDateDeNaissance() const { return Date_de_naissance; }
    QString getPaysOrigine() const { return Pays_origine; }
    QString getPosition() const { return Position; }
    float getSalaire() const { return salaire; }


private:
    int id_joueur;
    QString Nom;
    QString Prenom;
    QDate Date_de_naissance;
    QString Pays_origine;
    QString Position;
    float salaire;
};

#endif // JOUEUR_H
