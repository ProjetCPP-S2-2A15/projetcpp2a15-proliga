#include "joueur_utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLineEdit>
#include <QDateEdit>
#include <QMessageBox>

void createJoueurFromUI(MainWindow *mainWin) {
    if (!mainWin) return;

    Joueur j;

    Ui::MainWindow *ui = mainWin->getUi();
    if (!ui->AjouterButton->isEnabled()) {
        QMessageBox::warning(mainWin, "Input Error", "Please correct all invalid inputs before proceeding.");
        return;
    }

    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QList<QLineEdit*> inputs = joueurWidget->findChildren<QLineEdit*>();

    for (QLineEdit* input : inputs) {
        if (input->objectName()=="NomInput") {
            j.setNom(input->text());
        }
        if (input->objectName()=="PrenomInput") {
            j.setPrenom(input->text());
        }
        if (input->objectName()=="PositionInput") {
            j.setPosition(input->text());
        }
        if (input->objectName()=="NationaliteInput") {
            j.setPaysOrigine(input->text());
        }
    }

    QDateEdit* dsInput = joueurWidget->findChild<QDateEdit*>("dsInput");
    if (dsInput){
        j.setDateDeNaissance(dsInput->date());
    }else{
        qDebug() << "ds input not found!";
    }

    qDebug() << "Adding Joueur: " << j.getNom() << j.getPrenom()
             << j.getDateDeNaissance().toString() << j.getPaysOrigine();

    j.addJoueur();
    QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");
    if (tableWidgetPlayers){
        //refresh the table
        mainWin->setupTableWithDeleteButtons(ui->tableWidgetPlayers);
    }else{
        qDebug() << "table widgetPlayers not foud !";
    }

}

void deleteJoueurFromUI(MainWindow *mainWin, const QString &nom) {
    if (!mainWin) return;

    Ui::MainWindow *ui = mainWin->getUi();


    Joueur j;
    j.deleteJoueur(nom);

    // Refresh the table after deletion
    mainWin->setupTableWithDeleteButtons(ui->tableWidgetPlayers);
}

void updateJoueurFromUI(MainWindow *mainWin, int row){
    if (!mainWin) return;

    Ui::MainWindow *ui = mainWin->getUi();


    QString nom = ui->tableWidgetPlayers->item(row, 0)->text();
    QString prenom = ui->tableWidgetPlayers->item(row, 1)->text();
    QString position = ui->tableWidgetPlayers->item(row, 3)->text();
    QString pays_origine = ui->tableWidgetPlayers->item(row, 4)->text();
    QDate date = QDate::fromString(ui->tableWidgetPlayers->item(row, 2)->text(), "yyyy-MM-dd");

    ui->NomInput->setText(nom);
    ui->PrenomInput->setText(prenom);
    ui->dsInput->setDate(date);
    ui->PositionInput->setText(position);
    ui->NationaliteInput->setText(pays_origine);

}

void confirmUpdate(MainWindow *mainWin, int row){
    if (!mainWin || row == -1) return; // Ensure valid inputs

    Ui::MainWindow *ui = mainWin->getUi();
    if (!ui->modifierButton->isEnabled()) {
        QMessageBox::warning(mainWin, "Input Error", "Please correct all invalid inputs before proceeding.");
        return;
    }

    // Get updated values from input fields
    QString nom = ui->NomInput->text();
    QString prenom = ui->PrenomInput->text();
    QDate date = ui->dsInput->date();
    QString position = ui->PositionInput->text();
    QString paysOrigine = ui->NationaliteInput->text();

    // Update the table
    ui->tableWidgetPlayers->item(row, 0)->setText(nom);
    ui->tableWidgetPlayers->item(row, 1)->setText(prenom);
    ui->tableWidgetPlayers->item(row, 2)->setText(date.toString("yyyy-MM-dd"));
    ui->tableWidgetPlayers->item(row, 3)->setText(position);
    ui->tableWidgetPlayers->item(row, 4)->setText(paysOrigine);

    // Call the update function to update the database if needed
    Joueur j;
    j.updateJoueur(nom, prenom, date, position, paysOrigine);

    // Refresh the table to reflect changes
    mainWin->setupTableWithDeleteButtons(ui->tableWidgetPlayers);
}
