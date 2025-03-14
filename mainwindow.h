#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>

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

private slots:
    void ajoutChampB_clicked();
    void loadChampData();
    void SuppChamp(int ind);
    void loadForUpdate(int ind);
    void modifChampB_clicked();
    void validateInputs();
    void validateInputsUP();


};

#endif // MAINWINDOW_H
