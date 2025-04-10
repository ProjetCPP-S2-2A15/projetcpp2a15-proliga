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
#include <QRegularExpression>
#include <QInputDialog>
#include <QWidget>
#include <QTimer>
#include <QHBoxLayout>
#include <QDate>
#include <QTextDocument>
#include <QTextCursor>
#include <QPrinter>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->programme->setModel(Eq->loadequipeData());
    addActionColumn();

    // Apply design
    applyDesign(ui);

    // Initialize search timer
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    ui->lineEditRecherche->setPlaceholderText("Rechercher par ville ou entraîneur...");

    // Connections
    connect(ui->lineEditRecherche, &QLineEdit::textChanged, [this]() {
        searchTimer->start(300); // Delay of 300ms
    });
    connect(searchTimer, &QTimer::timeout, this, &MainWindow::rechercheEquipe);

    // Connect buttons to their respective slots
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

    // Connect QLineEdit fields to validation methods
    connect(ui->nome, &QLineEdit::textChanged, this, &MainWindow::checkInput);
    connect(ui->Pays, &QLineEdit::textChanged, this, &MainWindow::checkInput);
    connect(ui->coach, &QLineEdit::textChanged, this, &MainWindow::checkInput);
    connect(ui->nbmarquee, &QLineEdit::textChanged, this, &MainWindow::checkInput);
    connect(ui->nbj, &QSpinBox::valueChanged, this, &MainWindow::checkInput);
    connect(ui->telechargement, &QPushButton::clicked, this, &MainWindow::exportTableToPDF);

    // Populate sorting combo boxes
    ui->sortFieldComboBox->addItems({"NOMEQUIPE", "TYPE"}); // Add fields to sort by
    ui->sortOrderComboBox->addItems({"ASC", "DESC"}); // Add sorting orders

    // Connect sorting button
    connect(ui->triButton, &QPushButton::clicked, this, &MainWindow::trieEquipe);
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTable() {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;
    query.prepare("SELECT * FROM equipe");

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id_equipe"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom_equipe"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom_ville"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Nom_entraineur"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Nombre_joueurs"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Nombrebuts_marquees"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Debut_contrat"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Fin_contrat"));

        ui->programme->setModel(model);
        addActionColumn();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données : " + query.lastError().text());
    }
}

void MainWindow::addActionColumn() {
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->programme->model());

    if (model) {
        model->insertColumn(model->columnCount());
        model->setHeaderData(model->columnCount() - 1, Qt::Horizontal, QObject::tr("Action"));
        addButtonsToTable();
    }
}

void MainWindow::addButtonsToTable() {
    for (int row = 0; row < ui->programme->model()->rowCount(); ++row) {
        QPushButton *modifyButton = new QPushButton("Modifier");
        QPushButton *deleteButton = new QPushButton("Supprimer");

        int equipeId = ui->programme->model()->data(ui->programme->model()->index(row, 0)).toInt();

        connect(modifyButton, &QPushButton::clicked, [this, equipeId]() {
            openModificationDialog(equipeId);
        });

        connect(deleteButton, &QPushButton::clicked, [this, equipeId]() {
            if (QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette équipe ?") == QMessageBox::Yes) {
                if (Eq->Delete(equipeId)) {
                    QMessageBox::information(this, "Succès", "Équipe supprimée avec succès !");
                    refreshTable();
                } else {
                    QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'équipe.");
                }
            }
        });

        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(modifyButton);
        layout->addWidget(deleteButton);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);

        QWidget *buttonWidget = new QWidget();
        buttonWidget->setLayout(layout);
        ui->programme->setIndexWidget(ui->programme->model()->index(row, ui->programme->model()->columnCount() - 1), buttonWidget);
    }
}

bool MainWindow::isTeamNameUnique(const QString& teamName, int excludedId) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM equipe WHERE NOMEQUIPE = :nomE AND IDEQUIPE != :excludedId");
    query.bindValue(":nomE", teamName);
    query.bindValue(":excludedId", excludedId);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count == 0;
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du nom de l'équipe : " + query.lastError().text());
        return false;
    }
}

bool MainWindow::validateName() {
    bool valide = true;
    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->nome->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid name. It should start with a capital letter and contain only letters.");
        ui->nome->setFocus();
        valide = false;
    } else if (!isTeamNameUnique(ui->nome->text())) {
        QMessageBox::warning(this, "Input Error", "Le nom de l'équipe doit être unique.");
        ui->nome->setFocus();
        valide = false;
    }
    return valide;
}

bool MainWindow::validateContractDates() {
    QDate dateDebut = ui->dbcnt->date();
    QDate dateFin = ui->fincnt->date();

    if (dateFin <= dateDebut) {
        QMessageBox::warning(this, "Input Error", "La date de fin de contrat doit être postérieure à la date de début de contrat.");
        ui->fincnt->setFocus();
        return false;
    }
    return true;
}

bool MainWindow::validateTypeSelection() {
    bool normalChecked = ui->Normal->isChecked();
    bool nationalChecked = ui->Nationnal->isChecked();

    if (!normalChecked && !nationalChecked) {
        QMessageBox::warning(this, "Input Error", "Veuillez sélectionner un type (Normal ou National).");
        return false;
    } else if (normalChecked && nationalChecked) {
        QMessageBox::warning(this, "Input Error", "Veuillez sélectionner un seul type (Normal ou National).");
        return false;
    }
    return true;
}

bool MainWindow::validateCountry() {
    bool valide = true;
    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->Pays->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid country name. It should start with a capital letter and contain only letters.");
        ui->Pays->setFocus();
        valide = false;
    }
    return valide;
}

bool MainWindow::validateCoach() {
    bool valide = true;
    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->coach->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid coach name. It should start with a capital letter and contain only letters.");
        ui->coach->setFocus();
        valide = false;
    }
    return valide;
}

bool MainWindow::validateGoals() {
    bool valide = true;
    QRegularExpression numberRegex("^[0-9]+$");
    if (!numberRegex.match(ui->nbmarquee->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid number of goals. It should contain only digits.");
        ui->nbmarquee->setFocus();
        valide = false;
    } else if (ui->nbmarquee->text().toInt() < 0) {
        QMessageBox::warning(this, "Input Error", "The number of goals scored must be non-negative.");
        ui->nbmarquee->setFocus();
        valide = false;
    }
    return valide;
}

bool MainWindow::validatePlayers() {
    bool valide = true;
    QRegularExpression numberRegex("^[0-9]+$");
    if (!numberRegex.match(ui->nbj->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid number of players. It should contain only digits.");
        ui->nbj->setFocus();
        valide = false;
    } else if (ui->nbj->value() < 11) {
        QMessageBox::warning(this, "Input Error", "The number of players must be at least 11.");
        ui->nbj->setFocus();
        valide = false;
    }
    return valide;
}

void MainWindow::checkInput() {
    bool isValid = validateName() && validateCountry() && validateCoach() && validateGoals() && validatePlayers() && validateContractDates() && validateTypeSelection();
    ui->Ajouter->setEnabled(isValid);
}

void MainWindow::Ajouter_clicked() {
    if (!validateName() || !validateCountry() || !validateCoach() || !validateGoals() || !validatePlayers() || !validateContractDates() || !validateTypeSelection()) {
        return; // Stop if any validation fails
    }

    QString nomE = ui->nome->text();
    QString nomv = ui->Pays->text();
    QString nomC = ui->coach->text();
    int nbj = ui->nbj->value();
    QString type = ui->Normal->isChecked() ? "Normal" : "National";
    int nbm = ui->nbmarquee->text().toInt();
    QDate dateDebutContrat = ui->dbcnt->date();
    QDate dateFinContrat = ui->fincnt->date();

    Equipe EQUIPE(nomE, nomv, nomC, nbj, type, nbm, dateDebutContrat, dateFinContrat);
    bool success = EQUIPE.Ajouter();
    if (success) {
        QMessageBox::information(this, "Success", "Equipe added successfully!");
        refreshTable();
        ui->nome->clear();
        ui->Pays->clear();
        ui->coach->clear();
        ui->nbj->setValue(0);
        ui->nbmarquee->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the EQUIPE: " + QSqlDatabase::database().lastError().text());
    }
}

void MainWindow::openModificationDialog(int equipeId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM equipe WHERE IDEQUIPE = :id");
    query.bindValue(":id", equipeId);
    query.exec();

    if (query.next()) {
        ui->nome->setText(query.value("NOMEQUIPE").toString());
        ui->Pays->setText(query.value("NOMVILLE").toString());
        ui->coach->setText(query.value("NOMENTRAINEUR").toString());
        ui->nbj->setValue(query.value("NOMBREJOUEURS").toInt());
        ui->nbmarquee->setText(query.value("NOMBREBUTSMARQUES").toString());

        if (query.value("TYPE").toString() == "Normal") {
            ui->Normal->setChecked(true);
        } else {
            ui->Nationnal->setChecked(true);
        }

        ui->nome->setEnabled(false);
        ui->Ajouter->setText("Modifier");

        disconnect(ui->Ajouter, &QPushButton::clicked, nullptr, nullptr);
        connect(ui->Ajouter, &QPushButton::clicked, [this, equipeId]() { modifyEquipe(equipeId); });
    }
}

void MainWindow::modifyEquipe(int equipeId) {
    if (!validateCountry() || !validateCoach() || !validateGoals() || !validatePlayers() || !validateContractDates() || !validateTypeSelection()) {
        return; // Stop if any validation fails
    }

    QString nomE = ui->nome->text();
    QString nomv = ui->Pays->text();
    QString nomC = ui->coach->text();
    int nbj = ui->nbj->value();
    int nbm = ui->nbmarquee->text().toInt();
    QString type = ui->Normal->isChecked() ? "Normal" : "National";

    QSqlQuery query;
    query.prepare("UPDATE equipe SET NOMVILLE = :nomv, NOMENTRAINEUR = :nomC, "
                  "NOMBREJOUEURS = :nbj, NOMBREBUTSMARQUES = :nbm, TYPE = :type WHERE IDEQUIPE = :id");
    query.bindValue(":nomv", nomv);
    query.bindValue(":nomC", nomC);
    query.bindValue(":nbj", nbj);
    query.bindValue(":nbm", nbm);
    query.bindValue(":type", type);
    query.bindValue(":id", equipeId);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Equipe modified successfully!");
        refreshTable();
        ui->nome->clear();
        ui->Pays->clear();
        ui->coach->clear();
        ui->nbj->setValue(0);
        ui->nbmarquee->clear();
        ui->nome->setEnabled(true);
        ui->Ajouter->setText("Ajouter");

        disconnect(ui->Ajouter, &QPushButton::clicked, nullptr, nullptr);
        connect(ui->Ajouter, &QPushButton::clicked, this, &MainWindow::Ajouter_clicked);
    } else {
        QMessageBox::critical(this, "Error", "Failed to modify the equipe: " + query.lastError().text());
    }
}

void MainWindow::exportTableToPDF() {
    QTextDocument document;
    QTextCursor cursor(&document);
    QString html = "<table border='1' cellpadding='5'>";

    html += "<tr>";
    for (int col = 0; col < ui->programme->model()->columnCount() - 1; ++col) {
        QString header = ui->programme->model()->headerData(col, Qt::Horizontal).toString();
        html += "<th>" + header + "</th>";
    }
    html += "</tr>";

    for (int row = 0; row < ui->programme->model()->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->programme->model()->columnCount() - 1; ++col) {
            QString data = ui->programme->model()->data(ui->programme->model()->index(row, col)).toString();
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    cursor.insertHtml(html);
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) {
        return; // Cancel if the user did not select a file
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    document.print(&printer);

    QMessageBox::information(this, "Succès", "Le tableau a été exporté en PDF avec succès !");
}

void MainWindow::rechercheEquipe() {
    QString keyword = ui->lineEditRecherche->text().trimmed();
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->programme->model());
    if (!model) return;

    QString queryStr = "SELECT * FROM equipe WHERE 1=1";
    if (!keyword.isEmpty()) {
        queryStr += " AND (NOMEQUIPE LIKE :keyword OR NOMENTRAINEUR LIKE :keyword)";
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (!keyword.isEmpty()) {
        query.bindValue(":keyword", "%" + keyword + "%");
    }

    if (query.exec()) {
        model->setQuery(query);
        addActionColumn();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche : " + query.lastError().text());
    }
}

void MainWindow::trieEquipe() {
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->programme->model());
    if (!model) return;

    QString queryStr = "SELECT * FROM equipe WHERE 1=1";

    // Add sorting
    QString sortField = ui->sortFieldComboBox->currentText(); // Assuming you have a combo box for sort field
    QString sortOrder = ui->sortOrderComboBox->currentText(); // Assuming you have a combo box for sort order

    // Ensure the sort field is valid
    if (!sortField.isEmpty()) {
        queryStr += " ORDER BY " + sortField + " " + sortOrder;
    }

    QSqlQuery query;
    query.prepare(queryStr);

    if (query.exec()) {
        model->setQuery(query);
        addActionColumn();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri : " + query.lastError().text());
    }
}
//statistiques
void MainWindow::afficherStatistiques() {
    // Récupérer les données
    int totalButs = 0;
    int totalJoueurs = 0;

    QSqlQuery query;
    query.prepare("SELECT SUM(NOMBREBUTSMARQUES) AS totalButs, SUM(NOMBREJOUEURS) AS totalJoueurs FROM equipe");

    if (query.exec() && query.next()) {
        totalButs = query.value("totalButs").toInt();
        totalJoueurs = query.value("totalJoueurs").toInt();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des statistiques : " + query.lastError().text());
        return;
    }

    // Créer le graphique circulaire
    QPieSeries *series = new QPieSeries();
    series->append("Buts Marqués", totalButs);
    series->append("Nombre de Joueurs", totalJoueurs);

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Équipes");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Afficher le graphique dans QGraphicsView
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(chart);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}
