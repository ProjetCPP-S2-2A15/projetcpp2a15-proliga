#include "design.h"
#include <QLineEdit>

void applyDesign(Ui::MainWindow *ui)
{

    ui->iconOnlySidebar->hide();
    ui->competition1->setChecked(true);
    ui->stackedWidget->setCurrentIndex(1);

    ui->iconTextSidebar->setStyleSheet(
        "QWidget {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
        "                stop:0 #CCFFCC, stop:1 #99CCFF);"
        "    border-radius: 10px; "
        "} "
        );

    QList<QPushButton*> buttons = ui->iconOnlySidebar->findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        button->setStyleSheet(
            "QPushButton { "
            "    border: none; "
            "    background-color: transparent; "
            "    color: white; "
            "    margin-bottom: 50px;"
            "    min-width: 60px; "
            "    min-height: 30px; "
            "} "
            "QPushButton:hover { background-color: #56657380; }"
            "QPushButton:checked { "
            "    background-color: #4CAF50; "
            "    color: black; "
            "} "
            );
    }

    QList<QPushButton*> buttons2 = ui->iconTextSidebar->findChildren<QPushButton*>();
    for (QPushButton* button : buttons2) {
        button->setStyleSheet(
            "QPushButton { "
            "    border: none; "
            "    background-color: transparent; "
            "    color: white; "
            "    margin-bottom: 15px;"
            "} "
            "QPushButton:hover { background-color: #56657380; }"
            "QPushButton:checked { "
            "    background-color: #4CAF50; "
            "    color: black; "
            "} "
            );
    }

    ui->logo_text->setStyleSheet(
        "QPushButton {"
        "    min-width: 120px; "
        "    min-height: 120px; "
        "    font-size: 12px; "
        "    padding-top: 15px; "
        "    padding-left: 10px; "
        "    padding-right: 10px; "
        "    margin-top: 0px;"
        "    margin-bottom: 15px;"
        "    background-color: transparent; "
        "    color: white; "
        "    border: none;"
        "    text-align: left;"
        "    display: inline-flex;"
        "    align-items: center;"
        "} "
        "QPushButton:hover {"
        "    background-color: #56657380; "
        "} "
        "QPushButton:checked {"
        "    background-color: #4CAF50; "
        "    color: black;"
        "} "
        );

    ui->logo_text->setIconSize(QSize(29, 29));

    //joueur page
    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    if (joueurWidget) {
        //labels
        /*QList<QLabel*> labels = joueurWidget->findChildren<QLabel*>();

        for (QLabel* label : labels) {
            if (label->objectName().endsWith("Label")) {
                label->setStyleSheet("color: black; font-weight: bold;");
            }

            if (label->objectName().endsWith("Error")){
                label->setStyleSheet("color: red; font-weight: bold;");
                label->setText("Invalide!");
            }
        }*/
        //inputs
        QList<QLineEdit*> inputs = joueurWidget->findChildren<QLineEdit*>();

        for (QLineEdit* input : inputs) {
            if (input->objectName().endsWith("Input")) {
                input->setStyleSheet("background-color: black; color: white; border: 2px solid red;");
            }
        }

        //buttons
        QList<QPushButton*> buttons = joueurWidget->findChildren<QPushButton*>();

        for (QPushButton* button : buttons) {
            if (button->objectName().endsWith("Button")) {
                button->setStyleSheet("background-color: green; color: white; border: 2px solid blue;");
            }
        }

        QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");


    }





}
