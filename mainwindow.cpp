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

    A.setSmtpCredentials("smtp.gmail.com", 465,
                         "heditrabelsi412@gmail.com",
                         "kton jsoh jtmh blrf");

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
    connect(ui->pushButton_stats, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_stats_clicked);
    connect(ui->pushButton_submit, &QPushButton::clicked,
            this, [this]() { A.envoyerConfirmationsArbitres(); });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::remplirFormulaireArbitre(QString nom, QString prenom, int age, int experience, QString sexe, QString email, QString telephone) {
    ui->lineEdit_nom->setText(nom);
    ui->lineEdit_prenom->setText(prenom);
    ui->lineEdit_age->setValue(age);
    ui->lineEdit_experience->setValue(experience);
    ui->comboBox_sexe->setCurrentText(sexe);
    ui->lineEdit_email->setText(email);
    ui->lineEdit_telephone->setText(telephone);
}

void MainWindow::setCurrentArbitreId(int id) {
    currentArbitreId = id;
}

void MainWindow::on_pushButton_add_clicked()
{
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    int age = ui->lineEdit_age->value();
    int experience = ui->lineEdit_experience->value();
    QString sexe = ui->comboBox_sexe->currentText();
    QString email = ui->lineEdit_email->text();
    QString telephone = ui->lineEdit_telephone->text();

    // Use constructor with telephone
    Arbitre arbitre(nom, prenom, age, experience, sexe, email, telephone, nullptr);

    if (arbitre.ajouter()) {
        QMessageBox::information(this, "Success", "Arbitre added successfully!");
        A.afficher(ui->tableWidget_arbitres); // Refresh table
    } else {
        QMessageBox::critical(this, "Error", "Failed to add Arbitre.");
    }
}

void MainWindow::on_pushButton_delete_clicked()
{
    int id = ui->lineEdit_id->text().toInt();

    if (id == 0) {
        QMessageBox::warning(this, "Warning", "Please enter a valid ID.");
        return;
    }

    if (A.supprimer(id)) {
        QMessageBox::information(this, "Success", "Arbitre deleted successfully!");
        A.afficher(ui->tableWidget_arbitres);
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete Arbitre. ID not found.");
    }
}

void MainWindow::on_pushButton_update_clicked() {
    int id = ui->lineEdit_id->text().toInt();

    if (id == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun arbitre sélectionné !");
        return;
    }

    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    int age = ui->lineEdit_age->value();
    int experience = ui->lineEdit_experience->value();
    QString sexe = ui->comboBox_sexe->currentText();
    QString email = ui->lineEdit_email->text();
    QString telephone = ui->lineEdit_telephone->text();

    // Call modifier with telephone
    if (A.modifier(id, nom, prenom, age, experience, sexe, email, telephone)) {
        QMessageBox::information(this, "Succès", "Arbitre modifié avec succès !");
        A.afficher(ui->tableWidget_arbitres);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

void MainWindow::on_pushButton_show_clicked()
{
    A.afficher(ui->tableWidget_arbitres);
}

void MainWindow::on_pushButton_exporter_clicked()
{
    QTableWidget *table = ui->tableWidget_arbitres;

    if (table->rowCount() == 0) {
        QMessageBox::warning(this, "Avertissement", "Aucune donnée à exporter!");
        return;
    }

    A.exporterEnPDF(table);
    QMessageBox::information(this, "Succès", "Export PDF lancé avec succès");
}

void MainWindow::on_pushButton_tri_clicked()
{
    QTableWidget *table = ui->tableWidget_arbitres;
    table->sortItems(1, Qt::AscendingOrder);
    ui->pushButton_tri->setText("Trié (A-Z)");
    QTimer::singleShot(1500, [this]() {
        ui->pushButton_tri->setText("Tri");
    });
}

void MainWindow::on_pushButton_stats_clicked() {
    QString statsHtml = A.getExperienceStats();

    QMessageBox statsBox(this);
    statsBox.setWindowTitle("Referee Statistics");
    statsBox.setTextFormat(Qt::RichText);
    statsBox.setText(statsHtml);
    statsBox.setIconPixmap(QPixmap(":/images/stats_icon.png").scaled(64, 64)); // Optional icon
    statsBox.setStyleSheet("QLabel{min-width: 300px; min-height: 200px;}");
    statsBox.exec();
}
