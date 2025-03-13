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

    ui->comboBox->setStyleSheet(
        "QComboBox {"
        "   background-color: rgba(70, 130, 120, 1); "
        "   color: white; "
        "   border: 2px solid #468278; "
        "   border-radius: 6px; "
        "   padding: 6px 10px; "
        "   font-size: 14px; "
        "   font-weight: bold; "
        "} "
        "QComboBox:hover {"
        "   background-color: rgba(90, 150, 140, 1); "
        "} "
        "QComboBox:focus {"
        "   border-color: #5FD3BC; "
        "   box-shadow: 0 0 5px rgba(95, 211, 188, 0.7); "
        "} "

        "QComboBox QAbstractItemView {"
        "   background-color: #468278; "
        "   color: white; "
        "   selection-background-color: #5FD3BC; "
        "   border: 2px solid #356A60; "
        "   border-radius: 6px; "
        "   padding: 5px; "
        "}"
        );

    //joueur page
    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    if (joueurWidget) {
        //labels
        QList<QLabel*> labels = joueurWidget->findChildren<QLabel*>();

        for (QLabel* label : labels) {
            if (label->objectName().endsWith("Label")) {
                label->setStyleSheet("color: white; font-weight: bold;background-color: rgba(70, 130, 120, 255); border-radius: 6px; padding: 2px;");
            }

            if (label->objectName().endsWith("Error")){
                label->setText("invalide !");
                label->setStyleSheet(
                    "color: #D32F2F; "
                    "font-size: 10px; "
                    "font-weight: bold; "
                    "padding: 2px; ");
            }
        }
        //inputs
        QList<QLineEdit*> inputs = joueurWidget->findChildren<QLineEdit*>();

        for (QLineEdit* input : inputs) {
            if (input->objectName().endsWith("Input")) {
                input->setStyleSheet(
                    "QLineEdit {"
                    "   background-color: rgba(70, 130, 120, 0.9); "  // Slight transparency for a modern effect
                    "   color: white; "
                    "   border: 2px solid #468278; "  // Softer color than red
                    "   border-radius: 6px; "
                    "   padding: 6px; "
                    "   font-size: 10px; "
                    "} "
                    "QLineEdit:hover {"
                    "   background-color: rgba(90, 150, 140, 0.9); "  // Lighten background slightly
                    "} "
                    "QLineEdit:focus {"
                    "   border-color: #5FD3BC; "  // Light cyan-green for focus
                    "   box-shadow: 0 0 5px rgba(95, 211, 188, 0.7); "  // Glow effect on focus
                    "}"
                    );
            }
        }

        //buttons
        QList<QPushButton*> buttons = joueurWidget->findChildren<QPushButton*>();

        for (QPushButton* button : buttons) {
            if (button->objectName().endsWith("Button")) {
                button->setStyleSheet(
                    "QPushButton {"
                    "   background-color: #E0F2F1; "
                    "   color: #356A60; "
                    "   border: 2px solid #468278; "
                    "   border-radius: 8px; "
                    "   padding: 6px 12px; "
                    "   font-size: 10px; "
                    "   font-family: 'Poppins', sans-serif; "
                    "   transition: all 0.3s ease; "
                    "} "
                    "QPushButton:hover {"
                    "   background-color: #B2DFDB; "
                    "} "
                    "QPushButton:pressed {"
                    "   box-shadow: inset 2px 2px 5px rgba(0, 0, 0, 0.2); "
                    "}"
                    );
            }
        }

        QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");
    }
}
