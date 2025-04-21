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
#include "statistique.h" // Include the Statistique header
#include "contratdialog.h"
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <iostream> // For std::cout
#include <exception> // For std::exception
// if the library uses this namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , statistique(new Statistique(this)) // Initialize here
    , futureWatcher(new QFutureWatcher<QString>(this)) // Initialize the future watcher
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
    chartView = nullptr;

    // Create an instance of Statistique
    if (!ui->widget_6->layout()) {
        ui->widget_6->setLayout(new QVBoxLayout());
        ui->widget_6->layout()->setContentsMargins(0, 0, 0, 0);
    }

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
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &MainWindow::afficherStatistiques); // Connect statistics button

    // Connect QLineEdit fields to validation methods
   /* connect(ui->nome, &QLineEdit::textChanged, this, &MainWindow:: validateInputs);
    connect(ui->Pays, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->coach, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->nbmarquee, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->nbj, &QSpinBox::valueChanged, this, &MainWindow::validateInputs);*/
    connect(ui->telechargement, &QPushButton::clicked, this, &MainWindow::exportTableToPDF);
    connect(ui->voc_nom, &QPushButton::clicked, this, &MainWindow::on_voc_nom_clicked);
    connect(ui->voc_coach, &QPushButton::clicked, this, &MainWindow::on_voc_coach_clicked);
    connect(ui->voc_nb, &QPushButton::clicked, this, &MainWindow::on_voc_nb_clicked);
    connect(ui->voc_pays, &QPushButton::clicked, this, &MainWindow::on_voc_pays_clicked);


    // Populate sorting combo boxes
    ui->sortFieldComboBox->addItems({"NOMEQUIPE", "TYPE"}); // Add fields to sort by
    ui->sortOrderComboBox->addItems({"ASC", "DESC"}); // Add sorting orders

    // Connect sorting button
    connect(ui->triButton, &QPushButton::clicked, this, &MainWindow::trieEquipe);

    // Connect future watcher
    connect(futureWatcher, &QFutureWatcher<QString>::finished, this, [this]() {
        QString text = futureWatcher->result();
        if (!text.isEmpty()) {
            // Update the appropriate UI field based on the last clicked button
            if (lastClickedField == "nome") {
                ui->nome->setText(text.toUpper());
            } else if (lastClickedField == "coach") {
                ui->coach->setText(text.toUpper());
            } else if (lastClickedField == "nb") {
                ui->nbmarquee->setText(text.toUpper());
            } else if (lastClickedField == "pays") {
                ui->Pays->setText(text.toUpper());
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete chartView;  // Clean up the chart view
    delete statistique;
    delete ui;
}

void MainWindow::afficherStatistiques() {
    // Clear previous chart if it exists
    if (chartView) {
        ui->widget_6->layout()->removeWidget(chartView);
        delete chartView;
        chartView = nullptr;
    }

    // Create new chart view with team goals statistics
    chartView = statistique->creerGraphiqueButsParEquipe();

    // Style the chart view
    chartView->setStyleSheet("background: transparent; border: none;");
    chartView->setRenderHint(QPainter::Antialiasing);

    // Add to layout
    ui->widget_6->layout()->addWidget(chartView);

    // Ensure the widget is visible
    ui->widget_6->show();
}
// Other methods remain unchanged...
void MainWindow::refreshTable() {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;
    query.prepare("SELECT * FROM equipe");

    if (query.exec()) {
        // Remove std::move and pass the query directly
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
void MainWindow::Ajouter_clicked() {
    // Retrieve data from the UI
    QString teamName = ui->nome->text().trimmed();
    QString country = ui->Pays->text().trimmed();
    QString coach = ui->coach->text().trimmed();
    QString goals = ui->nbmarquee->text().trimmed();
    QString players = ui->nbj->text().trimmed();
    QDate dateDebutContrat = ui->dbcnt->date();
    QDate dateFinContrat = ui->fincnt->date();
    bool normalChecked = ui->Normal->isChecked();
    bool nationalChecked = ui->Nationnal->isChecked();

    // Regular expressions
    static const QRegularExpression alphaRegex("^[A-Z][a-zA-Z]*$");
    static const QRegularExpression numberRegex("^[0-9]+$");

    // Validate Team Name
    if (teamName.isEmpty() || !isTeamNameUnique(teamName) || !alphaRegex.match(teamName).hasMatch()) {
        ui->nome->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid team name. It should be unique and start with a capital letter.");
        return;
    } else {
        ui->nome->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Country
    if (country.isEmpty() || !alphaRegex.match(country).hasMatch()) {
        ui->Pays->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid country name. It should start with a capital letter.");
        return;
    } else {
        ui->Pays->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Coach
    if (coach.isEmpty() || !alphaRegex.match(coach).hasMatch()) {
        ui->coach->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid coach name. It should start with a capital letter.");
        return;
    } else {
        ui->coach->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Goals
    if (!numberRegex.match(goals).hasMatch() || goals.toInt() < 0) {
        ui->nbmarquee->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid number of goals. It should be a non-negative integer.");
        return;
    } else {
        ui->nbmarquee->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Players
    if (!numberRegex.match(players).hasMatch() || players.toInt() < 11) {
        ui->nbj->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "The number of players must be at least 11.");
        return;
    } else {
        ui->nbj->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Contract Dates
    if (dateFinContrat <= dateDebutContrat) {
        ui->dbcnt->setStyleSheet("border: 1px solid #D32F2F;");
        ui->fincnt->setStyleSheet("border: 1px solid #D32F2F;");

        QMessageBox::warning(this, "Input Error", "The end date must be after the start date.");
        return;
    } else {
        ui->fincnt->setStyleSheet("border: 1px solid #2E7D32;");
         ui->dbcnt->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Type Selection
    if (!normalChecked && !nationalChecked) {
        QMessageBox::warning(this, "Input Error", "Please select a type (Normal or National).");
        return;
    } else if (normalChecked && nationalChecked) {
        QMessageBox::warning(this, "Input Error", "Please select only one type (Normal or National).");
        return;
    }

    // If all validations pass, proceed to add the team
    QString type = normalChecked ? "Normal" : "National";
    Equipe EQUIPE(teamName, country, coach, players.toInt(), type, goals.toInt(), dateDebutContrat, dateFinContrat);
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

        // Retrieve and set the contract dates
        QDate dateDebutContrat = query.value("DATEDEBUTCONTRAT").toDate(); // Assuming the column name is DATEDEBUT
        QDate dateFinContrat = query.value("DATEFINCONTRAT").toDate(); // Assuming the column name is DATEFIN
        ui->dbcnt->setDate(dateDebutContrat);
        ui->fincnt->setDate(dateFinContrat);

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
    // Retrieve data from the UI
    QString teamName = ui->nome->text().trimmed();
    QString country = ui->Pays->text().trimmed();
    QString coach = ui->coach->text().trimmed();
    QString goals = ui->nbmarquee->text().trimmed();
    QString players = ui->nbj->text().trimmed();
    bool normalChecked = ui->Normal->isChecked();
    bool nationalChecked = ui->Nationnal->isChecked();

    // Convert QDate to QString, trim it, and convert back to QDate
    QDate dateDebutContrat = QDate::fromString(ui->dbcnt->date().toString("yyyy-MM-dd").trimmed(), "yyyy-MM-dd");
    QDate dateFinContrat = QDate::fromString(ui->fincnt->date().toString("yyyy-MM-dd").trimmed(), "yyyy-MM-dd");

    // Regular expressions
    static const QRegularExpression alphaRegex("^[A-Z][a-zA-Z]*$");
    static const QRegularExpression numberRegex("^[0-9]+$");

    // Validate Team Name
    if (teamName.isEmpty() || !isTeamNameUnique(teamName, equipeId) || !alphaRegex.match(teamName).hasMatch()) {
        ui->nome->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid team name. It should be unique and start with a capital letter.");
        return;
    } else {
        ui->nome->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Country
    if (country.isEmpty() || !alphaRegex.match(country).hasMatch()) {
        ui->Pays->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid country name. It should start with a capital letter.");
        return;
    } else {
        ui->Pays->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Coach
    if (coach.isEmpty() || !alphaRegex.match(coach).hasMatch()) {
        ui->coach->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid coach name. It should start with a capital letter.");
        return;
    } else {
        ui->coach->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Goals
    if (!numberRegex.match(goals).hasMatch() || goals.toInt() < 0) {
        ui->nbmarquee->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "Invalid number of goals. It should be a non-negative integer.");
        return;
    } else {
        ui->nbmarquee->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Players
    if (!numberRegex.match(players).hasMatch() || players.toInt() < 11) {
        ui->nbj->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "The number of players must be at least 11.");
        return;
    } else {
        ui->nbj->setStyleSheet("border: 1px solid #2E7D32;");
    }

    // Validate Contract Dates
    if (dateFinContrat <= dateDebutContrat) {
        ui->dbcnt->setStyleSheet("border: 1px solid #D32F2F;");
        ui->fincnt->setStyleSheet("border: 1px solid #D32F2F;");
        QMessageBox::warning(this, "Input Error", "The end date must be after the start date.");
        return;
    } else {
        ui->fincnt->setStyleSheet("border: 1px solid #2E7D32;");
        ui->dbcnt->setStyleSheet("border: 1px solid # 2E7D32;");
    }

    // Validate Type Selection
    if (!normalChecked && !nationalChecked) {
        QMessageBox::warning(this, "Input Error", "Please select a type (Normal or National).");
        return;
    } else if (normalChecked && nationalChecked) {
        QMessageBox::warning(this, "Input Error", "Please select only one type (Normal or National).");
        return;
    }

    // If all validations pass, proceed to modify the team
    QString type = normalChecked ? "Normal" : "National";

    QSqlQuery query;
    query.prepare("UPDATE equipe SET NOMVILLE = :nomv, NOMENTRAINEUR = :nomC, "
                  "NOMBREJOUEURS = :nbj, NOMBREBUTSMARQUES = :nbm, TYPE = :type WHERE IDEQUIPE = :id");
    query.bindValue(":nomv", country);
    query.bindValue(":nomC", coach);
    query.bindValue(":nbj", players.toInt());
    query.bindValue(":nbm", goals.toInt());
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
        queryStr += " AND (NOMVILLE LIKE :keyword OR NOMENTRAINEUR LIKE :keyword)";
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
//metier avancee
void MainWindow::on_genererContratButton_clicked()
{
    // Assurez-vous que ces pointeurs correspondent à vos widgets dans mainwindow.ui
    ContratDialog dialog(
        ui->equipeComboBox,    // QComboBox*
        ui->contratTextEdit,   // QTextEdit*
        ui->genererContratButton,     // QPushButton*
        QSqlDatabase::database(),
        this
        );

    dialog.exec();
}//metier avance speechto text
#include <cstdlib> // Pour system()
void MainWindow::on_voc_nom_clicked() {
    lastClickedField = "nome";
    QString result = recordText(); // Appel direct au lieu de QtConcurrent
    if (!result.isEmpty()) {
        ui->nome->setText(result.toUpper());
    }
}

// Faites de même pour les autres boutons vocaux...

void MainWindow::on_voc_coach_clicked() {
    lastClickedField = "coach";

    QString result = recordText(); // Appel direct au lieu de QtConcurrent
    if (!result.isEmpty()) {
        ui->coach->setText(result.toUpper());
    }
}

// Faites de même pour les autres boutons vocaux...

void MainWindow::on_voc_nb_clicked() {
    lastClickedField = "nb";

    QString result = recordText(); // Appel direct au lieu de QtConcurrent
    if (!result.isEmpty()) {
        ui->nbmarquee->setText(result.toUpper());
    }
}

// Faites de même pour les autres boutons vocaux...


void MainWindow::on_voc_pays_clicked() {
    lastClickedField = "pays";

    QString result = recordText(); // Appel direct au lieu de QtConcurrent
    if (!result.isEmpty()) {
        ui->Pays->setText(result.toUpper());
    }
}
#include <QProcess>
#include <QFile>
#include <QMessageBox>

QString MainWindow::recordText()
{
    QString pythonScriptPath = "C:/Users/PC_DELL/Downloads/c++/voc exec/voc.py";
    QString pythonExecutable = "C:/Python312/python.exe";

    QProcess process;

    // Clear PYTHONHOME and PYTHONPATH environment variables
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");
    process.setProcessEnvironment(env);

    QMessageBox::information(this, "Enregistrement", "Parlez maintenant pendant 5 secondes...");

    process.start(pythonExecutable, QStringList() << pythonScriptPath);

    if (!process.waitForFinished(10000)) { // 10 second timeout
        QMessageBox::warning(this, "Error",
                             process.error() == QProcess::Timedout ?
                                 "Process timed out" : "Failed to execute the Python script");
        return "";
    }

    QString output = process.readAllStandardOutput().trimmed();
    QString errorOutput = process.readAllStandardError().trimmed();

    if (!errorOutput.isEmpty()) {
        QMessageBox::warning(this, "Error", "Error in Python script: " + errorOutput);
        return "";
    }

    return output;
}
