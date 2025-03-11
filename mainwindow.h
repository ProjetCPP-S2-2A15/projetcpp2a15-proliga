#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include<match.h>
#include <QWidget>

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




private:
    Ui::MainWindow *ui;
    void loadMatchesIntoTable();
    QPushButton *newButton;
    MatchManager *matchmanager;

private slots:
    void on_addMatchButton_clicked();
    void toggleIconOnlySidebar();
    void on_logoutButton_clicked();
    void editMatch();
    void filterTable();
    void deleteMatch();
    void refreshTable();

    void handleRandomReferees(int state);

    void loadRefereesIntoComboBox();
};
#endif // MAINWINDOW_H






