#ifndef JOUEUR_UTILS_H
#define JOUEUR_UTILS_H

#include "mainwindow.h"
#include "joueur.h"

void createJoueurFromUI(MainWindow *mainWin);
void deleteJoueurFromUI(MainWindow *mainWin, const QString &nom);
void updateJoueurFromUI(MainWindow *mainWin, int row);
void confirmUpdate(MainWindow *mainWin, int row);
void rechercheJoueurFromUI(MainWindow *mainWin);

#endif // JOUEUR_UTILS_H
