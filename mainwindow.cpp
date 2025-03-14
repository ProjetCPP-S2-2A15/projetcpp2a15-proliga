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
#include <QRegularExpression> // Assurez-vous d'inclure cette bibliothèque

#include <QInputDialog>
#include <QWidget>
#include "mainwindow.h"
#include "loginwindow.h" // Include the chatbot widget header



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->programme->setModel(Eq->loadequipeData());

addActionColumn();


    // Apply design
    applyDesign(ui);

    // Create and add the ChatBotWidget to widget_8

    // Ensure widget_8 has a layout and then add the ChatBotWidget to it

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
    connect(ui->Ajouter, &QPushButton::clicked, this, &MainWindow::Ajouter_clicked);

    connect(ui->nome, &QLineEdit::textChanged, this, &MainWindow::validateName);
    connect(ui->Pays, &QLineEdit::textChanged, this, &MainWindow::validateCountry);
    connect(ui->coach, &QLineEdit::textChanged, this, &MainWindow::validateCoach);
    connect(ui->nbmarquee, &QLineEdit::textChanged, this, &MainWindow::validateGoals);
    connect(ui->nbj, &QSpinBox::textChanged, this, &MainWindow::validatePlayers);

}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QRegularExpression> // Assurez-void vous d'inclure cette bibliothèque

void MainWindow::refreshTable() {
    // Créer un nouveau modèle
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

        // Définir le modèle dans la table
        ui->programme->setModel(model);

        // Ajouter la colonne "Action" et les boutons "Modifier" et "Supprimer"
        addActionColumn(); // Ajouter la colonne "Action" après avoir défini le modèle
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données : " + query.lastError().text());
    }
}
void MainWindow::addActionColumn() {
    // Récupérer le modèle de données actuel
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->programme->model());

    if (model) {
        // Ajouter une colonne "Action" au modèle
        model->insertColumn(model->columnCount()); // Ajouter une nouvelle colonne à la fin
        model->setHeaderData(model->columnCount() - 1, Qt::Horizontal, QObject::tr("Action"));

        // Ajouter les boutons "Modifier" et "Supprimer" dans la colonne "Action"
        addButtonsToTable();
    }
}

void MainWindow::addButtonsToTable() {
    for (int row = 0; row < ui->programme->model()->rowCount(); ++row) {
        // Créer les boutons "Modifier" et "Supprimer"
        QPushButton *modifyButton = new QPushButton("Modifier");
        QPushButton *deleteButton = new QPushButton("Supprimer");

        // Récupérer l'ID de l'équipe pour la ligne actuelle
        int equipeId = ui->programme->model()->data(ui->programme->model()->index(row, 0)).toInt();

        // Connecter le bouton "Modifier" à la méthode openModificationDialog
        connect(modifyButton, &QPushButton::clicked, [this, equipeId]() {
            openModificationDialog(equipeId);
        });

        // Connecter le bouton "Supprimer" à la méthode de suppression
        connect(deleteButton, &QPushButton::clicked, [this, equipeId]() {
            if (QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette équipe ?") == QMessageBox::Yes) {
                if (Eq->Delete(equipeId)) {
                    QMessageBox::information(this, "Succès", "Équipe supprimée avec succès !");
                    refreshTable(); // Rafraîchir la table après la suppression
                } else {
                    QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'équipe.");
                }
            }
        });

        // Créer un layout horizontal pour les boutons
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(modifyButton);
        layout->addWidget(deleteButton);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);

        // Créer un widget pour contenir les boutons
        QWidget *buttonWidget = new QWidget();
        buttonWidget->setLayout(layout);

        // Ajouter le widget dans la colonne "Action" (dernière colonne)
        ui->programme->setIndexWidget(ui->programme->model()->index(row, ui->programme->model()->columnCount() - 1), buttonWidget);
    }
}
    // Connecter chaque QLineEdit à sa fonction de validation


bool MainWindow::validateName() {
    bool valide=true;

    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->nome->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid name. It should start with a capital letter and contain only letters.");
        ui->nome->setFocus();
        valide = false;

    }
    return valide;

}

bool MainWindow::validateCountry() {
    bool valide=true;

    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->Pays->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid country name. It should start with a capital letter and contain only letters.");
        ui->Pays->setFocus();
        valide = false;


    }
    return valide;

}

bool MainWindow::validateCoach() {
    bool valide=true;

    QRegularExpression regex("^[A-Z][a-zA-Z]*$");
    if (!regex.match(ui->coach->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid coach name. It should start with a capital letter and contain only letters.");
        ui->coach->setFocus();
        valide = false;


    }
    return valide;

}

bool MainWindow::validateGoals() {
    bool valide=true;
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
    bool valide=true;

    QRegularExpression numberRegex("^[0-9]+$");
    if (!numberRegex.match(ui->nbj->text()).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Invalid number of players. It should contain only digits.");
        ui->nbj->setFocus();
        valide = false;

    } else if (ui->nbj->text().toInt() < 11) {
        QMessageBox::warning(this, "Input Error", "The number of players must be at least 11.");
        ui->nbj->setFocus();
        valide = false;

    }
    return valide;

}

void MainWindow::checkInput() {
    // Vérifier que tous les champs sont valides avant d'activer le bouton "Ajouter"
    bool isValid = validateName() && validateCountry() && validateCoach() && validateGoals() && validatePlayers();
    ui->Ajouter->setEnabled(isValid);
}
void MainWindow::Ajouter_clicked() {
    QString nomE = ui->nome->text();
    QString nomv = ui->Pays->text();
    QString nomC = ui->coach->text();
    int nbj = ui->nbj->value();

    // Vérifier quel bouton est coché et définir le type en conséquence
    QString type;
    if (ui->Normal->isChecked()) {
        type = "Normal";
    } else if (ui->Nationnal->isChecked()) {
        type = "National";
    } else {
        QMessageBox::warning(this, "Warning", "Please select a type.");
        return; // Quitter si aucun type n'est sélectionné
    }

    int nbm = ui->nbmarquee->text().toInt();
    QDate dateDebutContrat = ui->dbcnt->date(); // Supposons que dbcnt est un QDateEdit
    QDate dateFinContrat = ui->fincnt->date();  // Supposons que fincnt est un QDateEdit

    // Créer un objet Equipe avec les données
    Equipe EQUIPE(nomE, nomv, nomC, nbj, type, nbm, dateDebutContrat, dateFinContrat);
    bool success = EQUIPE.Ajouter();
    if (success) {
        QMessageBox::information(this, "Success", "Equipe added successfully!");
        refreshTable(); // Rafraîchir la table après l'ajout
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the EQUIPE: " + QSqlDatabase::database().lastError().text());
    }
}
void MainWindow::openModificationDialog(int equipeId) {
    // Récupérer les données de l'équipe
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

        // Gérer le type (Normal ou National)
        if (query.value("TYPE").toString() == "Normal") {
            ui->Normal->setChecked(true);
        } else {
            ui->Nationnal->setChecked(true);
        }

        // Changer le texte du bouton "Ajouter" en "Modifier"
        ui->Ajouter->setText("Modifier");

        // Déconnecter tous les signaux précédents du bouton "Ajouter"
        disconnect(ui->Ajouter, &QPushButton::clicked, nullptr, nullptr);

        // Connecter le signal du bouton "Ajouter" à la méthode modifyEquipe
        connect(ui->Ajouter, &QPushButton::clicked, [this, equipeId]() { modifyEquipe(equipeId); });
    }
}
void MainWindow::modifyEquipe(int equipeId) {
    QString nomE = ui->nome->text();
    QString nomv = ui->Pays->text();
    QString nomC = ui->coach->text();
    int nbj = ui->nbj->value();
    int nbm = ui->nbmarquee->text().toInt();
    QString type = ui->Normal->isChecked() ? "Normal" : "National";

    QSqlQuery query;
    query.prepare("UPDATE equipe SET NOMEQUIPE = :nomE, NOMVILLE = :nomv, NOMENTRAINEUR = :nomC, "
                  "NOMBREJOUEURS = :nbj, NOMBREBUTSMARQUES = :nbm, TYPE = :type WHERE IDEQUIPE = :id");
    query.bindValue(":nomE", nomE);
    query.bindValue(":nomv", nomv);
    query.bindValue(":nomC", nomC);
    query.bindValue(":nbj", nbj);
    query.bindValue(":nbm", nbm);
    query.bindValue(":type", type);
    query.bindValue(":id", equipeId);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Equipe modified successfully!");

        // Réinitialiser les champs
        ui->nome->clear();
        ui->Pays->clear();
        ui->coach->clear();
        ui->nbj->setValue(0);
        ui->nbmarquee->clear();

        // Réinitialiser le bouton "Ajouter"
        ui->Ajouter->setText("Ajouter");

        // Déconnecter tous les signaux précédents du bouton "Ajouter"
        disconnect(ui->Ajouter, &QPushButton::clicked, nullptr, nullptr);

        // Reconnecter le bouton "Ajouter" à la méthode Ajouter_clicked
        connect(ui->Ajouter, &QPushButton::clicked, this, &MainWindow::Ajouter_clicked);

        // Recharger les données
        refreshTable();
    } else {
        QMessageBox::critical(this, "Error", "Failed to modify the equipe: " + query.lastError().text());
    }
}
