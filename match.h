#ifndef MATCH_H
#define MATCH_H

#include <QString>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTableWidget>

class Match
{
public:
    Match(const QDate& date = QDate(), const QString& type = "", const QString& equipe1 = "", const QString& equipe2 = "", const QString& score = "0-0");

    // Getters
    int idMatch() const;
    QDate dateMatch() const;
    QString typeMatch() const;
    QString scoreMatch() const;
    QString equipe1() const;
    QString equipe2() const;

    // Setters
    void setDateMatch(const QDate& date);
    void setTypeMatch(const QString& type);
    void setScoreMatch(const QString& score);
    void setEquipe1(const QString& equipe1);
    void setEquipe2(const QString& equipe2);

private:
    int m_idMatch;
    QDate m_dateMatch;
    QString m_typeMatch;
    QString m_equipe1;
    QString m_equipe2;
    QString m_scoreMatch;

};



class MatchManager
{
public:
    MatchManager();


private:
    QSqlDatabase m_db;
};

class ChatBotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatBotWidget(QWidget *parent = nullptr);

private slots:
    void onSendButtonClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString getChatBotResponse(const QString &userMessage);
    void initChatInterface();

    QLineEdit *lineEdit_5;
    QPushButton *submitButton;
    QTextEdit *chatBox;
};

#endif // MATCH_H
