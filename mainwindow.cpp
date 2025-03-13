#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "joueur_utils.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //design
    applyDesign(ui);

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

    //crud joueur
    connect(ui->AjouterButton, &QPushButton::clicked, this, &MainWindow::onAjouterButtonClicked);
    connect(ui->rechercherButton, &QPushButton::clicked, this, &MainWindow::onrechercherButtonClicked);

    //connect(ui->deleteButton, &QPushButton::clicked, this, [=]() {deleteJoueurFromUI(this);});
    //connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::uploadImage);

    connect(ui->modifierButton, &QPushButton::clicked, this, [this]() {
        if (selected_row == -1) return;
        confirmUpdate(this, selected_row);
        selected_row = -1;
    });

    //controle de saisie
    connect(ui->NomInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->PrenomInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->PositionInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->NationaliteInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->dsInput, &QDateEdit::dateChanged, this, &MainWindow::validateInputs);



    //read joueur
    Joueur j;

    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");

    //j.readJoueur(tableWidgetPlayers);
    setupTableWithDeleteButtons(tableWidgetPlayers);


    //export pdf
    connect(ui->pdfButton, &QPushButton::clicked, this, &MainWindow::exportToPDF);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAjouterButtonClicked() {
    createJoueurFromUI(this);
}

void MainWindow::onrechercherButtonClicked(){
    rechercheJoueurFromUI(this);
}

/*
void MainWindow::uploadImage() {
    // Open file dialog to select an image
    QString filePath = QFileDialog::getOpenFileName(this, "Choose an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        ui->imgUploaded->setPixmap(pixmap.scaled(ui->imgUploaded->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
*/


void MainWindow::setupTableWithDeleteButtons(QTableWidget* tableWidgetPlayers) {
    Joueur j;
    j.readJoueur(tableWidgetPlayers); // Refresh the table

    // Clear previous widgets in column 6
    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        tableWidgetPlayers->removeCellWidget(row, 5);
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
        tableWidgetPlayers->setCellWidget(row, 5, buttonContainer);
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
        tableWidgetPlayers->removeCellWidget(row, 5);
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
        tableWidgetPlayers->setCellWidget(row, 5, buttonContainer);
    }
}



void MainWindow::validateInputs() {
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



void MainWindow::exportToPDF() {
    // Step 1: Select file to save
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;  // User canceled

    // Step 2: Set up database connection
    Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    // Step 3: Fetch data from the database
    QSqlQuery query(db);
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
    painter.setFont(QFont("Arial", 10));

    int y = 50;

    // Title
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(200, y, "Players List");
    painter.setFont(QFont("Arial", 10));
    y += 350;

    // Column Headers
    painter.drawText(250, y, "Nom");
    painter.drawText(500, y, "Prenom");
    painter.drawText(750, y, "Position");
    painter.drawText(1100, y, "Nationalite");
    painter.drawText(1500, y, "Date Naissance");
    y += 100;

    painter.drawLine(250, y, 1900, y);
    y += 200;

    // Step 5: Write player data
    while (query.next()) {
        painter.drawText(250, y, query.value(0).toString());
        painter.drawText(500, y, query.value(1).toString());
        painter.drawText(750, y, query.value(3).toString());
        painter.drawText(1100, y, query.value(2).toString());
        painter.drawText(1500, y, query.value(4).toDate().toString("yyyy-MM-dd"));
        y += 100;
    }

    painter.end();

    // Step 6: Success Message
    QMessageBox::information(this, "Success", "PDF exported successfully!");
}

void MainWindow::freeInputs(){
    ui->NomInput->setText("");
    ui->PrenomInput->setText("");
    ui->PositionInput->setText("");
    ui->NationaliteInput->setText("");
}

