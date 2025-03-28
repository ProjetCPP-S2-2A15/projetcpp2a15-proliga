#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "stades.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QAxObject>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(Stade().afficher());
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
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


void MainWindow::on_tableView_clicked(const QModelIndex &index) {
    int id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();
    Stade stadeInstance;
    Stade stade = stadeInstance.getStade(id);
    ui->lineEdit_nom2->setText(stade.getNom());
    ui->lineEdit_lieu2->setText(stade.getLieu());
    ui->lineEdit_capacite2->setText(QString::number(stade.getCapacite()));
    ui->lineEdit_tickets2->setText(QString::number(stade.getNbrTicketsVd()));
    ui->dateEdit_creation2->setDate(stade.getDateCreation());
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

    QRegularExpression regexlocation("^[a-zA-Z\\s]+$");
    if (!regexlocation.match(location).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "La location du stade doit contenir uniquement des lettres et des espaces.");
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
    if (dateCreation > QDate::currentDate()) {
        QMessageBox::critical(this, "Erreur de validation", "La date de création ne peut pas dépasser la date actuelle.");
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

    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner un stade à modifier.");
        return;
    }

    int row = selection.first().row();
    int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();

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
    QRegularExpression regexNumber("^[0-9]+$");
    if (!regexNumber.match(capacityStr).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "La capacité doit être un nombre valide.");
        return;
    }
    QRegularExpression regexlocation("^[a-zA-Z\\s]+$");
    if (!regexlocation.match(location).hasMatch()) {
        QMessageBox::critical(this, "Erreur de validation", "La location du stade doit contenir uniquement des lettres et des espaces.");
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
    if (dateCreation > QDate::currentDate()) {
        QMessageBox::critical(this, "Erreur de validation", "La date de création ne peut pas dépasser la date actuelle.");
        return;
    }

    Stade stade(name, location, capacity, ticketsSold, dateCreation);


    bool success = stade.modifier(id);

    if (success) {
        QMessageBox::information(this, "Succès", "Stade modifié avec succès !");
        ui->tableView->setModel(Stade().afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du stade.");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{

    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner un stade à supprimer.");
        return;
    }

    int row = selection.first().row();
    int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();

    Stade stadeInstance;
    if(!stadeInstance.idExiste(id)){
        QMessageBox::critical(this, "Erreur", "L'ID du stade n'existe pas.");
        return;
    }

    QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Confirmation de suppression",
                                                                     "Êtes-vous sûr de vouloir supprimer ce stade ?",
                                                                     QMessageBox::Yes | QMessageBox::No);

    if (confirmation == QMessageBox::Yes) {
        bool test = stadeInstance.supprimer(id);

        if (test) {
            QMessageBox::information(this, "Succès", "Suppression effectuée avec succès.", QMessageBox::Ok);
            ui->tableView->setModel(stadeInstance.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du stade.");
        }
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

void MainWindow::on_pushButton_genererPDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "*.pdf");
    if (fileName.isEmpty())
        return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    int pageWidth = pdfWriter.width();
    int xStart;
    int yStart = 200;
    int rowHeight = 200;
    int colWidth = 400;
    int tableWidth = 5 * colWidth;

    xStart = (pageWidth - tableWidth) / 2;

    QFont titleFont = painter.font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(Qt::green);

    QString title = "Liste des Stades";
    int titleWidth = painter.fontMetrics().horizontalAdvance(title);
    int titleX = (pageWidth - titleWidth) / 2;
    painter.drawText(titleX, 100, title);

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    painter.setPen(Qt::black);


    painter.drawRect(xStart, yStart, tableWidth, rowHeight);
    painter.drawText(xStart + 50, yStart + 100, "Nom");
    painter.drawText(xStart + colWidth + 50, yStart + 100, "Lieu");
    painter.drawText(xStart + 2 * colWidth + 50, yStart + 100, "Capacité");
    painter.drawText(xStart + 3 * colWidth + 50, yStart + 100, "Tickets Vendus");
    painter.drawText(xStart + 4 * colWidth + 50, yStart + 100, "Date de Création");

    yStart += rowHeight;


    QSqlQuery query("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades");
    while (query.next()) {
        painter.drawRect(xStart, yStart, tableWidth, rowHeight);


        QString nom = query.value("nom").toString();
        QString lieu = query.value("lieu").toString();
        int capacite = query.value("capacite").toInt();
        int tickets = query.value("nbr_tickets_vd").toInt();
        QDate dateCreation = query.value("date_creation").toDate();


        painter.drawText(xStart + 50, yStart + 100, nom);
        painter.drawText(xStart + colWidth + 50, yStart + 100, lieu);
        painter.drawText(xStart + 2 * colWidth + 50, yStart + 100, QString::number(capacite));
        painter.drawText(xStart + 3 * colWidth + 50, yStart + 100, QString::number(tickets));
        painter.drawText(xStart + 4 * colWidth + 50, yStart + 100, dateCreation.toString("dd/MM/yyyy"));

        yStart += rowHeight;


        if (yStart > pdfWriter.height() - 100) {
            pdfWriter.newPage();
            yStart = 200;
        }
    }


    for (int i = 0; i <= 5; i++) {
        painter.drawLine(xStart + i * colWidth, 200, xStart + i * colWidth, yStart);
    }

    painter.end();

    QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès !");
}

void MainWindow::on_pushButton_genererExcel_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le fichier Excel", "", "*.xlsx");
    if (fileName.isEmpty())
        return;

    QAxObject *excel = new QAxObject("Excel.Application", this);
    if (!excel) {
        QMessageBox::critical(this, "Erreur", "Impossible d'initialiser Excel.");
        return;
    }

    excel->dynamicCall("SetVisible(bool)", false); // Keep Excel hidden
    QAxObject *workbook = excel->querySubObject("Workbooks")->querySubObject("Add()");
    QAxObject *sheet = workbook->querySubObject("Sheets(int)", 1);
    sheet->dynamicCall("SetName(const QString&)", "Liste des Stades");

    // Add column headers
    sheet->querySubObject("Cells(int,int)", 1, 1)->setProperty("Value", "Nom");
    sheet->querySubObject("Cells(int,int)", 1, 2)->setProperty("Value", "Lieu");
    sheet->querySubObject("Cells(int,int)", 1, 3)->setProperty("Value", "Capacité");
    sheet->querySubObject("Cells(int,int)", 1, 4)->setProperty("Value", "Tickets Vendus");
    sheet->querySubObject("Cells(int,int)", 1, 5)->setProperty("Value", "Date de Création");

    QSqlQuery query("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades");
    int row = 2; // Start from the second row for data
    while (query.next()) {
        sheet->querySubObject("Cells(int,int)", row, 1)->setProperty("Value", query.value("nom").toString());
        sheet->querySubObject("Cells(int,int)", row, 2)->setProperty("Value", query.value("lieu").toString());
        sheet->querySubObject("Cells(int,int)", row, 3)->setProperty("Value", query.value("capacite").toInt());
        sheet->querySubObject("Cells(int,int)", row, 4)->setProperty("Value", query.value("nbr_tickets_vd").toInt());
        sheet->querySubObject("Cells(int,int)", row, 5)->setProperty("Value", query.value("date_creation").toDate().toString("dd/MM/yyyy"));
        row++;
    }

    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete excel;

    QMessageBox::information(this, "Succès", "Le fichier Excel a été généré avec succès !");
}
void MainWindow::on_pushButton_generer_clicked()
{
    QString selectedFormat = ui->comboBox_format->currentText();

    if (selectedFormat == "PDF") {
        on_pushButton_genererPDF_clicked(); // Appelle la fonction pour générer le PDF
    } else if (selectedFormat == "Excel") {
        on_pushButton_genererExcel_clicked(); // Appelle la fonction pour générer l'Excel
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un format valide.");
    }
}

