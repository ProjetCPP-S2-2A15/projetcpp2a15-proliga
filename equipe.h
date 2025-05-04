

#ifndef EQUIPE_H
#define EQUIPE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Equipe {
private:
    int idEquipe;                // Identifiant de l'équipe (clé primaire)
    QString nomEquipe;           // Nom de l'équipe
    QString nomVille;            // Nom de la ville
    QString nomEntraineur;       // Nom de l'entraîneur
    int nombreJoueurs;           // Nombre de joueurs
    QString type;                // Type (national ou normal)
    int nombreButsMarques;       // Nombre de buts marqués
    QDate dateDebutContrat;      // Date de début de contrat
    QDate dateFinContrat;        // Date de fin de contrat

public:
    // Constructeur


    Equipe( const QString& nom, const QString& ville,
           const QString& entraineur, int nbJoueurs, const QString& typeEquipe,
           int butsMarques, const QDate& debutContrat, const QDate& finContrat);
    Equipe(); // Add this line

    // Accesseurs
    int getIdEquipe() const;
    QString getNomEquipe() const;
    QString getNomVille() const;
    QString getNomEntraineur() const;
    int getNombreJoueurs() const;
    QString getType() const;
    int getNombreButsMarques() const;
    QDate getDateDebutContrat() const;
    QDate getDateFinContrat() const;

    // Mutateurs
    void setIdEquipe(int id);

    void setNomEquipe(const QString& nom);
    void setNomVille(const QString& ville);
    void setNomEntraineur(const QString& entraineur);
    void setNombreJoueurs(int nbJoueurs);
    void setType(const QString& typeEquipe);
    void setNombreButsMarques(int butsMarques);
    void setDateDebutContrat(const QDate& debutContrat);
    void setDateFinContrat(const QDate& finContrat);

    bool Ajouter();
    QSqlQueryModel *loadequipeData();
    bool Delete(int id); // Add this method to delete a team by ID


};

#endif // EQUIPE_H
