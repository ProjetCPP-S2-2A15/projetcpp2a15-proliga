#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "design.h"
#include <QPropertyAnimation>
#include <utility>
#include "changeWidget.h"
#include "equipe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression> // Assurez-vous d'inclure cette bibliothèque

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

    connect(ui->nome, &QLineEdit::textChanged, this, &MainWindow::validateName);
    connect(ui->Pays, &QLineEdit::textChanged, this, &MainWindow::validateCountry);
    connect(ui->coach, &QLineEdit::textChanged, this, &MainWindow::validateCoach);
    connect(ui->nbmarquee, &QLineEdit::textChanged, this, &MainWindow::validateGoals);
    connect(ui->nbj, &QSpinBox::textChanged, this, &MainWindow::validatePlayers);

}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QRegularExpression> // Assurez-void vous d'inclure cette bibliothèque

void MainWindow::refreshTable()
{
    // Check if the input fields are empty
    if (ui->nome->text().isEmpty() || ui->Pays->text().isEmpty() ||
        ui->coach->text().isEmpty() || ui->nbj->text().isEmpty() ||
        ui->nbmarquee->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields before refreshing the table.");
        return; // Exit the function if any field is empty
    }

    // Create a new model
    QSqlQueryModel *model = new QSqlQueryModel(this); // Set 'this' as the parent for automatic deletion

    // Create a query to select all data from the equipe table
    QSqlQuery query;
    query.prepare("SELECT NOMEQUIPE, NOMVILLE, NOMENTRAINEUR, NOMBREJOUEURS, TYPE, NOMBREBUTSMARQUES, DATEDEBUTCONTRAT, DATEFINCONTRAT FROM equipe");

    if (query.exec()) {
        model->setQuery(std::move(query)); // Use std::move to pass the query
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom Equipe"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Ville"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom Entraineur"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Nombre Joueurs"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Nombre Buts Marqués"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Debut Contrat"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Fin Contrat"));

        // Set the model to the QTableView
        ui->programme->setModel(model);
    } else {
        // Handle query error if there is one
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données : " + query.lastError().text());
    }
}

    // Connecter chaque QLineEdit à sa fonction de validation


bool MainWindow::validateName() {
    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->nome->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid name. It should start with a capital letter and contain only letters.");
        ui->nome->setFocus();
        return false;
    }
}

bool MainWindow::validateCountry() {
    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->Pays->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid country name. It should start with a capital letter and contain only letters.");
        ui->Pays->setFocus();
        return false;

    }
}

bool MainWindow::validateCoach() {
    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->coach->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid coach name. It should start with a capital letter and contain only letters.");
        ui->coach->setFocus();
        return false;

    }
}

bool MainWindow::validateGoals() {
    QRegularExpression numberRegex("^[0-9]+$");
    if (!numberRegex.match(ui->nbmarquee->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid number of goals. It should contain only digits.");
        ui->nbmarquee->setFocus();
        return false;

    } else if (ui->nbmarquee->text().toInt() < 0) {
        QMessageBox::warning(this, "Input Error", "The number of goals scored must be non-negative.");
        ui->nbmarquee->setFocus();
        return false;

    }
}

bool MainWindow::validatePlayers() {
    QRegularExpression numberRegex("^[0-9]+$");
    if (!numberRegex.match(ui->nbj->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid number of players. It should contain only digits.");
        ui->nbj->setFocus();
        return false;

    } else if (ui->nbj->text().toInt() < 11) {
        QMessageBox::warning(this, "Input Error", "The number of players must be at least 11.");
        ui->nbj->setFocus();
        return false;

    }
}

void MainWindow::checkInput() {
    // Vérifier que tous les champs sont valides avant d'activer le bouton "Ajouter"
    bool isValid = validateName() && validateCountry() && validateCoach() && validateGoals() && validatePlayers();
    ui->Ajouter->setEnabled(isValid);
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
        refreshTable(); // Call refreshTable to update the displayed data
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the EQUIPE: " + QSqlDatabase::database().lastError().text());
    }
}
