#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
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


};



#endif // MAINWINDOW_H
