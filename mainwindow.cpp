#include "mainwindow.h"
#include "./ui_mainwindow.h"
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

#include "loginwindow.h"// Include the chatbot widget header


void MainWindow::on_logoutButton_clicked()
{
    // Close the current MainWindow
    this->close();

    // Create and show the login window
    LoginWindow *loginWindow = new LoginWindow(this);  // Assuming you have a LoginWindow class
    loginWindow->show();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadMatchesIntoTable();

    loadRefereesIntoComboBox();



    // Apply design
    applyDesign(ui);

    // Create and add the ChatBotWidget to widget_8
    ChatBotWidget *chatBot = new ChatBotWidget(this);

    // Ensure widget_8 has a layout and then add the ChatBotWidget to it
    if (ui->widget_8->layout() == nullptr) {
        ui->widget_8->setLayout(new QVBoxLayout());
    }
    ui->widget_8->layout()->addWidget(chatBot);

    //changeWidget
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
    connect(ui->add_match, &QPushButton::clicked, this, &MainWindow::on_addMatchButton_clicked);
    connect(ui->edit_match, &QPushButton::clicked, this, &MainWindow::editMatch);
    connect(ui->delete_match, &QPushButton::clicked, this, &MainWindow::deleteMatch);
    connect(ui->refresh, &QPushButton::clicked, this, &MainWindow::refreshTable);
    connect(ui->random_arbitre, &QCheckBox::stateChanged, this, &MainWindow::handleRandomReferees);
   connect(ui->chercher, &QLineEdit::textChanged, this, &MainWindow::filterTable);



}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::toggleIconOnlySidebar()
{
    if (ui->iconOnlySidebar->isVisible()) {
        // Switch to full sidebar mode (restore original sizes)
        ui->iconOnlySidebar->hide();
        ui->iconTextSidebar->show();

        // Restore the original size of widgets
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
        // Switch to icon-only sidebar mode (expand widgets)
        ui->iconOnlySidebar->show();
        ui->iconTextSidebar->hide();

        // Expand widgets
        ui->stackedWidget->setMinimumWidth(1500);
        ui->stackedWidget->setMaximumWidth(1500);


        ui->widget_6->setMinimumWidth(400);
        ui->widget_6->setMaximumWidth(800);

        ui->widget_7->setMinimumWidth(300);
        ui->widget_7->setMaximumWidth(600);

        ui->widget_8->setMinimumWidth(400);
        ui->widget_8->setMaximumWidth(800);
    }

    // Force UI refresh
    ui->stackedWidget->update();

    ui->widget_6->update();
    ui->widget_7->update();
    ui->widget_8->update();
}
bool isInsertingMatch = false;  // Flag to prevent double insertion

void MainWindow::on_addMatchButton_clicked() {
    qDebug() << "Add Match Button Clicked";

    if (isInsertingMatch) {
        qDebug() << "Match is already being inserted, skipping.";
        return;
    }

    isInsertingMatch = true;
    ui->pushButton->setEnabled(false);

    QDate date = ui->dateEdit_2->date();
    QString type = ui->comboBox->currentText();
    QString equipe1 = ui->comboBox_2->currentText();
    QString equipe2 = ui->comboBox_3->currentText();

    if (equipe1 == equipe2) {
        QMessageBox::warning(this, "Error", "Les deux équipes ne peuvent pas être les mêmes.");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    QStringList arbitreNames;

    if (ui->random_arbitre->isChecked()) {
        // Fetch all referees from the database
        QSqlQuery query("SELECT NOM FROM ARBITRES");
        QStringList allArbitres;

        while (query.next()) {
            allArbitres.append(query.value(0).toString());
        }

        if (allArbitres.size() < 4) {
            QMessageBox::warning(this, "Error", "Il n'y a pas assez d'arbitres pour une sélection aléatoire.");
            ui->pushButton->setEnabled(true);
            isInsertingMatch = false;
            return;
        }

        // Shuffle and pick 4 random referees
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allArbitres.begin(), allArbitres.end(), g);

        arbitreNames = allArbitres.mid(0, 4); // Select first 4 random referees
    } else {
        // Manual selection
        arbitreNames = {
            ui->comboBox_arbitre1->currentText(),
            ui->comboBox_arbitre2->currentText(),
            ui->comboBox_arbitre3->currentText(),
            ui->comboBox_arbitre4->currentText()
        };
    }

    // Ensure all referees are different
    QSet<QString> uniqueReferees(arbitreNames.begin(), arbitreNames.end());
    if (uniqueReferees.size() < 4) {
        QMessageBox::warning(this, "Error", "Les arbitres doivent être différents.");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    // Get referee IDs from the database
    QList<int> arbitreIds;
    QSqlQuery refereeQuery;

    for (const QString& name : arbitreNames) {
        if (!name.isEmpty()) {
            refereeQuery.prepare("SELECT ID_ARBITRE FROM ARBITRES WHERE NOM = :nom");
            refereeQuery.bindValue(":nom", name);

            if (refereeQuery.exec() && refereeQuery.next()) {
                arbitreIds.append(refereeQuery.value(0).toInt());
            } else {
                QMessageBox::critical(this, "Error", "Erreur lors de la récupération de l'ID de l'arbitre: " + name);
                ui->pushButton->setEnabled(true);
                isInsertingMatch = false;
                return;
            }
        } else {
            arbitreIds.append(QVariant().toInt());  // Ajout de valeur NULL si pas d'arbitre sélectionné
        }
    }

    // Ensure exactly 4 elements (NULL for missing selections)
    while (arbitreIds.size() < 4) {
        arbitreIds.append(QVariant().toInt());
    }

    // Insert match with referees
    QSqlQuery query;
    query.prepare("INSERT INTO MATCHES (DATE_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4) "
                  "VALUES (TO_DATE(:date, 'YYYY-MM-DD'), :type, :equipe1, :equipe2, :score, :id_arbitre1, :id_arbitre2, :id_arbitre3, :id_arbitre4)");
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":type", type);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":score", "0-0");
    query.bindValue(":id_arbitre1", arbitreIds[0]);
    query.bindValue(":id_arbitre2", arbitreIds[1]);
    query.bindValue(":id_arbitre3", arbitreIds[2]);
    query.bindValue(":id_arbitre4", arbitreIds[3]);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Match ajouté avec succès avec 4 arbitres !");
    } else {
        QMessageBox::critical(this, "Error", "Ajout du match échoué: " + query.lastError().text());
    }

    ui->pushButton->setEnabled(true);
    isInsertingMatch = false;
}



void MainWindow::loadMatchesIntoTable()
{
    QSqlQuery query("SELECT ID_MATCH, TYPE_MATCH,  EQUIPE1, EQUIPE2,STADE, DATE_MATCH,ID_ARBITRE1,ID_ARBITRE2,ID_ARBITRE3 ,ID_ARBITRE4  FROM matches");

    // Clear existing data
    ui->programmation_2->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->programmation_2->insertRow(row);

        // Insert data into each column
        for (int col = 0; col < 11; col++) {
            ui->programmation_2->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        row++;
    }
}






void MainWindow::filterTable()
{
    QString filterText = ui->chercher->text().trimmed();
    QString filterOption = ui->tri_options->currentText();

    for (int i = 0; i < ui->programmation_2->rowCount(); i++) {
        bool match = false;

        if (filterOption == "date") {
            QString dateValue = ui->programmation_2->item(i, 5)->text();  // Colonne date
            match = dateValue.contains(filterText, Qt::CaseInsensitive);
        } else if (filterOption == "équipe") {
            QString equipe1 = ui->programmation_2->item(i, 2)->text();  // Colonne équipe 1
            QString equipe2 = ui->programmation_2->item(i, 3)->text();  // Colonne équipe 2
            match = equipe1.contains(filterText, Qt::CaseInsensitive) || equipe2.contains(filterText, Qt::CaseInsensitive);
        }

        ui->programmation_2->setRowHidden(i, !match);
    }
}
void MainWindow::deleteMatch()
{
    int row = ui->programmation_2->currentRow(); // Get the selected row
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un match à supprimer.");
        return;
    }

    // Retrieve ID of the match from the first column (ID_MATCH)
    QString id = ui->programmation_2->item(row, 0)->text();

    // Ask for confirmation before deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce match ?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return; // If No is selected, abort the deletion
    }

    // Prepare SQL query to delete the match by ID
    QSqlQuery query;
    query.prepare("DELETE FROM MATCHES WHERE ID_MATCH = :id");
    query.bindValue(":id", id);

    // Execute the query
    if (query.exec()) {
        QMessageBox::information(this, "Suppression réussie", "Le match a été supprimé avec succès de la base de données !");

        // Remove the row from the table view
        ui->programmation_2->removeRow(row); // This will remove the selected row from the view
    } else {
        // Print error for debugging
        QString error = query.lastError().text();
        qDebug() << "SQL Error: " << error;
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression du match dans la base de données : " + error);
    }
}

void MainWindow::refreshTable()
{
    // Clear existing rows in the table
    ui->programmation_2->setRowCount(0);

    // Create a query to select all data from the database
    QSqlQuery query;
    query.prepare("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, STADE, DATE_MATCH FROM MATCHES");

    if (query.exec()) {
        // Loop through the query results and insert them into the table
        while (query.next()) {
            int row = ui->programmation_2->rowCount();
            ui->programmation_2->insertRow(row);

            // Set the values from the database into the table cells
            ui->programmation_2->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // ID_MATCH
            ui->programmation_2->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // TYPE

            ui->programmation_2->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // EQUIPE1
            ui->programmation_2->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // EQUIPE2
            ui->programmation_2->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // SCORE
            ui->programmation_2->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
                // DATE_MATCH
        }
    } else {
        // Handle query error if there is one
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données : " + query.lastError().text());
    }
}


void MainWindow::editMatch()
{
    int row = ui->programmation_2->currentRow(); // Get the selected row
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un match à modifier.");
        return;
    }

    // Retrieve current values
    QString id = ui->programmation_2->item(row, 0)->text();
    QString type = ui->programmation_2->item(row, 4)->text();
    QString stade = ui->programmation_2->item(row, 2)->text();
    QString equipe1 = ui->programmation_2->item(row, 2)->text();
    QString equipe2 = ui->programmation_2->item(row, 3)->text();
    QString score = ui->programmation_2->item(row, 6)->text();
    QString date = ui->programmation_2->item(row, 5)->text();

    // Dialogs to modify the values
    bool ok;
    type = QInputDialog::getText(this, "Modifier le type ", "Type :", QLineEdit::Normal, type, &ok);
    if (!ok) return;


    equipe1 = QInputDialog::getText(this, "Modifier l'équipe 1", "Équipe 1 :", QLineEdit::Normal, equipe1, &ok);
    if (!ok) return;

    equipe2 = QInputDialog::getText(this, "Modifier l'équipe 2", "Équipe 2 :", QLineEdit::Normal, equipe2, &ok);
    if (!ok) return;
    stade = QInputDialog::getText(this, "Modifier le stade", "Stade :", QLineEdit::Normal, stade, &ok);
    if (!ok) return;
    score = QInputDialog::getText(this, "Modifier le score", "Score :", QLineEdit::Normal, score, &ok);
    if (!ok) return;

    date = QInputDialog::getText(this, "Modifier la date", "Date :", QLineEdit::Normal, date, &ok);
    if (!ok) return;

    // Prepare the SQL update query without modifying ID_MATCH
    QSqlQuery query;
    query.prepare("UPDATE MATCHES SET "
                  "TYPE_MATCH = :type, "
                  "STADE = :stade, "
                  "EQUIPE1 = :equipe1, "
                  "EQUIPE2 = :equipe2, "
                  "SCORE = :score, "
                  "DATE_MATCH = TO_DATE(:date, 'YYYY-MM-DD') "
                  "WHERE ID_MATCH = :id");

    query.bindValue(":stade", stade);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":type", type);
    query.bindValue(":score", score);
    query.bindValue(":date", date);  // Ensure date is formatted as a valid DATE type
    query.bindValue(":id", id);      // Don't modify ID_MATCH as it's auto-generated

    // Debug output
    qDebug() << "Executing Query: " << query.executedQuery();

    if (query.exec()) {
        QMessageBox::information(this, "Modification réussie", "Le match a été modifié avec succès dans la base de données !");

        // Update the table view directly with the new values for the edited row
        ui->programmation_2->item(row, 1)->setText(type);
        ui->programmation_2->item(row, 4)->setText(stade); // Update the stade column
        ui->programmation_2->item(row, 2)->setText(equipe1); // Update the equipe1 column
        ui->programmation_2->item(row, 3)->setText(equipe2); // Update the equipe2 column
        ui->programmation_2->item(row, 6)->setText(score);
        ui->programmation_2->item(row, 5)->setText(date);    // Update the date column
    } else {
        // Print error for debugging
        QString error = query.lastError().text();
        qDebug() << "SQL Error: " << error;
        QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour du match dans la base de données : " + error);
    }
}
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

    // Disable/Enable manual selection
    ui->comboBox_arbitre1->setEnabled(!randomMode);
    ui->comboBox_arbitre2->setEnabled(!randomMode);
    ui->comboBox_arbitre3->setEnabled(!randomMode);
    ui->comboBox_arbitre4->setEnabled(!randomMode);

    if (randomMode) {
        // Fetch all referees from the database
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

        // Shuffle and pick 4 unique referees
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
