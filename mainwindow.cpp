#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "championnats.h"
#include "stades.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QAxObject>
#include <QSqlRecord>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QQuickView>
#include <QQmlContext>
#include <QVariantMap>
#include <QQuickView>
#include <QWidget>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedChamp("")
{
    ui->setupUi(this);
    ui->tableView->setModel(Stade().afficher());
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewChamps->setModel(Championnats().afficher());
    ui->tableViewChamps->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewChamps->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewStadeLarge->setModel(Stade().afficherCapaciteSuperieure(40000));
    ui->tableViewStadeLarge->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewStadeLarge->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewConsulter->setModel(afficherConsulter());
    ui->tableViewConsulter->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewConsulter->setSelectionMode(QAbstractItemView::SingleSelection);
    applyDesign(ui);

    QQuickWidget *mapWidget = new QQuickWidget(this);
    mapWidget->setObjectName("mapWidget");
    mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout_5->addWidget(mapWidget);
    refreshMap();

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
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refreshMap);

    connect(ui->lineEdit_rechercheNom, &QLineEdit::textChanged, this, &MainWindow::on_rechercheNom_textChanged);
    connect(ui->lineEdit_capaciteMin, &QLineEdit::textChanged, this, &MainWindow::on_capaciteMinMax_textChanged);
    connect(ui->lineEdit_capaciteMax, &QLineEdit::textChanged, this, &MainWindow::on_capaciteMinMax_textChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableView_clicked(const QModelIndex &index) {
    QString name = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toString();
    Stade stadeInstance;
    Stade stade = stadeInstance.getStadeByName(name);
    ui->lineEdit_nom2->setText(stade.getNom());
    ui->lineEdit_lieu2->setText(stade.getLieu());
    ui->lineEdit_capacite2->setText(QString::number(stade.getCapacite()));
    ui->lineEdit_tickets2->setText(QString::number(stade.getNbrTicketsVd()));
    ui->dateEdit_creation2->setDate(stade.getDateCreation());
}

void MainWindow::on_addStadiumbutton_clicked() {
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
    if (dateCreation > QDate::currentDate()) {
        QMessageBox::critical(this, "Erreur de validation", "La date de création ne peut pas dépasser la date actuelle.");
        return;
    }

    Stade stadeInstance;
    if (stadeInstance.idExisteParNom(name)) {
        QMessageBox::critical(this, "Erreur de validation", "Le nom du stade existe déjà. Veuillez choisir un autre nom.");
        return;
    }

    Stade newStade(name, location, capacity, ticketsSold, dateCreation);

    bool success = newStade.ajouter();

    if (success) {
        QMessageBox::information(this, "Succès", "Stade ajouté avec succès !");
        ui->tableView->setModel(Stade().afficher());
        ui->tableViewStadeLarge->setModel(Stade().afficherCapaciteSuperieure(40000));
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du stade : " + QSqlDatabase::database().lastError().text());
    }
}

void MainWindow::on_pushbuttonmodifieR_clicked() {
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner un stade à modifier.");
        return;
    }

    int row = selection.first().row();
    QString name = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();

    QString newName = ui->lineEdit_nom2->text();
    QString location = ui->lineEdit_lieu2->text();
    QString capacityStr = ui->lineEdit_capacite2->text();
    QString ticketsSoldStr = ui->lineEdit_tickets2->text();
    QDate dateCreation = ui->dateEdit_creation2->date();

    QRegularExpression regexName("^[a-zA-Z\\s]+$");
    if (!regexName.match(newName).hasMatch()) {
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
    if (dateCreation > QDate::currentDate()) {
        QMessageBox::critical(this, "Erreur de validation", "La date de création ne peut pas dépasser la date actuelle.");
        return;
    }

    Stade stadeInstance;
    if (newName != name && stadeInstance.idExisteParNom(newName)) {
        QMessageBox::critical(this, "Erreur de validation", "Le nouveau nom du stade existe déjà. Veuillez choisir un autre nom.");
        return;
    }

    Stade stade(newName, location, capacity, ticketsSold, dateCreation);

    bool success = stade.modifierParNom(name);

    if (success) {
        QMessageBox::information(this, "Succès", "Stade modifié avec succès !");
        ui->tableView->setModel(Stade().afficher());
        ui->tableViewStadeLarge->setModel(Stade().afficherCapaciteSuperieure(40000));
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
    QString name = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();

    Stade stadeInstance;
    if (!stadeInstance.idExisteParNom(name)) {
        QMessageBox::critical(this, "Erreur", "Le nom du stade n'existe pas.");
        return;
    }

    QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Confirmation de suppression",
                                                                     "Êtes-vous sûr de vouloir supprimer ce stade ?",
                                                                     QMessageBox::Yes | QMessageBox::No);

    if (confirmation == QMessageBox::Yes) {
        bool test = stadeInstance.supprimer(name);

        if (test) {
            QMessageBox::information(this, "Succès", "Suppression effectuée avec succès.", QMessageBox::Ok);
            ui->tableView->setModel(stadeInstance.afficher());
            ui->tableViewStadeLarge->setModel(Stade().afficherCapaciteSuperieure(40000));
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du stade.");
        }
    }
}

void MainWindow::on_rechercheNom_textChanged(const QString &text)
{
    Stade stade;
    if (text.isEmpty()) {
        ui->tableView->setModel(stade.afficher());
    } else {
        QSqlQueryModel* model = stade.rechercherParNom(text);
        if (model->rowCount() > 0) {
            ui->tableView->setModel(model);
        } else {
            ui->tableView->setModel(model);
            QMessageBox::information(this, "Résultat", "Aucun stade trouvé avec ce nom.");
        }
    }
}

void MainWindow::on_capaciteMinMax_textChanged()
{
    QString minStr = ui->lineEdit_capaciteMin->text();
    QString maxStr = ui->lineEdit_capaciteMax->text();

    bool minOk, maxOk;
    int capaciteMin = minStr.toInt(&minOk);
    int capaciteMax = maxStr.toInt(&maxOk);

    Stade stade;
    if (!minOk || !maxOk || minStr.isEmpty() || maxStr.isEmpty()) {
        ui->tableView->setModel(stade.afficher());
        return;
    }

    if (capaciteMin <= 0 || capaciteMax <= 0) {
        ui->tableView->setModel(stade.afficher());
        QMessageBox::critical(this, "Erreur", "Veuillez saisir des valeurs positives pour la capacité.");
        return;
    }
    if (capaciteMin > capaciteMax) {
        ui->tableView->setModel(stade.afficher());
        QMessageBox::critical(this, "Erreur", "La capacité minimale doit être inférieure ou égale à la capacité maximale.");
        return;
    }

    QSqlQueryModel* model = stade.rechercherParCapacite(capaciteMin, capaciteMax);
    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        ui->tableView->setModel(model);
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

    excel->dynamicCall("SetVisible(bool)", false);
    QAxObject *workbook = excel->querySubObject("Workbooks")->querySubObject("Add()");
    QAxObject *sheet = workbook->querySubObject("Sheets(int)", 1);
    sheet->dynamicCall("SetName(const QString&)", "Liste des Stades");


    sheet->querySubObject("Cells(int,int)", 1, 1)->setProperty("Value", "Nom");
    sheet->querySubObject("Cells(int,int)", 1, 2)->setProperty("Value", "Lieu");
    sheet->querySubObject("Cells(int,int)", 1, 3)->setProperty("Value", "Capacité");
    sheet->querySubObject("Cells(int,int)", 1, 4)->setProperty("Value", "Tickets Vendus");
    sheet->querySubObject("Cells(int,int)", 1, 5)->setProperty("Value", "Date de Création");

    QSqlQuery query("SELECT nom, lieu, capacite, nbr_tickets_vd, date_creation FROM Stades");
    int row = 2;
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
        on_pushButton_genererPDF_clicked();
    } else if (selectedFormat == "Excel") {
        on_pushButton_genererExcel_clicked();
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un format valide.");
    }
}
void MainWindow::on_pushButton_afficherStats_clicked() {
    QSqlQueryModel* model = Stade().afficher();

    QBarSet *setTicketsVendus = new QBarSet("Tickets Vendus");
    QBarSet *setCapacite = new QBarSet("Capacité");
    QStringList categories;

    int maxTicketsVendus = 0;
    int maxCapacite = 0;

    for (int i = 0; i < model->rowCount(); ++i) {
        QString nom = model->record(i).value("nom").toString();
        int ticketsVendus = model->record(i).value("nbr_tickets_vd").toInt();
        int capacite = model->record(i).value("capacite").toInt();
        *setTicketsVendus << ticketsVendus;
        *setCapacite << capacite;
        categories << nom;

        if (ticketsVendus > maxTicketsVendus) {
            maxTicketsVendus = ticketsVendus;
        }
        if (capacite > maxCapacite) {
            maxCapacite = capacite;
        }
    }

    QBarSeries *series = new QBarSeries();
    series->append(setTicketsVendus);
    series->append(setCapacite);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Comparaison des Tickets Vendus et Capacité par Stade");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, std::max(maxTicketsVendus, maxCapacite));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *statsWindow = new QMainWindow(this);
    statsWindow->setCentralWidget(chartView);
    statsWindow->resize(800, 600);
    statsWindow->show();
}
void MainWindow::refreshMap() {
    QQuickWidget *mapWidget = findChild<QQuickWidget*>("mapWidget");
    if (!mapWidget) {
        qDebug() << "Erreur : mapWidget n'est pas défini.";
        return;
    }

    Stade stadeInstance;
    QSqlQueryModel *model = stadeInstance.afficher();
    QVariantList stadiumList;

    for (int i = 0; i < model->rowCount(); ++i) {
        QVariantMap stadium;
        QString name = model->data(model->index(i, 0)).toString();
        QString lieu = model->data(model->index(i, 1)).toString();
        qDebug() << "Stade:" << name << "Lieu:" << lieu;

        Stade tempStade;
        tempStade.setLieu(lieu);
        double latitude, longitude;
        if (tempStade.getCoordinatesFromLieu(latitude, longitude)) {
            stadium["name"] = name;
            stadium["latitude"] = latitude;
            stadium["longitude"] = longitude;
            stadiumList.append(stadium);
            qDebug() << "Ajouté: name=" << name << "lat=" << latitude << "lon=" << longitude;
        } else {
            qDebug() << "Erreur: Coordonnées invalides pour" << name << "lieu=" << lieu;
        }
    }

    qDebug() << "stadiumList:" << stadiumList;
    mapWidget->rootContext()->setContextProperty("stadiumModel", stadiumList);
    mapWidget->setSource(QUrl(QStringLiteral("qrc:/MapView.qml")));
    mapWidget->show();
}
QSqlQueryModel* MainWindow::afficherConsulter() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT nom_champ, nom_stade FROM CONSULTER");
    query.exec();

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Championnat"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Stade"));

    return model;
}

void MainWindow::on_tableViewChamps_clicked(const QModelIndex &index) {
    if (!index.isValid()) {
        selectedChamp = "";
        return;
    }

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableViewChamps->model());
    if (!model) {
        selectedChamp = "";
        return;
    }

    selectedChamp = model->data(model->index(index.row(), 0)).toString();
}

void MainWindow::on_tirageButton_clicked() {

    if (selectedChamp.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner un championnat dans la table.");
        return;
    }

    QSqlQuery typeQuery;
    typeQuery.prepare("SELECT type FROM Championnats WHERE nom = :nom");
    typeQuery.bindValue(":nom", selectedChamp);
    if (!typeQuery.exec() || !typeQuery.next()) {
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier le type du championnat.");
        return;
    }
    if (typeQuery.value("type").toString() != "E") {
        QMessageBox::critical(this, "Erreur", "Le championnat sélectionné doit être de type 'E'.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT 1 FROM CONSULTER WHERE nom_champ = :nom_champ");
    checkQuery.bindValue(":nom_champ", selectedChamp);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::critical(this, "Erreur", "Ce championnat a déjà un stade assigné.");
        return;
    }

    QStringList stadeNames;
    QSqlQuery stadeQuery;
    stadeQuery.prepare("SELECT nom FROM Stades WHERE capacite > :minCapacity "
                       "AND nom NOT IN (SELECT nom_stade FROM CONSULTER)");
    stadeQuery.bindValue(":minCapacity", 40000);
    if (stadeQuery.exec()) {
        while (stadeQuery.next()) {
            stadeNames << stadeQuery.value("nom").toString();
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les stades.");
        return;
    }

    if (stadeNames.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Aucun stade disponible avec une capacité supérieure à 40 000.");
        return;
    }

    QRandomGenerator rng(QRandomGenerator::securelySeeded());
    QString selectedStade = stadeNames[rng.bounded(stadeNames.size())];

    int nextId = 1;
    QSqlQuery idQuery;
    idQuery.prepare("SELECT NVL(MAX(ID_CONSULTER), 0) + 1 FROM CONSULTER");
    if (idQuery.exec() && idQuery.next()) {
        nextId = idQuery.value(0).toInt();
    }

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO CONSULTER (ID_CONSULTER, nom_champ, nom_stade) "
                        "VALUES (:id, :nom_champ, :nom_stade)");
    insertQuery.bindValue(":id", nextId);
    insertQuery.bindValue(":nom_champ", selectedChamp);
    insertQuery.bindValue(":nom_stade", selectedStade);
    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout au tirage : " + insertQuery.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succès", QString("Tirage effectué : %1 assigné à %2 !")
                                                 .arg(selectedChamp, selectedStade));
    ui->tableViewConsulter->setModel(afficherConsulter());
}
void MainWindow::on_clearConsulterbutton_clicked() {
    QSqlQuery clearQuery;
    clearQuery.prepare("DELETE FROM CONSULTER");
    if (!clearQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de vider CONSULTER : " + clearQuery.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succès", "Tableau CONSULTER vidé.");
    ui->tableViewConsulter->setModel(afficherConsulter());
}
