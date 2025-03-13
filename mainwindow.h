#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include<match.h>
#include <QWidget>

#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
 void checkForNullScoreEdit();



private:
    Ui::MainWindow *ui;
    void loadMatchesIntoTable();
    QPushButton *newButton;
    MatchManager *matchmanager;
     QString getColumnName(int column);
       QMap<int, bool> scoreModifiedMap;

     QLabel *warningLabel;
       // Declare the original widths as member variables
     int originalTabWidth;
     int originalTableWidth;
     int originalWidget6Width;
     bool isExpanded;
     int originalDeleteMatchX;
     int originalChercherWidth;
private slots:
    void on_addMatchButton_clicked();
    void toggleIconOnlySidebar();
    void on_logoutButton_clicked();

    void filterTable();
    void deleteMatch();
    void refreshTable();

    void handleRandomReferees(int state);

    void loadRefereesIntoComboBox();
   void  loadEquipes();
    void onCellDoubleClicked(int row, int column);
void onHistoriqueCellDoubleClicked(int row, int column);
void onItemChanged(QTableWidgetItem *item);
void onShowButtonClicked();
void on_exporter_match_clicked(); // Slot pour le bouton exporter_match
void exportTableToPDF(QTableWidget *table, const QString &filename); // Fo
};
#endif // MAINWINDOW_H






