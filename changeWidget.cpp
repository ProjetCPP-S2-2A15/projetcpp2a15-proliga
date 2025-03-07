#include "changeWidget.h"
#include "mainwindow.h"

void on_home1_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("homePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_home2_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("homePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_competition1_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("competitionPage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_competition2_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("competitionPage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_match1_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("matchPage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_match2_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("matchPage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_equipe1_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("equipePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_equipe2_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("equipePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_joueur1_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("joueurPage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_joueur2_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("joueurPage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_arbitre1_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("arbitrePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_arbitre2_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("arbitrePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_stade1_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("stadePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}

void on_stade2_toggled(Ui::MainWindow* ui, bool checked)
{
    if (checked){
        QWidget* page = ui->stackedWidget->findChild<QWidget*>("stadePage");
        if (page) {
            ui->stackedWidget->setCurrentWidget(page);
        }
    }
}
