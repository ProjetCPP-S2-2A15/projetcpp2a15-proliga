#include "equipe.h"


// Constructeur
Equipe::Equipe(int id, const QString& nom, const QString& ville,
               const QString& entraineur, int nbJoueurs, const QString& typeEquipe,
               int butsMarques, const QDate& debutContrat, const QDate& finContrat)
 {this->idEquipe=id;
     this->nomEquipe=nom;
     this->nomVille=ville;
     this->nomEntraineur=entraineur;
     this->nombreJoueurs=nbJoueurs;
     this->type=typeEquipe;
     this->nombreButsMarques=butsMarques;
     this->dateDebutContrat=debutContrat;
     this->dateFinContrat=finContrat;

 }

// Accesseurs
int Equipe::getIdEquipe() const {
    return idEquipe;
}

QString Equipe::getNomEquipe() const {
    return nomEquipe;
}

QString Equipe::getNomVille() const {
    return nomVille;
}
QString Equipe::getNomEntraineur() const {
    return nomEntraineur;
}
int Equipe::getNombreJoueurs() const {
    return nombreJoueurs;
}
QString Equipe::getType() const {
    return type;
}
int Equipe::getNombreButsMarques() const {
    return nombreButsMarques;
}
QDate Equipe::getDateDebutContrat() const {
    return dateDebutContrat;
}

QDate Equipe::getDateFinContrat() const {
    return dateFinContrat;
}

// Mutateurs
void Equipe::setNomEquipe(const QString& nom) {
    nomEquipe = nom;
}

void Equipe::setNomVille(const QString& ville) {
    nomVille = ville;
}

void Equipe::setNomEntraineur(const QString& entraineur) {
    nomEntraineur = entraineur;
}

void Equipe::setNombreJoueurs(int nbJoueurs) {
    nombreJoueurs = nbJoueurs;
}

void Equipe::setType(const QString& typeEquipe) {
    type = typeEquipe;
}

void Equipe::setNombreButsMarques(int butsMarques) {
    nombreButsMarques = butsMarques;
}

void Equipe::setDateDebutContrat(const QDate& debutContrat) {
    dateDebutContrat = debutContrat;
}
void Equipe::setDateFinContrat(const QDate& finContrat) {
    dateFinContrat = finContrat;
}
bool Equipe::ajouter() {
    QSqlQuery query;

    // Prepare the SQL query
    query.prepare("INSERT INTO equipe (IDEQUIPE, NOMEQUIPE, NOMVILLE, NOMENTRAINEUR, NOMBREJOUEURS, TYPE, NOMBREBUTSMARQUES, DATEDEBUTCONTRAT, DATEFINCONTRAT) "
                  "VALUES (:idEquipe, :nomEquipe, :nomVille, :nomEntraineur, :nombreJoueurs, :type, :nombreButsMarques, :dateDebutContrat, :dateFinContrat)");

    // Bind the values
    query.bindValue(":idEquipe", idEquipe);
    query.bindValue(":nomEquipe", nomEquipe);
    query.bindValue(":nomVille", nomVille);
    query.bindValue(":nomEntraineur", nomEntraineur);
    query.bindValue(":nombreJoueurs", nombreJoueurs);
    query.bindValue(":type", type);
    query.bindValue(":nombreButsMarques", nombreButsMarques);
    query.bindValue(":dateDebutContrat", dateDebutContrat);
    query.bindValue(":dateFinContrat", dateFinContrat);

    // Execute the query and check if it was successful
    if (query.exec()) {
        return true; // Insertion successful
    } else {
        qDebug() << "Error inserting into database: " << query.lastError().text();
        return false; // Insertion failed
    }
}
