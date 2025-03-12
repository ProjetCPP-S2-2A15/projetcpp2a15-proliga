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
    void on_addStadiumbutton_clicked(); // Slot for adding a stadium
    void on_pushbuttonmodifier_clicked();
    void on_pushButton_supprimer_clicked();
};

#endif // MAINWINDOW_H
