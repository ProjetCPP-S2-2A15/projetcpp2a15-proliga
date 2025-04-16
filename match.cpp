#include "Match.h"

// Default constructor
Match::Match() : nomChamp(""), idMatch(0), score(""), type(""), dateMatch(QDate::currentDate()) {}

// Parameterized constructor
Match::Match(const QString& nomChamp, int idMatch, const QString& score, const QString& type, const QDate& dateMatch)
    : nomChamp(nomChamp), idMatch(idMatch), score(score), type(type), dateMatch(dateMatch) {}

// Getters
QString Match::getNomChamp() const { return nomChamp; }
int Match::getIdMatch() const { return idMatch; }
QString Match::getScore() const { return score; }
QString Match::getType() const { return type; }
QDate Match::getDateMatch() const { return dateMatch; }

// Setters
void Match::setNomChamp(const QString& nomChamp) { this->nomChamp = nomChamp; }
void Match::setIdMatch(int idMatch) { this->idMatch = idMatch; }
void Match::setScore(const QString& score) { this->score = score; }
void Match::setType(const QString& type) { this->type = type; }
void Match::setDateMatch(const QDate& dateMatch) { this->dateMatch = dateMatch; }
bool Match::addMatch() {
    QSqlQuery query;
    query.prepare("INSERT INTO MATCHES (NOM_CHAMP, ID_MATCH, SCORE, TYPE, DATE_MATCH) "
                  "VALUES (:nomChamp, :idMatch, :score, :type, :dateMatch)");
    query.bindValue(":nomChamp", nomChamp);
    query.bindValue(":idMatch", idMatch);
    query.bindValue(":score", score);
    query.bindValue(":type", type);
    query.bindValue(":dateMatch", dateMatch);

    if (!query.exec()) {
        qDebug() << "Error adding match:" << query.lastError().text();
        return false;
    }
    return true;
}
