#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "joueur_utils.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
}

MainWindow::~MainWindow()
{
    delete ui;
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
void MainWindow::on_pushButton_update_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    if (id == 0) {
        QMessageBox::warning(this, "Warning", "Please enter a valid ID.");
        return;
    }

    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    int age = ui->lineEdit_age->value();
    int experience = ui->lineEdit_experience->value();
    QString sexe = ui->comboBox_sexe->currentText();
    QString email = ui->lineEdit_email->text();

    if (A.modifier(id, nom, prenom, age, experience, sexe, email)) {
        QMessageBox::information(this, "Success", "Arbitre modified successfully!");
        A.afficher(ui->tableWidget_arbitres); // Refresh table
    } else {
        QMessageBox::critical(this, "Error", "Failed to modify Arbitre. ID not found.");
    }
}

// ✅ Show button functionality
void MainWindow::on_pushButton_show_clicked()
{
    A.afficher(ui->tableWidget_arbitres);
}
