#ifndef DESIGN_H
#define DESIGN_H

#include <QPushButton>
#include "ui_mainwindow.h"


#include <QProxyStyle>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QTableWidget>
void StyleTW(QTableWidget *tableWidget);

void applyDesign(Ui::MainWindow *ui);

#endif // DESIGN_H
