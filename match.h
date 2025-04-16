#ifndef MATCH_H
#define MATCH_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include "ui_mainwindow.h"

class Match {
private:
    QString nomChamp;
    int idMatch;
    QString score;
    QString type;
    QDate dateMatch;

public:

    Match();
    Match(const QString& nomChamp, int idMatch, const QString& score, const QString& type, const QDate& dateMatch);


    QString getNomChamp() const;
    int getIdMatch() const;
    QString getScore() const;
    QString getType() const;
    QDate getDateMatch() const;


    void setNomChamp(const QString& nomChamp);
    void setIdMatch(int idMatch);
    void setScore(const QString& score);
    void setType(const QString& type);
    void setDateMatch(const QDate& dateMatch);
    bool addMatch();

};

#endif
