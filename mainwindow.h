#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include "arbitre.h"
#include "connexion.h"
#include "ui_mainwindow.h"

#include "connexion.h"

#include <QFileDialog>
#include <QPixmap>
#include<match.h>
#include <QWidget>
#include <QLabel>
#include <QtCharts/QChartView>
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
#include <QSerial>
#include <QSerialPort>  // Include for serial port
#include <QSerialPortInfo>

#include "championnats.h"
#include "match.h"
#include "scene1.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QTimer>
#include <QAxObject>
#include <QPdfWriter>
#include <QPainter>
#include <QDate>
#include <QFileDialog>
#include <QDate>
#include <QtCharts>
#include <QVBoxLayout>
#include <QVector>
#include <QList>
#include "scene1.h"
#include <QPushButton>
#include <QSqlQueryModel>


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
    Arbitre A; // Instance of Arbitre class for handling CRUD
    bool validerDonnees(QString& erreur);
    void remplirFormulaireArbitre(QString nom, QString prenom, int age, int experience, QString sexe, QString email, QString telephone);
    void setCurrentArbitreId(int id);
    int currentArbitreId;
    void checkForNullScoreEdit();



private:
    Ui::MainWindow *ui;
    QTimer *validationTimer;
    QTimer *validationTimerUP;
    QString rechercheact;
    QString triact;
    QPushButton *newButton;
    QSqlQueryModel* afficherConsulter();
    QString selectedChamp;
    Scene1 *scene1;
    void loadMatchesIntoTable();
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
     QSerialPort *serial;  // Declare QSerialPort pointer for communication

 QPushButton *nextButton;
     QChartView *chartView = nullptr; // Stocker le QChartView
 bool eventFilter(QObject *watched, QEvent *event);


     bool arduinoMode = false;

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
    void on_historique_table_cellClicked(int row, int column);
   void on_showArduinoButton_clicked();
    void ajoutChampB_clicked();
    void loadChampData(const QString &searchTerm = "");
    void SuppChamp(int ind);
    void loadForUpdate(int ind);
    void modifChampB_clicked();
    bool validateInputs();
    bool validateInputsUP();
    void rechrecheparnom();
    void freeallfields();
    void exportToExcel();
    void selectExp();
    void exportToPDF();
    void sortByName();
    void on_sortBut_clicked();
    void on_statGEN_clicked();
    void loadCBchampL();
    void on_MsL_currentIndexChanged(const QString &selectedText);
    void displayCoupeBracket(const QString& champname);
    void on_LSRbut_clicked();
    void loadLigueTable(const QString& nomChamp);
    void displayCompetitionStats(const QString& champName);
    void generatePlanning(QComboBox* comboBoxChamp, int intervalDays, int year, int numberOfMatches);
    bool isDatePlanned(const QString& champname, const QDate& matchDate);
    void on_planBUT_clicked();
    void loadChampL();
    void on_CBT_clicked();
    void on_HideCW_clicked();
    void on_SendARD_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_addStadiumbuttoN_clicked();
    void on_pushbuttonmodifieR_clicked();
    void on_pushButton_supprimer_clicked();
    void on_comboBoxTri_currentIndexChanged(int index);
    void on_pushButton_genererPDF_clicked();
    void on_pushButton_genererExcel_clicked();
    void on_pushButton_generer_clicked();
    void on_pushButton_afficherStats_clicked();
    void refreshMap();
    void on_tirageButton_clicked();
    void on_tableViewChamps_clicked(const QModelIndex &index);
    void on_clearConsulterbutton_clicked();
    void on_rechercheNom_textChanged(const QString &text);
    void on_capaciteMinMax_textChanged();
    QString ARD_consulter(const QString &champName);
    //void checkSerialData();
    void on_pushButton_add_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_show_clicked();
    void on_pushButton_exporter_clicked();
    void on_pushButton_tri_clicked();
    void on_pushButton_stats_clicked();

};


#endif // MAINWINDOW_H











