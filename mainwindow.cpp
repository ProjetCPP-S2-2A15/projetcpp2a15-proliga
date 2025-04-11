#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "joueur_utils.h"
#include <QMessageBox>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QStandardPaths>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , A(this)
{
    ui->setupUi(this);

    // Apply design styles
    applyDesign(ui);

    // Show data at startup
    A.afficher(ui->tableWidget_arbitres);

    // Change Widget connections
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
    connect(ui->pushButton_tri, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_tri_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::remplirFormulaireArbitre(QString nom, QString prenom, int age, int experience, QString sexe, QString email) {
    ui->lineEdit_nom->setText(nom);
    ui->lineEdit_prenom->setText(prenom);
    ui->lineEdit_age->setValue(age);
    ui->lineEdit_experience->setValue(experience);
    ui->comboBox_sexe->setCurrentText(sexe);
    ui->lineEdit_email->setText(email);
}

void MainWindow::setCurrentArbitreId(int id) {
    currentArbitreId = id;
}

// ✅ Add button functionality (ID is auto-incremented, so do not set it)
void MainWindow::on_pushButton_add_clicked()
{
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    int age = ui->lineEdit_age->value();  // QSpinBox, use value() instead of text().toInt()
    int experience = ui->lineEdit_experience->value();
    QString sexe = ui->comboBox_sexe->currentText();
    QString email = ui->lineEdit_email->text();

    // Use constructor without ID (auto-incremented in Oracle)
    Arbitre arbitre(nom, prenom, age, experience, sexe, email);

    if (arbitre.ajouter()) {
        QMessageBox::information(this, "Success", "Arbitre added successfully!");
        A.afficher(ui->tableWidget_arbitres); // Refresh table
    } else {
        qDebug() << "❌ SQL Insert Error: Check your database constraints!";
        QMessageBox::critical(this, "Error", "Failed to add Arbitre.");
    }
}



// ✅ Delete button functionality
void MainWindow::on_pushButton_delete_clicked()
{
    int id = ui->lineEdit_id->text().toInt();

    if (id == 0) {
        QMessageBox::warning(this, "Warning", "Please enter a valid ID.");
        return;
    }

    if (A.supprimer(id)) {
        QMessageBox::information(this, "Success", "Arbitre deleted successfully!");
        A.afficher(ui->tableWidget_arbitres); // Refresh table
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete Arbitre. ID not found.");
    }
}

// ✅ Update button functionality
void MainWindow::on_pushButton_update_clicked() {
    // Get the ID from the form
    int id = ui->lineEdit_id->text().toInt();

    if (id == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun arbitre sélectionné !");
        return;
    }

    // Get the updated values
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    int age = ui->lineEdit_age->value();
    int experience = ui->lineEdit_experience->value();
    QString sexe = ui->comboBox_sexe->currentText();
    QString email = ui->lineEdit_email->text();

    // Call the modifier function
    if (A.modifier(id, nom, prenom, age, experience, sexe, email)) {
        QMessageBox::information(this, "Succès", "Arbitre modifié avec succès !");
        A.afficher(ui->tableWidget_arbitres);  // Refresh table
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}


// ✅ Show button functionality
void MainWindow::on_pushButton_show_clicked()
{
    A.afficher(ui->tableWidget_arbitres);

}


void MainWindow::on_pushButton_exporter_clicked()
{
    // 1. Get reference to the table widget
    QTableWidget *table = ui->tableWidget_arbitres; // Use your actual table widget name

    // 2. Verify table has data
    if (table->rowCount() == 0) {
        QMessageBox::warning(this, "Avertissement", "Aucune donnée à exporter!");
        return;
    }

    // 3. Call the export function directly on the A instance
    A.exporterEnPDF(table); // Note the dot (.) operator for direct member access

    // Optional: Show confirmation message
    QMessageBox::information(this, "Succès", "Export PDF lancé avec succès");
}

void MainWindow::on_pushButton_tri_clicked()
{
    // Get reference to your table
    QTableWidget *table = ui->tableWidget_arbitres;

    // Sort by name (column 1) in ascending order
    table->sortItems(1, Qt::AscendingOrder);  // 1 = name column index

    // Optional: Visual feedback
    ui->pushButton_tri->setText("Trié (A-Z)");
    QTimer::singleShot(1500, [this]() {
        ui->pushButton_tri->setText("Tri");
    });
}


