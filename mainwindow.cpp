#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "championnats.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //design
    applyDesign(ui);

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
    ui->tabWidget->setTabVisible(3, false);
    connect(ui->PBA, &QPushButton::clicked, this, &MainWindow::ajoutChampB_clicked);
    connect(ui->majbutton, &QPushButton::clicked, this, &MainWindow::modifChampB_clicked);

    connect(ui->lineEdit_nom, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->lineEdit_nbrE, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->lineEdit_Org, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->lineEdit_PG, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->comboBox_T, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::validateInputs);

    connect(ui->majNom, &QLineEdit::textChanged, this, &MainWindow::validateInputsUP);
    connect(ui->majNbrE, &QLineEdit::textChanged, this, &MainWindow::validateInputsUP);
    connect(ui->majOrg, &QLineEdit::textChanged, this, &MainWindow::validateInputsUP);
    connect(ui->majPG, &QLineEdit::textChanged, this, &MainWindow::validateInputsUP);
    connect(ui->majT, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::validateInputsUP);

    loadChampData();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ajoutChampB_clicked(){

    validateInputs();

    int nbr_eq = ui->lineEdit_nbrE->text().toInt();
    QString type = ui->comboBox_T->currentText();
    QString nom = ui->lineEdit_nom->text();
    int pool = ui->lineEdit_PG->text().toInt();
    QString Org = ui->lineEdit_Org->text();


    Championnats champ(nbr_eq, type, nom, Org, pool);


    bool success = champ.ajoutChamp();

    // Check for success and show a message box
    if (success) {
        QMessageBox::information(this, "Success", "Champs added successfully!");
        freeallfields();
        loadChampData();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the Champs: " + QSqlDatabase::database().lastError().text());
    }
}

//void MainWindow::freespaces(){}

void MainWindow::loadForUpdate(int ind) {

    QSqlQuery query;
    int id = ui->tableWidget->item(ind, 0)->text().toInt();

    query.prepare("SELECT NBR_EQUIPE, TYPE, NOM, ORGANIZATEUR, POOL_GAINS FROM CHAMPIONNATS WHERE ID_CHAMP = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Fill the update form but DO NOT update the database yet
        ui->majNbrE->setText(query.value(0).toString());
        QString typeValue = query.value(1).toString();
        int index = ui->majT->findText(typeValue); // Find the corresponding index
        if (index != -1) {
            ui->majT->setCurrentIndex(index); // Set the selected value
        }
        ui->majNom->setText(query.value(2).toString());
        ui->majOrg->setText(query.value(3).toString());
        ui->majPG->setText(query.value(4).toString());

        // Store ID for later update
        ui->IDchamphidden->setText(QString::number(id));
        ui->tabWidget->setCurrentIndex(3);
    } else {
        qDebug() << "Error fetching championnat: " << query.lastError().text();
    }
}
void MainWindow::modifChampB_clicked(){

    validateInputsUP();

    int id = ui->IDchamphidden->text().toInt();
    int nbrEquipe = ui->majNbrE->text().toInt();
    QString type = ui->comboBox_T->currentText();
    QString nom = ui->majNom->text();
    QString org = ui->majOrg->text();
    int pool = ui->majPG->text().toInt();

    Championnats champ(nbrEquipe, type, nom, org, pool);

    champ.setIdChamp(id);

    bool success = champ.saveUpdates();

    if (success) {
        QMessageBox::information(nullptr, "Updated", "Championship updated successfully.");
        freeallfields();
        loadChampData();
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to update championship.");
    }
}


void MainWindow::SuppChamp(int ind){


    QSqlQuery query;

    int champID = ui->tableWidget->item(ind, 0)->text().toInt();

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this record?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    // Delete from database
    query.prepare("DELETE FROM CHAMPIONNATS WHERE ID_CHAMP = :id");
    query.bindValue(":id", champID);

    if (query.exec()) {
        QMessageBox::information(this, "Deleted", "Champ deleted successfully.");
        ui->tableWidget->removeRow(ind);
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete champ.");
        qDebug() << "Delete error: " << query.lastError().text();
    }
}

void MainWindow::loadChampData() {

    QSqlQuery query;
    query.prepare("SELECT * FROM CHAMPIONNATS");

    if (!query.exec()) {
        qDebug() << "Error retrieving data: " << query.lastError().text();
        return;
    }


    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);  // Insert a new row

        // Fill columns
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));

        QPushButton* deleteButton = new QPushButton("Delete");
        QPushButton* updateButton = new QPushButton("update");

        ui->tableWidget->setCellWidget(row, 6, deleteButton);
        ui->tableWidget->setCellWidget(row, 7, updateButton);

        connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
        SuppChamp(row);
        });
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
        loadForUpdate(row);
        });

        row++;
    }
}


void MainWindow::validateInputs() {
    bool allValid = true;


    QRegularExpression alphaRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");


    if (ui->lineEdit_nom->text().trimmed().isEmpty() ||
        !alphaRegex.match(ui->lineEdit_nom->text().trimmed()).hasMatch() || ui->lineEdit_nom->text().length() < 7) {
        ui->err_nom->setText("Nom doit etre alphabetique de longuer 8 min");
        ui->err_nom->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_nom->setText("valide");
        ui->err_nom->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    if (ui->lineEdit_nbrE->text().trimmed().isEmpty() ||
        ui->lineEdit_nbrE->text().toInt() < 0 || ui->lineEdit_nbrE->text().toInt() > 56 || ui->lineEdit_nbrE->text().toInt() < 8) {
        ui->err_nbrE->setText("le nombre doit etre positive et entre 8 est 56!");
        ui->err_nbrE->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_nbrE->setText("valide");
        ui->err_nbrE->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    if (ui->comboBox_T->currentText() == "--SELECT--") {
        ui->err_T->setText("Choisissez une champ valide!");
        ui->err_T->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; "
            );
        allValid = false;
    } else {
        ui->err_T->setText("valide");
        ui->err_T->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; "
            );
    }

    if (ui->lineEdit_Org->text().trimmed().isEmpty() ||
        !alphaRegex.match(ui->lineEdit_Org->text().trimmed()).hasMatch() || ui->lineEdit_Org->text().length() < 7) {
        ui->err_Org->setText("le nom d'Org doit etre alphabetique !");
        ui->err_Org->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_Org->setText("valide");
        ui->err_Org->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }

    if (ui->lineEdit_PG->text().trimmed().isEmpty() ||
        ui->lineEdit_PG->text().toInt() < 0 || (ui->lineEdit_PG->text().toInt() / ui->lineEdit_nbrE->text().toInt()) >= 2000) {
        ui->err_pool->setText("le montant est issufisant (min 2000 par équipe)!");
        ui->err_pool->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_pool->setText("valide");
        ui->err_pool->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    ui->PBA->setEnabled(allValid);
}

void MainWindow::validateInputsUP() {
    bool allValid = true;


    QRegularExpression alphaRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");


    if (ui->majNom->text().trimmed().isEmpty() ||
        !alphaRegex.match(ui->majNom->text().trimmed()).hasMatch() || ui->majNom->text().length() < 7) {
        ui->err_nom_2->setText("Nom doit etre alphabetique de longuer 8 min");
        ui->err_nom_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_nom_2->setText("valide");
        ui->err_nom_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    if (ui->majNbrE->text().trimmed().isEmpty() ||
        ui->majNbrE->text().toInt() < 0 || ui->majNbrE->text().toInt() > 56 || ui->majNbrE->text().toInt() < 8) {
        ui->err_nbrE_2->setText("le nombre doit etre positive et entre 8 est 56!");
        ui->err_nbrE_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_nbrE_2->setText("valide");
        ui->err_nbrE_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    if (ui->majT->currentText() == "--SELECT--") {
        ui->err_T_2->setText("Choisissez une champ valide!");
        ui->err_T_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; "
            );
        allValid = false;
    } else {
        ui->err_T_2->setText("valide");
        ui->err_T_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; "
            );
    }

    if (ui->majOrg->text().trimmed().isEmpty() ||
        !alphaRegex.match(ui->majOrg->text().trimmed()).hasMatch() || ui->majOrg->text().length() < 7) {
        ui->err_Org_2->setText("le nom d'Org doit etre alphabetique !");
        ui->err_Org_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_Org_2->setText("valide");
        ui->err_Org_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }

    if (ui->majPG->text().trimmed().isEmpty() ||
        ui->majPG->text().toInt() < 0 || (ui->majPG->text().toInt() / ui->majNbrE->text().toInt()) <= 2000) {
        ui->err_pool_2->setText("le montant est issufisant (min 2000 par équipe)!");
        ui->err_pool_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_pool_2->setText("valide");
        ui->err_pool_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    ui->majbutton->setEnabled(allValid);
}

void MainWindow::freeallfields(){
    ui->lineEdit_nbrE->setText("");
    ui->lineEdit_nom->setText("");
    ui->lineEdit_Org->setText("");
    ui->lineEdit_PG->setText("");
    ui->comboBox_T->setCurrentIndex(0);

    ui->majNbrE->setText("");
    ui->majNom->setText("");
    ui->majOrg->setText("");
    ui->majPG->setText("");
    ui->majT->setCurrentIndex(0);
}
