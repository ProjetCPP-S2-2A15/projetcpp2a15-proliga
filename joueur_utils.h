#ifndef JOUEUR_UTILS_H
#define JOUEUR_UTILS_H

#include "mainwindow.h"
#include "joueur.h"

void createJoueurFromUI(MainWindow *mainWin);
void deleteJoueurFromUI(MainWindow *mainWin, const QString &nom);

#endif // JOUEUR_UTILS_H
