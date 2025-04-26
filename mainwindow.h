#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
#include"championnats.h"
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
    Ui::MainWindow* getUi() const { return ui; }


private:
    Ui::MainWindow *ui;
    QTimer *validationTimer;
    QTimer *validationTimerUP;
    QString rechercheact;
    QString triact;
    Scene1 *scene1;


private slots:
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
    void on_addMatchButton_clicked();
    void on_CBT_clicked();
    void on_HideCW_clicked();
    void on_SendARD_clicked();
    //void checkSerialData();




};



#endif // MAINWINDOW_H
