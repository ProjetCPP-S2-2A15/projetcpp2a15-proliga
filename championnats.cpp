#include "championnats.h"
#include <QString>

Championnats::Championnats() {
    this->idChamp = 0;
    this->nbrEquipe = 0;
    this->type = "";
    this->nom = "";
    this->org = "";
    this->poolGains = 0;
}

Championnats::Championnats( int nbrEquipe, const QString& type, const QString& nom, const QString& org, int poolGains) {

    this->nbrEquipe = nbrEquipe;
    this->type = type;
    this->nom = nom;
    this->org = org;
    this->poolGains = poolGains;
}

int Championnats::getIdChamp() const
{
    return this->idChamp;
}

void Championnats::setIdChamp(int idChamp)
{
    this->idChamp = idChamp;
}

int Championnats::getNbrEquipe() const
{
    return this->nbrEquipe;
}

void Championnats::setNbrEquipe(int nbrEquipe)
{
    this->nbrEquipe = nbrEquipe;
}

QString Championnats::getType() const
{
    return this->type;
}

void Championnats::setType(const QString& type)
{
    this->type = type;
}

QString Championnats::getNom() const
{
    return this->nom;
}

void Championnats::setNom(const QString& nom)
{
    this->nom = nom;
}

QString Championnats::getOrg() const
{
    return this->org;
}

void Championnats::setOrg(const QString& org)
{
    this->org = org;
}

int Championnats::getPoolGains() const
{
    return this->poolGains;
}

void Championnats::setPoolGains(int poolGains)
{
    this->poolGains = poolGains;
}

bool Championnats::ajoutChamp() {

    QSqlQuery query;
    query.prepare("INSERT INTO CHAMPIONNATS (NBR_EQUIPE, TYPE, NOM, ORGANIZATEUR, POOL_GAINS) "
                  "VALUES (:nbrEquipe, :type, :nom, :org, :poolGains)"
                  "RETURNING ID_CHAMP INTO :idChamp");

    query.bindValue(":nbrEquipe", nbrEquipe);
    query.bindValue(":type", type);
    query.bindValue(":nom", nom);
    query.bindValue(":org", org);
    query.bindValue(":poolGains", poolGains);

    int lastID;
    query.bindValue(":idChamp", lastID, QSql::Out);


    if (query.exec()) {
        idChamp=lastID;
        qDebug() << "Championnat added successfully.";
        return true;
    } else {
        qDebug() << "Error adding Championnat:" << query.lastError().text();
        return false;
    }
}


bool Championnats::saveUpdates() {

    QSqlQuery query;
    query.prepare("UPDATE CHAMPIONNATS SET NBR_EQUIPE = :nbrEquipe, TYPE = :type, NOM = :nom, ORGANIZATEUR = :org, POOL_GAINS = :poolGains WHERE ID_CHAMP = :id");
    query.bindValue(":nbrEquipe", nbrEquipe);
    query.bindValue(":type", type);
    query.bindValue(":nom", nom);
    query.bindValue(":org", org);
    query.bindValue(":poolGains", poolGains);
    query.bindValue(":id", idChamp);

    if (query.exec()) {
        return true;
    } else {
        return false;
        qDebug() << "Update error: " << query.lastError().text();
    }
}

static QSqlQuery getChampionnatsData() {
    QSqlQuery query("SELECT NOM, POOL_GAINS, NBR_EQUIPE FROM CHAMPIONNATS");
    return query;
}
