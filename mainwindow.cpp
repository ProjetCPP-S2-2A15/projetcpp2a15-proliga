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

void MainWindow::setupTableWithDeleteButtons(QTableWidget* tableWidgetPlayers) {
    Joueur j;
    j.readJoueur(tableWidgetPlayers);

    // Clear any existing buttons in the last column before adding new ones
    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        QWidget *widget = tableWidgetPlayers->cellWidget(row, 5); // 5th column for delete button
        if (widget) {
            delete widget;  // Remove the old delete button if it exists
        }
    }

    // Now, add new delete buttons to each row
    for (int row = 0; row < tableWidgetPlayers->rowCount(); ++row) {
        QPushButton *deleteButton = new QPushButton("Delete");

        // Get the player's name (assuming the name is in the first column)
        QString nom = tableWidgetPlayers->item(row, 0)->text();

        // Connect the delete button to a slot (delete function in this case)
        connect(deleteButton, &QPushButton::clicked, this, [this, nom]() {
            deleteJoueurFromUI(this, nom);
        });

        // Insert the button into the last column (5th column in this case)
        tableWidgetPlayers->setCellWidget(row, 5, deleteButton);
    }
}




