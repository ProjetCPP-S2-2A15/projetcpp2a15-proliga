#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connexion.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QTableWidget>

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
    void validateInputs();

private slots:
    void onAjouterButtonClicked();   \
        void onrechercherButtonClicked();
    void exportToPDF();
    //void uploadImage();

private:
    Ui::MainWindow *ui;
    int selected_row=-1;
};
#endif // MAINWINDOW_H
