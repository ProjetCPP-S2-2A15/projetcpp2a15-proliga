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
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QDebug>

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
        refreshStats();
        selected_row = -1;
    });

    //controle de saisie
    connect(ui->NomInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->PrenomInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    //connect(ui->PositionInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->NationaliteInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);
    connect(ui->dsInput, &QDateEdit::dateChanged, this, &MainWindow::validateInputs);
    connect(ui->Img_pathInput, &QLineEdit::textChanged, this, &MainWindow::validateInputs);

    //read joueur
    Joueur j;

    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");

    j.readJoueur(tableWidgetPlayers);
    setupTableWithDeleteButtons(tableWidgetPlayers);

    //export pdf
    connect(ui->pdfButton, &QPushButton::clicked, this, &MainWindow::exportToPDF);
    refreshStats();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshStats() {
    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QTabWidget* tabWidget = joueurWidget->findChild<QTabWidget*>("tabWidget");

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
}


void MainWindow::onAjouterButtonClicked() {
    createJoueurFromUI(this);
    refreshStats();
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
        tableWidgetPlayers->setCellWidget(row, 5, buttonContainer);
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
    if (ui->Img_pathInput->text().trimmed().isEmpty() ||
        !alphaRegex.match(ui->Img_pathInput->text().trimmed()).hasMatch() || ui->Img_pathInput->text().length() > 10 || ui->Img_pathInput->text().length() <3) {
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

