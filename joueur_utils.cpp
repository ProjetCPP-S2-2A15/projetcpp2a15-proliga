#include "joueur_utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLineEdit>
#include <QDateEdit>

void createJoueurFromUI(MainWindow *mainWin) {
    if (!mainWin) return;

    Joueur j;

    Ui::MainWindow *ui = mainWin->getUi();

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
        //j.readJoueur(tableWidgetPlayers);
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
