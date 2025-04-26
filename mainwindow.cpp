#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "championnats.h"
#include <statisticscontainer.h>
#include <QMessageBox>
#include <QSqlRecord>
#include "changeWidget.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedChamp("")
{
    ui->setupUi(this);

    scene1 = new Scene1();
    //design
    applyDesign(ui);
    StyleTW(ui->tableWidget);
    ui->CW->setVisible(false);

    //changeWidget


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

    ui->tabWidget->setTabVisible(3, false);
    connect(ui->PBA, &QPushButton::clicked, this, &MainWindow::ajoutChampB_clicked);
    connect(ui->majbutton, &QPushButton::clicked, this, &MainWindow::modifChampB_clicked);
    connect(ui->recherche, &QPushButton::clicked, this, &MainWindow::rechrecheparnom);
    connect(ui->exp_b,&QPushButton::clicked,this,&MainWindow::selectExp);
    ui->tableWidget->setSortingEnabled(true);
    connect(ui->addStadiumbuttoN,&QPushButton::clicked,this, &MainWindow::on_addStadiumbuttoN_clicked);




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
        }
    });
    serialTimer->start(2500);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene1;

}

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
        // **Draw Row Line**
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
    QString sql = QString("SELECT E1.NOM_EQUIPE AS HomeTeam, E2.NOM_EQUIPE AS VisitorTeam, "
                          "M.DATE_MATCH AS MatchDate, M.ID_MATCH AS MatchID, M.SCORE AS MatchScore, "
                          "M.TYPE AS MatchType "
                          "FROM MALEK.MATCHES M "
                          "JOIN MALEK.JOUER J ON M.ID_MATCH = J.ID_MATCH "
                          "JOIN MALEK.EQUIPES E1 ON J.ID_EQUIPEH = E1.ID_EQUIPE "
                          "JOIN MALEK.EQUIPES E2 ON J.ID_EQUIPEV = E2.ID_EQUIPE "
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
                  "EH.NOM_EQUIPE AS HOMETEAM, EV.NOM_EQUIPE AS AWAYTEAM "
                  "FROM MATCHES M "
                  "JOIN JOUER J ON M.ID_MATCH = J.ID_MATCH "
                  "JOIN EQUIPES EH ON J.ID_EQUIPEH = EH.ID_EQUIPE "
                  "JOIN EQUIPES EV ON J.ID_EQUIPEV = EV.ID_EQUIPE "
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
                            E.NOM_EQUIPE,
                            SUM(
                                CASE
                                    WHEN (J.ID_EQUIPEH = E.ID_EQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) >
                                                                      TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))) THEN 3
                                    WHEN (J.ID_EQUIPEV = E.ID_EQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) >
                                                                      TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))) THEN 3
                                    WHEN (TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) =
                                          TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) AND
                                         (J.ID_EQUIPEH = E.ID_EQUIPE OR J.ID_EQUIPEV = E.ID_EQUIPE)) THEN 1
                                    ELSE 0
                                END
                            ) AS POINTS,

                            SUM(
                                CASE
                                    WHEN J.ID_EQUIPEH = E.ID_EQUIPE THEN
                                        TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) -
                                        TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))
                                    WHEN J.ID_EQUIPEV = E.ID_EQUIPE THEN
                                        TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) -
                                        TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))
                                    ELSE 0
                                END
                            ) AS GOAL_DIFFERENCE

                        FROM EQUIPES E
                        JOIN JOUER J ON E.ID_EQUIPE = J.ID_EQUIPEH OR E.ID_EQUIPE = J.ID_EQUIPEV
                        JOIN MATCHES M ON J.ID_MATCH = M.ID_MATCH
                        WHERE M.NOM_CHAMP = :nomChamp
                        GROUP BY E.NOM_EQUIPE
                        ORDER BY POINTS DESC, GOAL_DIFFERENCE DESC
                    )
                    WHERE ROWNUM = 1
        )";
        query.prepare(leagueWinnerQuery);
        query.bindValue(":nomChamp", nomChamp);

        if (query.exec() && query.next()) {
            winner = query.value("NOM_EQUIPE").toString();
        } else {
            qDebug() << "Ligue Winner Query failed: " << query.lastError();
        }
    } else {
        // Winner is the best attack team
        QString bestAttackQuery = R"(
            SELECT * FROM (
                SELECT
                    E.NOM_EQUIPE,
                    SUM(CASE
                            WHEN J.ID_EQUIPEH = E.ID_EQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))
                            WHEN J.ID_EQUIPEV = E.ID_EQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))
                            ELSE 0
                        END) AS TOTAL_GOALS
                FROM EQUIPES E
                JOIN JOUER J ON E.ID_EQUIPE = J.ID_EQUIPEH OR E.ID_EQUIPE = J.ID_EQUIPEV
                JOIN MATCHES M ON J.ID_MATCH = M.ID_MATCH
                WHERE M.NOM_CHAMP = :nomChamp
                GROUP BY E.NOM_EQUIPE
                ORDER BY TOTAL_GOALS DESC
            )
            WHERE ROWNUM = 1
        )";
        query.prepare(bestAttackQuery);
        query.bindValue(":nomChamp", nomChamp);

        if (query.exec() && query.next()) {
            winner = query.value("NOM_EQUIPE").toString();
        } else {
            qDebug() << "Best Attack Query failed: " << query.lastError();
        }
    }

    // Best Defense Query
    QString bestDefenseQuery = R"(
        SELECT * FROM (
            SELECT
                E.NOM_EQUIPE,
                SUM(CASE
                        WHEN J.ID_EQUIPEH = E.ID_EQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1))
                        WHEN J.ID_EQUIPEV = E.ID_EQUIPE THEN TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1))
                        ELSE 0
                    END) AS GOALS_CONCEDED
            FROM EQUIPES E
            JOIN JOUER J ON E.ID_EQUIPE = J.ID_EQUIPEH OR E.ID_EQUIPE = J.ID_EQUIPEV
            JOIN MATCHES M ON J.ID_MATCH = M.ID_MATCH
            WHERE M.NOM_CHAMP = :nomChamp
            GROUP BY E.NOM_EQUIPE
            ORDER BY GOALS_CONCEDED ASC
        )
        WHERE ROWNUM = 1
    )";

    query.prepare(bestDefenseQuery);
    query.bindValue(":nomChamp", nomChamp);
    QString bestDefense = "Unknown";

    if (query.exec() && query.next()) {
        bestDefense = query.value("NOM_EQUIPE").toString();
    } else {
        qDebug() << "Best Defense Query failed: " << query.lastError();
    }

    // Clean Sheets Query
    QString cleanSheetsQuery = R"(
                SELECT * FROM (
                    SELECT
                        E.NOM_EQUIPE,
                        COUNT(DISTINCT M.ID_MATCH) AS CLEAN_SHEETS
                    FROM EQUIPES E
                    JOIN JOUER J ON E.ID_EQUIPE = J.ID_EQUIPEH OR E.ID_EQUIPE = J.ID_EQUIPEV
                    JOIN MATCHES M ON J.ID_MATCH = M.ID_MATCH
                    WHERE M.NOM_CHAMP = 'LALIGAB'
                      AND (
                          (J.ID_EQUIPEH = E.ID_EQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, INSTR(M.SCORE, '-') + 1)) = 0) OR
                          (J.ID_EQUIPEV = E.ID_EQUIPE AND TO_NUMBER(SUBSTR(M.SCORE, 1, INSTR(M.SCORE, '-') - 1)) = 0)
                      )
                    GROUP BY E.NOM_EQUIPE
                    ORDER BY CLEAN_SHEETS DESC
                ) CLEAN_STATS
                WHERE ROWNUM = 1;
    )";

    query.prepare(cleanSheetsQuery);
    query.bindValue(":nomChamp", nomChamp);
    QString cleanSheets = "Unknown";

    if (query.exec() && query.next()) {
        cleanSheets = query.value("NOM_EQUIPE").toString();
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

void MainWindow::on_addMatchButton_clicked()
{

    QDate selectedDate = ui->dateEdit->date();
    QString nomChamp = ui->listChamp_3->currentText();


    if (!isDatePlanned(nomChamp, selectedDate)) {
        QMessageBox::warning(this, "Date Not Planned", "This date is not planned for the selected championship.");
        return;
    }


    int idMatch = ui->ID_m->text().toInt();
    QString score = ui->Scor->text();
    QString type = ui->TYP->text();


    Match m(nomChamp, idMatch, score, type, selectedDate);


    if (m.addMatch()) {
        QMessageBox::information(this, "Match Added", "The match was successfully added.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to add the match.");
    }
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
