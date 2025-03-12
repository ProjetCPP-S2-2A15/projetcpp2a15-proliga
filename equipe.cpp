#include "equipe.h"
#include <QDebug>
#include <QSqlError>

// Constructeur

Equipe::Equipe( const QString& nom, const QString& ville,
               const QString& entraineur, int nbJoueurs, const QString& typeEquipe,
               int butsMarques, const QDate& debutContrat, const QDate& finContrat)
{
    this->nomEquipe = nom;
    this->nomVille = ville;
    this->nomEntraineur = entraineur;
    this->nombreJoueurs = nbJoueurs;
    this->type = typeEquipe;
    this->nombreButsMarques = butsMarques;
    this->dateDebutContrat = debutContrat;
    this->dateFinContrat = finContrat;
}

// Accesseurs and Mutateurs remain the same

bool Equipe::Ajouter() {
    QSqlQuery query;

    // Prepare the SQL query without IDEQUIPE (assuming it's auto-incremented)
    query.prepare("INSERT INTO equipe (NOMEQUIPE, NOMVILLE, NOMENTRAINEUR, NOMBREJOUEURS, TYPE, NOMBREBUTSMARQUES, DATEDEBUTCONTRAT, DATEFINCONTRAT) "
                  "VALUES (:nomEquipe, :nomVille, :nomEntraineur, :nombreJoueurs, :type, :nombreButsMarques, :dateDebutContrat, :dateFinContrat)");

    // Bind the values
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
QSqlQueryModel * Equipe::loadequipeData(){
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select * from equipe");
  //  model->setHeaderData(0,Qt::Horizontal,QObject::tr("Id_equipe"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Nom_equipe"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Nom_ville"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Nom_entraineur"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("Nombre_joueurs"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("Type"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("Nombrebuts_marquees"));
    model->setHeaderData(7,Qt::Horizontal,QObject::tr("Debut_contrat"));
    model->setHeaderData(8,Qt::Horizontal,QObject::tr("Fin_contrat"));
    return model ;
}
