#include "design.h"
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
    ui->tableView->setStyleSheet(
                 "QTableView::item {"
                 "    padding: 10px; "
                 "    margin-right: 2px; "
                 "} "
                 "QTableView::item:selected {"
                 "    background-color: rgb(51, 217, 153); "
                 "    color: black; "
                 "} "
                 "QTableView::item:hover {"
                 "    background-color: #56657380; "
                 "} "
                 "QTableView {"
                 "    background-color: rgb(239, 239, 239); "
                 "    border: none; "
                 "}"
        "QHeaderView::section {"
        "    background-color: rgb(144, 238, 144); "
        "    color: black; "
        "    padding: 4px; "
        "    border: 1px solid #6c6c6c; "
        "}"
        "QScrollBar:vertical {"
        "    border: none; "
        "    background: rgb(239, 239, 239); "
        "    width: 10px; "
        "    margin: 0px 0px 0px 0px; "
        "} "
        "QScrollBar::handle:vertical {"
        "    background: rgb(100, 200, 100); "
        "    min-height: 20px; "
        "} "
        "QScrollBar::add-line:vertical {"
        "    border: none; "
        "    background: none; "
        "} "
        "QScrollBar::sub-line:vertical {"
        "    border: none; "
        "    background: none; "
        "}"
        "QScrollBar:horizontal {"
        "    border: none; "
        "    background: rgb(239, 239, 239); "
        "    height: 10px; "
        "    margin: 0px 0px 0px 0px; "
        "} "
        "QScrollBar::handle:horizontal {"
        "    background: rgb(100, 200, 100); "
        "    min-width: 20px; "
        "} "
        "QScrollBar::add-line:horizontal {"
        "    border: none; "
        "    background: none; "
        "} "
        "QScrollBar::sub-line:horizontal {"
        "    border: none; "
        "    background: none; "
        "}"
        );
    // Style for tabs and their content
    ui->tabWidget_2->setStyleSheet(
        "QTabWidget::pane {"
        "    background-color: white; "  // Ensure content area inside the tabs is white
        "    border: none; "
        "} "
        "QTabBar::tab {"
        "    background-color: transparent; "  // Transparent background for tabs
        "    color: black; " // Always black text
        "    padding: 10px; "
        "    margin-right: 2px; "

        "} "
        "QTabBar::tab:selected {"
        "  background-color: rgb(144,238,144);"  // Green when selected
        "    color: black; "  // Ensure selected tab text remains black
        "} "
        "QTabBar::tab:hover {"
        "    background-color: #56657380; "  // Hover effect
        "} "
        "QTabWidget QWidget {"
        "   background-color: rgb(210, 247, 221); "  // Set content background to white explicitly
        "} "
        );
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
}
