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
#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>


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
    loadEquipes();
    checkForNullScoreEdit();
    originalTabWidth = ui->tabWidget->geometry().width();
    originalTableWidth = ui->programmation_2->geometry().width();
    originalWidget6Width = ui->widget_6->geometry().width();
    originalDeleteMatchX = ui->delete_match->geometry().x();
    originalChercherWidth = ui->chercher->geometry().width();
    isExpanded = false; // Initially, widgets are not expanded
    // Apply design
    applyDesign(ui);

    // Create and add the ChatBotWidget to widget_8
    ChatBotWidget *chatBot = new ChatBotWidget(this);
    ui->programmation_2->setEditTriggers(QAbstractItemView::DoubleClicked); // This enables editing on double-click
    ui->historique_table->setEditTriggers(QAbstractItemView::DoubleClicked);

    // Ensure widget_8 has a layout and then add the ChatBotWidget to it
    if (ui->widget_8->layout() == nullptr) {
        ui->widget_8->setLayout(new QVBoxLayout());
    }
    ui->widget_8->layout()->addWidget(chatBot);

    // Set up the timer to check for changes in the database every 5 seconds
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkForNullScoreEdit);
    timer->start(5000);  // Check every 5 seconds

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
    connect(ui->add_match, &QPushButton::clicked, this, &MainWindow::on_addMatchButton_clicked);

    connect(ui->delete_match, &QPushButton::clicked, this, &MainWindow::deleteMatch);
    connect(ui->refresh, &QPushButton::clicked, this, &MainWindow::loadMatchesIntoTable);

    connect(ui->random_arbitre, &QCheckBox::stateChanged, this, &MainWindow::handleRandomReferees);
    connect(ui->chercher, &QLineEdit::textChanged, this, &MainWindow::filterTable);

    connect(ui->programmation_2, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onCellDoubleClicked(int, int)));
    connect(ui->historique_table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onHistoriqueCellDoubleClicked(int, int)));
    connect(ui->historique_table, &QTableWidget::itemChanged, this, &MainWindow::onItemChanged);

     connect(ui->show_2, &QPushButton::clicked, this, &MainWindow::onShowButtonClicked);

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

    // Get date and time from QDateTimeEdit
    QDateTime dateTime = ui->dateTimeEdit_2->dateTime();  // Use dateTimeEdit_2 for date and time
    QString type = ui->comboBox->currentText();
    QString equipe1 = ui->comboBox_2->currentText();
    QString equipe2 = ui->comboBox_3->currentText();

    // Check if the teams are different
    if (equipe1 == equipe2) {
        QMessageBox::warning(this, "Error", "The two teams cannot be the same.");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    // Select referees
    QStringList arbitreNames;

    if (ui->random_arbitre->isChecked()) {
        // Retrieve all referees from the database
        QSqlQuery query("SELECT NOM FROM ARBITRES");
        QStringList allArbitres;

        while (query.next()) {
            allArbitres.append(query.value(0).toString());
        }

        if (allArbitres.size() < 4) {
            QMessageBox::warning(this, "Error", "Not enough referees for random selection.");
            ui->pushButton->setEnabled(true);
            isInsertingMatch = false;
            return;
        }

        // Shuffle and select 4 random referees
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allArbitres.begin(), allArbitres.end(), g);

        arbitreNames = allArbitres.mid(0, 4); // Select 4 random referees
    } else {
        // Manual selection
        arbitreNames = {
            ui->comboBox_arbitre1->currentText(),
            ui->comboBox_arbitre2->currentText(),
            ui->comboBox_arbitre3->currentText(),
            ui->comboBox_arbitre4->currentText()
        };
    }

    // Check if all referees are unique
    QSet<QString> uniqueReferees(arbitreNames.begin(), arbitreNames.end());
    if (uniqueReferees.size() < 4) {
        QMessageBox::warning(this, "Error", "Referees must be unique.");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    // Retrieve referee IDs
    QList<int> arbitreIds;
    QSqlQuery refereeQuery;

    for (const QString& name : arbitreNames) {
        if (!name.isEmpty()) {
            refereeQuery.prepare("SELECT ID_ARBITRE FROM ARBITRES WHERE NOM = :nom");
            refereeQuery.bindValue(":nom", name);

            if (refereeQuery.exec() && refereeQuery.next()) {
                arbitreIds.append(refereeQuery.value(0).toInt());
            } else {
                QMessageBox::critical(this, "Error", "Error retrieving referee ID: " + name);
                ui->pushButton->setEnabled(true);
                isInsertingMatch = false;
                return;
            }
        } else {
            arbitreIds.append(QVariant().toInt());  // Add NULL if no referee selected
        }
    }

    // Ensure exactly 4 referee IDs
    while (arbitreIds.size() < 4) {
        arbitreIds.append(QVariant().toInt());
    }

    // Insert the match into the database with both date and time
    QSqlQuery query;
    query.prepare("INSERT INTO MATCHES (DATE_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4) "
                  "VALUES (TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS'), :type, :equipe1, :equipe2, :score, :id_arbitre1, :id_arbitre2, :id_arbitre3, :id_arbitre4)");
    query.bindValue(":date", dateTime.toString("yyyy-MM-dd HH:mm:ss"));  // Format date and time
    query.bindValue(":type", type);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":score", "0-0");
    query.bindValue(":id_arbitre1", arbitreIds[0]);
    query.bindValue(":id_arbitre2", arbitreIds[1]);
    query.bindValue(":id_arbitre3", arbitreIds[2]);
    query.bindValue(":id_arbitre4", arbitreIds[3]);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Match successfully added with 4 referees!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to add match: " + query.lastError().text());
    }

    ui->pushButton->setEnabled(true);
    isInsertingMatch = false;
}
void MainWindow::loadMatchesIntoTable()
{
    // Modify the query to select SCORE and SCOREEDIT
    QSqlQuery query("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, DATE_MATCH, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4, SCOREEDIT FROM MATCHES");

    // Clear existing tables
    ui->programmation_2->setRowCount(0);
    ui->historique_table->setRowCount(0);

    QDate today = QDate::currentDate();
    int progRow = 0, histRow = 0;

    while (query.next()) {
        QDate matchDate = query.value(5).toDate(); // DATE_MATCH is now at index 5 (6th column)
        int scoreEdit = query.value(10).toInt(); // Fetch the SCOREEDIT value (index 10)
        QString score = query.value(4).toString(); // SCORE is at index 4

        QColor rowColor;
        // Check the score and SCOREEDIT value to determine row color
        if (scoreEdit ==0 ) {
            rowColor = QColor(255, 200, 200);  // Light red color
        } else if (score != "0-0") {
            rowColor = QColor(197, 255, 217);  // Light green color
        } else {
            rowColor = QColor(197, 255, 217);  // White color if not modified
        }

        if (matchDate < today) {
            // Insert match into the historical table
            ui->historique_table->insertRow(histRow);
            for (int col = 0; col < 10; col++) { // Now we have 10 columns (including SCORE before DATE_MATCH)
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    // Add the SCORE value in the 5th column (index 4)
                    item->setText(score);
                } else if (col == 5) {
                    // Add the DATE_MATCH value in the 6th column (index 5)
                    item->setText(query.value(5).toString());
                } else if (col == 6) {
                    // Add the ID_ARBITRE1 value in the 7th column (index 6)
                    item->setText(query.value(6).toString());
                } else if (col == 7) {
                    // Add the ID_ARBITRE2 value in the 8th column (index 7)
                    item->setText(query.value(7).toString());
                } else if (col == 8) {
                    // Add the ID_ARBITRE3 value in the 9th column (index 8)
                    item->setText(query.value(8).toString());
                } else if (col == 9) {
                    // Add the ID_ARBITRE4 value in the 10th column (index 9)
                    item->setText(query.value(9).toString());
                } else {
                    // Insert other columns, shifting them correctly
                    item->setText(query.value(col < 4 ? col : col + 2).toString());
                }

                // Apply the color to the entire row
                item->setBackground(rowColor);
                ui->historique_table->setItem(histRow, col, item);
            }
            histRow++;
        } else {
            // Insert match into the programming table, but without the SCORE column
            ui->programmation_2->insertRow(progRow);
            for (int col = 0; col < 9; col++) { // Now only 9 columns (excluding SCORE)
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    // Add the DATE_MATCH value in the 5th column (index 4)
                    item->setText(query.value(5).toString());
                } else if (col == 5) {
                    // Add the ID_ARBITRE1 value in the 6th column (index 5)
                    item->setText(query.value(6).toString());
                } else if (col == 6) {
                    // Add the ID_ARBITRE2 value in the 7th column (index 6)
                    item->setText(query.value(7).toString());
                } else if (col == 7) {
                    // Add the ID_ARBITRE3 value in the 8th column (index 7)
                    item->setText(query.value(8).toString());
                } else if (col == 8) {
                    // Add the ID_ARBITRE4 value in the 9th column (index 8)
                    item->setText(query.value(9).toString());
                } else {
                    // Insert other columns, shifting them correctly (excluding SCORE)
                    item->setText(query.value(col < 4 ? col : col + 1).toString());
                }

                ui->programmation_2->setItem(progRow, col, item);
            }
            progRow++;
        }
    }

}



void MainWindow::filterTable()
{
    QString filterText = ui->chercher->text().trimmed();
    QString filterOption = ui->tri_options->currentText();

    for (int i = 0; i < ui->programmation_2->rowCount(); i++) {
        bool match = false;

        if (filterOption == "date") {
            QString dateValue = ui->programmation_2->item(i, 4)->text();  // Colonne date
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
    query.prepare("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, DATE_MATCH FROM MATCHES");

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
//---------------------------------------------------------------------------------

void MainWindow::onCellDoubleClicked(int row, int column)
{
    // Ensure the clicked cell has a valid item
    if (!ui->programmation_2->item(row, column)) {
        qDebug() << "Cell at row " << row << ", column " << column << " is null!";
        return;
    }

    // Get current value and column name
    QString oldValue = ui->programmation_2->item(row, column)->text();
    QString columnName = getColumnName(column); // Function to get the actual column name from index

    // Prevent modification of ID_MATCH
    if (columnName == "ID_MATCH") {
        QMessageBox::warning(this, "Modification Interdite", "Vous ne pouvez pas modifier l'ID du match.");
        return;
    }

    // Debugging output
    qDebug() << "Clicked column: " << column << ", Column Name: " << columnName << ", Current Value: " << oldValue;

    // Ask for confirmation before editing
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation :", "Êtes-vous sûr de vouloir modifier cette valeur ?", QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    bool ok;
    QString newValue;

    // Handle TYPE_MATCH dropdown
    if (columnName == "TYPE_MATCH") {
        QStringList typeOptions = { "Amical", "Groupe", "Championnat", "Coupe", "Qualification", "PlayOff" };
        newValue = QInputDialog::getItem(this, "Modification :", "Sélectionnez un nouveau type:", typeOptions, 0, false, &ok);
    }
    // Handle EQUIPE1 and EQUIPE2 dropdown
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
    }
    // Handle Referee Selection for ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4
    else if (columnName.toUpper().contains("ID_ARBITRE")) {
        QMap<QString, QString> refereeMap; // Map NOM to ID_ARBITRE
        QStringList refereeNames;
        QSqlQuery query("SELECT ID_ARBITRE, NOM FROM ARBITRES");

        while (query.next()) {
            QString idArbitre = query.value(0).toString();
            QString nomArbitre = query.value(1).toString();
            refereeNames << nomArbitre;
            refereeMap[nomArbitre] = idArbitre;
        }

        if (refereeNames.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun arbitre trouvé dans la base de données.");
            return;
        }

        QString selectedNom = QInputDialog::getItem(this, "Modification :", "Sélectionnez un arbitre :", refereeNames, 0, false, &ok);
        if (ok && !selectedNom.isEmpty()) {
            newValue = refereeMap[selectedNom]; // Get the corresponding ID_ARBITRE
            qDebug() << "Selected referee: " << selectedNom << " with ID: " << newValue;

            // Check if the referee is already assigned in another column
            for (int i = 5; i <= 8; ++i) { // Columns for ID_ARBITRE1 to ID_ARBITRE4
                if (i != column) { // Skip the current column being modified
                    QTableWidgetItem* item = ui->programmation_2->item(row, i);
                    if (item && item->text() == newValue) {
                        QMessageBox::warning(this, "Erreur", "Cet arbitre est déjà assigné à ce match. Veuillez en choisir un autre.");
                        return;
                    }
                }
            }
        }
    }
    // Handle Date column (DATE_MATCH)
    else if (columnName == "DATE_MATCH") {
        // Create a DateTime edit dialog
        QDateTime currentDateTime = QDateTime::fromString(oldValue, "yyyy-MM-dd HH:mm:ss");
        QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(currentDateTime);
        dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        dateTimeEdit->setCalendarPopup(true);
        dateTimeEdit->setDateTime(currentDateTime);

        // Create a custom dialog to show the QDateTimeEdit
        QDialog *dateDialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout(dateDialog);
        layout->addWidget(dateTimeEdit);
        QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::accepted, dateDialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, dateDialog, &QDialog::reject);

        if (dateDialog->exec() == QDialog::Accepted) {
            newValue = dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
        }

        delete dateDialog; // Clean up the dialog
    }

    // Update the table item with the new value
    if (!newValue.isEmpty()) {
        ui->programmation_2->item(row, column)->setText(newValue);

        // Update the match in the database with the new value
        QSqlQuery query;
        if (columnName == "DATE_MATCH") {
            query.prepare("UPDATE MATCHES SET DATE_MATCH = TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS') WHERE ID_MATCH = :id_match");
            query.bindValue(":date", newValue);
        } else if (columnName == "TYPE_MATCH" || columnName == "EQUIPE1" || columnName == "EQUIPE2" || columnName.contains("ID_ARBITRE")) {
            query.prepare("UPDATE MATCHES SET " + columnName + " = :value WHERE ID_MATCH = :id_match");
            query.bindValue(":value", newValue);
        }

        query.bindValue(":id_match", ui->programmation_2->item(row, 0)->text()); // Assuming ID_MATCH is in column 0

        if (query.exec()) {
            QMessageBox::information(this, "Success", "La valeur a été modifiée avec succès.");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de la valeur : " + query.lastError().text());
        }
    }
}


//-----------------------------------------------------------------------------------------------------------

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
    default: return "";
    }
}
void MainWindow::onHistoriqueCellDoubleClicked(int row, int col)
{
    // Assuming score is in the 4th column (index 4) in historique_table
    if (col == 4) {
        // Ask for confirmation before editing the score
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Edit", "Are you sure you want to edit the score?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Get the current value in the score cell
            QString oldScore = ui->historique_table->item(row, col)->text();

            // Create a QRegularExpression to validate the X-Y format
            QRegularExpression regExp("^\\d+-\\d+$"); // Regular expression to ensure X-Y format with positive integers
            QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);

            // Create an input dialog with the old score prefilled and the placeholder set to "X-Y"
            bool ok;
            QString newScore = QInputDialog::getText(this, "Edit Score", "New Score:", QLineEdit::Normal,
                                                     oldScore, &ok);

            if (ok && !newScore.isEmpty()) {
                // Validate the entered score
                int pos = 0;
                if (validator->validate(newScore, pos) == QValidator::Acceptable) {
                    // Update the score in the table
                    ui->historique_table->item(row, col)->setText(newScore);

                    // Now update the database
                    QSqlQuery query;
                    query.prepare("UPDATE MATCHES SET SCORE = :score, SCOREEDIT = 1 WHERE ID_MATCH = :id_match");
                    query.bindValue(":score", newScore);
                    query.bindValue(":id_match", ui->historique_table->item(row, 0)->text()); // Assuming the ID_MATCH is in column 0

                    if (query.exec()) {
                        QMessageBox::information(this, "Success", "Change successful!");
                    } else {
                        QMessageBox::warning(this, "Error", "Failed to update score in the database.");
                    }
                } else {
                    QMessageBox::warning(this, "Invalid Format", "Please enter a valid score in the format X-Y.");
                }
            }
        }
    }
}

void MainWindow::onItemChanged(QTableWidgetItem *item)
{
    int row = item->row();
    int col = item->column();

    // Check if we are editing the SCORE column (index 4)
    if (col == 4) {
        QString newScore = item->text();

        // Check the SCOREEDIT value from the database to determine if it's been modified before
        QSqlQuery query;
        query.prepare("SELECT SCOREEDIT FROM MATCHES WHERE ID_MATCH = :id_match");
        query.bindValue(":id_match", ui->historique_table->item(row, 0)->text()); // Assuming ID_MATCH is in column 0

        if (query.exec() && query.next()) {
            int scoreEditValue = query.value(0).toInt();

            QColor rowColor;
            if (newScore == "0-0") {
                if (scoreEditValue == 1) {
                    // If SCOREEDIT is 1 (score already modified), apply white background to the entire row
                    rowColor = QColor(197, 255, 217);
                } else {
                    // If SCOREEDIT is not 1 (score has not been modified), apply red color to the entire row
                    rowColor = QColor(255, 200, 200);  // Light red color
                }
            } else {
                // If the score is not 0-0, apply green color to the entire row
                rowColor = QColor(197, 255, 217);  // Light green color
            }

            // Iterate through all columns in the row and set the background color
            for (int c = 0; c < ui->historique_table->columnCount(); ++c) {
                QTableWidgetItem *rowItem = ui->historique_table->item(row, c);
                if (rowItem) {
                    rowItem->setBackground(rowColor);
                }
            }
        } else {
            // Handle query failure, if needed
        }
    }

    // You can add your database update logic here to save the new score to the database
}
void MainWindow::checkForNullScoreEdit()
{
    // Get today's date
    QDate today = QDate::currentDate();

    // Query to check for any NULL values in the SCOREEDIT column and compare the date
    QSqlQuery query("SELECT SCOREEDIT, DATE_MATCH FROM MATCHES WHERE SCOREEDIT IS NULL OR SCOREEDIT = '0'");

    // Clear any previous "!" label if it exists, except label_21
    QList<QLabel*> existingLabels = ui->widget_7->findChildren<QLabel*>();
    for (QLabel* label : existingLabels) {
        if (label != ui->label_21) {  // Make sure not to remove label_21
            label->deleteLater();  // Remove the previous label
        }
    }

    // Check if any result is returned (i.e., there are NULL or "0" values in the SCOREEDIT column)
    bool labelDisplayed = false;  // To ensure only one label is shown
    while (query.next()) {
        QString scoreEdit = query.value(0).toString();
        QDateTime dateTimeMatch = query.value(1).toDateTime();

        // Extract the date from the QDateTime
        QDate matchDate = dateTimeMatch.date();

        // Debug output to check the score and match date
        qDebug() << "ScoreEdit: " << scoreEdit << ", Match Date: " << matchDate.toString() << ", Today: " << today.toString();

        // Check if the score is "0" or NULL and the match date is before today
        if ((scoreEdit.isEmpty() || scoreEdit == "0") && matchDate.isValid() && matchDate < today) {
            // Display the "!" emoji on the top left of widget_7
            QLabel *label = new QLabel("❗", ui->widget_7);
            label->setStyleSheet("font-size: 15px; color: red;");
            label->move(10, 10);  // Position it at the top left
            label->show();
            labelDisplayed = true;
            break;  // Exit the loop after displaying the label
        }
    }

    // If no label was displayed, debug output
    if (!labelDisplayed) {
        qDebug() << "No matching conditions found.";
    }
}
void MainWindow::onShowButtonClicked()
{
    // Get current widths and positions of the widgets
    int currentTabWidth = ui->tabWidget->geometry().width();
    int currentTableWidth = ui->programmation_2->geometry().width();
    int currentWidget6Width = ui->widget_6->geometry().width();
    int currentDeleteMatchX = ui->delete_match->geometry().x();
    int currentChercherWidth = ui->chercher->geometry().width();

    int currentShow2X = ui->show_2->geometry().x();

    // Start animations for the tabWidget, tableWidget, and widget_6 (expand/contract)
    QPropertyAnimation *tabWidgetAnimation = new QPropertyAnimation(ui->tabWidget, "geometry");
    tabWidgetAnimation->setDuration(500);
    QRect tabWidgetNewRect = ui->tabWidget->geometry();
    if (isExpanded) {
        tabWidgetNewRect.setWidth(currentTabWidth - 300);
    } else {
        tabWidgetNewRect.setWidth(currentTabWidth + 300);
    }
    tabWidgetAnimation->setEndValue(tabWidgetNewRect);

    QPropertyAnimation *tableWidgetAnimation = new QPropertyAnimation(ui->programmation_2, "geometry");
    tableWidgetAnimation->setDuration(500);
    QRect tableWidgetNewRect = ui->programmation_2->geometry();
    if (isExpanded) {
        tableWidgetNewRect.setWidth(currentTableWidth - 400);
    } else {
        tableWidgetNewRect.setWidth(currentTableWidth + 400);
    }
    tableWidgetAnimation->setEndValue(tableWidgetNewRect);

    QPropertyAnimation *widget6Animation = new QPropertyAnimation(ui->widget_6, "geometry");
    widget6Animation->setDuration(500);
    QRect widget6NewRect = ui->widget_6->geometry();
    if (isExpanded) {
        widget6NewRect.setWidth(currentWidget6Width - 100);
        widget6NewRect.moveLeft(widget6NewRect.left() - 300);
    } else {
        widget6NewRect.setWidth(currentWidget6Width + 100);
        widget6NewRect.moveLeft(widget6NewRect.left() + 300);
    }
    widget6Animation->setEndValue(widget6NewRect);

    // Animation for moving the delete_match button to the left
    QPropertyAnimation *deleteMatchAnimation = new QPropertyAnimation(ui->delete_match, "geometry");
    deleteMatchAnimation->setDuration(500);
    QRect deleteMatchNewRect = ui->delete_match->geometry();
    if (isExpanded) {
        deleteMatchNewRect.moveLeft(currentDeleteMatchX - 290);
    } else {
        deleteMatchNewRect.moveLeft(currentDeleteMatchX + 290);
    }
    deleteMatchAnimation->setEndValue(deleteMatchNewRect);

    // Animation for expanding the chercher QLineEdit
    QPropertyAnimation *chercherAnimation = new QPropertyAnimation(ui->chercher, "geometry");
    chercherAnimation->setDuration(500);
    QRect chercherNewRect = ui->chercher->geometry();
    if (isExpanded) {
        chercherNewRect.setWidth(currentChercherWidth - 290);
    } else {
        chercherNewRect.setWidth(currentChercherWidth + 290);
    }
    chercherAnimation->setEndValue(chercherNewRect);


    // Animation for moving the show_2 button to the left
    QPropertyAnimation *show2ButtonAnimation = new QPropertyAnimation(ui->show_2, "geometry");
    show2ButtonAnimation->setDuration(500);
    QRect show2ButtonNewRect = ui->show_2->geometry();
    if (isExpanded) {
        show2ButtonNewRect.moveLeft(currentShow2X - 290);  // Move button left by 290px
    } else {
        show2ButtonNewRect.moveLeft(currentShow2X + 290);  // Move button back to original position
    }
    show2ButtonAnimation->setEndValue(show2ButtonNewRect);

    // Start all animations
    tabWidgetAnimation->start();
    tableWidgetAnimation->start();
    widget6Animation->start();
    deleteMatchAnimation->start();
    chercherAnimation->start();

    show2ButtonAnimation->start();

    // Toggle the expanded state
    isExpanded = !isExpanded;
}

void MainWindow::on_exporter_match_clicked()
{
    // Demander à l'utilisateur quoi exporter
    QMessageBox msgBox;
    msgBox.setWindowTitle("Exporter");
    msgBox.setText("Choisissez ce que vous voulez exporter :");

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
    else {
        QMessageBox::information(this, "Export annulé", "Vous avez annulé l'exportation.");
    }
}

void MainWindow::exportTableToPDF(QTableWidget *table, const QString &filename)
{
    if (!table) return;

    QPdfWriter pdfWriter(filename);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    QFont font("Arial", 14); // Plus grand pour occuper l'espace
    painter.setFont(font);

    int leftMargin = 20;
    int topMargin = 50;
    int rowHeight = 80; // Encore plus d'espace !
    int textPadding = 10; // Marge interne pour éviter que le texte touche les bords

    int columnCount = table->columnCount();
    int pageWidth = pdfWriter.width() - 2 * leftMargin;
    int columnWidth = pageWidth / columnCount;

    int y = topMargin;

    // **Dessiner l'en-tête de la table**
    painter.setPen(Qt::black);
    painter.setBrush(Qt::lightGray);
    painter.drawRect(leftMargin, y, pageWidth, rowHeight);

    for (int col = 0; col < columnCount; ++col) {
        QString headerText = table->horizontalHeaderItem(col) ? table->horizontalHeaderItem(col)->text() : "";
        QRect headerRect(leftMargin + col * columnWidth + textPadding, y, columnWidth - 2 * textPadding, rowHeight);
        painter.drawText(headerRect, Qt::AlignCenter, headerText); // Centré dans la cellule
    }

    y += rowHeight;

    // **Dessiner les données de la table**
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < table->rowCount(); ++row) {
        painter.drawRect(leftMargin, y, pageWidth, rowHeight);

        for (int col = 0; col < columnCount; ++col) {
            QString cellText = table->item(row, col) ? table->item(row, col)->text() : "";
            QRect cellRect(leftMargin + col * columnWidth + textPadding, y, columnWidth - 2 * textPadding, rowHeight);
            painter.drawText(cellRect, Qt::AlignCenter, cellText); // Centré dans la cellule
        }
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Exportation réussie", "Le fichier PDF a été enregistré avec succès !");
}
