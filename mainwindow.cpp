#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "joueur_utils.h"
#include "arduino.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QDebug>
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
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include "design.h"
#include "joueur_utils.h"
#include <QStandardPaths>
#include "arduinoshiraz.h"
#include "match.h"
#include <QInputDialog>
#include <random>
#include "loginwindow.h"
#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFileDialog>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>
#include <QChartView>
#include <QPieSeries>
#include <QChart>
#include <QCursor>
#include <QDate>
#include <QToolTip>
#include <QStringList>
#include <QtPrintSupport/QPrinter>
#include <QPropertyAnimation>
#include <QPieSlice>
#include <QGraphicsOpacityEffect>
#include <QAxObject>
#include <QDir>
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "championnats.h"
#include <statisticscontainer.h>
#include <QMessageBox>
#include "stades.h"
#include <QSqlQuery>
#include <QSqlError>
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
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextStream>
#include <QSerialPort>
#include <QSerialPortInfo>


QSerialPort *serial;

// if the library uses this namespace
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , A(this)
    , statistique(new Statistique(this)) // Initialize here
    , futureWatcher(new QFutureWatcher<QString>(this))
    , ui(new Ui::MainWindow)
    ,selectedChamp("")
{
    ui->setupUi(this);
    //ala
    arduino = new Arduino();
    //alaend
    ui->programme->setModel(Eq->loadequipeData());
    loadTeamsIntoComboBox();
    addActionColumn();
    A.setSmtpCredentials("smtp.gmail.com", 465,
                         "heditrabelsi412@gmail.com",
                         "kton jsoh jtmh blrf");

    // Apply design styles
    applyDesign(ui);
//khadija
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    ui->lineEditRecherche->setPlaceholderText("Rechercher par ville ou entraîneur...");
    chartView2 = nullptr;

    // Create an instance of Statistique
    if (!ui->widget_9->layout()) {
        ui->widget_9->setLayout(new QVBoxLayout());
        ui->widget_9->layout()->setContentsMargins(0, 0, 0, 0);
    }

    // Connections
    connect(ui->lineEditRecherche, &QLineEdit::textChanged, [this]() {
        searchTimer->start(300); // Delay of 300ms
    });
    connect(searchTimer, &QTimer::timeout, this, &MainWindow::rechercheEquipe);
    // Show data at startup
    A.afficher(ui->tableWidget_arbitres);

    scene1 = new Scene1();
    //design
    StyleTW(ui->tableWidget);
  //  ui->CW->setVisible(false);

    //changeWidget
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

    QQuickWidget *mapWidget = new QQuickWidget(this);
    mapWidget->setObjectName("mapWidget");
    mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout_5->addWidget(mapWidget);
    refreshMap();
    //---------------------------------------------------------------------------------------------------------------------------
    loadMatchesIntoTable();
    loadRefereesIntoComboBox();
    loadEquipes();
    checkForNullScoreEdit();
    showMonthlyMatchStatistics();
    programmation_2 = ui->programmation_2;
    historique_table=ui->historique_table;
    originalTabWidth = ui->tabWidget_match->geometry().width();
    originalTableWidth = ui->programmation_2->geometry().width();
    originalWidget6Width = ui->widget_6_match->geometry().width();
    originalDeleteMatchX = ui->delete_match->geometry().x();
    originalChercherWidth = ui->chercher_match->geometry().width();
    isExpanded = false;





    //-----------------------------------------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------------------------------------
    ChatBotWidget *chatBot = new ChatBotWidget(this);
    ui->programmation_2->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->historique_table->setEditTriggers(QAbstractItemView::DoubleClicked);

    if (ui->widget_8_match->layout() == nullptr) {
        ui->widget_8_match->setLayout(new QVBoxLayout());
    }
    ui->widget_8_match->layout()->addWidget(chatBot);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkForNullScoreEdit);
    timer->start(5000);

    //-----------------------------------------------------------------------------------------------------------------------------------------------

    // Connect other signals and slots
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

    //crud joueur
    connect(ui->AjouterButton, &QPushButton::clicked, this, &MainWindow::onAjouterButtonClicked);
    connect(ui->rechercherButton, &QPushButton::clicked, this, &MainWindow::onrechercherButtonClicked);

    //connect(ui->deleteButton, &QPushButton::clicked, this, [=]() {deleteJoueurFromUI(this);});
    //connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::uploadImage);

    connect(ui->modifierButton, &QPushButton::clicked, this, [this]() {
        if (selected_row == -1) return;
        confirmUpdate(this, selected_row);
        refreshStats();
        selected_row = -1;
    });

    //controle de saisie
    connect(ui->NomInput, &QLineEdit::textChanged, this, &MainWindow::validateInputsJ);
    connect(ui->PrenomInput, &QLineEdit::textChanged, this, &MainWindow::validateInputsJ);
    //connect(ui->PositionInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->NationaliteInput, &QLineEdit::textChanged, this, &MainWindow::validateInputsJ);
    connect(ui->dsInput, &QDateEdit::dateChanged, this, &MainWindow::validateInputsJ);
    connect(ui->Img_pathInput, &QLineEdit::textChanged, this, &MainWindow::validateInputsJ);

    //read joueur
    Joueur j;

    j.getListe(ui->liste_Joueurs);
    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");

    j.readJoueur(tableWidgetPlayers);
    setupTableWithDeleteButtons(tableWidgetPlayers);

    //export pdf
    connect(ui->pdfButton, &QPushButton::clicked, this, &MainWindow::exportToPDFJ);
    refreshStats();

    //arduino
    setupArduinoConnection();

    connect(ui->Ajouter, &QPushButton::clicked, this, &MainWindow::Ajouter_clicked);
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &MainWindow::afficherStatistiques); // Connect statistics button

    // Connect QLineEdit fields to validation methods
    /* connect(ui->nome, &QLineEdit::textChanged, this, &MainWindow:: validateInputs);
    connect(ui->Pays, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->coach, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->nbmarquee, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->nbj, &QSpinBox::valueChanged, this, &MainWindow::validateInputs);*/
    connect(ui->telechargement, &QPushButton::clicked, this, &MainWindow::exportTableToPDFK);
    connect(ui->voc_nom, &QPushButton::clicked, this, &MainWindow::on_voc_nom_clicked);
    connect(ui->voc_coach, &QPushButton::clicked, this, &MainWindow::on_voc_coach_clicked);
    connect(ui->voc_nb, &QPushButton::clicked, this, &MainWindow::on_voc_nb_clicked);
    connect(ui->voc_pays, &QPushButton::clicked, this, &MainWindow::on_voc_pays_clicked);
    connect(ui->GenererContratJoueur, &QPushButton::clicked, this, &MainWindow::GenererContratJoueur);

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

    connect(ui->pushButton_tri, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_tri_clicked);
    connect(ui->pushButton_stats, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_stats_clicked);
    connect(ui->pushButton_submit, &QPushButton::clicked,
            this, [this]() { A.envoyerConfirmationsArbitres(); });
    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            &A, &Arbitre::searchTable);

    connect(ui->sidebar_2, &QPushButton::clicked, this, &MainWindow::toggleIconOnlySidebar);

    //--------------------------------------------------BOUTONS INTERFACE SHIRAZ------------------------------------------------------------------------------------------------------------------------------------------
    connect(ui->add_match, &QPushButton::clicked, this, &MainWindow::on_addMatchButton_clicked);
    connect(ui->delete_match, &QPushButton::clicked, this, &MainWindow::deleteMatch);
    connect(ui->refresh, &QPushButton::clicked, this, &MainWindow::loadMatchesIntoTable);
    connect(ui->random_arbitre_match, &QCheckBox::stateChanged, this, &MainWindow::handleRandomReferees);
    connect(ui->chercher_match, &QLineEdit::textChanged, this, &MainWindow::filterTable);
    connect(ui->chercher_histo, &QLineEdit::textChanged, this, &MainWindow::filterTable);
    connect(ui->programmation_2, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onCellDoubleClicked(int, int)));
    connect(ui->historique_table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onHistoriqueCellDoubleClicked(int, int)));
    connect(ui->historique_table, &QTableWidget::itemChanged, this, &MainWindow::onItemChanged);
    connect(ui->show_2, &QPushButton::clicked, this, &MainWindow::onShowButtonClicked);
     connect(ui->show_3, &QPushButton::clicked, this, &MainWindow::onShow3ButtonClicked);
    connect(ui->tri_prog, SIGNAL(clicked()), this, SLOT(on_tri_prog_clicked()));
    connect(ui->tri_histo, SIGNAL(clicked()), this, SLOT(on_tri_histo_clicked()));
    connect(ui->showarduino, &QPushButton::clicked,
            this, &MainWindow::on_showArduinoButton_clicked);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    ui->tabWidget->setTabVisible(3, false);
    connect(ui->PBA, &QPushButton::clicked, this, &MainWindow::ajoutChampB_clicked);
    connect(ui->majbutton, &QPushButton::clicked, this, &MainWindow::modifChampB_clicked);
    connect(ui->recherche, &QPushButton::clicked, this, &MainWindow::rechrecheparnom);
    connect(ui->exp_b,&QPushButton::clicked,this,&MainWindow::selectExp);
    ui->tableWidget->setSortingEnabled(true);
    //connect(ui->addStadiumbuttoN,&QPushButton::clicked,this, &MainWindow::on_addStadiumbuttoN_clicked);

    validationTimer = new QTimer(this);
    validationTimer->setInterval(200);  // 200ms delay
    validationTimer->setSingleShot(true);

    validationTimerUP = new QTimer(this);
    validationTimerUP->setInterval(200);  // Debounce time
    validationTimerUP->setSingleShot(true);

    connect(validationTimer, &QTimer::timeout, this, &MainWindow::validateInputs);
    connect(validationTimerUP, &QTimer::timeout, this, &MainWindow::validateInputsUP);

    connect(ui->lineEdit_nom, &QLineEdit::textChanged, this, [=]() { validationTimer->start(); });
    connect(ui->lineEdit_nbrE, &QLineEdit::textChanged, this, [=]() { validationTimer->start(); });
    connect(ui->comboBox_T, &QComboBox::currentTextChanged, this, [=]() { validationTimer->start(); });
    connect(ui->lineEdit_Org, &QLineEdit::textChanged, this, [=]() { validationTimer->start(); });
    connect(ui->lineEdit_PG, &QLineEdit::textChanged, this, [=]() { validationTimer->start(); });



    connect(ui->majNom, &QLineEdit::textChanged, this, [=]() { validationTimer->start(); });
    connect(ui->majNbrE, &QLineEdit::textChanged, this, [=]() { validationTimerUP->start(); });
    connect(ui->majOrg, &QLineEdit::textChanged, this, [=]() { validationTimerUP->start(); });
    connect(ui->majPG, &QLineEdit::textChanged, this, [=]() { validationTimerUP->start(); });
    connect(ui->majT, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() { validationTimerUP->start(); });
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refreshMap);

    connect(ui->lineEdit_rechercheNom, &QLineEdit::textChanged, this, &MainWindow::on_rechercheNom_textChanged);
    connect(ui->lineEdit_capaciteMin, &QLineEdit::textChanged, this, &MainWindow::on_capaciteMinMax_textChanged);
    connect(ui->lineEdit_capaciteMax, &QLineEdit::textChanged, this, &MainWindow::on_capaciteMinMax_textChanged);

    loadChampData();
    loadChampL();
    //checkSerialData();
    connect(ui->MsL, &QComboBox::currentTextChanged, this, &MainWindow::on_MsL_currentIndexChanged);
    //connect(ui->LSRbut, &QPushButton::clicked, this, &MainWindow::on_LSRbut_clicked);

    QTimer *serialTimer = new QTimer(this);
    connect(serialTimer, &QTimer::timeout, this, [=]() {
        QString name = scene1->readSerialData();
        if (!name.isEmpty()) {
            ui->ARD_R->setText("Received: " + name);
            QString RM = ARD_consulter(name);
            scene1->ARD_sendCS(RM);

        }
    });
    serialTimer->start(2500);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene1;
    delete chartView2;  // Clean up the chart view
    delete statistique;
    delete arduino;
}

//------------------------------------------------SIDE_BAR_ANIMATION----------------------------------------------------------------------------------

void MainWindow::toggleIconOnlySidebar()
{
    if (ui->iconOnlySidebar->isVisible()) {
        ui->iconOnlySidebar->hide();
        ui->iconTextSidebar->show();

        ui->stackedWidget->setMinimumWidth(1000);
        ui->stackedWidget->setMaximumWidth(1000);

        ui->widget_6_match->setMinimumWidth(200);
        ui->widget_6_match->setMaximumWidth(340);

        ui->widget_7_match->setMinimumWidth(200);
        ui->widget_7_match->setMaximumWidth(360);

        ui->widget_8_match->setMinimumWidth(200);
        ui->widget_8_match->setMaximumWidth(340);
    }
    else {

        ui->iconOnlySidebar->show();
        ui->iconTextSidebar->hide();

        ui->stackedWidget->setMinimumWidth(1500);
        ui->stackedWidget->setMaximumWidth(1500);

        ui->widget_6_match->setMinimumWidth(400);
        ui->widget_6_match->setMaximumWidth(800);

        ui->widget_7_match->setMinimumWidth(300);
        ui->widget_7_match->setMaximumWidth(600);

        ui->widget_8_match->setMinimumWidth(400);
        ui->widget_8_match->setMaximumWidth(800);
    }

    // Update chartView's size to follow widget_6 size
    chartView->resize(ui->widget_6_match->size());

    // Update widget and chart views
    ui->stackedWidget->update();
    ui->widget_6_match->update();
    ui->widget_7_match->update();
    ui->widget_8_match->update();
}

//------------------------------------------------AJOUT MATCH-------------------------------------------------------------------------------


bool isInsertingMatch = false;
void MainWindow::on_addMatchButton_clicked() {
    qDebug() << "Add Match Button Clicked";

    if (isInsertingMatch) {
        qDebug() << "Match is already being inserted, skipping.";
        return;
    }

    isInsertingMatch = true;
    ui->pushButton->setEnabled(false);

    QDateTime dateTime = ui->dateTimeEdit_2_match->dateTime();
    QString type = ui->comboBox_match->currentText();
    QString equipe1 = ui->comboBox_2->currentText();
    QString equipe2 = ui->comboBox_3->currentText();

    // controle saisie equipe
    if (equipe1 == equipe2) {
        QMessageBox::warning(this, "Erreur", "Les deux équipes ne peuvent pas être les mêmes !");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    QStringList arbitreNames;

    if (ui->random_arbitre_match->isChecked()) {
        QSqlQuery query("SELECT NOM FROM ARBITRES");
        QStringList allArbitres;

        while (query.next()) {
            allArbitres.append(query.value(0).toString());
        }

        if (allArbitres.size() < 4) {
            QMessageBox::warning(this, "Erreur", "Pas assez d'arbitres pour une sélection aléatoire ");
            ui->pushButton->setEnabled(true);
            isInsertingMatch = false;
            return;
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allArbitres.begin(), allArbitres.end(), g);
        arbitreNames = allArbitres.mid(0, 4);
    } else {
        arbitreNames = {
            ui->comboBox_arbitre1->currentText(),
            ui->comboBox_arbitre2->currentText(),
            ui->comboBox_arbitre3->currentText(),
            ui->comboBox_arbitre4->currentText()
        };
    }

    // controle saisie arbitre
    QSet<QString> uniqueReferees(arbitreNames.begin(), arbitreNames.end());
    if (uniqueReferees.size() < 4) {
        QMessageBox::warning(this, "Erreur", "Les arbitres doivent être uniques !");
        ui->pushButton->setEnabled(true);
        isInsertingMatch = false;
        return;
    }

    // fill with empty strings if needed
    while (arbitreNames.size() < 4) {
        arbitreNames.append("");
    }

    QSqlQuery query;
    query.prepare("INSERT INTO MATCHES (DATE_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4) "
                  "VALUES (TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS'), :type, :equipe1, :equipe2, :score, :arbitre1, :arbitre2, :arbitre3, :arbitre4)");
    query.bindValue(":date", dateTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":type", type);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":score", "0-0");
    query.bindValue(":arbitre1", arbitreNames[0]);
    query.bindValue(":arbitre2", arbitreNames[1]);
    query.bindValue(":arbitre3", arbitreNames[2]);
    query.bindValue(":arbitre4", arbitreNames[3]);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Match ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Echec dans l'ajout: " + query.lastError().text());
    }

    ui->pushButton->setEnabled(true);
    isInsertingMatch = false;
}




//------------------------------------------AFFICHAGE PROG ET HISTORIQUE DES MATCHES-------------------------------------------------------------------------------------------------

void MainWindow::loadMatchesIntoTable()
{
    QSqlQuery query("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, DATE_MATCH, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4, SCOREEDIT FROM MATCHES");

    ui->programmation_2->setRowCount(0);
    ui->historique_table->setRowCount(0);

    QDateTime now = QDateTime::currentDateTime(); // Get current date and time
    int progRow = 0, histRow = 0;

    while (query.next()) {
        QDateTime matchDateTime = query.value(5).toDateTime(); // Full timestamp with hours/minutes
        int scoreEdit = query.value(10).toInt();
        QString score = query.value(4).toString();

        // Calculate elapsed time since match
        qint64 secondsSinceMatch = matchDateTime.secsTo(now);
        bool isOlderThan2_5Hours = secondsSinceMatch > (2.5 * 3600);

        QColor rowColor;
        if (!isOlderThan2_5Hours) {
            rowColor = QColor(255, 165, 0); // orange: less than 2.5h
        } else {
            rowColor = (scoreEdit == 0) ? QColor(255, 200, 200) : QColor(197, 255, 217); // red or green
        }

        // Historique table if match is in the past
        if (matchDateTime < now) {
            ui->historique_table->insertRow(histRow);
            for (int col = 0; col < 10; col++) {
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    item->setText(score);
                } else if (col == 5) {
                    item->setText(matchDateTime.toString("yyyy-MM-dd HH:mm:ss"));
                } else if (col == 6) {
                    item->setText(query.value(6).toString());
                } else if (col == 7) {
                    item->setText(query.value(7).toString());
                } else if (col == 8) {
                    item->setText(query.value(8).toString());
                } else if (col == 9) {
                    item->setText(query.value(9).toString());
                } else {
                    item->setText(query.value(col < 4 ? col : col + 2).toString());
                }

                item->setBackground(rowColor);
                ui->historique_table->setItem(histRow, col, item);
            }
            histRow++;
        }
        // Programmation table if match is upcoming or current
        else {
            ui->programmation_2->insertRow(progRow);

            for (int col = 0; col < 9; col++) {
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    item->setText(matchDateTime.toString("yyyy-MM-dd HH:mm:ss"));
                } else if (col == 5) {
                    item->setText(query.value(6).toString());
                } else if (col == 6) {
                    item->setText(query.value(7).toString());
                } else if (col == 7) {
                    item->setText(query.value(8).toString());
                } else if (col == 8) {
                    item->setText(query.value(9).toString());
                } else {
                    item->setText(query.value(col < 4 ? col : col + 1).toString());
                }

                ui->programmation_2->setItem(progRow, col, item);
            }

            ui->programmation_2->setItem(progRow, 9, new QTableWidgetItem(""));

            predictWinner(progRow);

            progRow++;
        }
    }
}




//---------------------------------------RECHERCHE MATCHES----------------------------------------------------------------------------------------------

void MainWindow::filterTable()
{
    QString filterText = ui->chercher_match->text().trimmed();
    QString filterOption = ui->tri_prog_match->currentText();

    for (int i = 0; i < ui->programmation_2->rowCount(); i++) {
        bool match = false;

        if (filterOption == "date") {
            QString dateValue = ui->programmation_2->item(i, 4)->text();
            match = dateValue.contains(filterText, Qt::CaseInsensitive);
        } else if (filterOption == "équipe") {
            QString equipe1 = ui->programmation_2->item(i, 2)->text();
            QString equipe2 = ui->programmation_2->item(i, 3)->text();
            match = equipe1.contains(filterText, Qt::CaseInsensitive) || equipe2.contains(filterText, Qt::CaseInsensitive);
        }

        ui->programmation_2->setRowHidden(i, !match);
    }


    QString filterText2 = ui->chercher_histo->text().trimmed();
    QString filterOption2 = ui->tri_histo_match->currentText();

    for (int i = 0; i < ui->historique_table->rowCount(); i++) {
        bool match = false;

        if (filterOption2 == "date") {
            QString dateValue = ui->historique_table->item(i, 5)->text(); // Indice 5 pour la date
            match = dateValue.contains(filterText2, Qt::CaseInsensitive);
        } else if (filterOption2 == "équipe") {
            QString equipe1 = ui->historique_table->item(i, 2)->text();
            QString equipe2 = ui->historique_table->item(i, 3)->text();
            match = equipe1.contains(filterText2, Qt::CaseInsensitive) || equipe2.contains(filterText2, Qt::CaseInsensitive);
        }

        ui->historique_table->setRowHidden(i, !match);
    }}

// =============Malek=================


void MainWindow::on_SendARD_clicked(){
    scene1->getChampDB();
    qDebug() << "Available serial ports:";
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << " - " << info.portName() << ":" << info.description();
    }
}




void MainWindow::ajoutChampB_clicked(){

    bool test = validateInputs();

    if (test == true){

    int nbr_eq = ui->lineEdit_nbrE->text().toInt();
    QString type = ui->comboBox_T->currentText();
    QString nom = ui->lineEdit_nom->text();
    int pool = ui->lineEdit_PG->text().toInt();
    QString Org = ui->lineEdit_Org->text();


    Championnats champ(nbr_eq, type, nom, Org, pool);


    bool success = champ.ajoutChamp();


    if (success) {
        QMessageBox::information(this, "Success", "Champs added successfully!");
        freeallfields();
        loadChampData();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the Champs: " + QSqlDatabase::database().lastError().text());
    }

    }else{
        QMessageBox::critical(this, "error", "Certains champs ne sont pas valides ");
    }
}

void MainWindow::rechrecheparnom() {
    QString searchTerm = ui->recherche_LI->text().trimmed();


    loadChampData(searchTerm);
}

/*void MainWindow::tri() {
    int ind = ui->tri_opts->currentIndex();
    switch (ind) {
    case 1: ui->tableWidget->sortItems(5, Qt::AscendingOrder); break;  // Pool Prize ASC
    case 2: ui->tableWidget->sortItems(5, Qt::DescendingOrder); break; // Pool Prize DESC
    case 3: ui->tableWidget->sortItems(1, Qt::AscendingOrder); break;  // Number of Teams ASC
    case 4: ui->tableWidget->sortItems(1, Qt::DescendingOrder); break; // Number of Teams DESC
    default: break;
    }
}*/

void MainWindow::loadForUpdate(int ind) {

    QSqlQuery query;
    int id = ui->tableWidget->item(ind, 0)->text().toInt();

    query.prepare("SELECT NBR_EQUIPE, TYPE, NOM, ORGANIZATEUR, POOL_GAINS FROM CHAMPIONNATS WHERE ID_CHAMP = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Fill the update form but DO NOT update the database yet
        ui->majNbrE->setText(query.value(0).toString());
        QString typeValue = query.value(1).toString();
        int index = ui->majT->findText(typeValue); // Find the corresponding index
        if (index != -1) {
            ui->majT->setCurrentIndex(index); // Set the selected value
        }
        ui->majNom->setText(query.value(2).toString());
        ui->majOrg->setText(query.value(3).toString());
        ui->majPG->setText(query.value(4).toString());

        // Store ID for later update
        ui->IDchamphidden->setText(QString::number(id));
        ui->tabWidget->setCurrentIndex(3);
    } else {
        qDebug() << "Error fetching championnat: " << query.lastError().text();
    }
}
void MainWindow::modifChampB_clicked(){


    bool test = validateInputsUP();

    if(test == true ){
        int id = ui->IDchamphidden->text().toInt();
        int nbrEquipe = ui->majNbrE->text().toInt();
        QString type = ui->majT->currentText();
        QString nom = ui->majNom->text();
        QString org = ui->majOrg->text();
        int pool = ui->majPG->text().toInt();

        Championnats champ(nbrEquipe, type, nom, org, pool);

        champ.setIdChamp(id);

        bool success = champ.saveUpdates();

        if (success) {
            QMessageBox::information(nullptr, "Updated", "Championship updated successfully.");
            loadChampData();
            freeallfields();
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to update championship.");
        }
    }

    else {
        QMessageBox::critical(this, "error", "Certains champs ne sont pas valides ");
    }

}


// ============endmalek==============


//-----------------------------------------SUPPRIMER MATCH---------------------------------------------------------------------------------------------------
void MainWindow::deleteMatch()
{
    int row = ui->programmation_2->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un match à supprimer.");
        return;
    }

    QString id = ui->programmation_2->item(row, 0)->text();
    QString equipe1 = ui->programmation_2->item(row, 2)->text();
    QString equipe2 = ui->programmation_2->item(row, 3)->text();
    QString dateMatch = ui->programmation_2->item(row, 4)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer le match de " + equipe1 +
                                                            " et " + equipe2 + " prévu le " + dateMatch + " ?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    // 1. Récupérer les données du match
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT TYPE_MATCH, DATE_MATCH, EQUIPE1, EQUIPE2, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4 "
                        "FROM MATCHES WHERE ID_MATCH = :id");
    selectQuery.bindValue(":id", id);

    if (selectQuery.exec() && selectQuery.next()) {
        QString typeMatch = selectQuery.value(0).toString();
        QDateTime dateMatchDB = selectQuery.value(1).toDateTime(); // Correct type for TIMESTAMP
        QString eq1 = selectQuery.value(2).toString();
        QString eq2 = selectQuery.value(3).toString();
        QString arb1 = selectQuery.value(4).toString();
        QString arb2 = selectQuery.value(5).toString();
        QString arb3 = selectQuery.value(6).toString();
        QString arb4 = selectQuery.value(7).toString();

        // 2. Insérer dans CANCELLED_MATCH
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO CANCELLED_MATCH (ID_MATCH, TYPE_MATCH, DATE_MATCH, EQUIPE1, EQUIPE2, "
                            "ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4) "
                            "VALUES (:id, :type, :date, :eq1, :eq2, :arb1, :arb2, :arb3, :arb4)");
        insertQuery.bindValue(":id", id);
        insertQuery.bindValue(":type", typeMatch);
        insertQuery.bindValue(":date", dateMatchDB); // Correct binding for TIMESTAMP
        insertQuery.bindValue(":eq1", eq1);
        insertQuery.bindValue(":eq2", eq2);
        insertQuery.bindValue(":arb1", arb1);
        insertQuery.bindValue(":arb2", arb2);
        insertQuery.bindValue(":arb3", arb3);
        insertQuery.bindValue(":arb4", arb4);

        if (!insertQuery.exec()) {
            qDebug() << "Erreur d'insertion dans CANCELLED_MATCH:" << insertQuery.lastError().text();
            QMessageBox::critical(this, "Erreur", "Échec de la sauvegarde du match annulé.");
            return;
        }

        // 3. Supprimer le match de MATCHES
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM MATCHES WHERE ID_MATCH = :id");
        deleteQuery.bindValue(":id", id);

        if (deleteQuery.exec()) {
            QMessageBox::information(this, "Suppression réussie", "Le match de " + equipe1 + " et " + equipe2 +
                                                                      " prévu le " + dateMatch + " a été supprimé avec succès !");
            ui->programmation_2->removeRow(row);
        } else {
            QString error = deleteQuery.lastError().text();
            qDebug() << "SQL Error: " << error;
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression du match dans la base de données : " + error);
        }
    } else {
        qDebug() << "Erreur de récupération des données:" << selectQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les détails du match avant suppression.");
    }
}

//------------------------------------GESTION ARBITRES DANS MATCHES --------------------------------------------------------------------------------------
void MainWindow::loadRefereesIntoComboBox() {
    QSqlQuery query("SELECT NOM FROM ARBITRES");

    if (!query.exec("SELECT NOM FROM ARBITRES")) {
           qDebug() << "Query failed (arbitres):" << query.lastError().text();
           return;
       }

    while (query.next()) {
        QString refereeName = query.value(0).toString();
        ui->comboBox_arbitre1->addItem(refereeName);
        ui->comboBox_arbitre2->addItem(refereeName);
        ui->comboBox_arbitre3->addItem(refereeName);
        ui->comboBox_arbitre4->addItem(refereeName);
    }

}
void MainWindow::handleRandomReferees(int state) {
    bool randomMode = (state == Qt::Checked);

    ui->comboBox_arbitre1->setEnabled(!randomMode);
    ui->comboBox_arbitre2->setEnabled(!randomMode);
    ui->comboBox_arbitre3->setEnabled(!randomMode);
    ui->comboBox_arbitre4->setEnabled(!randomMode);

    if (randomMode) {
        QSqlQuery query("SELECT NOM FROM ARBITRES");
        QStringList allArbitres;

        while (query.next()) {
            allArbitres.append(query.value(0).toString());
        }

        if (allArbitres.size() < 4) {
            QMessageBox::warning(this, "Erreur", "Il n'y a pas assez d'arbitres pour une sélection aléatoire.");
            ui->random_arbitre_match->setChecked(false);
            ui->comboBox_arbitre1->setEnabled(true);
            ui->comboBox_arbitre2->setEnabled(true);
            ui->comboBox_arbitre3->setEnabled(true);
            ui->comboBox_arbitre4->setEnabled(true);
            return;
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allArbitres.begin(), allArbitres.end(), g);

        // Assign unique referees
        ui->comboBox_arbitre1->setCurrentText(allArbitres.at(0));
        ui->comboBox_arbitre2->setCurrentText(allArbitres.at(1));
        ui->comboBox_arbitre3->setCurrentText(allArbitres.at(2));
        ui->comboBox_arbitre4->setCurrentText(allArbitres.at(3));
    }
}




//---------------------------------GESTION EQUIPES DANS MATCHES-----------------------------------------------------------------------------------------------------

void MainWindow::loadEquipes() {
    QSqlQuery query("SELECT NOMEQUIPE FROM EQUIPE");

    ui->comboBox_2->clear();
    ui->comboBox_3->clear();

    while (query.next()) {
        QString equipe = query.value(0).toString();
        ui->comboBox_2->addItem(equipe);
        ui->comboBox_3->addItem(equipe);
    }
}



//------------------------------------MODIFICATION PROGRAMMATION MATCHES-------------------------------------------------------------------------------



//----------
QString MainWindow::getColumnName(int column)
{
    // This function maps column indices to the actual column names in the database
    switch (column) {
    case 0: return "ID_MATCH";
    case 1: return "TYPE_MATCH";
    case 2: return "EQUIPE1";
    case 3: return "EQUIPE2";
    case 4: return "DATE_MATCH";
    case 5: return "ID_ARBITRE1";
    case 6: return "ID_ARBITRE2";
    case 7: return "ID_ARBITRE3";
    case 8: return "ID_ARBITRE4";
    case 9: return "ATTENTES";
    default: return "";
    }
}
//------------


void MainWindow::onCellDoubleClicked(int row, int column)
{
    if (!ui->programmation_2->item(row, column)) {
        qDebug() << "Cell at row " << row << ", column " << column << " is null!";
        return;
    }

    QString oldValue = ui->programmation_2->item(row, column)->text();
    QString columnName = getColumnName(column);

    if (columnName == "ID_MATCH") {
        QMessageBox::warning(this, "Modification Interdite", "Vous ne pouvez pas modifier l'ID du match.");
        return;
    }

    qDebug() << "Clicked column: " << column << ", Column Name: " << columnName << ", Current Value: " << oldValue;


    if (columnName == "ATTENTES") {
        QMessageBox::warning(this, "Modification Interdite", "La prédiction est générée automatiquement en se basant sur l'historique des résultats");
        return;
    }

    qDebug() << "Clicked column: " << column << ", Column Name: " << columnName << ", Current Value: " << oldValue;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation :", "Êtes-vous sûr de vouloir modifier cette valeur ?", QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    bool ok = false;
    QString newValue;

    if (columnName == "TYPE_MATCH") {
        QStringList typeOptions = { "Amical", "Groupe", "Championnat", "Coupe", "Qualification", "PlayOff" };
        newValue = QInputDialog::getItem(this, "Modification :", "Sélectionnez un nouveau type:", typeOptions, 0, false, &ok);
    }

    else if (columnName == "EQUIPE1" || columnName == "EQUIPE2") {
        QStringList teamOptions;
        QSqlQuery query("SELECT NOMEQUIPE FROM EQUIPE");

        while (query.next()) {
            teamOptions << query.value(0).toString();
        }

        if (teamOptions.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucune équipe trouvée dans la base de données.");
            return;
        }

        newValue = QInputDialog::getItem(this, "Modification :", "Sélectionnez une nouvelle équipe :", teamOptions, 0, false, &ok);

        if (ok && !newValue.isEmpty()) {
            QString equipe1 = (columnName == "EQUIPE1") ? newValue : ui->programmation_2->item(row, 2)->text();
            QString equipe2 = (columnName == "EQUIPE2") ? newValue : ui->programmation_2->item(row, 3)->text();

            if (equipe1 == equipe2) {
                QMessageBox::warning(this, "Erreur", "Les deux équipes ne peuvent pas être les mêmes !");
                return;
            }
        }
    }

    else if (columnName.toUpper().contains("ID_ARBITRE")) {
        QStringList refereeNames;
        QSqlQuery query("SELECT NOM FROM ARBITRES");

        while (query.next()) {
            refereeNames << query.value(0).toString();
        }

        if (refereeNames.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun arbitre trouvé dans la base de données.");
            return;
        }

        QString selectedNom = QInputDialog::getItem(this, "Modification :", "Sélectionnez un arbitre :", refereeNames, 0, false, &ok);
        if (ok && !selectedNom.isEmpty()) {
            newValue = selectedNom;

            for (int i = 5; i <= 8; ++i) {
                if (i != column) {
                    QTableWidgetItem* item = ui->programmation_2->item(row, i);
                    if (item && item->text() == newValue) {
                        QMessageBox::warning(this, "Erreur", "Cet arbitre est déjà assigné à ce match. Veuillez en choisir un autre.");
                        return;
                    }
                }
            }
        }
    }

    else if (columnName == "DATE_MATCH") {
        QDateTime currentDateTime = QDateTime::fromString(oldValue, "yyyy-MM-dd HH:mm:ss");
        QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(currentDateTime);
        dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        dateTimeEdit->setCalendarPopup(true);
        dateTimeEdit->setDateTime(currentDateTime);

        QDialog *dateDialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout(dateDialog);
        layout->addWidget(dateTimeEdit);
        QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::accepted, dateDialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, dateDialog, &QDialog::reject);

        if (dateDialog->exec() == QDialog::Accepted) {
            newValue = dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
            ok = true;
        } else {
            delete dateDialog;
            return;
        }

        delete dateDialog;
    }

    if (!ok || newValue.isEmpty()) {
        return;
    }

    ui->programmation_2->item(row, column)->setText(newValue);

    QSqlQuery query;
    if (columnName == "DATE_MATCH") {
        query.prepare("UPDATE MATCHES SET DATE_MATCH = TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS') WHERE ID_MATCH = :id_match");
        query.bindValue(":date", newValue);
    } else {
        query.prepare("UPDATE MATCHES SET " + columnName + " = :value WHERE ID_MATCH = :id_match");
        query.bindValue(":value", newValue);
    }

    query.bindValue(":id_match", ui->programmation_2->item(row, 0)->text());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "La valeur a été modifiée avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de la valeur : " + query.lastError().text());
    }
}







//-------------------------------------MODIFICATION HISTORIQUE MATCHES---------------------------------------------------------------------------------------
void MainWindow::onHistoriqueCellDoubleClicked(int row, int col)
{
    if (col == 4) {
        QDateTime now = QDateTime::currentDateTime();
        QString dateTimeString = ui->historique_table->item(row, 5)->text().trimmed();

        // Try multiple formats
        QDateTime matchDateTime = QDateTime::fromString(dateTimeString, Qt::ISODateWithMs); // yyyy-MM-ddTHH:mm:ss.zzz
        if (!matchDateTime.isValid())
            matchDateTime = QDateTime::fromString(dateTimeString, "d/M/yyyy"); // fallback like 5/5/2025
        if (!matchDateTime.isValid())
            matchDateTime = QDateTime::fromString(dateTimeString, "dd/MM/yyyy"); // fallback like 05/05/2025

        if (!matchDateTime.isValid()) {
            QMessageBox::warning(this, "Erreur", "Format de date invalide pour ce match : " + dateTimeString);
            return;
        }

        qint64 secondsDiff = matchDateTime.secsTo(now);
        bool allowTracking = (secondsDiff >= 0 && secondsDiff <= 9000); // 2.5 hours

        if (allowTracking) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Match en cours");
            msgBox.setText("Le match est toujours en cours.\nChoisissez comment vous souhaitez mettre à jour le score :");
            QPushButton *manualButton = msgBox.addButton("Manuellement", QMessageBox::AcceptRole);
            QPushButton *realtimeButton = msgBox.addButton("Suivi en temps réel", QMessageBox::RejectRole);
            msgBox.exec();

            if (msgBox.clickedButton() == realtimeButton) {
                QString team1Name = ui->historique_table->item(row, 2)->text();
                QString team2Name = ui->historique_table->item(row, 3)->text();

                Arduinoshiraz realtimeDialog(this, team1Name, team2Name);
                if (realtimeDialog.exec() == QDialog::Accepted) {
                    QString finalScore = realtimeDialog.getFinalScore();
                    ui->historique_table->item(row, col)->setText(finalScore);

                    QStringList scores = finalScore.split("-");
                    int scoreTeam1 = scores[0].toInt();
                    int scoreTeam2 = scores[1].toInt();

                    QString winner;
                    if (scoreTeam1 > scoreTeam2) {
                        winner = team1Name;
                    } else if (scoreTeam1 < scoreTeam2) {
                        winner = team2Name;
                    } else {
                        winner = "égalité";
                    }

                    QSqlQuery query;
                    query.prepare("UPDATE MATCHES SET SCORE = :score, WINNER = :winner, SCOREEDIT = 1 WHERE ID_MATCH = :id_match");
                    query.bindValue(":score", finalScore);
                    query.bindValue(":winner", winner);
                    query.bindValue(":id_match", ui->historique_table->item(row, 0)->text());

                    if (query.exec()) {
                        QMessageBox::information(this, "Succès", "Score mis à jour avec succès !");
                    } else {
                        QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du score dans la base de données.");
                    }
                }
                return;
            }
        }

        // Manual edit
        QString oldScore = ui->historique_table->item(row, col)->text();
        QRegularExpression regExp("^\\d+-\\d+$");
        QRegularExpressionValidator validator(regExp, this);

        bool ok;
        QString newScore = QInputDialog::getText(this, "Modifier le score", "Nouveau score :", QLineEdit::Normal,
                                                 oldScore, &ok);

        if (ok && !newScore.isEmpty()) {
            int pos = 0;
            if (validator.validate(newScore, pos) == QValidator::Acceptable) {
                ui->historique_table->item(row, col)->setText(newScore);

                QStringList scores = newScore.split("-");
                int scoreTeam1 = scores[0].toInt();
                int scoreTeam2 = scores[1].toInt();

                QString winner;
                if (scoreTeam1 > scoreTeam2) {
                    winner = ui->historique_table->item(row, 2)->text();
                } else if (scoreTeam1 < scoreTeam2) {
                    winner = ui->historique_table->item(row, 3)->text();
                } else {
                    winner = "égalité";
                }

                QSqlQuery query;
                query.prepare("UPDATE MATCHES SET SCORE = :score, WINNER = :winner, SCOREEDIT = 1 WHERE ID_MATCH = :id_match");
                query.bindValue(":score", newScore);
                query.bindValue(":winner", winner);
                query.bindValue(":id_match", ui->historique_table->item(row, 0)->text());

                if (query.exec()) {
                    QMessageBox::information(this, "Succès", "Score mis à jour avec succès !");
                } else {
                    QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du score dans la base de données.");
                }
            } else {
                QMessageBox::warning(this, "Format invalide", "Veuillez saisir un score valide au format X-Y.");
            }
        }
    }
}


//--------------------------------------------DESIGN HISTORIQUE-----------------------------------------------------------------------------------------

void MainWindow::onItemChanged(QTableWidgetItem *item)
{
    int row = item->row();
    int col = item->column();

    // Check if we are editing the SCORE column (index 4)
    if (col == 4) {
        QString newScore = item->text();
        QString idMatch = ui->historique_table->item(row, 0)->text(); // ID_MATCH is in column 0

        // Fetch SCOREEDIT and DATE_MATCH from the database
        QSqlQuery query;
        query.prepare("SELECT SCOREEDIT, DATE_MATCH FROM MATCHES WHERE ID_MATCH = :id_match");
        query.bindValue(":id_match", idMatch);

        if (query.exec() && query.next()) {
            int scoreEditValue = query.value(0).toInt();
            QDateTime matchDateTime = query.value(1).toDateTime();
            QDateTime now = QDateTime::currentDateTime();

            qint64 diffInSeconds = matchDateTime.secsTo(now);
            bool isOlderThan2_5Hours = diffInSeconds > (2.5 * 3600); // 2.5 hours in seconds

            QColor rowColor;

            if (!isOlderThan2_5Hours) {
                // Match ended less than 2.5 hours ago → orange always
                rowColor = QColor(255, 165, 0); // Orange
            } else {
                // Match ended more than 2.5 hours ago → use original logic
                if (newScore == "0-0") {
                    rowColor = (scoreEditValue == 1) ? QColor(197, 255, 217) : QColor(255, 200, 200); // Green or Red
                } else {
                    rowColor = QColor(197, 255, 217); // Green
                }
            }

            // Apply the background color to the entire row
            for (int c = 0; c < ui->historique_table->columnCount(); ++c) {
                QTableWidgetItem *rowItem = ui->historique_table->item(row, c);
                if (rowItem) {
                    rowItem->setBackground(rowColor);
                }
            }
        }
    }
}



void MainWindow::checkForNullScoreEdit()
{

    QDate today = QDate::currentDate();
    QSqlQuery query("SELECT SCOREEDIT, DATE_MATCH FROM MATCHES WHERE SCOREEDIT IS NULL OR SCOREEDIT = '0'");
    QList<QLabel*> existingLabels = ui->widget_7_match->findChildren<QLabel*>();
    for (QLabel* label : existingLabels) {
        if (label != ui->label_21_match) {
            label->deleteLater();
        }
    }

    bool labelDisplayed = false;
    while (query.next()) {
        QString scoreEdit = query.value(0).toString();
        QDateTime dateTimeMatch = query.value(1).toDateTime();
        QDate matchDate = dateTimeMatch.date();
        qDebug() << "ScoreEdit: " << scoreEdit << ", Match Date: " << matchDate.toString() << ", Today: " << today.toString();

        if ((scoreEdit.isEmpty() || scoreEdit == "0") && matchDate.isValid() && matchDate < today) {

            QLabel *label = new QLabel("❗", ui->widget_7_match);
            label->setStyleSheet("font-size: 15px; color: red;");
            label->move(10, 10);
            label->show();
            labelDisplayed = true;
            break;
        }
    }

    if (!labelDisplayed) {
        qDebug() << "No matching conditions found.";
    }
}



//-------------------------------------ELARGIR WIDGETS-------------------------------------------------------------------------------

// Flag for show_2 expansion state
bool isExpandedShow2 = false;

// Flag for show_3 expansion state
bool isExpandedShow3 = false;

void MainWindow::onShowButtonClicked()
{
    // Save current dimensions of the widgets
    int currentTabWidth = ui->tabWidget_match->geometry().width();
    int currentTableWidth = ui->programmation_2->geometry().width();
    int currentWidget6Width = ui->widget_6_match->geometry().width();
    int currentDeleteMatchX = ui->delete_match->geometry().x();
    int currentChercherWidth = ui->chercher_match->geometry().width();
    int currentShow2X = ui->show_2->geometry().x();
    int currentTriProgX = ui->tri_prog->geometry().x();

    // Define the animation duration and the shift amount for all widgets
    int duration = 500;
    int shiftAmount = 280;

    // Create and configure the animation for tabWidget resizing
    QPropertyAnimation *tabWidgetAnimation = new QPropertyAnimation(ui->tabWidget_match, "geometry");
    tabWidgetAnimation->setDuration(duration);
    QRect tabWidgetNewRect = ui->tabWidget_match->geometry();
    tabWidgetNewRect.setWidth(isExpandedShow2 ? currentTabWidth - shiftAmount : currentTabWidth + shiftAmount);
    tabWidgetAnimation->setEndValue(tabWidgetNewRect);

    // Animation for the tableWidget resizing
    QPropertyAnimation *tableWidgetAnimation = new QPropertyAnimation(ui->programmation_2, "geometry");
    tableWidgetAnimation->setDuration(duration);
    QRect tableWidgetNewRect = ui->programmation_2->geometry();
    tableWidgetNewRect.setWidth(isExpandedShow2 ? currentTableWidth - shiftAmount : currentTableWidth + shiftAmount);
    tableWidgetAnimation->setEndValue(tableWidgetNewRect);

    // Animation for widget6 resizing and moving
    QPropertyAnimation *widget6Animation = new QPropertyAnimation(ui->widget_6_match, "geometry");
    widget6Animation->setDuration(duration);
    QRect widget6NewRect = ui->widget_6_match->geometry();
    widget6NewRect.setWidth(isExpandedShow2 ? currentWidget6Width - shiftAmount : currentWidget6Width + shiftAmount);
    widget6NewRect.moveLeft(isExpandedShow2 ? widget6NewRect.left() - shiftAmount : widget6NewRect.left() + shiftAmount);
    widget6Animation->setEndValue(widget6NewRect);

    // Animations for other widgets (delete_match, chercher, show_2, tri_prog)
    QPropertyAnimation *deleteMatchAnimation = new QPropertyAnimation(ui->delete_match, "geometry");
    deleteMatchAnimation->setDuration(duration);
    QRect deleteMatchNewRect = ui->delete_match->geometry();
    deleteMatchNewRect.moveLeft(isExpandedShow2 ? currentDeleteMatchX - shiftAmount : currentDeleteMatchX + shiftAmount);
    deleteMatchAnimation->setEndValue(deleteMatchNewRect);

    QPropertyAnimation *chercherAnimation = new QPropertyAnimation(ui->chercher_match, "geometry");
    chercherAnimation->setDuration(duration);
    QRect chercherNewRect = ui->chercher_match->geometry();
    chercherNewRect.setWidth(isExpandedShow2 ? currentChercherWidth - shiftAmount : currentChercherWidth + shiftAmount);
    chercherAnimation->setEndValue(chercherNewRect);

    QPropertyAnimation *show2ButtonAnimation = new QPropertyAnimation(ui->show_2, "geometry");
    show2ButtonAnimation->setDuration(duration);
    QRect show2ButtonNewRect = ui->show_2->geometry();
    show2ButtonNewRect.moveLeft(isExpandedShow2 ? currentShow2X - shiftAmount : currentShow2X + shiftAmount);
    show2ButtonAnimation->setEndValue(show2ButtonNewRect);

    QPropertyAnimation *triProgAnimation = new QPropertyAnimation(ui->tri_prog, "geometry");
    triProgAnimation->setDuration(duration);
    QRect triProgNewRect = ui->tri_prog->geometry();
    triProgNewRect.moveLeft(isExpandedShow2 ? currentTriProgX - shiftAmount : currentTriProgX + shiftAmount);
    triProgAnimation->setEndValue(triProgNewRect);

    // Start all animations
    tabWidgetAnimation->start();
    tableWidgetAnimation->start();
    widget6Animation->start();
    deleteMatchAnimation->start();
    chercherAnimation->start();
    show2ButtonAnimation->start();
    triProgAnimation->start();

    // Toggle the expanded state for show_2 only
    isExpandedShow2 = !isExpandedShow2;
}
void MainWindow::onShow3ButtonClicked()
{
    // Save the current dimensions and positions of the widgets
    int currentWidget7Width = ui->widget_7_match->geometry().width();
    int currentWidget8X = ui->widget_8_match->geometry().x();
    int currentHistoriqueTableWidth = ui->historique_table->geometry().width();
    int currentShow3X = ui->show_3->geometry().x();
    int currentTriHistoX = ui->tri_histo->geometry().x();
    int currentChercherHistoWidth = ui->chercher_histo->geometry().width();

    // Define the animation duration and the shift amount for all widgets
    int duration = 500;
    int shiftAmount = 280;

    // Create and configure the animation for widget_7 resizing
    QPropertyAnimation *widget7Animation = new QPropertyAnimation(ui->widget_7_match, "geometry");
    widget7Animation->setDuration(duration);
    QRect widget7NewRect = ui->widget_7_match->geometry();
    widget7NewRect.setWidth(isExpanded ? currentWidget7Width - shiftAmount : currentWidget7Width + shiftAmount);
    widget7Animation->setEndValue(widget7NewRect);

    // Create and configure the animation for widget_8 moving to the left (opposite direction)
    QPropertyAnimation *widget8Animation = new QPropertyAnimation(ui->widget_8_match, "geometry");
    widget8Animation->setDuration(duration);
    QRect widget8NewRect = ui->widget_8_match->geometry();
    widget8NewRect.moveLeft(isExpanded ? currentWidget8X - shiftAmount : currentWidget8X + shiftAmount); // Move widget_8
    widget8Animation->setEndValue(widget8NewRect);

    // Create and configure the animation for historique_table resizing
    QPropertyAnimation *historiqueTableAnimation = new QPropertyAnimation(ui->historique_table, "geometry");
    historiqueTableAnimation->setDuration(duration);
    QRect historiqueTableNewRect = ui->historique_table->geometry();
    historiqueTableNewRect.setWidth(isExpanded ? currentHistoriqueTableWidth - shiftAmount : currentHistoriqueTableWidth + shiftAmount);
    historiqueTableAnimation->setEndValue(historiqueTableNewRect);

    // Create and configure the animation for show_3 moving to the left
    QPropertyAnimation *show3ButtonAnimation = new QPropertyAnimation(ui->show_3, "geometry");
    show3ButtonAnimation->setDuration(duration);
    QRect show3ButtonNewRect = ui->show_3->geometry();
    show3ButtonNewRect.moveLeft(isExpanded ? currentShow3X - shiftAmount : currentShow3X + shiftAmount);
    show3ButtonAnimation->setEndValue(show3ButtonNewRect);

    // Create and configure the animation for tri_histo moving to the left
    QPropertyAnimation *triHistoAnimation = new QPropertyAnimation(ui->tri_histo, "geometry");
    triHistoAnimation->setDuration(duration);
    QRect triHistoNewRect = ui->tri_histo->geometry();
    triHistoNewRect.moveLeft(isExpanded ? currentTriHistoX - shiftAmount : currentTriHistoX + shiftAmount);
    triHistoAnimation->setEndValue(triHistoNewRect);

    // Create and configure the animation for chercher_histo resizing (making it larger)
    QPropertyAnimation *chercherHistoAnimation = new QPropertyAnimation(ui->chercher_histo, "geometry");
    chercherHistoAnimation->setDuration(duration);
    QRect chercherHistoNewRect = ui->chercher_histo->geometry();
    chercherHistoNewRect.setWidth(isExpanded ? currentChercherHistoWidth - shiftAmount : currentChercherHistoWidth + shiftAmount);
    chercherHistoAnimation->setEndValue(chercherHistoNewRect);



    // Start all animations
    widget7Animation->start();
    widget8Animation->start();
    historiqueTableAnimation->start();
    show3ButtonAnimation->start();
    triHistoAnimation->start();
    chercherHistoAnimation->start();


    // Toggle the expanded state
    isExpanded = !isExpanded;
}




//--------------------------------------EXPORTER PDF ---------------------------------------------------------------------------------------
void MainWindow::on_exporter_match_clicked() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Export");
    msgBox.setText("Choisissez ce que vous souhaitez exporter:");

    QPushButton *btnProgrammation = msgBox.addButton("Programmation", QMessageBox::AcceptRole);
    QPushButton *btnHistorique = msgBox.addButton("Historique", QMessageBox::AcceptRole);
    QPushButton *btnStatistics = msgBox.addButton("Statistiques", QMessageBox::RejectRole);

    msgBox.exec();

    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Exportation annulée", "Aucun fichier sélectionné.");
        return;
    }

    if (msgBox.clickedButton() == btnProgrammation) {
        exportTableToPDF(ui->programmation_2, filePath);
    }
    else if (msgBox.clickedButton() == btnHistorique) {
        exportTableToPDF(ui->historique_table, filePath);
    }
    else if (msgBox.clickedButton() == btnStatistics) {
        exportStatisticsToPDF(filePath);  // Pass the file path to avoid asking twice
    }
    else {
        QMessageBox::information(this, "Exportation annulée", ".");
    }
}


void MainWindow::exportTableToPDF(QTableWidget *table, const QString &filename)
{
    if (!table) return;

    QPdfWriter pdfWriter(filename);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    QFont font("Arial", 7);
    painter.setFont(font);

    int leftMargin = 20;
    int topMargin = 50;
    int rowHeight = 80;
    int textPadding = 10;
    int columnCount = table->columnCount();
    int pageWidth = pdfWriter.width() - 2 * leftMargin;
    int columnWidth = pageWidth / columnCount;

    int y = topMargin;

    // Load logo from resources
    QPixmap logo(":/interface_icons/log.png");

    if (logo.isNull()) {
        qDebug() << "Error loading logo image!";
    }

    // Increase logo size
    int logoWidth = 80;
    int logoHeight = 80;
    int logoX = pdfWriter.width() - logoWidth - 20;
    int logoY = 20;

    // Draw the logo
    painter.drawPixmap(logoX, logoY, logoWidth, logoHeight, logo);

    // Move the table lower by adjusting the top margin
    y += logoHeight + 20;

    // Draw table header
    painter.setPen(Qt::black);
    painter.setBrush(Qt::green);
    painter.drawRect(leftMargin, y, pageWidth, rowHeight);

    for (int col = 0; col < columnCount; ++col) {
        QString headerText = table->horizontalHeaderItem(col) ? table->horizontalHeaderItem(col)->text() : "";
        QRect headerRect(leftMargin + col * columnWidth + textPadding, y, columnWidth - 2 * textPadding, rowHeight);
        painter.drawText(headerRect, Qt::AlignCenter, headerText);
    }

    y += rowHeight;

    // Draw table rows
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < table->rowCount(); ++row) {
        painter.drawRect(leftMargin, y, pageWidth, rowHeight);

        for (int col = 0; col < columnCount; ++col) {
            QString cellText = table->item(row, col) ? table->item(row, col)->text() : "";
            QRect cellRect(leftMargin + col * columnWidth + textPadding, y, columnWidth - 2 * textPadding, rowHeight);
            painter.drawText(cellRect, Qt::AlignCenter, cellText);
        }
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Exportation réussie", "Le fichier PDF a été enregistré avec succès !");
}
void MainWindow::exportStatisticsToPDF(const QString &filePath) {

    if (!chartView) {
        QMessageBox::warning(this, "Erreur", "Aucun graphique à exporter !");
        return;
    }

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Export Canceled", "No file selected.");
        return;
    }

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);

    // Load logo image
    QImage logo(":/interface_icons/log.png");  // Use resource path or full path to logo
    if (logo.isNull()) {
        QMessageBox::warning(this, "Erreur", "pas d' image.");
    } else {
        // Scale the logo to a bigger size
        int scaledWidth = 80;  // Desired width for the logo
        int scaledHeight = 80; // Desired height for the logo
        QImage scaledLogo = logo.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio);

        // Calculate the top-right position based on the page width and logo width
        int xPos = writer.width() - scaledLogo.width() - 10;  // 10px margin from the right edge
        int yPos = 10; // 10px margin from the top

        // Draw the scaled logo at the top-right corner
        painter.drawImage(xPos, yPos, scaledLogo);
    }

    // Render the pie chart after drawing the logo
    chartView->render(&painter);

    // Retrieve data for the legend (month names and number of matches)
    QPieSeries *series = qobject_cast<QPieSeries *>(chartView->chart()->series().first());
    if (!series) {
        QMessageBox::warning(this, "erreur", "pas de data.");
        return;
    }

    // Set font for text (make it bigger)
    QFont font = painter.font();
    font.setPointSize(8); // Increase font size
    painter.setFont(font);

    // Draw the legend below the pie chart
    int x = 100; // X position for the legend
    int y = chartView->height() + 450; // Y position for the legend
    int rectangleSize = 15;  // Size of the color rectangle
    int rowHeight = rectangleSize + 50; // Spacing between rows

    // Month names
    QStringList monthNames = {
                              "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
                              "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"
    };
}


void MainWindow::SuppChamp(int ind){


    QSqlQuery query;

    int champID = ui->tableWidget->item(ind, 0)->text().toInt();


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this record?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;


    query.prepare("DELETE FROM CHAMPIONNATS WHERE ID_CHAMP = :id");
    query.bindValue(":id", champID);

    if (query.exec()) {
        QMessageBox::information(this, "Deleted", "Champ deleted successfully.");
        ui->tableWidget->removeRow(ind);
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete champ.");
        qDebug() << "Delete error: " << query.lastError().text();
    }
}

void MainWindow::loadChampData(const QString &searchTerm) {

    QSqlQuery query;
    if (searchTerm.isEmpty()) {
        query.prepare("SELECT * FROM CHAMPIONNATS");
    } else {
        // If search term is provided, search for similar names
        query.prepare("SELECT * FROM CHAMPIONNATS WHERE NOM LIKE :searchTerm");
        query.bindValue(":searchTerm", "%" + searchTerm + "%"); // Partial match
    }

    if (!query.exec()) {
        qDebug() << "Error retrieving data: " << query.lastError().text();
        return;
    }


    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);  // Insert a new row

        // Fill columns
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));

        QPushButton* deleteButton = new QPushButton("Delete");
        QPushButton* updateButton = new QPushButton("update");

        ui->tableWidget->setCellWidget(row, 6, deleteButton);
        ui->tableWidget->setCellWidget(row, 7, updateButton);

        connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
        SuppChamp(row);
        });
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
        loadForUpdate(row);
        });

        row++;
        StyleTW(ui->tableWidget);
    }
}

void MainWindow::sortByName()
{
    QString sortCriteria = ui->sortCC->currentText();
    QString sortOrder = ui->sortTP->currentText();


    QString order = (sortOrder == "Ascendant") ? "ASC" : "DESC";


    QString column;
    if (sortCriteria == "Pool de gains") {
        column = "POOL_GAINS";
    } else if (sortCriteria == "Nombre d'équipes") {
        column = "NBR_EQUIPE";
    } else if (sortCriteria == "Alphabétique") {
        column = "NOM";
    }



    QTableWidget *table = ui->tableWidget;


    QString sql = "SELECT * FROM CHAMPIONNATS ORDER BY " + column + " " + order;


    QSqlQuery query;
    query.prepare(sql);

    if (query.exec()) {

        table->clearContents();
        table->setRowCount(0);


        while (query.next()) {
            int row = table->rowCount();
            table->insertRow(row);


            table->setItem(row, 0, new QTableWidgetItem(query.value("ID_CHAMP").toString()));
            table->setItem(row, 1, new QTableWidgetItem(query.value("NBR_EQUIPE").toString()));
            table->setItem(row, 2, new QTableWidgetItem(query.value("TYPE").toString()));
            table->setItem(row, 3, new QTableWidgetItem(query.value("NOM").toString()));
            table->setItem(row, 4, new QTableWidgetItem(query.value("ORGANIZATEUR").toString()));
            table->setItem(row, 5, new QTableWidgetItem(query.value("POOL_GAINS").toString()));

            QPushButton* deleteButton = new QPushButton("Delete");
            QPushButton* updateButton = new QPushButton("update");

            ui->tableWidget->setCellWidget(row, 6, deleteButton);
            ui->tableWidget->setCellWidget(row, 7, updateButton);

            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                SuppChamp(row);
            });
            connect(updateButton, &QPushButton::clicked, this, [this, row]() {
                loadForUpdate(row);
            });
            StyleTW(ui->tableWidget);
        }
    }
    else {
        qDebug() << "Failed to execute query:" << query.lastError();
    }
}
void MainWindow::on_sortBut_clicked(){
    sortByName();
};



bool MainWindow::validateInputs() {
    bool allValid = true;


        static QRegularExpression alphaRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");


        if (ui->lineEdit_nom->text().trimmed().isEmpty() ||
            !alphaRegex.match(ui->lineEdit_nom->text().trimmed()).hasMatch() || ui->lineEdit_nom->text().length() < 7) {
            ui->err_nom->setText("Nom doit etre alphabetique de longuer 8 min");
            ui->err_nom->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->err_nom->setText("valide");
            ui->err_nom->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }


        if (ui->lineEdit_nbrE->text().trimmed().isEmpty() ||
            ui->lineEdit_nbrE->text().toInt() < 0 || ui->lineEdit_nbrE->text().toInt() > 56 || ui->lineEdit_nbrE->text().toInt() < 8) {
            ui->err_nbrE->setText("le nombre doit etre positive et entre 8 est 56!");
            ui->err_nbrE->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->err_nbrE->setText("valide");
            ui->err_nbrE->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }


        if (ui->comboBox_T->currentText() == "--SELECT--") {
            ui->err_T->setText("Choisissez une champ valide!");
            ui->err_T->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; "
                );
            allValid = false;
        } else {
            ui->err_T->setText("valide");
            ui->err_T->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; "
                );
        }

        if (ui->lineEdit_Org->text().trimmed().isEmpty() ||
            !alphaRegex.match(ui->lineEdit_Org->text().trimmed()).hasMatch() || ui->lineEdit_Org->text().length() < 7) {
            ui->err_Org->setText("le nom d'Org doit etre alphabetique !");
            ui->err_Org->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->err_Org->setText("valide");
            ui->err_Org->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }

        if (ui->lineEdit_PG->text().trimmed().isEmpty() ||
            ui->lineEdit_PG->text().toInt() < 0 || (ui->lineEdit_PG->text().toInt() / ui->lineEdit_nbrE->text().toInt()) <= 2000) {
            ui->err_pool->setText("le montant est issufisant (min 2000 par équipe)!");
            ui->err_pool->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->err_pool->setText("valide");
            ui->err_pool->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }

        return allValid;

        qDebug() << "validateInputs() called";
}

bool MainWindow::validateInputsUP() {
    bool allValid = true;


    static QRegularExpression alphaRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");


    if (ui->majNom->text().trimmed().isEmpty() ||
        !alphaRegex.match(ui->majNom->text().trimmed()).hasMatch() || ui->majNom->text().length() < 7) {
        ui->err_nom_2->setText("Nom doit etre alphabetique de longuer 8 min");
        ui->err_nom_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {

        ui->err_nom_2->setText("valide");
        ui->err_nom_2->setStyleSheet(

            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }
    if (ui->majNbrE->text().trimmed().isEmpty() ||
        ui->majNbrE->text().toInt() < 0 || ui->majNbrE->text().toInt() > 56 || ui->majNbrE->text().toInt() < 8) {
        ui->err_nbrE_2->setText("le nombre doit etre positive et entre 8 est 56!");
        ui->err_nbrE_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_nbrE_2->setText("valide");
        ui->err_nbrE_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    if (ui->majT->currentText() == "--SELECT--") {
        ui->err_T_2->setText("Choisissez une champ valide!");
        ui->err_T_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; "
            );
        allValid = false;
    } else {
        ui->err_T_2->setText("valide");
        ui->err_T_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; "
            );
    }

    if (ui->majOrg->text().trimmed().isEmpty() ||
        !alphaRegex.match(ui->majOrg->text().trimmed()).hasMatch() || ui->majOrg->text().length() < 7) {
        ui->err_Org_2->setText("le nom d'Org doit etre alphabetique !");
        ui->err_Org_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_Org_2->setText("valide");
        ui->err_Org_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }

    if (ui->majPG->text().trimmed().isEmpty() ||
        ui->majPG->text().toInt() < 0 || (ui->majPG->text().toInt() / ui->majNbrE->text().toInt()) <= 2000) {
        ui->err_pool_2->setText("le montant est issufisant (min 2000 par équipe)!");
        ui->err_pool_2->setStyleSheet(
            "color: #D32F2F; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
        allValid = false;
    } else {
        ui->err_pool_2->setText("valide");
        ui->err_pool_2->setStyleSheet(
            "color: #2E7D32; "
            "font-size: 10px; "
            "font-weight: bold; "
            "padding: 2px; ");
    }


    return allValid;
}



void MainWindow::freeallfields(){
    ui->lineEdit_nbrE->setText("");
    ui->lineEdit_nom->setText("");
    ui->lineEdit_Org->setText("");
    ui->lineEdit_PG->setText("");
    //ui->comboBox_T->setCurrentIndex(0);

    ui->majNbrE->setText("");
    ui->majNom->setText("");
    ui->majOrg->setText("");
    ui->majPG->setText("");
    //ui->majT->setCurrentIndex(0);
}

#include <QAxObject>

void MainWindow::exportToExcel() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", "C:/Utilisateurs/ADMIN/", "Excel Files (*.xlsx)");
    if (filePath.isEmpty()) return;
    QAxObject *excel = new QAxObject("Excel.Application", nullptr);
    excel->setProperty("Visible", false);
    QAxObject *workbook = excel->querySubObject("Workbooks")->querySubObject("Add");
    QAxObject *sheet = workbook->querySubObject("Sheets(int)", 1);

    int rowCount = ui->tableWidget->rowCount();
    int colCount = ui->tableWidget->columnCount();
    colCount -= 2;

    // Set Headers
    for (int col = 0; col < colCount; ++col) {
        sheet->querySubObject("Cells(int,int)", 1, col + 1)->setProperty("Value", ui->tableWidget->horizontalHeaderItem(col)->text());
    }


    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item) {
                sheet->querySubObject("Cells(int,int)", row + 2, col + 1)->setProperty("Value", item->text());
            }
        }
    }

    // Save and Close
    if (!filePath.isEmpty()) {
        workbook->querySubObject("SaveAs(const QString&)", filePath.replace("/", "\\"), 51);
    }

    workbook->querySubObject("Close");
    excel->querySubObject("Quit");

    delete sheet;
    delete workbook;
    delete excel;

    QMessageBox::information(this, "Export", "Exportation reussite.");
}

void MainWindow::exportToPDF() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");

    if (filePath.isEmpty()) {
        return;
    }

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);
    QPainter painter(&writer);

    if (!painter.isActive()) {
        QMessageBox::warning(this, "Export Failed", "Failed to open PDF for writing.");
        return;
    }

    int margin = 100;
    int titleHeight = 200;
    int rowHeight = 150;
    int columnWidth =350;
    int yPosition = margin;


    QFont titleFont("Arial", 14, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(margin, yPosition, "Championnats Report");


    QFont dateFont("Arial", 10);
    painter.setFont(dateFont);
    QString date = "Date: " + QDate::currentDate().toString("dd/MM/yyyy");
    painter.drawText(writer.width() - margin * 10, yPosition, date);  // Right-aligned date

    yPosition += titleHeight;


    QFont headerFont("Arial", 9, QFont::Bold);
    painter.setFont(headerFont);

    QStringList headers = {"ID", "Nb Equipes", "Type", "nom", "Organisateur", "Pool"};

    int xPosition = margin;
    for (const QString &header : headers) {
        painter.drawText(xPosition, yPosition, header);
        xPosition += columnWidth;
    }


    painter.drawLine(margin, yPosition + 20, xPosition, yPosition + 20);
    yPosition += rowHeight;  // Move to first data row

    QFont rowFont("Arial", 8);
    painter.setFont(rowFont);

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        xPosition = margin;
        for (int col = 0; col < headers.size(); ++col) {
            if (ui->tableWidget->item(row, col)) {
                painter.drawText(xPosition, yPosition, ui->tableWidget->item(row, col)->text());
            }
            xPosition += columnWidth;
        }
        // *Draw Row Line*
        painter.drawLine(margin, yPosition + 10, xPosition, yPosition + 10);
        yPosition += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Exportation Réussie", "PDF exporté avec succès à: " + filePath);
}

void MainWindow::selectExp(){
    QString type = ui->type_f->currentText();

    if (type == "Exel"){
       exportToExcel();
    }
    else{
       exportToPDF();
    }
}

void MainWindow::on_statGEN_clicked(){
    StatisticsContainer *statsWindow = new StatisticsContainer();
    statsWindow->setAttribute(Qt::WA_DeleteOnClose); // Auto delete on close
    statsWindow->setWindowTitle("Statistics");
    statsWindow->resize(800, 600);  // Optional: adjust size
    statsWindow->show();
}

void MainWindow::loadCBchampL() {
    QSqlQuery query;
    if (query.exec("SELECT NOM FROM CHAMPIONNATS")) {
        ui->listChamp->clear();
        while (query.next()) {
            QString nom = query.value(0).toString();
            ui->listChamp->addItem(nom);
            //ui->listChamp_2->addItem(nom);
        }
    } else {
        qDebug() << "Failed to load championnats:" << query.lastError().text();
    }
}

void MainWindow::loadChampL(){
    QSqlQuery query;
    if (query.exec("SELECT NOM FROM CHAMPIONNATS")) {
        ui->listChamp->clear();
        while (query.next()) {
            QString nom = query.value(0).toString();
            ui->listChamp_2->addItem(nom);
            ui->listChamp_3->addItem(nom);
        }
    } else {
        qDebug() << "Failed to load championnats:" << query.lastError().text();
    }
}

void MainWindow::on_MsL_currentIndexChanged(const QString &selectedText) {
     qDebug() << "Selected text: " << selectedText;
    if (selectedText.trimmed() == "Un seul championnat") {
        ui->listChamp->setEnabled(true);
        loadCBchampL();
    } else {
        ui->listChamp->setEnabled(false);
        ui->listChamp->clear();
    }
}

void MainWindow::displayCoupeBracket(const QString& champname) {
    QSqlQuery query;
    QString sql = QString("SELECT E1.NOMEQUIPE AS HomeTeam, E2.NOMEQUIPE AS VisitorTeam, "
                          "M.DATE_MATCH AS MatchDate, M.ID_MATCH AS MatchID, M.SCORE AS MatchScore, "
                          "M.TYPE AS MatchType "
                          "FROM MATCHESC M "
                          "JOIN JOUER J ON M.ID_MATCH = J.ID_MATCH "
                          "JOIN EQUIPE E1 ON J.ID_EQUIPEH = E1.IDEQUIPE "
                          "JOIN EQUIPE E2 ON J.ID_EQUIPEV = E2.IDEQUIPE "
                          "WHERE M.NOM_CHAMP = :champname "
                          "ORDER BY M.DATE_MATCH");

    query.prepare(sql);
    query.bindValue(":champname", champname);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return;
    }

    QMap<QString, QStringList> matchStages;
    QString finalWinner;

    while (query.next()) {
        QString homeTeam = query.value("HomeTeam").toString();
        QString visitorTeam = query.value("VisitorTeam").toString();
        QString score = query.value("MatchScore").toString();
        QString matchType = query.value("MatchType").toString();

        QString winner = "Draw";
        if (score.contains("-")) {
            QStringList scoreParts = score.split("-");
            if (scoreParts.size() == 2) {
                int visitorScore = scoreParts[0].toInt();
                int homeScore = scoreParts[1].toInt();

                if (homeScore > visitorScore)
                    winner = homeTeam;
                else if (visitorScore > homeScore)
                    winner = visitorTeam;
            }
        }

        matchStages[matchType].append(homeTeam);
        matchStages[matchType].append(visitorTeam);

        if (matchType == "F") {
            finalWinner = winner;
        }
    }

    // Layout setup
    QHBoxLayout* mainLayout = new QHBoxLayout();

    // Function to create styled labels
    auto createStyledLabel = [](const QString& text) {
        QLabel* label = new QLabel(text);
        label->setStyleSheet("background-color: black; color: white; padding: 6px 12px; "
                             "font-size: 14px; border-radius: 4px;");
        label->setAlignment(Qt::AlignCenter);
        return label;
    };

    QStringList stages = { "R16", "QF", "SF", "F" };

    for (const QString& stage : stages) {
        QVBoxLayout* roundLayout = new QVBoxLayout();
        if (matchStages.contains(stage)) {
            QStringList teams = matchStages[stage];
            for (int i = 0; i < teams.size(); i += 2) {
                QString team1 = teams[i];
                QString team2 = (i + 1 < teams.size()) ? teams[i + 1] : "N/A";

                QVBoxLayout* matchLayout = new QVBoxLayout();
                matchLayout->addWidget(createStyledLabel(team1));
                matchLayout->addWidget(new QLabel("vs"));
                matchLayout->addWidget(createStyledLabel(team2));

                roundLayout->addLayout(matchLayout);
                roundLayout->addSpacing(30);
            }
        } else {
            roundLayout->addWidget(createStyledLabel("No Matches"));
        }
        mainLayout->addLayout(roundLayout);
    }

    // Final winner
    QVBoxLayout* winnerLayout = new QVBoxLayout();
    winnerLayout->addSpacing(30);
    QLabel* winnerLabel = createStyledLabel(finalWinner.isEmpty() ? "No Winner" : "🏆 " + finalWinner);
    winnerLayout->addWidget(new QLabel("Winner:"));
    winnerLayout->addWidget(winnerLabel);
    winnerLayout->addSpacing(30);
    mainLayout->addLayout(winnerLayout);


    QLayout* oldLayout = ui->DLR->layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0))) {
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        delete oldLayout;
    }


    QScrollArea* scrollArea = new QScrollArea();
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setStyleSheet("background-color: white;");
    scrollWidget->setLayout(mainLayout);
    scrollWidget->setMinimumWidth(1000);

    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // Add scroll area to DLR
    QVBoxLayout* wrapper = new QVBoxLayout();
    wrapper->addWidget(scrollArea);
    ui->DLR->setLayout(wrapper);
}


void MainWindow::loadLigueTable(const QString& nomChamp) {
    qDebug() << "Loading Ligue Table for champ:" << nomChamp;

    QString trimmedChamp = nomChamp.trimmed();

    QSqlQuery query;
    QMap<int, QMap<QString, int>> stats;  // TeamID -> {pts, played, scored, conceded}
    QMap<int, QString> teamNames;


    query.prepare("SELECT M.ID_MATCH, M.SCORE, J.ID_EQUIPEH, J.ID_EQUIPEV, "
                  "EH.NOMEQUIPE AS HOMETEAM, EV.NOMEQUIPE AS AWAYTEAM "
                  "FROM MATCHESC M "
                  "JOIN JOUER J ON M.ID_MATCH = J.ID_MATCH "
                  "JOIN EQUIPE EH ON J.ID_EQUIPEH = EH.IDEQUIPE "
                  "JOIN EQUIPE EV ON J.ID_EQUIPEV = EV.IDEQUIPE "
                  "WHERE M.NOM_CHAMP = 'LALIGAB' AND M.SCORE IS NOT NULL");
    //query.bindValue(":champ", trimmedChamp);


    if (!query.exec()) {
        qDebug() << "Error executing match query:" << query.lastError().text();
        return;
    }

    qDebug() << "Query executed. Number of columns:" << query.record().count();

    int count = 0;
    while (query.next()) {
        count++;
        int idMatch = query.value(0).toInt();
        QString score = query.value(1).toString();
        int home = query.value(2).toInt();
        int away = query.value(3).toInt();
        QString homeTeamName = query.value(4).toString();
        QString awayTeamName = query.value(5).toString();

        qDebug() << "Match ID:" << idMatch << "| Score:" << score << "| Home:" << homeTeamName << "| Away:" << awayTeamName;

        // Save team names
        teamNames[home] = homeTeamName;
        teamNames[away] = awayTeamName;

        // Parse score
        QStringList parts = score.split("-");
        if (parts.size() != 2) continue;
        int gHome = parts[0].toInt();
        int gAway = parts[1].toInt();

        // Initialize team stats
        for (int team : {home, away}) {
            if (!stats.contains(team)) {
                stats[team]["pts"] = 0;
                stats[team]["played"] = 0;
                stats[team]["scored"] = 0;
                stats[team]["conceded"] = 0;
            }
        }

        // Update stats
        stats[home]["played"]++;
        stats[away]["played"]++;
        stats[home]["scored"] += gHome;
        stats[home]["conceded"] += gAway;
        stats[away]["scored"] += gAway;
        stats[away]["conceded"] += gHome;

        if (gHome > gAway) stats[home]["pts"] += 3;
        else if (gHome < gAway) stats[away]["pts"] += 3;
        else {
            stats[home]["pts"] += 1;
            stats[away]["pts"] += 1;
        }
    }

    qDebug() << "Number of matches fetched:" << count;
    if (stats.isEmpty()) {
        qDebug() << "No stats collected. Check your query and data.";
        return;
    }


    QList<QPair<int, QMap<QString, int>>> entries;
    for (auto it = stats.begin(); it != stats.end(); ++it)
        entries.append(qMakePair(it.key(), it.value()));

    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        if (a.second["pts"] != b.second["pts"])
            return a.second["pts"] > b.second["pts"];
        int gdA = a.second["scored"] - a.second["conceded"];
        int gdB = b.second["scored"] - b.second["conceded"];
        return gdA > gdB;
    });

    // Table model
    QStandardItemModel* model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"Team", "Pts", "Played", "Scored", "Conceded", "Goal Diff"});

    for (const auto& teamStat : entries) {
        int teamId = teamStat.first;
        const auto& stat = teamStat.second;
        int gd = stat["scored"] - stat["conceded"];

        QList<QStandardItem*> row;
        row << new QStandardItem(teamNames.value(teamId, "Unknown"))
            << new QStandardItem(QString::number(stat["pts"]))
            << new QStandardItem(QString::number(stat["played"]))
            << new QStandardItem(QString::number(stat["scored"]))
            << new QStandardItem(QString::number(stat["conceded"]))
            << new QStandardItem(QString::number(gd));
        model->appendRow(row);
    }

    qDebug() << "Table model populated with" << model->rowCount() << "rows.";

    QTableView* tableView = new QTableView();
    tableView->setModel(model);
    tableView->resizeColumnsToContents();
    tableView->resizeRowsToContents();
    tableView->horizontalHeader()->setStretchLastSection(true);

    QFont headerFont = tableView->horizontalHeader()->font();
    headerFont.setBold(true);
    headerFont.setPointSize(10);  // Adjust size as needed
    tableView->horizontalHeader()->setFont(headerFont);

    // Step 5: Apply custom style for sleek design
    tableView->setStyleSheet("QTableView {"
                             "border: 1px solid #ddd;"
                             "border-radius: 5px;"
                             "background-color: #f9f9f9;"
                             "selection-background-color: #b2d8f2;"
                             "selection-color: black;"
                             "}");


    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    if (!ui->DLR->layout())
        ui->DLR->setLayout(new QVBoxLayout());

    QLayout* layout = ui->DLR->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    layout->addWidget(tableView);
    qDebug() << "Table displayed successfully.";
}

void MainWindow::on_LSRbut_clicked() {

    QString championnatName = ui->listChamp->currentText();
    qDebug() << "Button clicked! Championnat:" << championnatName;

    QSqlQuery query;
    query.prepare("SELECT TYPE FROM CHAMPIONNATS WHERE NOM = :nom");
    query.bindValue(":nom", championnatName);

    if (query.exec() && query.next()) {
        QString type = query.value(0).toString().toLower();

        if (type == "ligue") {
            loadLigueTable(championnatName);
            displayCompetitionStats(championnatName);

        } else if (type == "coupe") {
            displayCoupeBracket(championnatName);
            displayCompetitionStats( championnatName);
        }
    } else {
        qDebug() << "Failed to retrieve championship type!";
    }
}

void MainWindow::displayCompetitionStats(const QString& nomChamp)
{
    QSqlQuery query;
    QString typeQuery = R"(
        SELECT TYPE FROM CHAMPIONNATS WHERE NOM = :nomChamp
    )";
    query.prepare(typeQuery);
    query.bindValue(":nomChamp", nomChamp);

    QString champType = "unknown";

    if (query.exec() && query.next()) {
        champType = query.value("TYPE").toString().toLower();
    } else {
        qDebug() << "Type Query failed: " << query.lastError();
        return;
    }

    // Winner logic (depends on type)
    QString winner = "Unknown";

    if (champType == "ligue") {
        // Winner is the team with the most points
        QString leagueWinnerQuery = R"(
                    SELECT * FROM (
                        SELECT
                            E.NOMEQUIPE,
                            SUM(
                                CASE
                                    WHEN (J.ID_EQUIPEH = E.IDEQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) >
                                                                      TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))) THEN 3
                                    WHEN (J.ID_EQUIPEV = E.IDEQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) >
                                                                      TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))) THEN 3
                                    WHEN (TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) =
                                          TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) AND
                                         (J.ID_EQUIPEH = E.IDEQUIPE OR J.ID_EQUIPEV = E.IDEQUIPE)) THEN 1
                                    ELSE 0
                                END
                            ) AS POINTS,

                            SUM(
                                CASE
                                    WHEN J.ID_EQUIPEH = E.IDEQUIPE THEN
                                        TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) -
                                        TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))
                                    WHEN J.ID_EQUIPEV = E.IDEQUIPE THEN
                                        TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) -
                                        TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))
                                    ELSE 0
                                END
                            ) AS GOAL_DIFFERENCE

                        FROM EQUIPE E
                        JOIN JOUER J ON E.IDEQUIPE = J.ID_EQUIPEH OR E.IDEQUIPE = J.ID_EQUIPEV
                        JOIN MATCHESC M ON J.ID_MATCH = M.ID_MATCH
                        WHERE M.NOM_CHAMP = :nomChamp
                        GROUP BY E.NOMEQUIPE
                        ORDER BY POINTS DESC, GOAL_DIFFERENCE DESC
                    )
                    WHERE ROWNUM = 1
        )";
        query.prepare(leagueWinnerQuery);
        query.bindValue(":nomChamp", nomChamp);

        if (query.exec() && query.next()) {
            winner = query.value("NOMEQUIPE").toString();
        } else {
            qDebug() << "Ligue Winner Query failed: " << query.lastError();
        }
    } else {
        // Winner is the best attack team
        QString bestAttackQuery = R"(
            SELECT * FROM (
                SELECT
                    E.NOMEQUIPE,
                    SUM(CASE
                            WHEN J.ID_EQUIPEH = E.IDEQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))
                            WHEN J.ID_EQUIPEV = E.IDEQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))
                            ELSE 0
                        END) AS TOTAL_GOALS
                FROM EQUIPE E
                JOIN JOUER J ON E.IDEQUIPE = J.ID_EQUIPEH OR E.IDEQUIPE = J.ID_EQUIPEV
                JOIN MATCHESC M ON J.ID_MATCH = M.ID_MATCH
                WHERE M.NOM_CHAMP = :nomChamp
                GROUP BY E.NOMEQUIPE
                ORDER BY TOTAL_GOALS DESC
            )
            WHERE ROWNUM = 1
        )";
        query.prepare(bestAttackQuery);
        query.bindValue(":nomChamp", nomChamp);

        if (query.exec() && query.next()) {
            winner = query.value("NOMEQUIPE").toString();
        } else {
            qDebug() << "Best Attack Query failed: " << query.lastError();
        }
    }

    // Best Defense Query
    QString bestDefenseQuery = R"(
        SELECT * FROM (
            SELECT
                E.NOMEQUIPE,
                SUM(CASE
                        WHEN J.ID_EQUIPEH = E.IDEQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))
                        WHEN J.ID_EQUIPEV = E.IDEQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))
                        ELSE 0
                    END) AS GOALS_CONCEDED
            FROM EQUIPE E
            JOIN JOUER J ON E.IDEQUIPE = J.ID_EQUIPEH OR E.IDEQUIPE = J.ID_EQUIPEV
            JOIN MATCHESC M ON J.ID_MATCH = M.ID_MATCH
            WHERE M.NOM_CHAMP = :nomChamp
            GROUP BY E.NOMEQUIPE
            ORDER BY GOALS_CONCEDED ASC
        )
        WHERE ROWNUM = 1
    )";

    query.prepare(bestDefenseQuery);
    query.bindValue(":nomChamp", nomChamp);
    QString bestDefense = "Unknown";

    if (query.exec() && query.next()) {
        bestDefense = query.value("NOMEQUIPE").toString();
    } else {
        qDebug() << "Best Defense Query failed: " << query.lastError();
    }

    // Clean Sheets Query
    QString cleanSheetsQuery = R"(
                SELECT * FROM (
                    SELECT
                        E.NOMEQUIPE,
                        COUNT(DISTINCT M.ID_MATCH) AS CLEAN_SHEETS
                    FROM EQUIPE E
                    JOIN JOUER J ON E.IDEQUIPE = J.ID_EQUIPEH OR E.IDEQUIPE = J.ID_EQUIPEV
                    JOIN MATCHESC M ON J.ID_MATCH = M.ID_MATCH
                    WHERE M.NOM_CHAMP = 'LALIGAB'
                      AND (
                          (J.ID_EQUIPEH = E.IDEQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) = 0) OR
                          (J.ID_EQUIPEV = E.IDEQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) = 0)
                      )
                    GROUP BY E.NOMEQUIPE
                    ORDER BY CLEAN_SHEETS DESC
                ) CLEAN_STATS
                WHERE ROWNUM = 1;
    )";

    query.prepare(cleanSheetsQuery);
    query.bindValue(":nomChamp", nomChamp);
    QString cleanSheets = "Unknown";

    if (query.exec() && query.next()) {
        cleanSheets = query.value("NOMEQUIPE").toString();
    } else {
        qDebug() << "Clean Sheets Query failed: " << query.lastError();
    }

    // Update UI labels
    ui->win_lab->setText("🏆 Winner: " + winner);
    ui->BA_lab->setText("🔥 Best Attack: " + winner);
    ui->MD_lab->setText("🛡️ Best Defense: " + bestDefense);
    ui->CS_lab->setText("🧤 Clean Sheets: " + cleanSheets);
}

void MainWindow::generatePlanning(QComboBox* comboBoxChamp, int intervalDays, int year, int numberOfMatches)
{
    QString nomChamp = comboBoxChamp->currentText();


    QSqlQuery getIdQuery;
    getIdQuery.prepare("SELECT ID_CHAMP FROM CHAMPIONNATS WHERE NOM = :nomChamp");
    getIdQuery.bindValue(":nomChamp", nomChamp);


    if (!getIdQuery.exec() || !getIdQuery.next()) {
        qDebug() << "Failed to fetch ID_CHAMP for" << nomChamp << ":" << getIdQuery.lastError().text();
        return;
    }

    int idChamp = getIdQuery.value(0).toInt();
    QDate currentDate(year, 9, 15);
    QVector<QDate> matchDates;

    for (int i = 0; i < numberOfMatches; ++i) {
        matchDates.append(currentDate);
        currentDate = currentDate.addDays(intervalDays);
    }


    for (const QDate& date : matchDates) {
        QSqlQuery insertPlan;
        insertPlan.prepare("INSERT INTO PLANIFIER (ID_CHAMP, MATCH_DATE) VALUES (:idChamp, :date)");
        insertPlan.bindValue(":idChamp", idChamp);
        insertPlan.bindValue(":date", date);


        if (!insertPlan.exec()) {
            qDebug() << "Failed to insert PLANIFIER for date" << date.toString() << ":" << insertPlan.lastError().text();
        }
    }

    QMessageBox::information(this, "Planification fait", "le plan a été ajouté.");
    qDebug() << "✅ Matches planned starting from September 15," << year << "for" << nomChamp;
}

bool MainWindow::isDatePlanned(const QString& champname, const QDate& matchDate)
{

    QSqlQuery getIdQuery;
    getIdQuery.prepare("SELECT ID_CHAMP FROM CHAMPIONNATS WHERE NOM = :nomChamp");
    getIdQuery.bindValue(":nomChamp", champname);

    if (!getIdQuery.exec()) {
        qDebug() << "Error getting ID:" << getIdQuery.lastError().text();
        return false;
    }

    if (!getIdQuery.next()) {
        qDebug() << "No result for championship name:" << champname;
        return false;
    }

    int idChamp = getIdQuery.value(0).toInt();

    QSqlQuery query;
    query.prepare(R"(
        SELECT COUNT(*)
        FROM PLANIFIER
        WHERE ID_CHAMP = :idChamp
        AND TRUNC(MATCH_DATE) = TO_DATE(:date, 'DD/MM/YYYY')
    )");

    query.bindValue(":idChamp", idChamp);
    QString formattedDate = matchDate.toString("dd/MM/yyyy");
    qDebug() << "date:" << formattedDate;
    qDebug() << "id: " <<idChamp;
    query.bindValue(":date", formattedDate);

    if (!query.exec()) {
        qDebug() << "Error checking planned match date:" << query.lastError().text();
        return false;
    }
    query.next();
    return query.value(0).toInt() > 0;
}

void MainWindow::on_planBUT_clicked(){
    int year = ui->year_l->text().toInt();


    int intervalDays = ui->inter_m->value();


    int numberOfMatches = ui->nbr_m->value();


    generatePlanning(ui->listChamp_2, intervalDays, year, numberOfMatches);
}

void MainWindow::on_CBT_clicked()
{
    ui->CW->setVisible(true);

    // Clear previous formatting (optional)
    QTextCharFormat defaultFormat;
    ui->CW->setDateTextFormat(QDate(), defaultFormat);

    // Get selected championnat name
    QString nomChamp = ui->listChamp_3->currentText();
    QSqlQuery getIdQuery;
    getIdQuery.prepare("SELECT ID_CHAMP FROM CHAMPIONNATS WHERE NOM = :nomChamp");
    getIdQuery.bindValue(":nomChamp", nomChamp);

    if (!getIdQuery.exec() || !getIdQuery.next()) {
        qDebug() << "Could not fetch ID_CHAMP for" << nomChamp << ":" << getIdQuery.lastError().text();
        return;
    }

    int idChamp = getIdQuery.value(0).toInt();

    // Fetch the planned dates for the selected championship
    QSqlQuery query;
    query.prepare("SELECT MATCH_DATE FROM PLANIFIER WHERE ID_CHAMP = :idChamp");
    query.bindValue(":idChamp", idChamp);

    if (!query.exec()) {
        qDebug() << "Failed to fetch match dates:" << query.lastError().text();
        return;
    }

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::yellow);
    highlightFormat.setFontWeight(QFont::Bold);

    while (query.next()) {
        QDate matchDate = query.value(0).toDate();
        ui->CW->setDateTextFormat(matchDate, highlightFormat);
    }
}

void MainWindow::on_HideCW_clicked(){
        ui->CW->setVisible(false);
}
//***************************************AHMED*********************************************************************************

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

void MainWindow::on_addStadiumbuttoN_clicked() {
    qDebug() << "Button clicked!";
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

//-----------------------------EXPORTER EXCEL ---------------------------------------------------------------------------------------------
void MainWindow::on_exporter_excel_match_clicked()
{
    // Ask the user whether they want to export "programmation" or "historique"
    bool ok;
    QString option = QInputDialog::getItem(this, "Choisir l'option d'export",
                                           "Voulez-vous exporter 'programmation' ou 'historique' ?",
                                           {"programmation", "historique"}, 0, false, &ok);
    if (!ok || option.isEmpty()) {
        // User canceled or entered an invalid option
        return;
    }

    // Ask for the file save location
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
    sheet->dynamicCall("SetName(const QString&)", "Liste des Matchs");

    // Decide which table to export based on the user's choice
    QTableWidget *tableToExport = (option == "programmation") ? programmation_2 : historique_table;

    // Access the chosen table widget (either programmation_2 or historique_table)
    int rowCount = tableToExport->rowCount();
    int columnCount = tableToExport->columnCount();

    // Dynamically add column headers from the table
    for (int col = 0; col < columnCount; col++) {
        QString headerText = tableToExport->horizontalHeaderItem(col)->text();
        sheet->querySubObject("Cells(int,int)", 1, col + 1)->setProperty("Value", headerText);
    }

    // Iterate through the rows and columns to fill the Excel sheet with data
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            QTableWidgetItem *item = tableToExport->item(row, col);
            if (item) {
                QString cellText = item->text();

                // Get the cell object
                QAxObject *excelCell = sheet->querySubObject("Cells(int,int)", row + 2, col + 1);

                // Check if the content looks like a score (e.g., "1-0" or "0-1")
                if (cellText.contains("-")) {
                    // Set the value for the score cell as text
                    excelCell->setProperty("Value", QVariant(cellText));
                    // Force Excel to treat it as text (this is more explicit)
                    excelCell->setProperty("NumberFormat", "@"); // Ensure it's formatted as text
                    excelCell->dynamicCall("SetValue(const QString&)", cellText); // Ensure it's treated as a string
                } else {
                    // For non-score cells, set value normally
                    excelCell->setProperty("Value", QVariant(cellText));
                }
            }
        }
    }

    // Save and close the Excel file
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete excel;

    QMessageBox::information(this, "Succès", "Le fichier Excel a été généré avec succès !");
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

//-----------------------------------------TRI MATCHES---------------------------------------------------------------------

void MainWindow::on_tri_prog_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, DATE_MATCH, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4 FROM MATCHES WHERE DATE_MATCH > CURRENT_DATE ORDER BY DATE_MATCH ASC");

    if (query.exec()) {
        ui->programmation_2->setRowCount(0);

        int row = 0;
        while (query.next()) {
            ui->programmation_2->insertRow(row);

            for (int col = 0; col < query.record().count(); col++) {
                ui->programmation_2->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }

            predictWinner(row);
            row++;
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de trier les matchs !");
    }
}
void MainWindow::on_tri_histo_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT ID_MATCH, TYPE_MATCH, EQUIPE1, EQUIPE2, SCORE, DATE_MATCH, ID_ARBITRE1, ID_ARBITRE2, ID_ARBITRE3, ID_ARBITRE4, SCOREEDIT FROM MATCHES WHERE DATE_MATCH < CURRENT_DATE ORDER BY DATE_MATCH DESC");

    if (query.exec()) {
        ui->historique_table->setRowCount(0);

        int row = 0;
        QDateTime now = QDateTime::currentDateTime();

        while (query.next()) {
            QDateTime matchDateTime = query.value(5).toDateTime();  // Full date + time
            int scoreEdit = query.value(10).toInt();
            QString score = query.value(4).toString();

            // Calculate elapsed time since match
            qint64 secondsSinceMatch = matchDateTime.secsTo(now);
            bool isOlderThan2_5Hours = secondsSinceMatch > (2.5 * 3600);

            // Set row color based on time and scoreEdit
            QColor rowColor;
            if (!isOlderThan2_5Hours) {
                rowColor = QColor(255, 165, 0); // orange: less than 2.5 hours
            } else {
                rowColor = (scoreEdit == 0) ? QColor(255, 200, 200) : QColor(197, 255, 217); // red or green
            }

            ui->historique_table->insertRow(row);
            for (int col = 0; col < 10; col++) {
                QTableWidgetItem *item = new QTableWidgetItem;

                if (col == 4) {
                    item->setText(score);
                } else {
                    item->setText(query.value(col).toString());
                }
                item->setBackground(rowColor);
                ui->historique_table->setItem(row, col, item);
            }
            row++;
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de trier les matchs !");
    }
}

//------------------------PREDICTION RESULTAT MATCH-----------------------------------------------------------------------------------------------

void MainWindow::predictWinner(int row)
{
   //------------------------------------------------
    if (!ui->programmation_2->item(row, 2) || !ui->programmation_2->item(row, 3)) {
        qDebug() << "Error: Missing team names in row" << row;
        return;
    }

    QString equipe1 = ui->programmation_2->item(row, 2)->text().trimmed();
    QString equipe2 = ui->programmation_2->item(row, 3)->text().trimmed();

    if (equipe1.isEmpty() || equipe2.isEmpty()) {
        qDebug() << "Error: One of the team names is empty in row" << row;
        return;
    }
    //-------------------------debut------------------------------------------
    QSqlQuery query;
    int countEquipe1 = 0, countEquipe2 = 0;


    query.prepare("SELECT COUNT(*) FROM MATCHES WHERE WINNER = :equipe");
    query.bindValue(":equipe", equipe1);
    if (query.exec() && query.next()) {
        countEquipe1 = query.value(0).toInt();
    } else {
        qDebug() << "Query Error for equipe1:" << query.lastError().text();
    }

    query.prepare("SELECT COUNT(*) FROM MATCHES WHERE WINNER = :equipe");
    query.bindValue(":equipe", equipe2);
    if (query.exec() && query.next()) {
        countEquipe2 = query.value(0).toInt();
    } else {
        qDebug() << "Query Error for equipe2:" << query.lastError().text();
    }


    QString predictedWinner;
    if (countEquipe1 > countEquipe2)
        predictedWinner = equipe1;
    else if (countEquipe1 < countEquipe2)
        predictedWinner = equipe2;
    else
        predictedWinner = "égalité";


    QTableWidgetItem *item = ui->programmation_2->item(row, 9);
    if (!item) {
        item = new QTableWidgetItem();
        ui->programmation_2->setItem(row, 9, item);
    }
    item->setText(predictedWinner);


    ui->programmation_2->viewport()->update();

    qDebug() << "Predicted winner for row" << row << ":" << predictedWinner;
}

//----------------------------------------STATS MATCH --------------------------------------------------------------------
void MainWindow::showMonthlyMatchStatistics() {

    QMap<int, int> monthCount;

    QSqlQuery query("SELECT DATE_MATCH FROM MATCHES WHERE EXTRACT(YEAR FROM DATE_MATCH) = 2025");

    while (query.next()) {
        QDate matchDate = query.value(0).toDate();
        int month = matchDate.month();
        monthCount[month]++;
    }


    QPieSeries *series = new QPieSeries();

    QStringList monthNames = {
        "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
        "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"
    };

    //---verts--
    QVector<QColor> greenShades = {
        QColor(0, 100, 0), QColor(34, 139, 34), QColor(50, 205, 50), QColor(60, 179, 113),
        QColor(46, 139, 87), QColor(107, 142, 35), QColor(154, 205, 50), QColor(85, 107, 47),
        QColor(144, 238, 144), QColor(173, 255, 47), QColor(0, 255, 127), QColor(0, 250, 154)
    };
    //-------

    QVector<QColor> sliceColors;
    QVector<QString> sliceLabels;

    int colorIndex = 0;
    for (int month = 1; month <= 12; month++) {
        int matchCount = monthCount.value(month, 0);
        if (matchCount > 0) {
            QPieSlice *slice = series->append(monthNames[month - 1] + " - " + QString::number(matchCount), matchCount);

            //----design------------------
            QColor sliceColor = greenShades[colorIndex % greenShades.size()];
            slice->setBrush(sliceColor);
            colorIndex++;
            sliceColors.append(sliceColor);
            sliceLabels.append(monthNames[month - 1]);
            slice->setLabelVisible(true);
            slice->setLabelColor(Qt::white);
            //----------------------------------

            connect(slice, &QPieSlice::hovered, [slice, monthNames, month, matchCount, monthCount](bool hovered) {
                if (hovered) {

                    int totalMatches = 0;
                    for (int i = 1; i <= 12; i++) {
                        totalMatches += monthCount.value(i, 0);
                    }

                    double percentage = (matchCount / double(totalMatches)) * 100;


                    //--design(tekber)----
                    slice->setExploded(true);
                    //-----------



                    QToolTip::showText(QCursor::pos(),
                                       QString("Mois: %1\nNombre de matchs: %2\nPourcentage: %3%")
                                           .arg(monthNames[month - 1])
                                           .arg(matchCount)
                                           .arg(QString::number(percentage, 'f', 0)));
                } else {
                    slice->setExploded(false);
                }
            });

        }
    }
//--design------------------------
    QChart *chart = new QChart();
    chart->addSeries(series);

    QFont titleFont = chart->titleFont();
    titleFont.setPointSize(10);
    titleFont.setBold(true);
    titleFont.setFamily("System");
    chart->setTitle("Statistiques des matches pour 2025");
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(Qt::black));

    chart->legend()->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->widget_6_match);
    chartView->resize(ui->widget_6_match->size());

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(chartView);
    chartView->setGraphicsEffect(opacityEffect);

    opacityEffect->setOpacity(0);


    QTimer::singleShot(0, [opacityEffect]() {
        QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
        animation->setDuration(5000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });


    QGraphicsScene *scene = chartView->scene();
    int xOffset = 20;
    int yOffset = chart->boundingRect().bottom() + 213;


    QGridLayout *legendLayout = new QGridLayout();
    legendLayout->setSpacing(5);


    QWidget *legendWidget = new QWidget();
    legendWidget->setLayout(legendLayout);
    legendWidget->setStyleSheet("background-color: transparent;");


    scene->addWidget(legendWidget);


    int columns = 4;
    for (int i = 0; i < sliceColors.size(); ++i) {
        int row = i / columns;
        int col = i % columns;

        QHBoxLayout *rowLayout = new QHBoxLayout();

        QFrame *colorBox = new QFrame();
        colorBox->setFrameShape(QFrame::Box);
        colorBox->setFixedSize(10, 10);
        colorBox->setStyleSheet(QString("background-color: %1").arg(sliceColors[i].name()));

        QLabel *textItem = new QLabel(sliceLabels[i]);
        textItem->setStyleSheet("font-size: 8px;");


        colorBox->installEventFilter(this);


        rowLayout->addWidget(colorBox);
        rowLayout->addWidget(textItem);


        legendLayout->addLayout(rowLayout, row, col);
    }


    legendWidget->setGeometry(xOffset, yOffset, 300, 10);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Enter) {

        if (QFrame *frame = qobject_cast<QFrame *>(watched)) {
            frame->setFixedSize(20, 20);
        }
    } else if (event->type() == QEvent::Leave) {

        if (QFrame *frame = qobject_cast<QFrame *>(watched)) {
            frame->setFixedSize(10, 10);
        }
    }
    return QObject::eventFilter(watched, event);
}

//---------------------------output arduino------------------------------------------------------

void MainWindow::on_showArduinoButton_clicked()
{
    arduinoMode = true;
    QMessageBox::information(this, "Afficher sur Arduino", "Veuillez sélectionner un match dans la table.");
}

void MainWindow::on_historique_table_cellClicked(int row, int column)
{
    if (!arduinoMode)
        return;

    if (column == 4) {
        QString team1Name = ui->historique_table->item(row, 2)->text().trimmed();
        QString team2Name = ui->historique_table->item(row, 3)->text().trimmed();
        QString score = ui->historique_table->item(row, column)->text();
        QStringList parts = score.split("-");

        if (parts.size() == 2) {
            bool ok1, ok2;
            int team1Score = parts[0].trimmed().toInt(&ok1);
            int team2Score = parts[1].trimmed().toInt(&ok2);

            if (!ok1 || !ok2) {
                QMessageBox::warning(this, "Erreur", "Format de score invalide");
                arduinoMode = false;
                return;
            }

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Envoyer le score",
                                          QString("Voulez-vous afficher les scores?\n"
                                                  "%1: %2 | %3: %4")
                                              .arg(team1Name).arg(team1Score)
                                              .arg(team2Name).arg(team2Score),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                QSerialPort serialPort;
                serialPort.setPortName("COM6");

                if (!serialPort.open(QIODevice::WriteOnly)) {
                    QMessageBox::warning(this, "Erreur",
                                         "Port série non disponible:\n" + serialPort.errorString());
                    arduinoMode = false;
                    return;
                }

                serialPort.setBaudRate(QSerialPort::Baud9600);

                QString message = QString("T1=%1;T2=%2;").arg(team1Score).arg(team2Score);

                if (serialPort.write(message.toUtf8())) {
                    serialPort.waitForBytesWritten(1000);
                    QMessageBox::information(this, "Succès",
                                             QString("Scores affichés:\n%1: %2 | %3: %4")
                                                 .arg(team1Name).arg(team1Score)
                                                 .arg(team2Name).arg(team2Score));
                }
                serialPort.close();
            }
            arduinoMode = false;
        }
    }
}

//========ahmed (mara o5ra)===============
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
    if (typeQuery.value("type").toString() != "Coupe") {
        QMessageBox::critical(this, "Erreur", "Le championnat sélectionné doit être de type 'Coupe'.");
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

QString MainWindow::ARD_consulter(const QString &champName)
{
    if (champName.isEmpty()) {
        return "Erreur: Aucun championnat sélectionné.";
    }

    QSqlQuery typeQuery;
    typeQuery.prepare("SELECT type FROM Championnats WHERE nom = :nom");
    typeQuery.bindValue(":nom", champName);
    if (!typeQuery.exec() || !typeQuery.next()) {
        return "Erreur: Impossible de vérifier le type du championnat.";
    }
    if (typeQuery.value("type").toString() != "Coupe") {
        return "Erreur: Le championnat sélectionné n'est pas de type 'Coupe'.";
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT 1 FROM CONSULTER WHERE nom_champ = :nom_champ");
    checkQuery.bindValue(":nom_champ", champName);
    if (checkQuery.exec() && checkQuery.next()) {
        return "Erreur: Ce championnat a déjà un stade assigné.";
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
        return "Erreur: Impossible de récupérer les stades.";
    }

    if (stadeNames.isEmpty()) {
        return "Erreur: Aucun stade disponible avec une capacité supérieure à 40 000.";
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
    insertQuery.bindValue(":nom_champ", champName);
    insertQuery.bindValue(":nom_stade", selectedStade);
    if (!insertQuery.exec()) {
        return "Erreur: Échec de l'ajout au tirage.";
    }

    ui->tableViewConsulter->setModel(afficherConsulter());

    return QString("Succès: %1 assigné à %2 !").arg(champName, selectedStade);
}

//=======ahmed (youfa hne)=============

//======hedi(debut)=============
void MainWindow::remplirFormulaireArbitre(QString nom, QString prenom, int age, int experience, QString email, QString sexe, QString telephone) {
    ui->lineEdit_nom_2->setText(nom);
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
    QString nom = ui->lineEdit_nom_2->text();
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

    QString nom = ui->lineEdit_nom_2->text();
    QString prenom = ui->lineEdit_prenom->text();
    int age = ui->lineEdit_age->value();
    int experience = ui->lineEdit_experience->value();
    QString sexe = ui->comboBox_sexe->currentText();
    QString email = ui->lineEdit_email->text();
    QString telephone = ui->lineEdit_telephone->text();

    // Call modifier with telephone
    if (A.modifier(id, nom, prenom, age, experience, email, sexe, telephone)) {
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
//=========hedi(fin)=================


//khadija----------------------------------------------------------------------------------------

void MainWindow::afficherStatistiques() {
    // Clear previous chart if it exists
    if (chartView2) {
        ui->widget_9->layout()->removeWidget(chartView2);
        delete chartView2;
        chartView2 = nullptr;
    }

    // Create new chart view with team goals statistics
    chartView2 = statistique->creerGraphiqueButsParEquipe();

    // Style the chart view
    chartView2->setStyleSheet("background: transparent; border: none;");
    chartView2->setRenderHint(QPainter::Antialiasing);

    // Add to layout
    ui->widget_9->layout()->addWidget(chartView2);

    // Ensure the widget is visible
    ui->widget_9->show();
}
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
                    loadTeamsIntoComboBox();
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
        loadTeamsIntoComboBox();
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
        loadTeamsIntoComboBox();
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
void MainWindow::exportTableToPDFK() {
    QTextDocument document;
    QTextCursor cursor(&document);

    // Style CSS pour le thème football (vert et noir)
    QString style = R"(
        <style>
            body { font-family: Arial, sans-serif; }
            h1 {
                color: #2E8B57; /* Forest green */
                text-align: center;
                margin-bottom: 20px;
            }
            table {
                border-collapse: collapse;
                width: 100%;
                margin-bottom: 25px;
            }
            th {
                background-color: #2E8B57; /* Forest green */
                color: white;
                font-weight: bold;
                padding: 10px;
                text-align: left;
                border: 1px solid #1A5D34;
            }
            td {
                padding: 8px;
                border: 1px solid #ddd;
            }
            tr:nth-child(even) {
                background-color: #f2f2f2;
            }
            tr:hover {
                background-color: #e6ffe6; /* Light green hover */
            }
            .footer {
                text-align: right;
                font-style: italic;
                color: #555;
                margin-top: 20px;
            }
        </style>
    )";

    // Début du document HTML avec style
    QString html = "<html><head>" + style + "</head><body>";

    // Ajout du titre
    html += "<h1>Rapport des Equipes </h1>";

    // Création du tableau
    html += "<table>";

    // En-têtes de colonnes
    html += "<tr>";
    for (int col = 0; col < ui->programme->model()->columnCount() - 1; ++col) {
        QString header = ui->programme->model()->headerData(col, Qt::Horizontal).toString();
        html += "<th>" + header + "</th>";
    }
    html += "</tr>";

    // Données du tableau
    for (int row = 0; row < ui->programme->model()->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->programme->model()->columnCount() - 1; ++col) {
            QString data = ui->programme->model()->data(ui->programme->model()->index(row, col)).toString();
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    // Pied de page
    html += "<div class='footer'>Généré le " + QDate::currentDate().toString("dd/MM/yyyy") + "</div>";
    html += "</body></html>";

    cursor.insertHtml(html);

    // Configuration de l'export PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15)); // Marges de 15mm

    // Optionnel : définir l'orientation (commenté par défaut)
    // printer.setPageOrientation(QPageLayout::Landscape);

    document.print(&printer);

    QMessageBox::information(this, "Succès", "Le tableau a été exporté en PDF avec succès !");
}void MainWindow::rechercheEquipe() {
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
void MainWindow::GenererContratJoueur()
{
    // Get selected team from the combo box
    if (ui->equipeComboBox->currentText().isEmpty() ||
        ui->equipeComboBox->currentText() == "Aucune équipe disponible") {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une équipe valide.");
        return;
    }

    QString teamName = ui->equipeComboBox->currentText();
    QString cityName; // Initialize city name
    QDate startDate, endDate;

    // Fetch the city name, start date, and end date from the database
    QSqlQuery query;
    query.prepare("SELECT NOMVILLE, DATEDEBUTCONTRAT, DATEFINCONTRAT FROM equipe WHERE NOMEQUIPE = :teamName");
    query.bindValue(":teamName", teamName);

    if (query.exec() && query.next()) {
        cityName = query.value("NOMVILLE").toString();
        startDate = query.value("DATEDEBUTCONTRAT").toDate();
        endDate = query.value("DATEFINCONTRAT").toDate();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la récupération des informations de l'équipe : " + query.lastError().text());
        return;
    }

    QString joueurNom ="xxxx";
    QString joueurDateNaissance = "xx/xx/xxxx";
    QString joueurNationalite ="xxxxxx";
    QString joueurAdresse ="xxx";
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le contrat", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);

    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::warning(this, "Erreur", "Impossible d'initialiser le PDF.");
        return;
    }

    // Charger les images de signature avec des chemins absolus pour test
    QImage signaturePresident(":/interface_icons/president.jpg"); // Modifié le chemin
    QImage signatureJoueur(":/interface_icons/joueur.jpg");       // Modifié le chemin

    // Debug: Vérifier le chargement des images
    if(signaturePresident.isNull()) {
        qDebug() << "Échec du chargement de l'image du président";
    }
    if(signatureJoueur.isNull()) {
        qDebug() << "Échec du chargement de l'image du joueur";
    }
    QRect printableArea = printer.pageLayout().paintRectPixels(printer.resolution());
    int x = 50; // Marge gauche
    int y = 50; // Position verticale initiale
    int lineHeight = 24;
    int smallSpace = 10;
    int pageWidth = printableArea.width();
    int textWidth = pageWidth - 2 * x;
    int signatureHeight = 60; // Hauteur des images de signature


    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(x, y, textWidth, lineHeight), Qt::AlignCenter, "CONTRAT DE JOUEUR");
    y += lineHeight + smallSpace;

    // Informations de l'équipe (espace réduit)
    painter.setFont(QFont("Arial", 12));
    QString teamInfo = QString("Le Club :\nNom : %1\nSiège social : %2\nReprésenté par : Monsieur le Président du club")
                           .arg(teamName, cityName);
    QRect teamInfoRect(x, y, textWidth, lineHeight * 3); // Hauteur réduite
    painter.drawText(teamInfoRect, Qt::AlignLeft | Qt::TextWordWrap, teamInfo);
    y += teamInfoRect.height() + smallSpace;

    // Informations du joueur (espace réduit)
    QString joueurInfo = QString("Le Joueur :\nNom : %1\nNé le : %2\nNationalité : %3\nDomicilié à : %4")
                             .arg(joueurNom, joueurDateNaissance, joueurNationalite, joueurAdresse);
    QRect joueurInfoRect(x, y, textWidth, lineHeight * 3); // Hauteur réduite
    painter.drawText(joueurInfoRect, Qt::AlignLeft | Qt::TextWordWrap, joueurInfo);
    y += joueurInfoRect.height() + smallSpace;

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(x, y, textWidth, lineHeight), Qt::AlignLeft, "Articles du contrat:");
    y += lineHeight;

    painter.setFont(QFont("Arial", 12));
    QStringList articles = {
        "Article 1: Objet du contrat\nLe présent contrat a pour objet d'établir les conditions dans lesquelles le Joueur s'engage à exercer à titre exclusif et professionnel l'activité de joueur de football au sein du Club.",
        QString("Article 2 : Durée\nLe présent contrat est conclu pour une durée déterminée des saisons sportives, prenant effet à compter du %1 jusqu'au %2, sous réserve de l'homologation par la Ligue de Football Professionnel (LFP).").arg(startDate.toString("dd/MM/yyyy"), endDate.toString("dd/MM/yyyy")),
        "Article 3 : Fonction\nLe Joueur exercera ses fonctions de joueur professionnel de football au sein de l'équipe première du Club, ou toute autre équipe selon les besoins sportifs du Club. Il participera à toutes les séances d'entraînement, compétitions officielles et matchs amicaux."
    };

    // Ajout de l'article 4 en fonction du radio button sélectionné

    QString article4;
    if (ui->Article1_equipe->isChecked()) {
        article4 = "Article 4 : Droit à l'image\n " + ui->Article1_equipe->text();
    }
    else if (ui->Article2_equipe->isChecked()) {
        article4 = "Article 4 : Obligation du joueur\n " + ui->Article2_equipe->text();
    }
    else if (ui->Article3_equipe->isChecked()) {
        article4 = "Article 4 : Litiges\n" + ui->Article3_equipe->text();
    }

    articles.append(article4);

    for (const QString &article : articles) {
        QRect articleRect(x, y, textWidth, lineHeight * 2.5);
        painter.drawText(articleRect, Qt::AlignLeft | Qt::TextWordWrap, "- " + article);
        y += articleRect.height() + smallSpace/2;
    }
    QString dateText = QString("Fait en quatre exemplaires originaux, à %1, le %2")
                           .arg(cityName, startDate.toString("dd/MM/yyyy"));
    painter.drawText(QRect(x, y, textWidth, lineHeight), Qt::AlignCenter, dateText);
    y += lineHeight + smallSpace;

    // Signature Président (gauche)
    int signatureWidth = (textWidth - 20) / 2;
    int signatureImageWidth = 150; // Largeur fixe pour les signatures

    if(!signaturePresident.isNull()) {
        // Ajustement de la taille en conservant le ratio
        signaturePresident = signaturePresident.scaledToWidth(signatureImageWidth, Qt::SmoothTransformation);
        painter.drawImage(QRect(x, y, signatureImageWidth, signatureHeight), signaturePresident);
        painter.drawText(QRect(x, y + signatureHeight, signatureWidth, lineHeight), "Représentant de l'équipe");
    } else {
        painter.drawText(QRect(x, y + lineHeight, signatureWidth, lineHeight), "Représentant de l'équipe");
    }

    // Signature Joueur (droite)
    if(!signatureJoueur.isNull()) {
        signatureJoueur = signatureJoueur.scaledToWidth(signatureImageWidth, Qt::SmoothTransformation);
        painter.drawImage(QRect(x + signatureWidth + 20, y, signatureImageWidth, signatureHeight), signatureJoueur);
        painter.drawText(QRect(x + signatureWidth + 20, y + signatureHeight, signatureWidth, lineHeight), "Le Joueur");
    } else {
        painter.drawText(QRect(x + signatureWidth + 20, y + lineHeight, signatureWidth, lineHeight),  " Joueur");
    }

    painter.end();
    QMessageBox::information(this, "Succès", "Le contrat de travail a été généré avec succès !");
}

//metier avance speechto text
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
    QString pythonScriptPath = "C:\\Users\\alabe\\Pictures\\metiers avances\\voice\\voc.py";
    QString pythonExecutable = "C:\\Users\\alabe\\AppData\\Local\\Programs\\Python\\Python312\\python.exe";

    QProcess process;

    // Clear PYTHONHOME and PYTHONPATH environment variables
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");
    process.setProcessEnvironment(env);

    QMessageBox::information(this, "Enregistrement", "Parlez maintenant pendant 5 secondes...");

    process.start(pythonExecutable, QStringList() << pythonScriptPath);

    if (!process.waitForFinished(8000)) { // 10 second timeout
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
void MainWindow::loadTeamsIntoComboBox()
{
    ui->equipeComboBox->clear(); // Vider le ComboBox actuel

    QSqlQuery query("SELECT NOMEQUIPE FROM equipe ORDER BY NOMEQUIPE");

    while (query.next()) {
        ui->equipeComboBox->addItem(query.value(0).toString());
    }

    if (ui->equipeComboBox->count() == 0) {
        ui->equipeComboBox->addItem("Aucune équipe disponible");
        ui->GenererContratJoueur->setEnabled(false); // Désactiver le bouton si aucune équipe
    } else {
        ui->GenererContratJoueur->setEnabled(true);
    }
}

//=======khadija(fin)============

//ala
void MainWindow::setupArduinoConnection() {
    // Attempt to connect to the Arduino
    int connectionStatus = arduino->connect_arduino();

    if (connectionStatus == -1) {
        qDebug() << "Arduino not found!";
        return;
    } else if (connectionStatus == 1) {
        qDebug() << "Failed to open serial port!";
        return;
    } else {
        qDebug() << "Connected to Arduino on port:" << arduino->getarduino_port_name();
    }

    // Connect the serial data handler immediately
    connect(arduino->getserial(), &QSerialPort::readyRead, this, &MainWindow::handleArduinoData);

    // Set up player name sending
    connect(ui->ArduinoButton, &QPushButton::clicked, this, [this]() {
        QString playerName = " " + ui->liste_Joueurs->currentText().trimmed();
        qDebug() << "Sending player name to Arduino:" << playerName;

        if (playerName.isEmpty()) {
            qDebug() << "Error: No player selected!";
            return;
        }

        if (!arduino || !arduino->getserial()->isOpen()) {
            qDebug() << "Error: Arduino not connected!";
            return;
        }



        QSqlQuery query;
        query.prepare("SELECT RED_CARD FROM joueur1 WHERE NOM = ?");
        query.addBindValue(playerName.trimmed());

        if (query.exec() && query.next()) {
            bool hasRedCard = query.value("RED_CARD").toBool();
            if (hasRedCard) {
                // Add special prefix to trigger buzzer
                playerName = " !" + playerName.trimmed();
                qDebug() << "Player has red card - buzzer will sound";
            }
        }


        QByteArray playerNameBytes = playerName.toUtf8();
        playerNameBytes.append('\n');
        arduino->write_to_arduino(playerNameBytes);
    });
}

void MainWindow::handleArduinoData() {
    if (!arduino || !arduino->getserial()->isOpen()) {
        qDebug() << "Arduino not connected in handler!";
        return;
    }

    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");

    // Read all available data
    QByteArray data = arduino->getserial()->readAll();
    QString message = QString::fromUtf8(data).trimmed();
    qDebug() << "Received from Arduino:" << message;

    if (message == "Y") {  // Handle both versions
        QString currentPlayer = ui->liste_Joueurs->currentText().trimmed();
        if (!currentPlayer.isEmpty()) {
            qDebug() << "Processing yellow card for:" << currentPlayer;
            incrementYellowCards(currentPlayer);
            setupTableWithDeleteButtons(ui->tableWidgetPlayers);

            // Optional: Send acknowledgment back to Arduino
            arduino->write_to_arduino("ACK\n");
        } else {
            qDebug() << "No player selected when yellow card received";
        }
    }

    if (message == "R") {  // Handle both versions
        QString currentPlayer = ui->liste_Joueurs->currentText().trimmed();
        if (!currentPlayer.isEmpty()) {
            qDebug() << "Processing red card for:" << currentPlayer;
            incrementRedCards(currentPlayer);
            setupTableWithDeleteButtons(ui->tableWidgetPlayers);

            // Optional: Send acknowledgment back to Arduino
            arduino->write_to_arduino("ACK\n");
        } else {
            qDebug() << "No player selected when yellow card received";
        }
    }
}

void MainWindow::incrementYellowCards(const QString &playerName) {
    /*Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Database connection failed!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();*/

    // First get current yellow card count
    QSqlQuery getQuery;
    getQuery.prepare("SELECT NB_YELLOW, RED_CARD FROM joueur1 WHERE NOM = ?");
    getQuery.addBindValue(playerName);

    if (!getQuery.exec() || !getQuery.next()) {
        qDebug() << "Failed to get current card counts:" << getQuery.lastError().text();
        return;
    }

    int currentYellows = getQuery.value("NB_YELLOW").toInt();
    int currentReds = getQuery.value("RED_CARD").toInt();

    QSqlQuery updateQuery;

    if (currentYellows >= 1) {  // If player will reach 2 yellows after increment
        // Convert 2 yellows to 1 red and reset yellows
        updateQuery.prepare("UPDATE joueur1 SET NB_YELLOW = 0, RED_CARD = 1 WHERE NOM = ?");
        qDebug() << "Converting 2 yellow cards to 1 red card for" << playerName;
    } else {
        // Just increment yellows normally
        updateQuery.prepare("UPDATE joueur1 SET NB_YELLOW = NB_YELLOW + 1 WHERE NOM = ?");
    }

    updateQuery.addBindValue(playerName);

    if (!updateQuery.exec()) {
        qDebug() << "Update failed:" << updateQuery.lastError().text();
    } else {
        qDebug() << "Successfully updated cards for" << playerName;
    }
}

void MainWindow::incrementRedCards(const QString &playerName) {
    /*Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Database connection failed!";
        return;
    }*/

    QSqlQuery query;
    query.prepare("UPDATE joueur1 SET RED_CARD = 1, NB_YELLOW = 0 WHERE NOM = ?");
    query.addBindValue(playerName);

    if (!query.exec()) {
        qDebug() << "Update failed:" << query.lastError().text();
    } else {
        qDebug() << "Successfully updated red cards for" << playerName;
    }
}

/*
void MainWindow::refreshStats() {
    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QTabWidget* tabWidget = joueurWidget->findChild<QTabWidget*>("tabWidget_5");

    if (tabWidget) {
        int tabIndex = tabWidget->indexOf(ui->statsTab);  // use tabWidget not ui->tabWidget
        //tabWidget->setCurrentIndex(tabIndex);

        QWidget* statsTab = tabWidget->widget(tabIndex);
        QVBoxLayout* statsLayout = statsTab->findChild<QVBoxLayout*>("statsLayout");

        if (statsLayout) {
            QLayoutItem* item;
            while ((item = statsLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }

            QChartView* chartView = createNationalityChart();
            if (chartView) {
                statsLayout->addWidget(chartView);
            }

            QChartView* positionChartView = createPositionChart();
            if (positionChartView) {
                statsLayout->addWidget(positionChartView);
            }
        } else {
            qDebug() << "Stats layout not found!";
        }
    } else {
        qDebug() << "joueurTabWidget not found!";
    }
}*/

void MainWindow::refreshStats() {
    // 1. Find the containing widgets
    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    if (pageIndex == -1) {
        qWarning() << "Player page not found in stacked widget";
        return;
    }

    QTabWidget* tabWidget = ui->stackedWidget->widget(pageIndex)->findChild<QTabWidget*>("tabWidget_5");
    if (!tabWidget) {
        qWarning() << "Tab widget not found";
        return;
    }

    // 2. Clear existing charts
    QVBoxLayout* statsLayout = ui->statsTab->findChild<QVBoxLayout*>("statsLayout");
    if (!statsLayout) {
        qWarning() << "Stats layout not found";
        return;
    }

    // Safely clear existing widgets
    QLayoutItem* item;
    while ((item = statsLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            // Special handling for QChartView
            if (auto chartView = qobject_cast<QChartView*>(widget)) {
                delete chartView->chart(); // Clean up chart first
            }
            widget->deleteLater();
        }
        delete item;
    }

    // 3. Create and add new charts
    try {
        if (QChartView* chartView = createNationalityChart()) {
            statsLayout->addWidget(chartView);
        }

        if (QChartView* positionChartView = createPositionChart()) {
            statsLayout->addWidget(positionChartView);
        }
    } catch (const std::exception& e) {
        qCritical() << "Chart creation failed:" << e.what();
    }
}

void MainWindow::onAjouterButtonClicked() {
    createJoueurFromUI(this);
    refreshStats();
    Joueur j;

    j.getListe(ui->liste_Joueurs);
}

void MainWindow::onrechercherButtonClicked(){
    rechercheJoueurFromUI(this);
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    this->setupTableWithDeleteButtons3(ui->tableWidgetPlayers, arg1);
}

void MainWindow::setupTableWithDeleteButtons(QTableWidget* tableWidgetPlayers) {
    Joueur j;
    j.readJoueur(tableWidgetPlayers); // Refresh the table

    // Clear previous widgets in column 6
    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        tableWidgetPlayers->removeCellWidget(row, 8);
    }

    // Add action buttons (Delete & Update)
    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        // Create a container widget
        QWidget *buttonContainer = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(buttonContainer);
        layout->setContentsMargins(0, 0, 0, 0); // Remove extra spacing

        QPushButton *deleteButton = new QPushButton("Delete");
        QPushButton *updateButton = new QPushButton("Update");

        QString nom = tableWidgetPlayers->item(row, 0)->text();

        connect(deleteButton, &QPushButton::clicked, this, [this, nom]() {
            deleteJoueurFromUI(this, nom);
            refreshStats();
        });

        // Connect Update button
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
            selected_row=row;
            updateJoueurFromUI(this, row); // Call update function
        });

        // Add buttons to layout
        layout->addWidget(updateButton);
        layout->addWidget(deleteButton);

        // Set layout to the container widget
        buttonContainer->setLayout(layout);

        // Insert the container widget into the 6th column (index 5)
        tableWidgetPlayers->setCellWidget(row, 8, buttonContainer);
    }
}

void MainWindow::setupTableWithDeleteButtons2(QTableWidget* tableWidgetPlayers, const QString nom) {
    // First, call rechercheJoueur to refresh the table with the search results
    Joueur j;
    j.rechercheJoueur(tableWidgetPlayers, nom); // Refresh the table

    // Ensure that the rows are filled after searching
    int rowCount = tableWidgetPlayers->rowCount();

    // If there are no rows, don't proceed with adding buttons
    if (rowCount == 0) {
        qDebug() << "No players found for the given name!" << nom;
        return;
    }

    // Clear previous widgets in column 5 (Delete and Update buttons)
    for (int row = 0; row < rowCount; ++row) {
        tableWidgetPlayers->removeCellWidget(row, 8);
    }

    // Add action buttons (Delete & Update)
    for (int row = 0; row < rowCount; ++row) {
        // Create a container widget for buttons
        QWidget *buttonContainer = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(buttonContainer);
        layout->setContentsMargins(0, 0, 0, 0); // Remove extra spacing

        QPushButton *deleteButton = new QPushButton("Delete");
        QPushButton *updateButton = new QPushButton("Update");

        // Get the player's name for delete action
        QString playerName = tableWidgetPlayers->item(row, 0)->text();

        // Connect Delete button
        connect(deleteButton, &QPushButton::clicked, this, [this, playerName]() {
            deleteJoueurFromUI(this, playerName); // Delete player
            refreshStats();
        });

        // Connect Update button
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
            selected_row = row;
            updateJoueurFromUI(this, row); // Update player info
        });

        // Add buttons to layout
        layout->addWidget(updateButton);
        layout->addWidget(deleteButton);

        // Set layout to the container widget
        buttonContainer->setLayout(layout);

        // Insert the container widget into the 6th column (index 5)
        tableWidgetPlayers->setCellWidget(row, 8, buttonContainer);
    }
}

//filter
void MainWindow::setupTableWithDeleteButtons3(QTableWidget* tableWidgetPlayers, const QString arg1) {
    Joueur j;
    if (arg1=="Nom"){
        j.setFilter(1);
    }else if (arg1=="Nationalite"){
        j.setFilter(2);
    }else{
        j.setFilter(3);
    }
    j.readJoueur(tableWidgetPlayers); // Refresh the table

    // Clear previous widgets in column 6
    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        tableWidgetPlayers->removeCellWidget(row, 8);
    }

    // Add action buttons (Delete & Update)
    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        // Create a container widget
        QWidget *buttonContainer = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(buttonContainer);
        layout->setContentsMargins(0, 0, 0, 0); // Remove extra spacing

        QPushButton *deleteButton = new QPushButton("Delete");
        QPushButton *updateButton = new QPushButton("Update");

        QString nom = tableWidgetPlayers->item(row, 0)->text();

        connect(deleteButton, &QPushButton::clicked, this, [this, nom]() {
            deleteJoueurFromUI(this, nom);
            refreshStats();
        });

        // Connect Update button
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
            selected_row=row;
            updateJoueurFromUI(this, row); // Call update function
        });

        // Add buttons to layout
        layout->addWidget(updateButton);
        layout->addWidget(deleteButton);

        // Set layout to the container widget
        buttonContainer->setLayout(layout);

        // Insert the container widget into the 6th column (index 5)
        tableWidgetPlayers->setCellWidget(row, 8, buttonContainer);
    }
}

void MainWindow::validateInputsJ() {
    bool allValid = true;

        // Regular expression for names (only letters)
        QRegularExpression alphaRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");

        // Validate Name
        if (ui->NomInput->text().trimmed().isEmpty() ||
            !alphaRegex.match(ui->NomInput->text().trimmed()).hasMatch() || ui->NomInput->text().length() > 10 || ui->NomInput->text().length() <3) {
            ui->NomError->setText("invalide !");
            ui->NomError->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->NomError->setText("valide");
            ui->NomError->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }

        // Validate Prenom
        if (ui->PrenomInput->text().trimmed().isEmpty() ||
            !alphaRegex.match(ui->PrenomInput->text().trimmed()).hasMatch() || ui->PrenomInput->text().length() > 10 || ui->PrenomInput->text().length() < 3) {
            ui->PrenomError->setText("invalide !");
            ui->PrenomError->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->PrenomError->setText("valide");
            ui->PrenomError->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }

        /*
        // Validate Position
        QStringList validPositions = {"GK", "LB", "RB", "CB", "CMD", "CM", "CAM", "LW", "RW", "ST"};
        if (ui->PositionInput->text().trimmed().isEmpty() ||
            !alphaRegex.match(ui->PositionInput->text().trimmed()).hasMatch() || !validPositions.contains(ui->PositionInput->text().trimmed().toUpper())) {
            ui->PositionError->setText("invalide !");
            ui->PositionError->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; "
                );
            allValid = false;
        } else {
            ui->PositionError->setText("valide");
            ui->PositionError->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; "
                );
        }*/
        // Validate Position
        QStringList validPositions = {"GK", "LB", "RB", "CB", "CMD", "CM", "CAM", "LW", "RW", "ST"};
        QString selectedPosition = ui->PositionInput2->currentText().trimmed().toUpper();

        if (selectedPosition.isEmpty() || !validPositions.contains(selectedPosition)) {
            ui->PositionError->setText("invalide !");
            ui->PositionError->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; "
                );
            allValid = false;
        } else {
            ui->PositionError->setText("valide");
            ui->PositionError->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; "
                );
        }

        // Validate Nationality
        if (ui->NationaliteInput->text().trimmed().isEmpty() ||
            !alphaRegex.match(ui->NationaliteInput->text().trimmed()).hasMatch()) {
            ui->NationaliteError->setText("invalide !");
            ui->NationaliteError->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->NationaliteError->setText("valide");
            ui->NationaliteError->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }

        //validate image path
        if (ui->Img_pathInput->text().trimmed().isEmpty()) {
            ui->ImgError->setText("invalide !");
            ui->ImgError->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->ImgError->setText("valide");
            ui->ImgError->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }

        // Validate Date of Birth (minimum age 10)
        QDate birthDate = ui->dsInput->date();
        QDate currentDate = QDate::currentDate();
        int age = birthDate.daysTo(currentDate) / 365;

        if (age < 10) {
            ui->DsError->setText("invalide !");
            ui->DsError->setStyleSheet(
                "color: #D32F2F; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
            allValid = false;
        } else {
            ui->DsError->setText("valide");
            ui->DsError->setStyleSheet(
                "color: #2E7D32; "
                "font-size: 10px; "
                "font-weight: bold; "
                "padding: 2px; ");
        }

        // Enable the button only if all inputs are valid
        ui->AjouterButton->setEnabled(allValid);
        ui->modifierButton->setEnabled(allValid);
}

void MainWindow::exportToPDFJ() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
        if (fileName.isEmpty()) return;  // User canceled

        // Step 2: Set up database connection
        /*Connection conn;
        if (!conn.createconnect()) {
            qDebug() << "Failed to connect to database!";
            return;
        }

        QSqlDatabase db = conn.getDatabase();
        if (!db.isOpen()) {
            qDebug() << "Database is not open!";
            return;
        }*/

        // Step 3: Fetch data from the database
        QSqlQuery query;
        query.prepare("SELECT Nom, Prenom, Pays_origine, Position, Date_de_naissance FROM joueur1");

        if (!query.exec()) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return;
        }

        // Step 4: Create PDF document
        QPdfWriter pdfWriter(fileName);
        pdfWriter.setPageSize(QPageSize(QPageSize::A4));
        pdfWriter.setResolution(300);

        QPainter painter(&pdfWriter);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setFont(QFont("Arial", 10));

        int margin = 100;  // Margins for left and top
        int y = margin;

        // Title
        painter.setFont(QFont("Arial", 18, QFont::Bold));
        painter.drawText(margin + 400, y, "Players List");
        painter.setFont(QFont("Arial", 12));
        y += 300;

        // Table Column Headers
        int x[] = {margin, margin + 250, margin + 500, margin + 800, margin + 1100};

        painter.setFont(QFont("Arial", 11, QFont::Bold));
        painter.drawText(x[0], y, "Nom");
        painter.drawText(x[1], y, "Prenom");
        painter.drawText(x[2], y, "Position");
        painter.drawText(x[3], y, "Nationalite");
        painter.drawText(x[4], y, "Date Naissance");

        y += 50;
        painter.drawLine(margin, y, 1900, y);
        y += 100;

        // Step 5: Write player data
        painter.setFont(QFont("Arial", 10));
        while (query.next()) {
            painter.drawText(x[0], y, query.value(0).toString());
            painter.drawText(x[1], y, query.value(1).toString());
            painter.drawText(x[2], y, query.value(3).toString());
            painter.drawText(x[3], y, query.value(2).toString());
            painter.drawText(x[4], y, query.value(4).toDate().toString("yyyy-MM-dd"));

            // Draw row separator
            y += 50;
            painter.drawLine(margin, y, 1900, y);
            y += 50;
        }

        // Footer
        y += 200;
        QFont footerFont("Arial", 9);
        footerFont.setItalic(true);
        painter.setFont(footerFont);

        painter.drawText(margin, y, "Generated on: " + QDate::currentDate().toString("dd MMM yyyy"));
        painter.drawText(1800, y, "Page 1");

        // End PDF
        painter.end();

        // Step 6: Success Message
        QMessageBox::information(this, "Success", "PDF exported successfully!");
}

void MainWindow::freeInputs(){
    ui->NomInput->setText("");
    ui->PrenomInput->setText("");
    //ui->PositionInput->setText("");
    ui->NationaliteInput->setText("");
    ui->Img_pathInput->setText("");
}

void MainWindow::on_deleteButton_clicked()
{
    // Open file dialog for user to select a license image
    QString filePath = QFileDialog::getOpenFileName(this, "Select License Image", "", "Images (*.png *.jpg *.jpeg)");
    if (filePath.isEmpty())
        return; // If no file selected, exit function

    filePath = filePath.replace("\\", "/");  // Normalize backslashes to forward slashes

    // Create a QProcess to run the Python script
    QProcess process;
    QString pythonPath = "C:/Users/alabe/AppData/Local/Programs/Python/Python312/python.exe";  // Use forward slashes
    QString scriptPath = "C:/Users/alabe/Pictures/metiers avances/face/zouz.py";  // Use forward slashes

    // Run Python script with the selected image path
    process.start(pythonPath, QStringList() << scriptPath << filePath);
    process.waitForFinished(); // Wait for the script to finish execution

    // Read output from the Python script
    QByteArray output = process.readAllStandardOutput();
    QByteArray error = process.readAllStandardError();

    if (!error.isEmpty()) {
        qDebug() << "Python script error: " << error;
        return;
    }

    // Convert the QByteArray to a QString and clean the output
    QString outputString = QString::fromUtf8(output).trimmed();

    // Debugging: print raw output
    qDebug() << "Raw Python Output: " << outputString;

    // Clean the output by removing "Extracted Information:" and "Player's face saved as..."
    int jsonStartIndex = outputString.indexOf("{");
    int jsonEndIndex = outputString.lastIndexOf("}");

    if (jsonStartIndex == -1 || jsonEndIndex == -1) {
        qDebug() << "Invalid JSON format: No JSON found in the output.";
        return;
    }

    // Extract valid JSON portion from the string
    QString jsonString = outputString.mid(jsonStartIndex, jsonEndIndex - jsonStartIndex + 1);

    // Debugging: check the cleaned-up JSON string
    qDebug() << "Cleaned JSON String: " << jsonString;

    // Parse the cleaned-up output as JSON
    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonResponse.isNull()) {
        qDebug() << "Error parsing JSON response." << jsonString;
        return;
    }

    QJsonObject jsonObject = jsonResponse.object();

    // Extract information from JSON and set the fields
    ui->NomInput->setText(jsonObject["Nom"].toString());
    ui->PrenomInput->setText(jsonObject["Prenom"].toString());
    ui->dsInput->setDate(QDate::fromString(jsonObject["Date de naissance"].toString(), "dd/MM/yyyy"));
    ui->NationaliteInput->setText(jsonObject["Nationalite"].toString());

    // Debugging output
    qDebug() << "Extracted License Info:\n" << jsonObject;

    // If face image path is provided, fill the input with the image path
    QString faceImagePath = jsonObject["FaceImagePath"].toString();
    if (!faceImagePath.isEmpty()) {
        qDebug() << "Face Image Path: " << faceImagePath;
        ui->Img_pathInput->setText(faceImagePath); // Fill the input field with the face image path
    }
}
//alaend
