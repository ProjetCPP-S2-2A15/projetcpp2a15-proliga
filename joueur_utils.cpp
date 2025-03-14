#include "joueur_utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
/*
void createJoueurFromUI(MainWindow *mainWin) {
    if (!mainWin) return;

    Joueur j;

    Ui::MainWindow *ui = mainWin->getUi();
    j.setNom(ui->NomInput->text());
    j.setPrenom(ui->PrenomInput->text());
    j.setDateDeNaissance(ui->DsInput->date());
    j.setPaysOrigine(ui->NationaliteInput->text());
    j.setPosition(ui->PositionInput->text());


    qDebug() << "Adding Joueur: " << j.getNom() << j.getPrenom()
             << j.getDateDeNaissance().toString() << j.getPaysOrigine();

    j.addJoueur();
}*/
