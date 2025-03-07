#ifndef CHAMPIONNATS_H
#define CHAMPIONNATS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

class Championnats
{
private:
    int idChamp;
    int nbrEquipe;
    QString type;
    QString nom;
    QString org;
    int poolGains;



public:
Championnats();
Championnats(int idChamp, int nbrEquipe, const QString& type, const QString& nom, const QString& org, int poolGains);

int getIdChamp() const;
void setIdChamp(int idChamp);

int getNbrEquipe() const;
void setNbrEquipe(int nbrEquipe);

QString getType() const;
void setType(const QString& type);

QString getNom() const;
void setNom(const QString& nom);

QString getOrg() const;
void setOrg(const QString& org);

int getPoolGains() const;
void setPoolGains(int poolGains);

bool ajoutChamp();

};

#endif // CHAMPIONNATS_H
