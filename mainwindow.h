#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QMainWindow>


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
    QPushButton *newButton;

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_addStadiumbutton_clicked();
    void on_pushbuttonmodifieR_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_rechercherNom_clicked();
    void on_pushButton_rechercherCapacite_clicked();
    void on_comboBoxTri_currentIndexChanged(int index);
    void on_pushButton_genererPDF_clicked();
    void on_pushButton_genererExcel_clicked();
    void on_pushButton_generer_clicked();
    void on_pushButton_afficherStats_clicked();
    void refreshMap();

};

#endif // MAINWINDOW_H
