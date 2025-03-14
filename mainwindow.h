#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include "connexion.h"
#include"equipe.h"
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

private slots:
    void Ajouter_clicked();
    bool validateName();
    bool validateCountry();
    bool validateCoach();
    bool validateGoals();
    bool validatePlayers();
    void checkInput();
    void refreshTable();
    void openModificationDialog(int idEquipe);
    void modifyEquipe(int equipeId);
    void addButtonsToTable();
    void addActionColumn();
    void exportTableToPDF();
private:
    Ui::MainWindow *ui;

  Equipe *Eq;

};
#endif // MAINWINDOW_H
