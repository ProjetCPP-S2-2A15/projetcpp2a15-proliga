#include <QSize>  // Add this line

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


    // Apply styles to buttons in iconOnlySidebar with more spacing and white text
    QList<QPushButton*> buttons = ui->iconOnlySidebar->findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        button->setStyleSheet(
            "QPushButton { "
            "    border: none; "
            "    background-color: transparent; "
            "    color: white; "  // White text
            "    margin-bottom: 50px;"  // Increased spacing
            "} "
            "QPushButton:hover { background-color: #56657380; }"
            "QPushButton:checked { "
            "    background-color: #4CAF50; "  // Darker green when selected
            "    color: white; "  // Keep white text when selected
            "} "
            );
    }
    ui->exporter_match->setStyleSheet(
        "QPushButton { "
        "    border: none; "
        "    background-color: transparent; "
        "    padding: 5px;"  // Ensure some padding for better appearance
        "} "
        "QPushButton:hover { background-color: #56657380; }"
        );
    ui->exporter_excel_match->setStyleSheet(
        "QPushButton { "
        "    border: none; "
        "    background-color: transparent; "
        "    padding: 5px;"  // Ensure some padding for better appearance
        "} "
        "QPushButton:hover { background-color: #56657380; }"
        );


    ui->sidebar_2->setStyleSheet(
        "QPushButton { "
        "    border: none; "
        "    background-color: transparent; "
        "    padding: 5px;"  // Ensure some padding for better appearance
        "} "
        "QPushButton:hover { background-color: #56657380; }"
        );

    // Apply styles to buttons in iconTextSidebar with more spacing and white text



    QList<QPushButton*> buttons2 = ui->iconTextSidebar->findChildren<QPushButton*>();
    for (QPushButton* button : buttons2) {
        button->setStyleSheet(
            "QPushButton { "
            "    border: none; "
            "    background-color: transparent; "
            "    color: white; " // White text
            "    margin-bottom: 15px;"  // Increased spacing
            "} "
            "QPushButton:hover { background-color: #56657380; }"
            "QPushButton:checked { "
            "    background-color: #4CAF50; "  // Darker green when selected
            "    color: white; "  // White text when selected
            "} "
            );
    }


    // Style for tabs and their content
    ui->tabWidget_match->setStyleSheet(
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
        "  background-color:  rgb(51, 217, 153);"  // Green when selected
        "    color: black; "  // Ensure selected tab text remains black
        "} "
        "QTabBar::tab:hover {"
        "    background-color: #56657380; "  // Hover effect
        "} "
        "QTabWidget QWidget {"
        "    background-color: rgb(239, 239, 239) "  // Set content background to white explicitly
        "} "
        );

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
    ui->tableViewChamps->setStyleSheet(
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
    ui->tableViewStadeLarge->setStyleSheet(
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
    ui->tableViewConsulter->setStyleSheet(
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
        "  background-color:  rgb(51, 217, 153);"  // Green when selected


        "    color: black; "  // Ensure selected tab text remains black
        "} "
        "QTabBar::tab:hover {"
        "    background-color: #56657380; "  // Hover effect
        "} "
        "QTabWidget QWidget {"
        "    background-color: rgb(239, 239, 239) "  // Set content background to white explicitly
        "} "
        );

    // Style for tri_options (dropdown)
    ui->tri_prog_match->setStyleSheet(
        "QComboBox {"
        "    background-color: #444;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-size: 10px;"
        "    border: 1px solid #666;"
        "} "
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 20px;"
        "} "
        "QComboBox::down-arrow {"
        "    image: url(:/down.png);"
        "    width: 12px;"
        "    height: 12px;"
        "} "
        "QComboBox QAbstractItemView {"
        "    background-color: #555;"
        "    color: white;"
        "    border-radius: 5px;"
        "    selection-background-color: #666;"
        "    padding: 3px;"
        "}"
        );
    ui->tri_histo_match->setStyleSheet(
        "QComboBox {"
        "    background-color: #444;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-size: 10px;"
        "    border: 1px solid #666;"
        "} "
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 20px;"
        "} "
        "QComboBox::down-arrow {"
        "    image: url(:/down.png);"
        "    width: 12px;"
        "    height: 12px;"
        "} "
        "QComboBox QAbstractItemView {"
        "    background-color: #555;"
        "    color: white;"
        "    border-radius: 5px;"
        "    selection-background-color: #666;"
        "    padding: 3px;"
        "}"
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

    // Style for QTableWidget with green-themed design
    ui->programmation_2->setStyleSheet(
        "QTableWidget {"
        "    background-color: white;"  // White background
        "    gridline-color: rgb(126, 235, 197);"  // Soft green grid lines
        "    border: 1px solid rgb(126, 235, 197);"  // Soft green border
        "    font-size: 12px;"
        "    alternate-background-color: rgb(240, 255, 244);"  // Very light green for alternating rows
        "}"
        "QHeaderView::section {"
        "    background-color: rgb(126, 235, 197);"  // Green header
        "    padding: 6px;"
        "    border: 1px solid rgb(126, 235, 197);"  // Green border around headers
        "    font-weight: bold;"
        "}"
        "QTableWidget::item {"
        "    border-bottom: 1px solid rgb(126, 235, 197);"  // Subtle green row separator
        "    padding: 4px;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: rgb(100, 200, 170);"  // Darker green for selection
        "    color: black;"
        "}"
        );

    // Enable grid lines in the QTableWidget
    ui->programmation_2->setShowGrid(true);
    ui->programmation_2->setGridStyle(Qt::SolidLine);

    // Enable alternating row colors
    ui->programmation_2->setAlternatingRowColors(true);



    ui->refresh->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"  // Transparent background
        "    color: white;"  // White text for the button
        "    border: 2px solid transparent;"  // Transparent border initially
        "    padding: 10px 20px;"  // Padding inside the button
        "    font-size: 14px;"  // Font size for the text
        "    border-radius: 5px;"  // Rounded corners
        "    transition: background-color 0.3s, border-color 0.3s;"  // Smooth transition for hover effect"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.2);"  // Light transparent background on hover
        "    border-color: rgba(255, 255, 255, 0.6);"  // Light border on hover
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.4);"  // Darker transparent background when pressed
        "    border-color: rgba(255, 255, 255, 0.8);"  // Darker border when pressed
        "}"
        "QPushButton:focus {"
        "    outline: none;"  // Remove the outline to keep it clean
        "    border-color: rgba(255, 255, 255, 1); "  // Stronger border color when the button is selected
        "}"
        );
ui->random_arbitre_match->setStyleSheet(
    "QCheckBox {"
    "    font-size: 12px; "  // Smaller font size
    "    color: red; "  // Red text initially
    "    padding-left: 5px; "
    "    background-color: transparent; "  // Transparent background
    "    border: none; "  // No border
    "}"
    "QCheckBox:checked {"
    "    color: green; "  // Green text when checked
    "}"
    "QCheckBox:hover {"
    "    color: #ff6666; "  // Slightly lighter red on hover
    "}"
    "QCheckBox:focus {"
    "    outline: none; "
    "}"
);




}


void StyleTW(QTableWidget *tableWidget)
{
    QString buttonStyle = R"(
    QPushButton {
        background-color: #000000;  /* Black background */
        color: white;                /* White text color */
        border: 2px solid #000000;   /* Black border */
        border-radius: 8px;          /* Rounded corners */
        padding: 3px 10px;           /* Reduced padding to make it smaller */
        font-weight: bold;           /* Bold text */
        font-size: 10px;             /* Smaller font size */
        min-width: 60px;             /* Smaller minimum width */
    }
    QPushButton:hover {
        background-color: #333333;   /* Darker shade when hovered */
    }
    QPushButton:focus {
        outline: none;               /* Remove outline on focus */
        box-shadow: 0 0 .25rem rgba(0, 0, 0, 0.5);  /* Optional: Add a shadow when focused */
    }
)";
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            // Get the item in the table cell
            QWidget *cellWidget = tableWidget->cellWidget(row, col);
            if (cellWidget) {
                QPushButton *button = qobject_cast<QPushButton *>(cellWidget);
                if (button) {
                    // Apply the custom style to the button
                    button->setStyleSheet(buttonStyle);
                    button->setCursor(Qt::PointingHandCursor); // Change cursor on hover
                }
            }
        }
    }
}

