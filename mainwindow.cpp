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

    QString name = ui->lineEdit_nomA->text();
    QString location = ui->lineEdit_lieuA->text();
    QString capacityStr = ui->lineEdit_capaciteA->text();
    QString ticketsSoldStr = ui->lineEdit_ticketsA->text();
    QDate dateCreation = ui->dateEdit_creationA->date();


    QRegularExpression regexName("^[a-zA-Z\\s]+$");
    if (!regexName.match(name).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "Le nom du stade doit contenir uniquement des lettres et des espaces.");
        return;
    }


    QRegularExpression regexNumber("^[0-9]+$");
    if (!regexNumber.match(capacityStr).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre valide.");
        return;
    }


    int capacity = capacityStr.toInt();
    if (capacity <= 0) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre supérieur à zéro.");
        return;
    }


    bool ticketsOk;
    int ticketsSold = ticketsSoldStr.toInt(&ticketsOk);
    if (!ticketsOk || ticketsSold < 0) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus doit être un nombre entier positif ou zéro.");
        return;
    }
    if (ticketsSold > capacity) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus ne peut pas dépasser la capacité.");
        return;
    }


    Stade newStade(name, location, capacity, ticketsSold, dateCreation);


    bool success = newStade.ajouter();


    if (success) {
        QMessageBox::information(this, "Succès", "Stade ajouté avec succès !");
        ui->tableView->setModel(Stade().afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du stade : " + QSqlDatabase::database().lastError().text());
    }
}
void MainWindow::on_pushbuttonmodifieR_clicked()
{

    int id = ui->lineEdit_ID2->text().toInt();


    if (id <= 0) {
        QMessageBox::critical(this, "Erreur de validation", "Veuillez saisir un ID valide pour le stade.");
        return;
    }


    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Stades WHERE ID_stade = :id");
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec lors de la vérification de l'ID : " + checkQuery.lastError().text());
        return;
    }


    checkQuery.next();
    if (checkQuery.value(0).toInt() == 0) {
        QMessageBox::critical(this, "Erreur", "L'ID spécifié n'existe pas dans la base de données.");
        return;
    }


    QString name = ui->lineEdit_nom2->text();
    QString location = ui->lineEdit_lieu2->text();
    QString capacityStr = ui->lineEdit_capacite2->text();
    QString ticketsSoldStr = ui->lineEdit_tickets2->text();
    QDate dateCreation = ui->dateEdit_creation2->date();


    QRegularExpression regexName("^[a-zA-Z\\s]+$");
    if (!regexName.match(name).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "Le nom du stade doit contenir uniquement des lettres et des espaces.");
        return;
    }
    QRegularExpression regexLocation("^[a-zA-Z\\s]+$");
    if (!regexLocation.match(location).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "Le lieu du stade doit contenir uniquement des lettres et des espaces.");
        return;
    }


    QRegularExpression regexNumber("^[0-9]+$");
    if (!regexNumber.match(capacityStr).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre valide.");
        return;
    }


    int capacity = capacityStr.toInt();
    if (capacity <= 0) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre supérieur à zéro.");
        return;
    }


    bool ticketsOk;
    int ticketsSold = ticketsSoldStr.toInt(&ticketsOk);
    if (!ticketsOk || ticketsSold < 0) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus doit être un nombre entier positif ou zéro.");
        return;
    }


    if (ticketsSold > capacity) {
        QMessageBox::critical(this, "Erreur de validation", "Le nombre de tickets vendus ne peut pas dépasser la capacité.");
        return;
    }


    Stade stade;
    stade.setNom(name);
    stade.setLieu(location);
    stade.setCapacite(capacity);
    stade.setNbrTicketsVd(ticketsSold);
    stade.setDateCreation(dateCreation);


    QSqlQuery query;
    query.prepare("UPDATE Stades SET nom = :nom, lieu = :lieu, capacite = :capacite, nbr_tickets_vd = :tickets, date_creation = :date "
                  "WHERE ID_stade = :id");
    query.bindValue(":nom", name);
    query.bindValue(":lieu", location);
    query.bindValue(":capacite", capacity);
    query.bindValue(":tickets", ticketsSold);
    query.bindValue(":date", dateCreation);
    query.bindValue(":id", id);


    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Le stade a été mis à jour avec succès !");
        ui->tableView->setModel(stade.afficher()); // Mettre à jour la vue
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour : " + query.lastError().text());
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{

    int id = ui->lineEdit_ID->text().toInt();


    Stade stade;
    bool test = stade.supprimer(id);


    if (test) {
        QMessageBox::information(this, QObject::tr("Succès"),
                                 QObject::tr("Suppression effectuée"),

                                 QMessageBox::Cancel);

        ui->tableView->setModel(stade.afficher());

}
}
void MainWindow::on_pushButton_rechercherNom_clicked()
{
    QString nomRecherche = ui->lineEdit_rechercheNom->text();


    if (nomRecherche.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez saisir un nom pour la recherche.");
        return;
    }


    Stade stade;
    QSqlQueryModel* model = stade.rechercherParNom(nomRecherche);

    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        QMessageBox::information(this, "Résultat", "Aucun stade trouvé avec ce nom.");
    }
}
void MainWindow::on_pushButton_rechercherCapacite_clicked()
{
    int capaciteMin = ui->lineEdit_capaciteMin->text().toInt();
    int capaciteMax = ui->lineEdit_capaciteMax->text().toInt();


    if (capaciteMin <= 0 || capaciteMax <= 0) {
        QMessageBox::critical(this, "Erreur", "Veuillez saisir des valeurs positives pour la capacité.");
        return;
    }
    if (capaciteMin > capaciteMax) {
        QMessageBox::critical(this, "Erreur", "La capacité minimale doit être inférieure ou égale à la capacité maximale.");
        return;
    }


    Stade stade;
    QSqlQueryModel* model = stade.rechercherParCapacite(capaciteMin, capaciteMax);

    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        QMessageBox::information(this, "Résultat", "Aucun stade trouvé dans cette plage de capacité.");
    }
}
void MainWindow::on_comboBoxTri_currentIndexChanged(int index)
{
    Stade stade;
    QSqlQueryModel* model;


    switch (index) {
    case 0:
        model = stade.trier("capacite", "ASC");
        break;
    case 1:
        model = stade.trier("capacite", "DESC");
        break;
    case 2:
        model = stade.trier("nbr_tickets_vd", "ASC");
        break;
    case 3:
        model = stade.trier("nbr_tickets_vd", "DESC");
        break;
    default:
        return;
    }


    ui->tableView->setModel(model);
}



