#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "design.h"
#include "changeWidget.h"
#include "joueur_utils.h"
#include <QFileDialog>
#include <QPixmap>

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

    //connect(ui->deleteButton, &QPushButton::clicked, this, [=]() {deleteJoueurFromUI(this);});
    //connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::uploadImage);

    connect(ui->modifierButton, &QPushButton::clicked, this, [this]() {
        if (selected_row == -1) return;
        confirmUpdate(this, selected_row);
        selected_row = -1;
    });



    //read joueur
    Joueur j;

    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");

    //j.readJoueur(tableWidgetPlayers);
    setupTableWithDeleteButtons(tableWidgetPlayers);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAjouterButtonClicked() {
    createJoueurFromUI(this);
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

/*
void MainWindow::setupTableWithDeleteButtons(QTableWidget* tableWidgetPlayers) {
    Joueur j;
    j.readJoueur(tableWidgetPlayers);

    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        QWidget *widget = tableWidgetPlayers->cellWidget(row, 5);
        if (widget) {
            delete widget;
        }
    }

    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        QPushButton *deleteButton = new QPushButton("Delete");

        QString nom = tableWidgetPlayers->item(row, 0)->text();

        connect(deleteButton, &QPushButton::clicked, this, [this, nom]() {
            deleteJoueurFromUI(this, nom);
        });

        tableWidgetPlayers->setCellWidget(row, 5, deleteButton);
    }
}*/

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





