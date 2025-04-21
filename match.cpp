#include "match.h"
#include "mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include "connexion.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>


Match::Match(const QDate& date, const QString& type, const QString& equipe1, const QString& equipe2, const QString& score)
    : m_dateMatch(date), m_typeMatch(type), m_equipe1(equipe1), m_equipe2(equipe2), m_scoreMatch(score)
{
    // Initializing Match object
}

// Getters
int Match::idMatch() const {
    return m_idMatch;
}

QDate Match::dateMatch() const {
    return m_dateMatch;
}

QString Match::typeMatch() const {
    return m_typeMatch;
}

QString Match::scoreMatch() const {
    return m_scoreMatch;
}

QString Match::equipe1() const {
    return m_equipe1;
}

QString Match::equipe2() const {
    return m_equipe2;
}

// Setters
void Match::setDateMatch(const QDate& date) {
    m_dateMatch = date;
}

void Match::setTypeMatch(const QString& type) {
    m_typeMatch = type;
}

void Match::setScoreMatch(const QString& score) {
    m_scoreMatch = score;
}

void Match::setEquipe1(const QString& equipe1) {
    m_equipe1 = equipe1;
}

void Match::setEquipe2(const QString& equipe2) {
    m_equipe2 = equipe2;
}


//-------------------------------------------------------------------------------------------------------

ChatBotWidget::ChatBotWidget(QWidget *parent)
    : QWidget(parent)
{
    // Display the start image before showing the chatbot
    QLabel* startImage = new QLabel(this);
    QPixmap pixmap(":/images/start.png");

    // Scale the image to a smaller size (you can adjust the width and height)
    QPixmap scaledPixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    startImage->setPixmap(scaledPixmap);
    startImage->setAlignment(Qt::AlignCenter);

    // Create a timer to remove the image after a few seconds (e.g., 3 seconds)
    QTimer::singleShot(5000, [startImage, this] {
        startImage->hide();
        initChatInterface();
    });

    resize(400, 400);
}

void ChatBotWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QLabel* startImage = findChild<QLabel*>();
    if (startImage) {
        QPixmap pixmap(":/interface_icons/start.png");
        QPixmap scaledPixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        startImage->setPixmap(scaledPixmap);

        int x = (width() - scaledPixmap.width()) / 2;
        int y = (height() - scaledPixmap.height()) / 2;
        startImage->setGeometry(x, y, scaledPixmap.width(), scaledPixmap.height());
    }
}

//---------------------------------------------------------------------------------------------------------------------------


void ChatBotWidget::handleTeamInput() {
    QString teamName = lineEdit_5->text();
    // Handle the team input logic
    QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'><img src=':/interface_icons/logb.png' width='17' height='20' /> : </span></b> <span>" + teamName + "</span></div>";
    chatBox->append(htmlResponse);

    // Then call the function to display the team score
    displayTeamScore(teamName);
}



void ChatBotWidget::initChatInterface()
{
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    // 🔙 Back button
    QPushButton *backButton = new QPushButton("←");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        " background-color: transparent;"
        " color: #007acc;"
        " font-weight: bold;"
        " border: 1px solid #d3d3d3;"
        " border-radius: 8px;"
        " padding: 2px 2px;"
        " }"
        "QPushButton:hover {"
        " background-color: #f0f0f0; "
        " }"
        );
    backButton->setVisible(false);
    layout->addWidget(backButton, 0, Qt::AlignLeft);

    // 🟦 Question container
    questionContainer = new QWidget(this);
    QVBoxLayout *questionLayout = new QVBoxLayout(questionContainer);
    questionLayout->setSpacing(8);
    questionLayout->setContentsMargins(5, 5, 5, 5);

    QStringList questions = {
        "Analyse programmation 🔍",
        "Scores finales 🔍",
        "Meilleurs scores 🔍",
        "Suivi d'équipes 🔍"
    };

    layout->addWidget(questionContainer);

    // 🔤 Chat boxes (styled similarly)
    QString chatStyle = "background-color: #f9f9f9; border: 1px solid #ccc; padding: 10px; border-radius: 10px;";

    chatBox = new QTextEdit(this);
    chatBox->setReadOnly(true);
    chatBox->setStyleSheet(chatStyle);
    chatBox->hide();
    layout->addWidget(chatBox);

    suiviChatBox = new QTextEdit(this);
    suiviChatBox->setReadOnly(true);
    suiviChatBox->setStyleSheet(chatStyle);
    suiviChatBox->hide();
    layout->addWidget(suiviChatBox);

    // 📝 Input fields
    lineEdit_5 = new QLineEdit(this);
    lineEdit_5->setPlaceholderText("Enterez le nom de l'equipe...");
    lineEdit_5->setStyleSheet("padding: 10px; border-radius: 10px; border: 1px solid #ccc;");
    lineEdit_5->hide();
    layout->addWidget(lineEdit_5);

    lineEdit_year = new QLineEdit(this);
    lineEdit_year->setPlaceholderText("Entrez une année (ex: 2023)...");
    lineEdit_year->setStyleSheet("padding: 10px; border-radius: 10px; border: 1px solid #ccc;");
    lineEdit_year->hide();
    layout->addWidget(lineEdit_year);

    for (const QString &question : questions) {
        QPushButton *button = new QPushButton(question);
        button->setCursor(Qt::PointingHandCursor);
        button->setStyleSheet(
            "QPushButton {"
            " background-color: #f8f8f8;"
            " border-radius: 14px;"
            " padding: 2px 12px;"
            " font-size: 10px;"
            " border: 1px solid #bbb;"
            " margin-bottom: 8px;"
            " min-height: 25px;"
            " color: #54c43a;"
            " font-weight: bold;"
            " text-align: left;"
            "}"
            "QPushButton:hover {"
            " background-color: #e3e3e3;"
            "}");

        questionLayout->addWidget(button);

        connect(button, &QPushButton::clicked, this, [=]() {
            questionContainer->hide();
            backButton->setVisible(true);

            // Disconnect all previous connections to avoid multiple triggers
            disconnect(lineEdit_5, nullptr, nullptr, nullptr);
            disconnect(lineEdit_year, nullptr, nullptr, nullptr);

            if (question == "Scores finales 🔍") {
                chatBox->clear();
                chatBox->show();
                suiviChatBox->hide();
                lineEdit_year->hide();
                lineEdit_5->clear();
                lineEdit_5->show();

                connect(lineEdit_5, &QLineEdit::returnPressed, this, &ChatBotWidget::handleTeamInput);

            } else if (question == "Suivi d'équipes 🔍") {
                lineEdit_5->clear();
                chatBox->clear();
                suiviChatBox->clear();
                chatBox->hide();
                suiviChatBox->show();
                lineEdit_5->hide();

                QString infoMsg = "Veuillez entrer l'année (ex : 2024).";
                QString htmlInfo = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                   "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                   "<span>" + infoMsg + "</span></div>";
                suiviChatBox->append(htmlInfo);

                lineEdit_year->clear();
                lineEdit_year->show();

                connect(lineEdit_year, &QLineEdit::returnPressed, this, [=]() {
                    QString year = lineEdit_year->text().trimmed();
                    QRegularExpression yearRegex("^\\d{4}$");
                    QRegularExpressionMatch match = yearRegex.match(year);

                    if (!match.hasMatch()) {
                        QString errorMsg = "Veuillez entrer une année valide (ex : 2024).";
                        QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                            "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                            "<span style='color: red;'>" + errorMsg + "</span></div>";
                        suiviChatBox->append(htmlError);
                        return;
                    }

                    QString htmlYearResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                               "<img src=':/interface_icons/logb.png' width='17' height='20' /> : </span></b> <span>" + year + "</span></div>";
                    suiviChatBox->append(htmlYearResponse);

                    lineEdit_year->clear();
                    lineEdit_year->hide();

                    QString askTeam = "Merci ! Maintenant, entrez le nom de l'équipe.";
                    QString htmlAskTeam = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                          "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                          "<span>" + askTeam + "</span></div>";
                    suiviChatBox->append(htmlAskTeam);

                    lineEdit_5->clear();
                    lineEdit_5->show();

                    connect(lineEdit_5, &QLineEdit::returnPressed, this, [=]() {
                        QString team = lineEdit_5->text().trimmed().toUpper();

                        if (team.isEmpty()) {
                            QString errorMsg = "Veuillez entrer un nom d'équipe valide.";
                            QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                                "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                                "<span style='color: red;'>" + errorMsg + "</span></div>";
                            suiviChatBox->append(htmlError);
                            return;
                        }

                        QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                               "<img src=':/interface_icons/logb.png' width='17' height='20' /> : </span></b> <span>" + team + "</span></div>";
                        suiviChatBox->append(htmlResponse);

                        checkTeamMatches(year, team);
                        lineEdit_5->hide();
                    });
                });

            } else if (question == "Analyse programmation 🔍") {
                chatBox->clear();
                chatBox->show();
                suiviChatBox->hide();
                lineEdit_5->hide();
                lineEdit_year->show();

                QString message = " Entrez une année pour analyser les matchs.";
                QString htmlMessage = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                      "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                      "<span>" + message + "</span></div>";
                chatBox->append(htmlMessage);

                connect(lineEdit_year, &QLineEdit::returnPressed, this, [=]() {
                    QString year = lineEdit_year->text().trimmed();
                    int totalMatches = 0;
                    int playedMatches = 0;
                    int upcomingMatches = 0;

                    QVector<int> monthCounts(12, 0);
                    QVector<QMap<QString, int>> monthTypeCounts(12);

                    QSqlQuery query;
                    query.prepare("SELECT DATE_MATCH, TYPE_MATCH FROM MATCHES WHERE EXTRACT(YEAR FROM DATE_MATCH) = :year");
                    query.bindValue(":year", year.toInt());

                    if (query.exec()) {
                        while (query.next()) {
                            QDateTime matchDate = query.value("DATE_MATCH").toDateTime();
                            QString typeMatch = query.value("TYPE_MATCH").toString();
                            int month = matchDate.date().month();

                            monthCounts[month - 1]++;
                            monthTypeCounts[month - 1][typeMatch]++;

                            if (matchDate < QDateTime::currentDateTime()) {
                                playedMatches++;
                            } else {
                                upcomingMatches++;
                            }

                            totalMatches++;
                        }

                        QString response = QString("Analyse de l'année %1 : %2 match(s) trouvé(s), dont <span style='color:red;'>%3</span> déjà joué(s) et <span style='color:green;'>%4</span> à venir.")
                                               .arg(year)
                                               .arg(totalMatches)
                                               .arg(playedMatches)
                                               .arg(upcomingMatches);

                        QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                               "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                               "<span>" + response + "</span></div>";
                        chatBox->append(htmlResponse);

                        QStringList months = {
                            "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
                            "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"
                        };

                        for (int i = 0; i < 12; ++i) {
                            if (monthCounts[i] > 0) {
                                QString detail = QString("%1 : %2 match(s)").arg(months[i]).arg(monthCounts[i]);

                                QStringList typeDetails;
                                for (auto it = monthTypeCounts[i].begin(); it != monthTypeCounts[i].end(); ++it) {
                                    typeDetails << QString("&nbsp;&nbsp;&nbsp;&nbsp;- %1 : %2 match(s)").arg(it.key()).arg(it.value());
                                }

                                QString htmlDetail = "<div style='text-align: left; margin-top: 5px;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                                     "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                                     +
                                                     detail + "<br>" + typeDetails.join("<br>") + "</span></div>";

                                chatBox->append(htmlDetail);
                            }
                        }

                        QSqlQuery cancelledQuery;
                        cancelledQuery.prepare("SELECT DATE_MATCH, EQUIPE1, EQUIPE2, TYPE_MATCH FROM CANCELLED_MATCH WHERE EXTRACT(YEAR FROM DATE_MATCH) = :year");
                        cancelledQuery.bindValue(":year", year.toInt());

                        int cancelledCount = 0;
                        QStringList cancelledDetails;

                        if (cancelledQuery.exec()) {
                            while (cancelledQuery.next()) {
                                cancelledCount++;

                                QDateTime date = cancelledQuery.value("DATE_MATCH").toDateTime();
                                QString equipe1 = cancelledQuery.value("EQUIPE1").toString();
                                QString equipe2 = cancelledQuery.value("EQUIPE2").toString();
                                QString type = cancelledQuery.value("TYPE_MATCH").toString();

                                cancelledDetails << QString("🟥 %1 vs %2 — %3 (%4)")
                                                        .arg(equipe1)
                                                        .arg(equipe2)
                                                        .arg(type)
                                                        .arg(date.toString("dd/MM/yyyy"));
                            }

                            if (cancelledCount > 0) {
                                QString cancelledHeader = QString("⚠️ Match(s) annulé(s) en %1 : %2 match(s)").arg(year).arg(cancelledCount);
                                QString htmlCancelled = "<div style='text-align: left; margin-top: 15px;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                                        "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                                        "<span style='color: orange;'><b>" + cancelledHeader + "</b><br>" + cancelledDetails.join("<br>") + "</span></div>";
                                chatBox->append(htmlCancelled);
                            }

                        } else {
                            QString error = "Erreur lors de la récupération des matchs annulés.";
                            QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                                "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                                "<span style='color: red;'>" + error + "</span></div>";
                            chatBox->append(htmlError);
                        }

                    } else {
                        QString error = "Erreur lors de la récupération des données.";
                        QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                            "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                            "<span style='color: red;'>" + error + "</span></div>";
                        chatBox->append(htmlError);
                    }

                    lineEdit_year->clear();
                    lineEdit_year->hide();
                });

            } else if (question == "Meilleurs scores 🔍") {
                chatBox->clear();
                lineEdit_5->clear();
                lineEdit_year->clear();
                suiviChatBox->clear();
                chatBox->hide();
                suiviChatBox->show();
                lineEdit_5->hide();

                QString infoMsg = "Veuillez entrer l'année (ex : 2024).";
                QString htmlInfo = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                   "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                   "<span>" + infoMsg + "</span></div>";
                suiviChatBox->append(htmlInfo);

                lineEdit_year->clear();
                lineEdit_year->show();

                connect(lineEdit_year, &QLineEdit::returnPressed, this, [=]() {
                    QString year = lineEdit_year->text().trimmed();
                    QRegularExpression yearRegex("^\\d{4}$");
                    QRegularExpressionMatch match = yearRegex.match(year);

                    if (!match.hasMatch()) {
                        QString errorMsg = "Veuillez entrer une année valide (ex : 2024).";
                        QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                            "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                            "<span style='color: red;'>" + errorMsg + "</span></div>";
                        suiviChatBox->append(htmlError);
                        return;
                    }

                    QString htmlYearResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                               "<img src=':/interface_icons/logb.png' width='17' height='20' /> : </span></b> <span>" + year + "</span></div>";
                    suiviChatBox->append(htmlYearResponse);

                    lineEdit_year->clear();
                    lineEdit_year->hide();

                    QString askTeam = "Merci ! Maintenant, entrez le nom de l'équipe.";
                    QString htmlAskTeam = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                          "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                          "<span>" + askTeam + "</span></div>";
                    suiviChatBox->append(htmlAskTeam);

                    lineEdit_5->clear();
                    lineEdit_5->show();

                    connect(lineEdit_5, &QLineEdit::returnPressed, this, [=]() {
                        QString team = lineEdit_5->text().trimmed().toUpper();;


                        if (team.isEmpty()) {
                            QString errorMsg = "Veuillez entrer un nom d'équipe valide.";
                            QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                                "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                                "<span style='color: red;'>" + errorMsg + "</span></div>";
                            suiviChatBox->append(htmlError);
                            return;
                        }

                        QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                               "<img src=':/interface_icons/logb.png' width='17' height='20' /> : </span></b> <span>" + team + "</span></div>";
                        suiviChatBox->append(htmlResponse);

                        checkBestScore(year, team);
                        lineEdit_5->hide();
                    });
                });
            } else {
                chatBox->hide();
                suiviChatBox->hide();
                lineEdit_5->hide();
                lineEdit_year->hide();
            }
        });
    }

    // 🔁 Back button logic
    connect(backButton, &QPushButton::clicked, this, [=]() {
        chatBox->clear();
        chatBox->hide();
        suiviChatBox->clear();
        suiviChatBox->hide();
        lineEdit_5->clear();
        lineEdit_5->hide();
        lineEdit_year->clear();
        lineEdit_year->hide();
        questionContainer->show();
        backButton->setVisible(false);

        // Disconnect all signals to prevent multiple connections
        disconnect(lineEdit_5, nullptr, nullptr, nullptr);
        disconnect(lineEdit_year, nullptr, nullptr, nullptr);
    });
}





void ChatBotWidget::checkBestScore(const QString& year, const QString& team) {
    QSqlQuery query;

    QString queryString = QString(
        "SELECT EQUIPE1, EQUIPE2, SCORE, DATE_MATCH, WINNER "
        "FROM MATCHES "
        "WHERE (EQUIPE1 = :team OR EQUIPE2 = :team) "
        "AND EXTRACT(YEAR FROM DATE_MATCH) = :year "
        "AND WINNER = :team");

    query.prepare(queryString);
    query.bindValue(":team", team);
    query.bindValue(":year", year.toInt());

    if (query.exec()) {
        int maxTeamScore = -1;
        QList<QString> bestMatches;

        while (query.next()) {
            QString score = query.value("SCORE").toString();
            QStringList scoreParts = score.split('-');

            if (scoreParts.size() == 2) {
                int team1Score = scoreParts[0].toInt();
                int team2Score = scoreParts[1].toInt();

                QString equipe1 = query.value("EQUIPE1").toString();
                QString equipe2 = query.value("EQUIPE2").toString();
                QString dateMatch = query.value("DATE_MATCH").toString();

                int teamScore = (equipe1 == team) ? team1Score : team2Score;
                int opponentScore = (teamScore == team1Score) ? team2Score : team1Score;
                QString opponentTeam = (equipe1 == team) ? equipe2 : equipe1;

                if (teamScore > maxTeamScore) {
                    // New best score found, reset list
                    maxTeamScore = teamScore;
                    bestMatches.clear();
                    bestMatches.append(QString("%1-%2 contre %3, joué le %4")
                                           .arg(teamScore).arg(opponentScore).arg(opponentTeam).arg(dateMatch));
                } else if (teamScore == maxTeamScore) {
                    // Add to existing list of best scores
                    bestMatches.append(QString("%1-%2 contre %3, joué le %4")
                                           .arg(teamScore).arg(opponentScore).arg(opponentTeam).arg(dateMatch));
                }
            }
        }

        if (!bestMatches.isEmpty()) {
            QString response = QString("Le(s) meilleur(s) score(s) de l'équipe %1 en %2 :<br>").arg(team, year);
            for (const QString& matchInfo : bestMatches) {
                response += "• " + matchInfo + "<br>";
            }

            QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                   "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                   "<span>" + response + "</span></div>";
            suiviChatBox->append(htmlResponse);
        } else {
            QString errorMsg = "Aucun match gagné trouvé pour cette équipe dans l'année spécifiée.";
            QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                "<span style='color: red;'>" + errorMsg + "</span></div>";
            suiviChatBox->append(htmlError);
        }
    } else {
        QString errorMsg = "Erreur lors de la récupération des données.";
        QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                            "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                            "<span style='color: red;'>" + errorMsg + "</span></div>";
        suiviChatBox->append(htmlError);
    }
}



bool ChatBotWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Enter) {
        if (QLabel *label = qobject_cast<QLabel *>(watched)) {
            label->setStyleSheet("background-color: #c0c0c0; border-radius: 12px; padding: 2px 8px; font-size: 10px; "
                                 "border: 1px solid #bbb; margin-bottom: 8px; min-height: 25px; color: black; "
                                 "text-align: center;");
        }
    } else if (event->type() == QEvent::Leave) {
        if (QLabel *label = qobject_cast<QLabel *>(watched)) {
            label->setStyleSheet("background-color: #e0e0e0; border-radius: 12px; padding: 2px 8px; font-size: 10px; "
                                 "border: 1px solid #bbb; margin-bottom: 8px; min-height: 25px; color: black; "
                                 "text-align: center;");
        }
    }

    return QWidget::eventFilter(watched, event);
}




void ChatBotWidget::checkTeamMatches(const QString& year, const QString& team) {
    QSqlQuery query;

    QString trimmedTeam = team.trimmed();
    QString queryString = QString(
        "SELECT DATE_MATCH "
        "FROM MATCHES "
        "WHERE (EQUIPE1 = :team OR EQUIPE2 = :team) "
        "AND EXTRACT(YEAR FROM DATE_MATCH) = :year");

    query.prepare(queryString);
    query.bindValue(":team", trimmedTeam);
    query.bindValue(":year", year.toInt());  // Use int for year comparison

    if (query.exec()) {
        int playedCount = 0;
        int upcomingCount = 0;
        int wins = 0;
        int losses = 0;

        while (query.next()) {
            QDateTime matchDate = query.value("DATE_MATCH").toDateTime();
            int matchYear = matchDate.date().year();

            // Determine if the match is played or upcoming
            if (matchDate < QDateTime::currentDateTime()) {
                playedCount++; // It's played
                if (query.value("win").toInt() == 1) {
                    wins++;
                } else {
                    losses++;
                }
            } else {
                upcomingCount++; // It's upcoming
            }
        }

        // Prepare the response
        QString response = QString("Dans l'année %1, l'équipe %2 joue %3 matchs, dont %4 sont déjà joués avec %5 victoire(s) et %6 défaite(s), "
                                   "et %7 matchs à venir.")
                               .arg(year)
                               .arg(trimmedTeam)
                               .arg(playedCount + upcomingCount)
                               .arg(playedCount)
                               .arg(wins)
                               .arg(losses)
                               .arg(upcomingCount);

        // Display the response in the chatbox
        QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                               "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                               "<span>" + response + "</span></div>";
        suiviChatBox->append(htmlResponse);

    } else {
        QString errorMsg = "Erreur de récupération des données.";
        QString htmlError = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                            "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                            "<span style='color: red;'>" + errorMsg + "</span></div>";
        suiviChatBox->append(htmlError);
    }
}




void ChatBotWidget::displayTeamScore(const QString &teamNameInput)
{
    QString teamName = teamNameInput.toUpper();
    QDate currentDate = QDate::currentDate();
    qDebug() << "Recherche du score pour l'équipe :" << teamName << "avant la date :" << currentDate.toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare("SELECT SCORE FROM ("
                  "SELECT SCORE FROM MATCHES "
                  "WHERE (EQUIPE1 = :teamName OR EQUIPE2 = :teamName) "
                  "AND DATE_MATCH <= TO_TIMESTAMP(:currentDate, 'YYYY-MM-DD') "
                  "AND SCORE != '0-0' "
                  "ORDER BY DATE_MATCH DESC) "
                  "WHERE ROWNUM = 1");

    query.bindValue(":teamName", teamName);
    query.bindValue(":currentDate", currentDate.toString("yyyy-MM-dd"));

    if (query.exec() && query.next()) {
        QString score = query.value(0).toString();

        // Nouvelle requête pour les infos supplémentaires
        QSqlQuery queryOpponent;
        queryOpponent.prepare("SELECT DATE_MATCH, EQUIPE1, EQUIPE2, WINNER FROM ("
                              "SELECT DATE_MATCH, EQUIPE1, EQUIPE2, WINNER FROM MATCHES "
                              "WHERE (EQUIPE1 = :teamName OR EQUIPE2 = :teamName) "
                              "AND SCORE != '0-0' "
                              "AND DATE_MATCH <= TO_TIMESTAMP(:currentDate, 'YYYY-MM-DD') "
                              "ORDER BY DATE_MATCH DESC) "
                              "WHERE ROWNUM = 1");

        queryOpponent.bindValue(":teamName", teamName);
        queryOpponent.bindValue(":currentDate", currentDate.toString("yyyy-MM-dd"));

        if (queryOpponent.exec() && queryOpponent.next()) {
            QDateTime matchDateTime = queryOpponent.value(0).toDateTime();
            QString equipe1 = queryOpponent.value(1).toString();
            QString equipe2 = queryOpponent.value(2).toString();
            QString winner = queryOpponent.value(3).toString();

            QString opponent = (equipe1 == teamName) ? equipe2 : equipe1;
            QString formattedDate = matchDateTime.toString("dd-MM-yyyy à HH:mm");

            QString response = "Le dernier match de l’équipe " + teamName +
                               " a été joué contre " + opponent + " le " + formattedDate +
                               ", le match s’est terminé avec le score de : " + score +
                               " au profit de " + winner;

            QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                   "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                   "<span>" + response + "</span></div>";
            chatBox->append(htmlResponse);
        } else {
            QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                                   "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                                   "<span>Aucun match trouvé pour l'équipe " + teamName + " avant aujourd'hui.</span></div>";
            chatBox->append(htmlResponse);
        }
    } else {
        QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                               "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                               "<span>Erreur lors de la récupération des données du match.</span></div>";
        chatBox->append(htmlResponse);
    }

    lineEdit_5->clear();
}



//---------------------------------------------------
void ChatBotWidget::handleSuiviInput(const QString &year)
{
    QString htmlResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                           "<img src=':/interface_icons/logb.png' width='17' height='20' /> : </span></b> "
                           "<span>Année sélectionnée : " + year + "</span></div>";
    suiviChatBox->append(htmlResponse);

    // Optionally: add a fake response from bot
    QString htmlBotResponse = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'>"
                              "<img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> "
                              "<span>Merci, nous allons afficher les matchs de l’année " + year + ".</span></div>";
    suiviChatBox->append(htmlBotResponse);

    lineEdit_year->hide();
}
