#ifndef JOUEUR_H
#define JOUEUR_H

#include <QString>
#include <QDate>
#include <QTableWidget>
#include <QComboBox>

class Joueur
{
public:
    Joueur();
    void addJoueur();
    void deleteJoueur(const QString &nom);
    bool updateJoueur(const QString &nom, const QString &prenom, const QDate &date, const QString &position, const QString &paysOrigine);
    void readJoueur(QTableWidget *tableWidget);
    void rechercheJoueur(QTableWidget *tableWidget, const QString &nom);
    void rechercheJoueurFilter(QTableWidget *tableWidget, const QString &arg1);
    QVector<Joueur> getJoueurs();
    void getListe(QComboBox* liste);

    // Setters
    void setNom(const QString &nom) { Nom = nom; }
    void setPrenom(const QString &prenom) { Prenom = prenom; }
    void setDateDeNaissance(const QDate &date) { Date_de_naissance = date; }
    void setPaysOrigine(const QString &pays) { Pays_origine = pays; }
    void setPosition(const QString &position) { Position = position; }
    void setImgPath(const QString &imgPath) { ImgPath = imgPath; }
    void setSalaire(float s) { salaire = s; }
    void setFilter(int x) { filter = x; }
    void setNom_Equipe(const QString &nom_Equipe) { Nom_Equipe=nom_Equipe; }

    // Getters
    QString getNom() const { return Nom; }
    QString getPrenom() const { return Prenom; }
    QDate getDateDeNaissance() const { return Date_de_naissance; }
    QString getPaysOrigine() const { return Pays_origine; }
    QString getPosition() const { return Position; }
    QString getImgPath() const { return ImgPath; }
    QString getNom_Equipe() const { return Nom_Equipe; }
    float getSalaire() const { return salaire; }


private:
    int id_joueur;
    QString Nom;
    QString Prenom;
    QDate Date_de_naissance;
    QString Pays_origine;
    QString Position;
    QString ImgPath;
    QString Nom_Equipe;
    float salaire;
    int filter = 1;
};

#endif // JOUEUR_H
