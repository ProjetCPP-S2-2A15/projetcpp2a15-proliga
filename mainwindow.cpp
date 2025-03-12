#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "design.h"
#include <QPropertyAnimation>

#include "changeWidget.h"
#include "equipe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include <QInputDialog>
#include <QWidget>
#include "mainwindow.h"
#include "loginwindow.h" // Include the chatbot widget header



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->programme->setModel(Eq->loadequipeData());




    // Apply design
    applyDesign(ui);

    // Create and add the ChatBotWidget to widget_8

    // Ensure widget_8 has a layout and then add the ChatBotWidget to it

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
    connect(ui->Ajouter, &QPushButton::clicked, this, &MainWindow::Ajouter_clicked);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Ajouter_clicked() {
    QString nomE = ui->nome->text();
    QString nomv = ui->Pays->text();
    QString nomC = ui->coach->text();
    int nbj = ui->nbj->text().toInt();

    // Check which checkbox is checked and set type accordingly
    QString type;
    if (ui->Normal->isChecked()) {
        type = "Normal";
    } else if (ui->Nationnal->isChecked()) {
        type = "National";
    } else {
        QMessageBox::warning(this, "Warning", "Please select a type.");
        return; // Exit if none is selected
    }

    int nbm = ui->nbmarquee->text().toInt();
    QDate dateDebutContrat = ui->dbcnt->date(); // Assuming dbcnt is a QDateEdit widget
    QDate dateFinContrat = ui->fincnt->date();  // Assuming fincnt is a QDateEdit widget

    // Create an Equipe object with the data
    Equipe EQUIPE(nomE, nomv, nomC, nbj, type, nbm, dateDebutContrat, dateFinContrat);
    bool success = EQUIPE.Ajouter();
    if (success) {
        QMessageBox::information(this, "Success", "Equipe added successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the EQUIPE: " + QSqlDatabase::database().lastError().text());
    }
}


