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

    applyDesign(ui);
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


void MainWindow::on_addStadiumbutton_clicked()
{
    // Récupérer les données depuis l'interface utilisateur
    QString name = ui->lineEdit_nomA->text();
    QString location = ui->lineEdit_lieuA->text();
    QString capacityStr = ui->lineEdit_capaciteA->text(); // Capacité saisie par l'utilisateur
    QString ticketsSoldStr = ui->lineEdit_ticketsA->text();
    QDate dateCreation = ui->dateEdit_creationA->date();

    // Validation : Nom doit contenir uniquement des lettres
    QRegularExpression regexName("^[a-zA-Z\\s]+$");
    if (!regexName.match(name).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "Le nom du stade doit contenir uniquement des lettres et des espaces.");
        return; // Arrêter le processus d'ajout
    }

    // Validation : Capacite doit être un nombre
    QRegularExpression regexNumber("^[0-9]+$"); // Expression régulière pour vérifier uniquement les chiffres
    if (!regexNumber.match(capacityStr).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre valide.");
        return; // Arrêter le processus d'ajout
    }

    // Conversion de capacité et validation pour un entier positif
    int capacity = capacityStr.toInt();
    if (capacity <= 0) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre supérieur à zéro.");
        return;
    }

    // Validation : Nombre de tickets vendus doit être un nombre entier positif ou zéro
    bool ticketsOk;
    int ticketsSold = ticketsSoldStr.toInt(&ticketsOk);
    if (!ticketsOk || ticketsSold < 0) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus doit être un nombre entier positif ou zéro.");
        return; // Arrêter le processus d'ajout
    }
    if (ticketsSold > capacity) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus ne peut pas dépasser la capacité.");
        return; // Arrêter le processus d'ajout
    }

    // Créer un objet Stade avec les données validées
    Stade newStade(name, location, capacity, ticketsSold, dateCreation);

    // Appeler la méthode ajouter
    bool success = newStade.ajouter();

    // Vérifier si l'ajout a réussi
    if (success) {
        QMessageBox::information(this, "Succès", "Stade ajouté avec succès !");
        ui->tableView->setModel(Stade().afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du stade : " + QSqlDatabase::database().lastError().text());
    }
}
void MainWindow::on_pushbuttonmodifier_clicked()
{
    // Récupérer l'ID du stade à modifier
    int id = ui->lineEdit_ID2->text().toInt();

    // Vérifier si l'ID est valide
    if (id <= 0) {
        QMessageBox::critical(this, "Erreur de validation", "Veuillez saisir un ID valide pour le stade.");
        return;
    }

    // Vérifier si l'ID existe dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Stades WHERE ID_stade = :id");
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec lors de la vérification de l'ID : " + checkQuery.lastError().text());
        return;
    }

    // Lire le résultat
    checkQuery.next();
    if (checkQuery.value(0).toInt() == 0) {
        QMessageBox::critical(this, "Erreur", "L'ID spécifié n'existe pas dans la base de données.");
        return; // Arrêter le processus si l'ID n'existe pas
    }

    // Récupérer les nouvelles données depuis les champs de saisie
    QString name = ui->lineEdit_nom2->text();
    QString location = ui->lineEdit_lieu2->text();
    QString capacityStr = ui->lineEdit_capacite2->text();
    QString ticketsSoldStr = ui->lineEdit_tickets2->text();
    QDate dateCreation = ui->dateEdit_creation2->date();

    // Validation : Nom doit contenir uniquement des lettres
    QRegularExpression regexName("^[a-zA-Z\\s]+$");
    if (!regexName.match(name).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "Le nom du stade doit contenir uniquement des lettres et des espaces.");
        return; // Arrêter le processus de modification
    }
    QRegularExpression regexLocation("^[a-zA-Z\\s]+$");
    if (!regexLocation.match(location).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "Le lieu du stade doit contenir uniquement des lettres et des espaces.");
        return; // Arrêter le processus de modification
    }

    // Validation : Capacité doit être un nombre
    QRegularExpression regexNumber("^[0-9]+$");
    if (!regexNumber.match(capacityStr).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre valide.");
        return;
    }

    // Conversion de capacité et validation pour un entier positif
    int capacity = capacityStr.toInt();
    if (capacity <= 0) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre supérieur à zéro.");
        return;
    }

    // Validation : Nombre de tickets vendus doit être un nombre entier positif ou zéro
    bool ticketsOk;
    int ticketsSold = ticketsSoldStr.toInt(&ticketsOk);
    if (!ticketsOk || ticketsSold < 0) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus doit être un nombre entier positif ou zéro.");
        return; // Arrêter le processus de modification
    }

    // Nouvelle validation : Nombre de tickets vendus <= Capacité
    if (ticketsSold > capacity) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus ne peut pas dépasser la capacité.");
        return; // Arrêter le processus de modification
    }

    // Créer un objet Stade avec les nouvelles données validées
    Stade stade;
    stade.setNom(name);
    stade.setLieu(location);
    stade.setCapacite(capacity);
    stade.setNbrTicketsVd(ticketsSold);
    stade.setDateCreation(dateCreation);

    // Appeler une fonction pour mettre à jour l'entrée dans la base de données
    QSqlQuery query;
    query.prepare("UPDATE Stades SET nom = :nom, lieu = :lieu, capacite = :capacite, nbr_tickets_vd = :tickets, date_creation = :date "
                  "WHERE ID_stade = :id");
    query.bindValue(":nom", name);
    query.bindValue(":lieu", location);
    query.bindValue(":capacite", capacity);
    query.bindValue(":tickets", ticketsSold);
    query.bindValue(":date", dateCreation);
    query.bindValue(":id", id);

    // Vérifier si la requête de mise à jour réussit
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Le stade a été mis à jour avec succès !");
        ui->tableView->setModel(stade.afficher()); // Mettre à jour la vue
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour : " + query.lastError().text());
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    // Récupérer l'ID depuis le champ de texte
    int id = ui->lineEdit_ID->text().toInt();

    // Appeler la méthode supprimer de la classe Stade
    Stade stade;
    bool test = stade.supprimer(id);

    // Vérifier si la suppression a réussi
    if (test) {
        QMessageBox::information(this, QObject::tr("Succès"),
                                 QObject::tr("Suppression effectuée"),

                                 QMessageBox::Cancel);
        // Mettre à jour la vue
        ui->tableView->setModel(stade.afficher());

}
}

