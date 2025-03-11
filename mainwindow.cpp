#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "stades.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(Stade().afficher());
    // Design
    applyDesign(ui);

    // Widget state management
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

// Function to handle "Add Stadium" button click
void MainWindow::on_addStadiumButton_clicked()
{
    // Fetch data from UI elements (ID is no longer needed)
    QString name = ui->lineEdit_nom->text();
    QString location = ui->lineEdit_lieu->text();
    int capacity = ui->lineEdit_capacite->text().toInt();
    int ticketsSold = ui->lineEdit_tickets->text().toInt();
    QDate dateCreation = ui->dateEdit_creation->date();

    // Create a Stade object with the data (no need to pass ID)
    Stade newStade(name, location, capacity, ticketsSold, dateCreation);

    // Call the ajouter function
    bool success = newStade.ajouter();

    // Check for success and show a message box
    if (success) {
        QMessageBox::information(this, "Success", "Stadium added successfully!");
        ui->tableView->setModel(Stade().afficher());
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the stadium: " + QSqlDatabase::database().lastError().text());
    }
}
