#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include<match.h>
#include <QWidget>
#include <QLabel>
#include <QChartView>

#include <QPushButton>  // Include for QPushButton
  // Include for chart view
#include <QBarSet>      // Include for bar set
#include <QBarSeries>   // Include for bar series
#include <QPieSeries>   // Include for pie series
#include <QSqlQuery>    // Include for SQL query
#include <QGraphicsOpacityEffect>  // For opacity effect
#include <QPropertyAnimation>  // For animations
#include <QTimer>
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
  QTableWidget *programmation_2;
    QTableWidget * historique_table;
     QLabel *warningLabel;

     int originalTabWidth;
     int originalTableWidth;
     int originalWidget6Width;
     bool isExpanded;
     int originalDeleteMatchX;
     int originalChercherWidth;


 QPushButton *nextButton;
     QChartView *chartView = nullptr; // Stocker le QChartView
 bool eventFilter(QObject *watched, QEvent *event);


private slots:
    void on_addMatchButton_clicked();
    void toggleIconOnlySidebar();
    void filterTable();
    void deleteMatch();
    void handleRandomReferees(int state);
    void loadRefereesIntoComboBox();
    void  loadEquipes();
    void onCellDoubleClicked(int row, int column);
    void onHistoriqueCellDoubleClicked(int row, int column);
    void onItemChanged(QTableWidgetItem *item);
    void onShowButtonClicked();
    void onShow3ButtonClicked();
    void on_exporter_match_clicked();
     void exportStatisticsToPDF(const QString &filePath);
    void exportTableToPDF(QTableWidget *table, const QString &filename);
    void on_tri_prog_clicked();
    void on_tri_histo_clicked();
    void predictWinner(int row);
    void showMonthlyMatchStatistics();
   void on_exporter_excel_match_clicked();
};


#endif // MAINWINDOW_H






