#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
#include "arduino.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QTableWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

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
    void setupTableWithDeleteButtons(QTableWidget* tableWidgetPlayers);
    void setupTableWithDeleteButtons2(QTableWidget* tableWidgetPlayers, const QString nom);
    void setupTableWithDeleteButtons3(QTableWidget* tableWidgetPlayers, const QString arg1);
    void validateInputs();
    void freeInputs();
    void refreshStats();
    void setupArduinoConnection();
    void handleArduinoData();
    void incrementYellowCards(const QString &playerName);
    void incrementRedCards(const QString &playerName);

private slots:
    void onAjouterButtonClicked();   \
        void onrechercherButtonClicked();
    void exportToPDF();
        void on_deleteButton_clicked();

    //void uploadImage();

        void on_comboBox_currentTextChanged(const QString &arg1);

    private:
    Ui::MainWindow *ui;
    int selected_row=-1;
    Arduino* arduino = nullptr;

};
#endif // MAINWINDOW_H
