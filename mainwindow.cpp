#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "arduinoshiraz.h"
#include "design.h"
#include <QPropertyAnimation>
#include "changeWidget.h"
#include "match.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QWidget>
#include <random>
#include "loginwindow.h"
#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlRecord>
#include <QFileDialog>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>
#include <QChartView>
#include <QPieSeries>
#include <QChart>
#include <QToolTip>
#include <QCursor>
#include <QDate>
#include <QPieSlice>
#include <QToolTip>
#include <QStringList>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPieSlice>
#include <QGraphicsOpacityEffect>
#include <QAxObject>
#include <QDir>










#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextStream>


QSerialPort *serial;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    //---------------------------------------------------------------------------------------------------------------------------
    loadMatchesIntoTable();
    loadRefereesIntoComboBox();
    loadEquipes();
    checkForNullScoreEdit();
    showMonthlyMatchStatistics();
    programmation_2 = ui->programmation_2;
    historique_table=ui->historique_table;
    originalTabWidth = ui->tabWidget->geometry().width();
    originalTableWidth = ui->programmation_2->geometry().width();
    originalWidget6Width = ui->widget_6->geometry().width();
    originalDeleteMatchX = ui->delete_match->geometry().x();
    originalChercherWidth = ui->chercher->geometry().width();
    isExpanded = false;





    //-----------------------------------------------------------------------------------------------------------------------
    applyDesign(ui);
    //---------------------------------------------------------------------------------------------------------------------
    ChatBotWidget *chatBot = new ChatBotWidget(this);
    ui->programmation_2->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->historique_table->setEditTriggers(QAbstractItemView::DoubleClicked);

    if (ui->widget_8->layout() == nullptr) {
        ui->widget_8->setLayout(new QVBoxLayout());
    }
    ui->widget_8->layout()->addWidget(chatBot);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkForNullScoreEdit);
    timer->start(5000);

    //-----------------------------------------------------------------------------------------------------------------------------------------------

    // Connect other signals and slots
    connect(ui->competition1, &QPushButton::toggled, [this](bool checked) { on_competition1_toggled(ui, checked); });
    connect(ui->competition2, &QPushButton::toggled, [this](bool checked) { on_competition2_toggled(ui, checked); });
    connect(ui->match1, &QPushButton::toggled, [this](bool checked) { on_match1_toggled(ui, checked); });
    connect(ui->match2, &QPushButton::toggled, [this](bool checked) { on_match2_toggled(ui, checked); });
    connect(ui->equipe1, &QPushButton::toggled, [this](bool checked) { on_equipe1_toggled(ui, checked); });
    connect(ui->equipe2, &QPushButton::toggled, [this](bool checked) { on_equipe2_toggled(ui, checked); });
    connect(ui->joueur1, &QPushButton::toggled, [this](bool checked) { on_joueur1_toggled(ui, checked); });
    connect(ui->joueur2, &QPushButton::toggled, [this](bool checked) { on_joueur2_toggled(ui, checked); });
    connect(ui->arbitre1, &QPushButton::toggled, [this](bool checked) { on_arbitre1_toggled(ui, checked); });
    connect(ui->arbitre2, &QPushButton::toggled, [this](bool checked) { on_arbitre2_toggled(ui, checked); });
    connect(ui->stade1, &QPushButton::toggled, [this](bool checked) { on_stade1_toggled(ui, checked); });
    connect(ui->stade2, &QPushButton::toggled, [this](bool checked) { on_stade2_toggled(ui, checked); });
    connect(ui->sidebar_2, &QPushButton::clicked, this, &MainWindow::toggleIconOnlySidebar);

    //--------------------------------------------------BOUTONS INTERFACE SHIRAZ------------------------------------------------------------------------------------------------------------------------------------------
    connect(ui->add_match, &QPushButton::clicked, this, &MainWindow::on_addMatchButton_clicked);
    connect(ui->delete_match, &QPushButton::clicked, this, &MainWindow::deleteMatch);
    connect(ui->refresh, &QPushButton::clicked, this, &MainWindow::loadMatchesIntoTable);
    connect(ui->random_arbitre, &QCheckBox::stateChanged, this, &MainWindow::handleRandomReferees);
    connect(ui->chercher, &QLineEdit::textChanged, this, &MainWindow::filterTable);
    connect(ui->chercher_histo, &QLineEdit::textChanged, this, &MainWindow::filterTable);
    connect(ui->programmation_2, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onCellDoubleClicked(int, int)));
    connect(ui->historique_table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onHistoriqueCellDoubleClicked(int, int)));
    connect(ui->historique_table, &QTableWidget::itemChanged, this, &MainWindow::onItemChanged);
    connect(ui->show_2, &QPushButton::clicked, this, &MainWindow::onShowButtonClicked);
     connect(ui->show_3, &QPushButton::clicked, this, &MainWindow::onShow3ButtonClicked);
    connect(ui->tri_prog, SIGNAL(clicked()), this, SLOT(on_tri_prog_clicked()));
    connect(ui->tri_histo, SIGNAL(clicked()), this, SLOT(on_tri_histo_clicked()));
    connect(ui->showarduino, &QPushButton::clicked,
            this, &MainWindow::on_showArduinoButton_clicked);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

}


MainWindow::~MainWindow()
{
    delete ui;
}


//------------------------------------------------SIDE_BAR_ANIMATION----------------------------------------------------------------------------------

void MainWindow::toggleIconOnlySidebar()
{
    if (ui->iconOnlySidebar->isVisible()) {
        ui->iconOnlySidebar->hide();
        ui->iconTextSidebar->show();

        ui->stackedWidget->setMinimumWidth(1000);
        ui->stackedWidget->setMaximumWidth(1000);

        ui->widget_6->setMinimumWidth(200);
        ui->widget_6->setMaximumWidth(340);

        ui->widget_7->setMinimumWidth(200);
        ui->widget_7->setMaximumWidth(360);

        ui->widget_8->setMinimumWidth(200);
        ui->widget_8->setMaximumWidth(340);
    }
    else {

        ui->iconOnlySidebar->show();
        ui->iconTextSidebar->hide();

        ui->stackedWidget->setMinimumWidth(1500);
        ui->stackedWidget->setMaximumWidth(1500);

        ui->widget_6->setMinimumWidth(400);
        ui->widget_6->setMaximumWidth(800);

        ui->widget_7->setMinimumWidth(300);
        ui->widget_7->setMaximumWidth(600);

        ui->widget_8->setMinimumWidth(400);
        ui->widget_8->setMaximumWidth(800);
    }

    // Update chartView's size to follow widget_6 size
    chartView->resize(ui->widget_6->size());

    // Update widget and chart views
    ui->stackedWidget->update();
    ui->widget_6->update();
    ui->widget_7->update();
    ui->widget_8->update();
}

//------------------------------------------------AJOUT MATCH-------------------------------------------------------------------------------


bool isInsertingMatch = false;
void MainWindow::on_addMatchButton_clicked() {
    qDebug() << "Add Match Button Clicked";

    if (isInsertingMatch) {
        qDebug() << "Match is already being inserted, skipping.";
        return;
    }

    isInsertingMatch = true;
    ui->pushButton->setEnabled(false);

    QDateTime dateTime = ui->dateTimeEdit_2->dateTime();
    QString type = ui->comboBox->currentText();
    QString equipe1 = ui->comboBox_2->currentText();
    QString equipe2 = ui->comboBox_3->currentText();

    // controle saisie equipe
    if (equipe1 == equipe2) {
        QMessageBox::warning(this, "Erreur", "Les deux équipes ne peuvent pas être les mêmes !");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    QStringList arbitreNames;

    if (ui->random_arbitre->isChecked()) {
        QSqlQuery query("SELECT NOM FROM ARBITRES");
        QStringList allArbitres;

        while (query.next()) {
            allArbitres.append(query.value(0).toString());
        }

        if (allArbitres.size() < 4) {
            QMessageBox::warning(this, "Erreur", "Pas assez d'arbitres pour une sélection aléatoire ");
            ui->pushButton->setEnabled(true);
            isInsertingMatch = false;
            return;
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allArbitres.begin(), allArbitres.end(), g);
        arbitreNames = allArbitres.mid(0, 4);
    } else {
        arbitreNames = {
            ui->comboBox_arbitre1->currentText(),
            ui->comboBox_arbitre2->currentText(),
            ui->comboBox_arbitre3->currentText(),
            ui->comboBox_arbitre4->currentText()
        };
    }

    // controle saisie arbitre
    QSet<QString> uniqueReferees(arbitreNames.begin(), arbitreNames.end());
    if (uniqueReferees.size() < 4) {
        QMessageBox::warning(this, "Erreur", "Les arbitres doivent être uniques !");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    // fill with empty strings if needed
    while (arbitreNames.size() < 4) {
        arbitreNames.append("");
    }

    QSqlQuery query;
    query.prepare("INSERT INTO MATCHES (DATE_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4) "
                  "VALUES (TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS'), :type, :equipe1, :equipe2, :score, :arbitre1, :arbitre2, :arbitre3, :arbitre4)");
    query.bindValue(":date", dateTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":type", type);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":score", "0-0");
    query.bindValue(":arbitre1", arbitreNames[0]);
    query.bindValue(":arbitre2", arbitreNames[1]);
    query.bindValue(":arbitre3", arbitreNames[2]);
    query.bindValue(":arbitre4", arbitreNames[3]);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Match ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Echec dans l'ajout: " + query.lastError().text());
    }

    ui->pushButton->setEnabled(true);
    isInsertingMatch = false;
}




//------------------------------------------AFFICHAGE PROG ET HISTORIQUE DES MATCHES-------------------------------------------------------------------------------------------------

void MainWindow::loadMatchesIntoTable()
{
    QSqlQuery query("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, DATE_MATCH, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4, SCOREEDIT FROM MATCHES");

    ui->programmation_2->setRowCount(0);
    ui->historique_table->setRowCount(0);

    QDateTime now = QDateTime::currentDateTime(); // Get current date and time
    int progRow = 0, histRow = 0;

    while (query.next()) {
        QDateTime matchDateTime = query.value(5).toDateTime(); // Full timestamp with hours/minutes
        int scoreEdit = query.value(10).toInt();
        QString score = query.value(4).toString();

        // Calculate elapsed time since match
        qint64 secondsSinceMatch = matchDateTime.secsTo(now);
        bool isOlderThan2_5Hours = secondsSinceMatch > (2.5 * 3600);

        QColor rowColor;
        if (!isOlderThan2_5Hours) {
            rowColor = QColor(255, 165, 0); // orange: less than 2.5h
        } else {
            rowColor = (scoreEdit == 0) ? QColor(255, 200, 200) : QColor(197, 255, 217); // red or green
        }

        // Historique table if match is in the past
        if (matchDateTime < now) {
            ui->historique_table->insertRow(histRow);
            for (int col = 0; col < 10; col++) {
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    item->setText(score);
                } else if (col == 5) {
                    item->setText(matchDateTime.toString("yyyy-MM-dd HH:mm:ss"));
                } else if (col == 6) {
                    item->setText(query.value(6).toString());
                } else if (col == 7) {
                    item->setText(query.value(7).toString());
                } else if (col == 8) {
                    item->setText(query.value(8).toString());
                } else if (col == 9) {
                    item->setText(query.value(9).toString());
                } else {
                    item->setText(query.value(col < 4 ? col : col + 2).toString());
                }

                item->setBackground(rowColor);
                ui->historique_table->setItem(histRow, col, item);
            }
            histRow++;
        }
        // Programmation table if match is upcoming or current
        else {
            ui->programmation_2->insertRow(progRow);

            for (int col = 0; col < 9; col++) {
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    item->setText(matchDateTime.toString("yyyy-MM-dd HH:mm:ss"));
                } else if (col == 5) {
                    item->setText(query.value(6).toString());
                } else if (col == 6) {
                    item->setText(query.value(7).toString());
                } else if (col == 7) {
                    item->setText(query.value(8).toString());
                } else if (col == 8) {
                    item->setText(query.value(9).toString());
                } else {
                    item->setText(query.value(col < 4 ? col : col + 1).toString());
                }

                ui->programmation_2->setItem(progRow, col, item);
            }

            ui->programmation_2->setItem(progRow, 9, new QTableWidgetItem(""));

            predictWinner(progRow);

            progRow++;
        }
    }
}




//---------------------------------------RECHERCHE MATCHES----------------------------------------------------------------------------------------------

void MainWindow::filterTable()
{
    QString filterText = ui->chercher->text().trimmed();
    QString filterOption = ui->tri_prog_match->currentText();

    for (int i = 0; i < ui->programmation_2->rowCount(); i++) {
        bool match = false;

        if (filterOption == "date") {
            QString dateValue = ui->programmation_2->item(i, 4)->text();
            match = dateValue.contains(filterText, Qt::CaseInsensitive);
        } else if (filterOption == "équipe") {
            QString equipe1 = ui->programmation_2->item(i, 2)->text();
            QString equipe2 = ui->programmation_2->item(i, 3)->text();
            match = equipe1.contains(filterText, Qt::CaseInsensitive) || equipe2.contains(filterText, Qt::CaseInsensitive);
        }

        ui->programmation_2->setRowHidden(i, !match);
    }


    QString filterText2 = ui->chercher_histo->text().trimmed();
    QString filterOption2 = ui->tri_histo_match->currentText();

    for (int i = 0; i < ui->historique_table->rowCount(); i++) {
        bool match = false;

        if (filterOption2 == "date") {
            QString dateValue = ui->historique_table->item(i, 5)->text(); // Indice 5 pour la date
            match = dateValue.contains(filterText2, Qt::CaseInsensitive);
        } else if (filterOption2 == "équipe") {
            QString equipe1 = ui->historique_table->item(i, 2)->text();
            QString equipe2 = ui->historique_table->item(i, 3)->text();
            match = equipe1.contains(filterText2, Qt::CaseInsensitive) || equipe2.contains(filterText2, Qt::CaseInsensitive);
        }

        ui->historique_table->setRowHidden(i, !match);
    }

}





//-----------------------------------------SUPPRIMER MATCH---------------------------------------------------------------------------------------------------
void MainWindow::deleteMatch()
{
    int row = ui->programmation_2->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un match à supprimer.");
        return;
    }

    QString id = ui->programmation_2->item(row, 0)->text();
    QString equipe1 = ui->programmation_2->item(row, 2)->text();
    QString equipe2 = ui->programmation_2->item(row, 3)->text();
    QString dateMatch = ui->programmation_2->item(row, 4)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer le match de " + equipe1 +
                                                            " et " + equipe2 + " prévu le " + dateMatch + " ?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    // 1. Récupérer les données du match
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT TYPE_MATCH, DATE_MATCH, EQUIPE1, EQUIPE2, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4 "
                        "FROM MATCHES WHERE ID_MATCH = :id");
    selectQuery.bindValue(":id", id);

    if (selectQuery.exec() && selectQuery.next()) {
        QString typeMatch = selectQuery.value(0).toString();
        QDateTime dateMatchDB = selectQuery.value(1).toDateTime(); // Correct type for TIMESTAMP
        QString eq1 = selectQuery.value(2).toString();
        QString eq2 = selectQuery.value(3).toString();
        QString arb1 = selectQuery.value(4).toString();
        QString arb2 = selectQuery.value(5).toString();
        QString arb3 = selectQuery.value(6).toString();
        QString arb4 = selectQuery.value(7).toString();

        // 2. Insérer dans CANCELLED_MATCH
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO CANCELLED_MATCH (ID_MATCH, TYPE_MATCH, DATE_MATCH, EQUIPE1, EQUIPE2, "
                            "ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4) "
                            "VALUES (:id, :type, :date, :eq1, :eq2, :arb1, :arb2, :arb3, :arb4)");
        insertQuery.bindValue(":id", id);
        insertQuery.bindValue(":type", typeMatch);
        insertQuery.bindValue(":date", dateMatchDB); // Correct binding for TIMESTAMP
        insertQuery.bindValue(":eq1", eq1);
        insertQuery.bindValue(":eq2", eq2);
        insertQuery.bindValue(":arb1", arb1);
        insertQuery.bindValue(":arb2", arb2);
        insertQuery.bindValue(":arb3", arb3);
        insertQuery.bindValue(":arb4", arb4);

        if (!insertQuery.exec()) {
            qDebug() << "Erreur d'insertion dans CANCELLED_MATCH:" << insertQuery.lastError().text();
            QMessageBox::critical(this, "Erreur", "Échec de la sauvegarde du match annulé.");
            return;
        }

        // 3. Supprimer le match de MATCHES
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM MATCHES WHERE ID_MATCH = :id");
        deleteQuery.bindValue(":id", id);

        if (deleteQuery.exec()) {
            QMessageBox::information(this, "Suppression réussie", "Le match de " + equipe1 + " et " + equipe2 +
                                                                      " prévu le " + dateMatch + " a été supprimé avec succès !");
            ui->programmation_2->removeRow(row);
        } else {
            QString error = deleteQuery.lastError().text();
            qDebug() << "SQL Error: " << error;
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression du match dans la base de données : " + error);
        }
    } else {
        qDebug() << "Erreur de récupération des données:" << selectQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les détails du match avant suppression.");
    }
}


//------------------------------------GESTION ARBITRES DANS MATCHES --------------------------------------------------------------------------------------
void MainWindow::loadRefereesIntoComboBox() {
    QSqlQuery query("SELECT NOM FROM ARBITRES");

    while (query.next()) {
        QString refereeName = query.value(0).toString();
        ui->comboBox_arbitre1->addItem(refereeName);
        ui->comboBox_arbitre2->addItem(refereeName);
        ui->comboBox_arbitre3->addItem(refereeName);
        ui->comboBox_arbitre4->addItem(refereeName);
    }
}
void MainWindow::handleRandomReferees(int state) {
    bool randomMode = (state == Qt::Checked);

    ui->comboBox_arbitre1->setEnabled(!randomMode);
    ui->comboBox_arbitre2->setEnabled(!randomMode);
    ui->comboBox_arbitre3->setEnabled(!randomMode);
    ui->comboBox_arbitre4->setEnabled(!randomMode);

    if (randomMode) {
        QSqlQuery query("SELECT NOM FROM ARBITRES");
        QStringList allArbitres;

        while (query.next()) {
            allArbitres.append(query.value(0).toString());
        }

        if (allArbitres.size() < 4) {
            QMessageBox::warning(this, "Erreur", "Il n'y a pas assez d'arbitres pour une sélection aléatoire.");
            ui->random_arbitre->setChecked(false);
            ui->comboBox_arbitre1->setEnabled(true);
            ui->comboBox_arbitre2->setEnabled(true);
            ui->comboBox_arbitre3->setEnabled(true);
            ui->comboBox_arbitre4->setEnabled(true);
            return;
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allArbitres.begin(), allArbitres.end(), g);

        // Assign unique referees
        ui->comboBox_arbitre1->setCurrentText(allArbitres.at(0));
        ui->comboBox_arbitre2->setCurrentText(allArbitres.at(1));
        ui->comboBox_arbitre3->setCurrentText(allArbitres.at(2));
        ui->comboBox_arbitre4->setCurrentText(allArbitres.at(3));
    }
}




//---------------------------------GESTION EQUIPES DANS MATCHES-----------------------------------------------------------------------------------------------------

void MainWindow::loadEquipes() {
    QSqlQuery query("SELECT NOM_EQUIPE FROM EQUIPES");

    ui->comboBox_2->clear();
    ui->comboBox_3->clear();

    while (query.next()) {
        QString equipe = query.value(0).toString();
        ui->comboBox_2->addItem(equipe);
        ui->comboBox_3->addItem(equipe);
    }
}



//------------------------------------MODIFICATION PROGRAMMATION MATCHES-------------------------------------------------------------------------------



//----------
QString MainWindow::getColumnName(int column)
{
    // This function maps column indices to the actual column names in the database
    switch (column) {
    case 0: return "ID_MATCH";
    case 1: return "TYPE_MATCH";
    case 2: return "EQUIPE1";
    case 3: return "EQUIPE2";
    case 4: return "DATE_MATCH";
    case 5: return "ID_ARBITRE1";
    case 6: return "ID_ARBITRE2";
    case 7: return "ID_ARBITRE3";
    case 8: return "ID_ARBITRE4";
    case 9: return "ATTENTES";
    default: return "";
    }
}
//------------


void MainWindow::onCellDoubleClicked(int row, int column)
{
    if (!ui->programmation_2->item(row, column)) {
        qDebug() << "Cell at row " << row << ", column " << column << " is null!";
        return;
    }

    QString oldValue = ui->programmation_2->item(row, column)->text();
    QString columnName = getColumnName(column);

    if (columnName == "ID_MATCH") {
        QMessageBox::warning(this, "Modification Interdite", "Vous ne pouvez pas modifier l'ID du match.");
        return;
    }

    qDebug() << "Clicked column: " << column << ", Column Name: " << columnName << ", Current Value: " << oldValue;


    if (columnName == "ATTENTES") {
        QMessageBox::warning(this, "Modification Interdite", "La prédiction est générée automatiquement en se basant sur l'historique des résultats");
        return;
    }

    qDebug() << "Clicked column: " << column << ", Column Name: " << columnName << ", Current Value: " << oldValue;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation :", "Êtes-vous sûr de vouloir modifier cette valeur ?", QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    bool ok = false;
    QString newValue;

    if (columnName == "TYPE_MATCH") {
        QStringList typeOptions = { "Amical", "Groupe", "Championnat", "Coupe", "Qualification", "PlayOff" };
        newValue = QInputDialog::getItem(this, "Modification :", "Sélectionnez un nouveau type:", typeOptions, 0, false, &ok);
    }

    else if (columnName == "EQUIPE1" || columnName == "EQUIPE2") {
        QStringList teamOptions;
        QSqlQuery query("SELECT NOM_EQUIPE FROM EQUIPES");

        while (query.next()) {
            teamOptions << query.value(0).toString();
        }

        if (teamOptions.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucune équipe trouvée dans la base de données.");
            return;
        }

        newValue = QInputDialog::getItem(this, "Modification :", "Sélectionnez une nouvelle équipe :", teamOptions, 0, false, &ok);

        if (ok && !newValue.isEmpty()) {
            QString equipe1 = (columnName == "EQUIPE1") ? newValue : ui->programmation_2->item(row, 2)->text();
            QString equipe2 = (columnName == "EQUIPE2") ? newValue : ui->programmation_2->item(row, 3)->text();

            if (equipe1 == equipe2) {
                QMessageBox::warning(this, "Erreur", "Les deux équipes ne peuvent pas être les mêmes !");
                return;
            }
        }
    }

    else if (columnName.toUpper().contains("ID_ARBITRE")) {
        QStringList refereeNames;
        QSqlQuery query("SELECT NOM FROM ARBITRES");

        while (query.next()) {
            refereeNames << query.value(0).toString();
        }

        if (refereeNames.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun arbitre trouvé dans la base de données.");
            return;
        }

        QString selectedNom = QInputDialog::getItem(this, "Modification :", "Sélectionnez un arbitre :", refereeNames, 0, false, &ok);
        if (ok && !selectedNom.isEmpty()) {
            newValue = selectedNom;

            for (int i = 5; i <= 8; ++i) {
                if (i != column) {
                    QTableWidgetItem* item = ui->programmation_2->item(row, i);
                    if (item && item->text() == newValue) {
                        QMessageBox::warning(this, "Erreur", "Cet arbitre est déjà assigné à ce match. Veuillez en choisir un autre.");
                        return;
                    }
                }
            }
        }
    }

    else if (columnName == "DATE_MATCH") {
        QDateTime currentDateTime = QDateTime::fromString(oldValue, "yyyy-MM-dd HH:mm:ss");
        QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(currentDateTime);
        dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        dateTimeEdit->setCalendarPopup(true);
        dateTimeEdit->setDateTime(currentDateTime);

        QDialog *dateDialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout(dateDialog);
        layout->addWidget(dateTimeEdit);
        QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::accepted, dateDialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, dateDialog, &QDialog::reject);

        if (dateDialog->exec() == QDialog::Accepted) {
            newValue = dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
            ok = true;
        } else {
            delete dateDialog;
            return;
        }

        delete dateDialog;
    }

    if (!ok || newValue.isEmpty()) {
        return;
    }

    ui->programmation_2->item(row, column)->setText(newValue);

    QSqlQuery query;
    if (columnName == "DATE_MATCH") {
        query.prepare("UPDATE MATCHES SET DATE_MATCH = TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS') WHERE ID_MATCH = :id_match");
        query.bindValue(":date", newValue);
    } else {
        query.prepare("UPDATE MATCHES SET " + columnName + " = :value WHERE ID_MATCH = :id_match");
        query.bindValue(":value", newValue);
    }

    query.bindValue(":id_match", ui->programmation_2->item(row, 0)->text());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "La valeur a été modifiée avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de la valeur : " + query.lastError().text());
    }
}







//-------------------------------------MODIFICATION HISTORIQUE MATCHES---------------------------------------------------------------------------------------
void MainWindow::onHistoriqueCellDoubleClicked(int row, int col)
{
    if (col == 4) {

        QDateTime now = QDateTime::currentDateTime();
        QString dateTimeString = ui->historique_table->item(row, 5)->text();
        QDateTime matchDateTime = QDateTime::fromString(dateTimeString, "yyyy-MM-dd HH:mm:ss");

        qint64 secondsDiff = matchDateTime.secsTo(now);
        bool askTrackOption = (secondsDiff <= 9000);

        if (askTrackOption) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Match en cours");
            msgBox.setText("Le match est toujours en cours.\nChoisissez comment vous souhaitez mettre à jour le score :");
            QPushButton *manualButton = msgBox.addButton("Manuellement", QMessageBox::AcceptRole);
            QPushButton *realtimeButton = msgBox.addButton("Suivi en temps réel", QMessageBox::RejectRole);
            msgBox.exec();

            if (msgBox.clickedButton() == realtimeButton) {
                QString team1Name = ui->historique_table->item(row, 2)->text();
                QString team2Name = ui->historique_table->item(row, 3)->text();

                Arduinoshiraz realtimeDialog(this, team1Name, team2Name);
                if (realtimeDialog.exec() == QDialog::Accepted) {
                    QString finalScore = realtimeDialog.getFinalScore();
                    ui->historique_table->item(row, col)->setText(finalScore);

                    QStringList scores = finalScore.split("-");
                    int scoreTeam1 = scores[0].toInt();
                    int scoreTeam2 = scores[1].toInt();

                    QString winner;
                    if (scoreTeam1 > scoreTeam2) {
                        winner = team1Name;
                    } else if (scoreTeam1 < scoreTeam2) {
                        winner = team2Name;
                    } else {
                        winner = "égalité";
                    }

                    QSqlQuery query;
                    query.prepare("UPDATE MATCHES SET SCORE = :score, WINNER = :winner, SCOREEDIT = 1 WHERE ID_MATCH = :id_match");
                    query.bindValue(":score", finalScore);
                    query.bindValue(":winner", winner);
                    query.bindValue(":id_match", ui->historique_table->item(row, 0)->text());

                    if (query.exec()) {
                        QMessageBox::information(this, "Succès", "Score mis à jour avec succès !");
                    } else {
                        QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du score dans la base de données.");
                    }
                }
                return;
            }
        }

        QString oldScore = ui->historique_table->item(row, col)->text();

        QRegularExpression regExp("^\\d+-\\d+$");
        QRegularExpressionValidator validator(regExp, this);

        bool ok;
        QString newScore = QInputDialog::getText(this, "Modifier le score", "Nouveau score :", QLineEdit::Normal,
                                                 oldScore, &ok);

        if (ok && !newScore.isEmpty()) {
            int pos = 0;
            if (validator.validate(newScore, pos) == QValidator::Acceptable) {
                ui->historique_table->item(row, col)->setText(newScore);

                QStringList scores = newScore.split("-");
                int scoreTeam1 = scores[0].toInt();
                int scoreTeam2 = scores[1].toInt();

                QString winner;
                if (scoreTeam1 > scoreTeam2) {
                    winner = ui->historique_table->item(row, 2)->text();
                } else if (scoreTeam1 < scoreTeam2) {
                    winner = ui->historique_table->item(row, 3)->text();
                } else {
                    winner = "égalité";
                }

                QSqlQuery query;
                query.prepare("UPDATE MATCHES SET SCORE = :score, WINNER = :winner, SCOREEDIT = 1 WHERE ID_MATCH = :id_match");
                query.bindValue(":score", newScore);
                query.bindValue(":winner", winner);
                query.bindValue(":id_match", ui->historique_table->item(row, 0)->text());

                if (query.exec()) {
                    QMessageBox::information(this, "Succès", "Score mis à jour avec succès !");
                } else {
                    QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du score dans la base de données.");
                }
            } else {
                QMessageBox::warning(this, "Format invalide", "Veuillez saisir un score valide au format X-Y.");
            }
        }
    }
}




//--------------------------------------------DESIGN HISTORIQUE-----------------------------------------------------------------------------------------

void MainWindow::onItemChanged(QTableWidgetItem *item)
{
    int row = item->row();
    int col = item->column();

    // Check if we are editing the SCORE column (index 4)
    if (col == 4) {
        QString newScore = item->text();
        QString idMatch = ui->historique_table->item(row, 0)->text(); // ID_MATCH is in column 0

        // Fetch SCOREEDIT and DATE_MATCH from the database
        QSqlQuery query;
        query.prepare("SELECT SCOREEDIT, DATE_MATCH FROM MATCHES WHERE ID_MATCH = :id_match");
        query.bindValue(":id_match", idMatch);

        if (query.exec() && query.next()) {
            int scoreEditValue = query.value(0).toInt();
            QDateTime matchDateTime = query.value(1).toDateTime();
            QDateTime now = QDateTime::currentDateTime();

            qint64 diffInSeconds = matchDateTime.secsTo(now);
            bool isOlderThan2_5Hours = diffInSeconds > (2.5 * 3600); // 2.5 hours in seconds

            QColor rowColor;

            if (!isOlderThan2_5Hours) {
                // Match ended less than 2.5 hours ago → orange always
                rowColor = QColor(255, 165, 0); // Orange
            } else {
                // Match ended more than 2.5 hours ago → use original logic
                if (newScore == "0-0") {
                    rowColor = (scoreEditValue == 1) ? QColor(197, 255, 217) : QColor(255, 200, 200); // Green or Red
                } else {
                    rowColor = QColor(197, 255, 217); // Green
                }
            }

            // Apply the background color to the entire row
            for (int c = 0; c < ui->historique_table->columnCount(); ++c) {
                QTableWidgetItem *rowItem = ui->historique_table->item(row, c);
                if (rowItem) {
                    rowItem->setBackground(rowColor);
                }
            }
        }
    }
}



void MainWindow::checkForNullScoreEdit()
{

    QDate today = QDate::currentDate();
    QSqlQuery query("SELECT SCOREEDIT, DATE_MATCH FROM MATCHES WHERE SCOREEDIT IS NULL OR SCOREEDIT = '0'");
    QList<QLabel*> existingLabels = ui->widget_7->findChildren<QLabel*>();
    for (QLabel* label : existingLabels) {
        if (label != ui->label_21) {
            label->deleteLater();
        }
    }

    bool labelDisplayed = false;
    while (query.next()) {
        QString scoreEdit = query.value(0).toString();
        QDateTime dateTimeMatch = query.value(1).toDateTime();
        QDate matchDate = dateTimeMatch.date();
        qDebug() << "ScoreEdit: " << scoreEdit << ", Match Date: " << matchDate.toString() << ", Today: " << today.toString();

        if ((scoreEdit.isEmpty() || scoreEdit == "0") && matchDate.isValid() && matchDate < today) {

            QLabel *label = new QLabel("❗", ui->widget_7);
            label->setStyleSheet("font-size: 15px; color: red;");
            label->move(10, 10);
            label->show();
            labelDisplayed = true;
            break;
        }
    }

    if (!labelDisplayed) {
        qDebug() << "No matching conditions found.";
    }
}



//-------------------------------------ELARGIR WIDGETS-------------------------------------------------------------------------------

// Flag for show_2 expansion state
bool isExpandedShow2 = false;

// Flag for show_3 expansion state
bool isExpandedShow3 = false;

void MainWindow::onShowButtonClicked()
{
    // Save current dimensions of the widgets
    int currentTabWidth = ui->tabWidget->geometry().width();
    int currentTableWidth = ui->programmation_2->geometry().width();
    int currentWidget6Width = ui->widget_6->geometry().width();
    int currentDeleteMatchX = ui->delete_match->geometry().x();
    int currentChercherWidth = ui->chercher->geometry().width();
    int currentShow2X = ui->show_2->geometry().x();
    int currentTriProgX = ui->tri_prog->geometry().x();

    // Define the animation duration and the shift amount for all widgets
    int duration = 500;
    int shiftAmount = 280;

    // Create and configure the animation for tabWidget resizing
    QPropertyAnimation *tabWidgetAnimation = new QPropertyAnimation(ui->tabWidget, "geometry");
    tabWidgetAnimation->setDuration(duration);
    QRect tabWidgetNewRect = ui->tabWidget->geometry();
    tabWidgetNewRect.setWidth(isExpandedShow2 ? currentTabWidth - shiftAmount : currentTabWidth + shiftAmount);
    tabWidgetAnimation->setEndValue(tabWidgetNewRect);

    // Animation for the tableWidget resizing
    QPropertyAnimation *tableWidgetAnimation = new QPropertyAnimation(ui->programmation_2, "geometry");
    tableWidgetAnimation->setDuration(duration);
    QRect tableWidgetNewRect = ui->programmation_2->geometry();
    tableWidgetNewRect.setWidth(isExpandedShow2 ? currentTableWidth - shiftAmount : currentTableWidth + shiftAmount);
    tableWidgetAnimation->setEndValue(tableWidgetNewRect);

    // Animation for widget6 resizing and moving
    QPropertyAnimation *widget6Animation = new QPropertyAnimation(ui->widget_6, "geometry");
    widget6Animation->setDuration(duration);
    QRect widget6NewRect = ui->widget_6->geometry();
    widget6NewRect.setWidth(isExpandedShow2 ? currentWidget6Width - shiftAmount : currentWidget6Width + shiftAmount);
    widget6NewRect.moveLeft(isExpandedShow2 ? widget6NewRect.left() - shiftAmount : widget6NewRect.left() + shiftAmount);
    widget6Animation->setEndValue(widget6NewRect);

    // Animations for other widgets (delete_match, chercher, show_2, tri_prog)
    QPropertyAnimation *deleteMatchAnimation = new QPropertyAnimation(ui->delete_match, "geometry");
    deleteMatchAnimation->setDuration(duration);
    QRect deleteMatchNewRect = ui->delete_match->geometry();
    deleteMatchNewRect.moveLeft(isExpandedShow2 ? currentDeleteMatchX - shiftAmount : currentDeleteMatchX + shiftAmount);
    deleteMatchAnimation->setEndValue(deleteMatchNewRect);

    QPropertyAnimation *chercherAnimation = new QPropertyAnimation(ui->chercher, "geometry");
    chercherAnimation->setDuration(duration);
    QRect chercherNewRect = ui->chercher->geometry();
    chercherNewRect.setWidth(isExpandedShow2 ? currentChercherWidth - shiftAmount : currentChercherWidth + shiftAmount);
    chercherAnimation->setEndValue(chercherNewRect);

    QPropertyAnimation *show2ButtonAnimation = new QPropertyAnimation(ui->show_2, "geometry");
    show2ButtonAnimation->setDuration(duration);
    QRect show2ButtonNewRect = ui->show_2->geometry();
    show2ButtonNewRect.moveLeft(isExpandedShow2 ? currentShow2X - shiftAmount : currentShow2X + shiftAmount);
    show2ButtonAnimation->setEndValue(show2ButtonNewRect);

    QPropertyAnimation *triProgAnimation = new QPropertyAnimation(ui->tri_prog, "geometry");
    triProgAnimation->setDuration(duration);
    QRect triProgNewRect = ui->tri_prog->geometry();
    triProgNewRect.moveLeft(isExpandedShow2 ? currentTriProgX - shiftAmount : currentTriProgX + shiftAmount);
    triProgAnimation->setEndValue(triProgNewRect);

    // Start all animations
    tabWidgetAnimation->start();
    tableWidgetAnimation->start();
    widget6Animation->start();
    deleteMatchAnimation->start();
    chercherAnimation->start();
    show2ButtonAnimation->start();
    triProgAnimation->start();

    // Toggle the expanded state for show_2 only
    isExpandedShow2 = !isExpandedShow2;
}
void MainWindow::onShow3ButtonClicked()
{
    // Save the current dimensions and positions of the widgets
    int currentWidget7Width = ui->widget_7->geometry().width();
    int currentWidget8X = ui->widget_8->geometry().x();
    int currentHistoriqueTableWidth = ui->historique_table->geometry().width();
    int currentShow3X = ui->show_3->geometry().x();
    int currentTriHistoX = ui->tri_histo->geometry().x();
    int currentChercherHistoWidth = ui->chercher_histo->geometry().width();

    // Define the animation duration and the shift amount for all widgets
    int duration = 500;
    int shiftAmount = 280;

    // Create and configure the animation for widget_7 resizing
    QPropertyAnimation *widget7Animation = new QPropertyAnimation(ui->widget_7, "geometry");
    widget7Animation->setDuration(duration);
    QRect widget7NewRect = ui->widget_7->geometry();
    widget7NewRect.setWidth(isExpanded ? currentWidget7Width - shiftAmount : currentWidget7Width + shiftAmount);
    widget7Animation->setEndValue(widget7NewRect);

    // Create and configure the animation for widget_8 moving to the left (opposite direction)
    QPropertyAnimation *widget8Animation = new QPropertyAnimation(ui->widget_8, "geometry");
    widget8Animation->setDuration(duration);
    QRect widget8NewRect = ui->widget_8->geometry();
    widget8NewRect.moveLeft(isExpanded ? currentWidget8X - shiftAmount : currentWidget8X + shiftAmount); // Move widget_8
    widget8Animation->setEndValue(widget8NewRect);

    // Create and configure the animation for historique_table resizing
    QPropertyAnimation *historiqueTableAnimation = new QPropertyAnimation(ui->historique_table, "geometry");
    historiqueTableAnimation->setDuration(duration);
    QRect historiqueTableNewRect = ui->historique_table->geometry();
    historiqueTableNewRect.setWidth(isExpanded ? currentHistoriqueTableWidth - shiftAmount : currentHistoriqueTableWidth + shiftAmount);
    historiqueTableAnimation->setEndValue(historiqueTableNewRect);

    // Create and configure the animation for show_3 moving to the left
    QPropertyAnimation *show3ButtonAnimation = new QPropertyAnimation(ui->show_3, "geometry");
    show3ButtonAnimation->setDuration(duration);
    QRect show3ButtonNewRect = ui->show_3->geometry();
    show3ButtonNewRect.moveLeft(isExpanded ? currentShow3X - shiftAmount : currentShow3X + shiftAmount);
    show3ButtonAnimation->setEndValue(show3ButtonNewRect);

    // Create and configure the animation for tri_histo moving to the left
    QPropertyAnimation *triHistoAnimation = new QPropertyAnimation(ui->tri_histo, "geometry");
    triHistoAnimation->setDuration(duration);
    QRect triHistoNewRect = ui->tri_histo->geometry();
    triHistoNewRect.moveLeft(isExpanded ? currentTriHistoX - shiftAmount : currentTriHistoX + shiftAmount);
    triHistoAnimation->setEndValue(triHistoNewRect);

    // Create and configure the animation for chercher_histo resizing (making it larger)
    QPropertyAnimation *chercherHistoAnimation = new QPropertyAnimation(ui->chercher_histo, "geometry");
    chercherHistoAnimation->setDuration(duration);
    QRect chercherHistoNewRect = ui->chercher_histo->geometry();
    chercherHistoNewRect.setWidth(isExpanded ? currentChercherHistoWidth - shiftAmount : currentChercherHistoWidth + shiftAmount);
    chercherHistoAnimation->setEndValue(chercherHistoNewRect);



    // Start all animations
    widget7Animation->start();
    widget8Animation->start();
    historiqueTableAnimation->start();
    show3ButtonAnimation->start();
    triHistoAnimation->start();
    chercherHistoAnimation->start();


    // Toggle the expanded state
    isExpanded = !isExpanded;
}




//--------------------------------------EXPORTER PDF ---------------------------------------------------------------------------------------
void MainWindow::on_exporter_match_clicked() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Export");
    msgBox.setText("Choisissez ce que vous souhaitez exporter:");

    QPushButton *btnProgrammation = msgBox.addButton("Programmation", QMessageBox::AcceptRole);
    QPushButton *btnHistorique = msgBox.addButton("Historique", QMessageBox::AcceptRole);
    QPushButton *btnStatistics = msgBox.addButton("Statistiques", QMessageBox::RejectRole);

    msgBox.exec();

    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Exportation annulée", "Aucun fichier sélectionné.");
        return;
    }

    if (msgBox.clickedButton() == btnProgrammation) {
        exportTableToPDF(ui->programmation_2, filePath);
    }
    else if (msgBox.clickedButton() == btnHistorique) {
        exportTableToPDF(ui->historique_table, filePath);
    }
    else if (msgBox.clickedButton() == btnStatistics) {
        exportStatisticsToPDF(filePath);  // Pass the file path to avoid asking twice
    }
    else {
        QMessageBox::information(this, "Exportation annulée", ".");
    }
}


void MainWindow::exportTableToPDF(QTableWidget *table, const QString &filename)
{
    if (!table) return;

    QPdfWriter pdfWriter(filename);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    QFont font("Arial", 7);
    painter.setFont(font);

    int leftMargin = 20;
    int topMargin = 50;
    int rowHeight = 80;
    int textPadding = 10;
    int columnCount = table->columnCount();
    int pageWidth = pdfWriter.width() - 2 * leftMargin;
    int columnWidth = pageWidth / columnCount;

    int y = topMargin;

    // Load logo from resources
    QPixmap logo(":/interface_icons/log.png");

    if (logo.isNull()) {
        qDebug() << "Error loading logo image!";
    }

    // Increase logo size
    int logoWidth = 80;
    int logoHeight = 80;
    int logoX = pdfWriter.width() - logoWidth - 20;
    int logoY = 20;

    // Draw the logo
    painter.drawPixmap(logoX, logoY, logoWidth, logoHeight, logo);

    // Move the table lower by adjusting the top margin
    y += logoHeight + 20;

    // Draw table header
    painter.setPen(Qt::black);
    painter.setBrush(Qt::green);
    painter.drawRect(leftMargin, y, pageWidth, rowHeight);

    for (int col = 0; col < columnCount; ++col) {
        QString headerText = table->horizontalHeaderItem(col) ? table->horizontalHeaderItem(col)->text() : "";
        QRect headerRect(leftMargin + col * columnWidth + textPadding, y, columnWidth - 2 * textPadding, rowHeight);
        painter.drawText(headerRect, Qt::AlignCenter, headerText);
    }

    y += rowHeight;

    // Draw table rows
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < table->rowCount(); ++row) {
        painter.drawRect(leftMargin, y, pageWidth, rowHeight);

        for (int col = 0; col < columnCount; ++col) {
            QString cellText = table->item(row, col) ? table->item(row, col)->text() : "";
            QRect cellRect(leftMargin + col * columnWidth + textPadding, y, columnWidth - 2 * textPadding, rowHeight);
            painter.drawText(cellRect, Qt::AlignCenter, cellText);
        }
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Exportation réussie", "Le fichier PDF a été enregistré avec succès !");
}
void MainWindow::exportStatisticsToPDF(const QString &filePath) {
    if (!chartView) {
        QMessageBox::warning(this, "Erreur", "Aucun graphique à exporter !");
        return;
    }

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Export Canceled", "No file selected.");
        return;
    }

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);

    // Load logo image
    QImage logo(":/interface_icons/log.png");  // Use resource path or full path to logo
    if (logo.isNull()) {
        QMessageBox::warning(this, "Erreur", "pas d' image.");
    } else {
        // Scale the logo to a bigger size
        int scaledWidth = 80;  // Desired width for the logo
        int scaledHeight = 80; // Desired height for the logo
        QImage scaledLogo = logo.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio);

        // Calculate the top-right position based on the page width and logo width
        int xPos = writer.width() - scaledLogo.width() - 10;  // 10px margin from the right edge
        int yPos = 10; // 10px margin from the top

        // Draw the scaled logo at the top-right corner
        painter.drawImage(xPos, yPos, scaledLogo);
    }

    // Render the pie chart after drawing the logo
    chartView->render(&painter);

    // Retrieve data for the legend (month names and number of matches)
    QPieSeries *series = qobject_cast<QPieSeries *>(chartView->chart()->series().first());
    if (!series) {
        QMessageBox::warning(this, "erreur", "pas de data.");
        return;
    }

    // Set font for text (make it bigger)
    QFont font = painter.font();
    font.setPointSize(8); // Increase font size
    painter.setFont(font);

    // Draw the legend below the pie chart
    int x = 100; // X position for the legend
    int y = chartView->height() + 450; // Y position for the legend
    int rectangleSize = 15;  // Size of the color rectangle
    int rowHeight = rectangleSize + 50; // Spacing between rows

    // Month names
    QStringList monthNames = {
                              "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
                              "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"

    };

    // Draw statistics (month name and match count)
    int i = 0;
    foreach (QPieSlice *slice, series->slices()) {
        QString monthName = monthNames[i];
        int matchCount = static_cast<int>(slice->value());
        QString matchText = (matchCount > 1) ? "matches" : "match";

        // Draw text with singular/plural form
        painter.setPen(Qt::black);
        painter.drawText(x + rectangleSize + 5, y + rectangleSize - 5,
                         QString("%1: %2 %3").arg(monthName).arg(matchCount).arg(matchText));

        y += rowHeight;
        i++;
    }

    painter.end();

    QMessageBox::information(this, "Exportation réussie", "Le graphique et les statistiques ont été exportés avec succès !");
}



//-----------------------------EXPORTER EXCEL ---------------------------------------------------------------------------------------------
void MainWindow::on_exporter_excel_match_clicked()
{
    // Ask the user whether they want to export "programmation" or "historique"
    bool ok;
    QString option = QInputDialog::getItem(this, "Choisir l'option d'export",
                                           "Voulez-vous exporter 'programmation' ou 'historique' ?",
                                           {"programmation", "historique"}, 0, false, &ok);
    if (!ok || option.isEmpty()) {
        // User canceled or entered an invalid option
        return;
    }

    // Ask for the file save location
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le fichier Excel", "", "*.xlsx");
    if (fileName.isEmpty())
        return;

    QAxObject *excel = new QAxObject("Excel.Application", this);
    if (!excel) {
        QMessageBox::critical(this, "Erreur", "Impossible d'initialiser Excel.");
        return;
    }

    excel->dynamicCall("SetVisible(bool)", false); // Keep Excel hidden
    QAxObject *workbook = excel->querySubObject("Workbooks")->querySubObject("Add()");
    QAxObject *sheet = workbook->querySubObject("Sheets(int)", 1);
    sheet->dynamicCall("SetName(const QString&)", "Liste des Matchs");

    // Decide which table to export based on the user's choice
    QTableWidget *tableToExport = (option == "programmation") ? programmation_2 : historique_table;

    // Access the chosen table widget (either programmation_2 or historique_table)
    int rowCount = tableToExport->rowCount();
    int columnCount = tableToExport->columnCount();

    // Dynamically add column headers from the table
    for (int col = 0; col < columnCount; col++) {
        QString headerText = tableToExport->horizontalHeaderItem(col)->text();
        sheet->querySubObject("Cells(int,int)", 1, col + 1)->setProperty("Value", headerText);
    }

    // Iterate through the rows and columns to fill the Excel sheet with data
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            QTableWidgetItem *item = tableToExport->item(row, col);
            if (item) {
                QString cellText = item->text();

                // Get the cell object
                QAxObject *excelCell = sheet->querySubObject("Cells(int,int)", row + 2, col + 1);

                // Check if the content looks like a score (e.g., "1-0" or "0-1")
                if (cellText.contains("-")) {
                    // Set the value for the score cell as text
                    excelCell->setProperty("Value", QVariant(cellText));
                    // Force Excel to treat it as text (this is more explicit)
                    excelCell->setProperty("NumberFormat", "@"); // Ensure it's formatted as text
                    excelCell->dynamicCall("SetValue(const QString&)", cellText); // Ensure it's treated as a string
                } else {
                    // For non-score cells, set value normally
                    excelCell->setProperty("Value", QVariant(cellText));
                }
            }
        }
    }

    // Save and close the Excel file
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete excel;

    QMessageBox::information(this, "Succès", "Le fichier Excel a été généré avec succès !");
}

//-----------------------------------------TRI MATCHES---------------------------------------------------------------------

void MainWindow::on_tri_prog_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, DATE_MATCH, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4 FROM MATCHES WHERE DATE_MATCH > CURRENT_DATE ORDER BY DATE_MATCH ASC");

    if (query.exec()) {
        ui->programmation_2->setRowCount(0);

        int row = 0;
        while (query.next()) {
            ui->programmation_2->insertRow(row);

            for (int col = 0; col < query.record().count(); col++) {
                ui->programmation_2->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }

            predictWinner(row);
            row++;
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de trier les matchs !");
    }
}
void MainWindow::on_tri_histo_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, DATE_MATCH, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4, SCOREEDIT FROM MATCHES WHERE DATE_MATCH < CURRENT_DATE ORDER BY DATE_MATCH DESC");

    if (query.exec()) {
        ui->historique_table->setRowCount(0);

        int row = 0;
        QDateTime now = QDateTime::currentDateTime();

        while (query.next()) {
            QDateTime matchDateTime = query.value(5).toDateTime();  // Full date + time
            int scoreEdit = query.value(10).toInt();
            QString score = query.value(4).toString();

            // Calculate elapsed time since match
            qint64 secondsSinceMatch = matchDateTime.secsTo(now);
            bool isOlderThan2_5Hours = secondsSinceMatch > (2.5 * 3600);

            // Set row color based on time and scoreEdit
            QColor rowColor;
            if (!isOlderThan2_5Hours) {
                rowColor = QColor(255, 165, 0); // orange: less than 2.5 hours
            } else {
                rowColor = (scoreEdit == 0) ? QColor(255, 200, 200) : QColor(197, 255, 217); // red or green
            }

            ui->historique_table->insertRow(row);
            for (int col = 0; col < 10; col++) {
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    item->setText(score);
                } else {
                    item->setText(query.value(col).toString());
                }
                item->setBackground(rowColor);
                ui->historique_table->setItem(row, col, item);
            }
            row++;
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de trier les matchs !");
    }
}


//------------------------PREDICTION RESULTAT MATCH-----------------------------------------------------------------------------------------------

void MainWindow::predictWinner(int row)
{
   //------------------------------------------------
    if (!ui->programmation_2->item(row, 2) || !ui->programmation_2->item(row, 3)) {
        qDebug() << "Error: Missing team names in row" << row;
        return;
    }

    QString equipe1 = ui->programmation_2->item(row, 2)->text().trimmed();
    QString equipe2 = ui->programmation_2->item(row, 3)->text().trimmed();

    if (equipe1.isEmpty() || equipe2.isEmpty()) {
        qDebug() << "Error: One of the team names is empty in row" << row;
        return;
    }
    //-------------------------debut------------------------------------------
    QSqlQuery query;
    int countEquipe1 = 0, countEquipe2 = 0;


    query.prepare("SELECT COUNT(*) FROM MATCHES WHERE WINNER = :equipe");
    query.bindValue(":equipe", equipe1);
    if (query.exec() && query.next()) {
        countEquipe1 = query.value(0).toInt();
    } else {
        qDebug() << "Query Error for equipe1:" << query.lastError().text();
    }

    query.prepare("SELECT COUNT(*) FROM MATCHES WHERE WINNER = :equipe");
    query.bindValue(":equipe", equipe2);
    if (query.exec() && query.next()) {
        countEquipe2 = query.value(0).toInt();
    } else {
        qDebug() << "Query Error for equipe2:" << query.lastError().text();
    }


    QString predictedWinner;
    if (countEquipe1 > countEquipe2)
        predictedWinner = equipe1;
    else if (countEquipe1 < countEquipe2)
        predictedWinner = equipe2;
    else
        predictedWinner = "égalité";


    QTableWidgetItem *item = ui->programmation_2->item(row, 9);
    if (!item) {
        item = new QTableWidgetItem();
        ui->programmation_2->setItem(row, 9, item);
    }
    item->setText(predictedWinner);


    ui->programmation_2->viewport()->update();

    qDebug() << "Predicted winner for row" << row << ":" << predictedWinner;
}


//----------------------------------------STATS MATCH --------------------------------------------------------------------
void MainWindow::showMonthlyMatchStatistics() {

    QMap<int, int> monthCount;

    QSqlQuery query("SELECT DATE_MATCH FROM MATCHES WHERE EXTRACT(YEAR FROM DATE_MATCH) = 2025");

    while (query.next()) {
        QDate matchDate = query.value(0).toDate();
        int month = matchDate.month();
        monthCount[month]++;
    }


    QPieSeries *series = new QPieSeries();

    QStringList monthNames = {
        "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
        "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"
    };

    //---verts--
    QVector<QColor> greenShades = {
        QColor(0, 100, 0), QColor(34, 139, 34), QColor(50, 205, 50), QColor(60, 179, 113),
        QColor(46, 139, 87), QColor(107, 142, 35), QColor(154, 205, 50), QColor(85, 107, 47),
        QColor(144, 238, 144), QColor(173, 255, 47), QColor(0, 255, 127), QColor(0, 250, 154)
    };
    //-------


    QVector<QColor> sliceColors;
    QVector<QString> sliceLabels;

    int colorIndex = 0;
    for (int month = 1; month <= 12; month++) {
        int matchCount = monthCount.value(month, 0);
        if (matchCount > 0) {
            QPieSlice *slice = series->append(monthNames[month - 1] + " - " + QString::number(matchCount), matchCount);

            //----design------------------
            QColor sliceColor = greenShades[colorIndex % greenShades.size()];
            slice->setBrush(sliceColor);
            colorIndex++;
            sliceColors.append(sliceColor);
            sliceLabels.append(monthNames[month - 1]);
            slice->setLabelVisible(true);
            slice->setLabelColor(Qt::white);
            //----------------------------------

            connect(slice, &QPieSlice::hovered, [slice, monthNames, month, matchCount, monthCount](bool hovered) {
                if (hovered) {

                    int totalMatches = 0;
                    for (int i = 1; i <= 12; i++) {
                        totalMatches += monthCount.value(i, 0);
                    }

                    double percentage = (matchCount / double(totalMatches)) * 100;


                    //--design(tekber)----
                    slice->setExploded(true);
                    //-----------



                    QToolTip::showText(QCursor::pos(),
                                       QString("Mois: %1\nNombre de matchs: %2\nPourcentage: %3%")
                                           .arg(monthNames[month - 1])
                                           .arg(matchCount)
                                           .arg(QString::number(percentage, 'f', 0)));
                } else {
                    slice->setExploded(false);
                }
            });

        }
    }
//--design------------------------
    QChart *chart = new QChart();
    chart->addSeries(series);

    QFont titleFont = chart->titleFont();
    titleFont.setPointSize(10);
    titleFont.setBold(true);
    titleFont.setFamily("System");
    chart->setTitle("Statistiques des matches pour 2025");
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(Qt::black));

    chart->legend()->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->widget_6);
    chartView->resize(ui->widget_6->size());

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(chartView);
    chartView->setGraphicsEffect(opacityEffect);

    opacityEffect->setOpacity(0);


    QTimer::singleShot(0, [opacityEffect]() {
        QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
        animation->setDuration(5000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });


    QGraphicsScene *scene = chartView->scene();
    int xOffset = 20;
    int yOffset = chart->boundingRect().bottom() + 213;


    QGridLayout *legendLayout = new QGridLayout();
    legendLayout->setSpacing(5);


    QWidget *legendWidget = new QWidget();
    legendWidget->setLayout(legendLayout);
    legendWidget->setStyleSheet("background-color: transparent;");


    scene->addWidget(legendWidget);


    int columns = 4;
    for (int i = 0; i < sliceColors.size(); ++i) {
        int row = i / columns;
        int col = i % columns;

        QHBoxLayout *rowLayout = new QHBoxLayout();

        QFrame *colorBox = new QFrame();
        colorBox->setFrameShape(QFrame::Box);
        colorBox->setFixedSize(10, 10);
        colorBox->setStyleSheet(QString("background-color: %1").arg(sliceColors[i].name()));

        QLabel *textItem = new QLabel(sliceLabels[i]);
        textItem->setStyleSheet("font-size: 8px;");


        colorBox->installEventFilter(this);


        rowLayout->addWidget(colorBox);
        rowLayout->addWidget(textItem);


        legendLayout->addLayout(rowLayout, row, col);
    }


    legendWidget->setGeometry(xOffset, yOffset, 300, 10);
}



bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Enter) {

        if (QFrame *frame = qobject_cast<QFrame *>(watched)) {
            frame->setFixedSize(20, 20);
        }
    } else if (event->type() == QEvent::Leave) {

        if (QFrame *frame = qobject_cast<QFrame *>(watched)) {
            frame->setFixedSize(10, 10);
        }
    }
    return QObject::eventFilter(watched, event);
}


//---------------------------output arduino------------------------------------------------------

void MainWindow::on_showArduinoButton_clicked()
{
    arduinoMode = true;
    QMessageBox::information(this, "Afficher sur Arduino", "Veuillez sélectionner un match dans la table.");
}

void MainWindow::on_historique_table_cellClicked(int row, int column)
{
    if (!arduinoMode)
        return;

    if (column == 4) {
        QString team1Name = ui->historique_table->item(row, 2)->text().trimmed();
        QString team2Name = ui->historique_table->item(row, 3)->text().trimmed();
        QString score = ui->historique_table->item(row, column)->text();
        QStringList parts = score.split("-");

        if (parts.size() == 2) {
            bool ok1, ok2;
            int team1Score = parts[0].trimmed().toInt(&ok1);
            int team2Score = parts[1].trimmed().toInt(&ok2);

            if (!ok1 || !ok2) {
                QMessageBox::warning(this, "Erreur", "Format de score invalide");
                arduinoMode = false;
                return;
            }

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Envoyer le score",
                                          QString("Voulez-vous afficher les scores?\n"
                                                  "%1: %2 | %3: %4")
                                              .arg(team1Name).arg(team1Score)
                                              .arg(team2Name).arg(team2Score),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                QSerialPort serialPort;
                serialPort.setPortName("COM6");

                if (!serialPort.open(QIODevice::WriteOnly)) {
                    QMessageBox::warning(this, "Erreur",
                                         "Port série non disponible:\n" + serialPort.errorString());
                    arduinoMode = false;
                    return;
                }

                serialPort.setBaudRate(QSerialPort::Baud9600);

                QString message = QString("T1=%1;T2=%2;").arg(team1Score).arg(team2Score);

                if (serialPort.write(message.toUtf8())) {
                    serialPort.waitForBytesWritten(1000);
                    QMessageBox::information(this, "Succès",
                                             QString("Scores affichés:\n%1: %2 | %3: %4")
                                                 .arg(team1Name).arg(team1Score)
                                                 .arg(team2Name).arg(team2Score));
                }
                serialPort.close();
            }
            arduinoMode = false;
        }
    }
}
