#include "joueur_utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLineEdit>
#include <QDateEdit>
#include <QMessageBox>
#include <QtCharts>
#include <QMap>

void createJoueurFromUI(MainWindow *mainWin) {
    if (!mainWin) return;

    Joueur j;

    Ui::MainWindow *ui = mainWin->getUi();
    if (!ui->AjouterButton->isEnabled()) {
        QMessageBox::warning(mainWin, "Input Error", "Please correct all invalid inputs before proceeding.");
        return;
    }

    int pageIndex = ui->stackedWidget->indexOf(ui->joueurPage);
    QWidget* joueurWidget = ui->stackedWidget->widget(pageIndex);
    QList<QLineEdit*> inputs = joueurWidget->findChildren<QLineEdit*>();

    for (QLineEdit* input : inputs) {
        if (input->objectName()=="NomInput") {
            j.setNom(input->text());
        }
        if (input->objectName()=="PrenomInput") {
            j.setPrenom(input->text());
        }
        /*
        if (input->objectName()=="PositionInput") {
            j.setPosition(input->text());
        }*/
        if (input->objectName()=="NationaliteInput") {
            j.setPaysOrigine(input->text());
        }
        if (input->objectName()=="Img_pathInput"){
            j.setImgPath(input->text());
        }
    }

    QComboBox* positionComboBox = joueurWidget->findChild<QComboBox*>("PositionInput2");
    if (positionComboBox) {
        j.setPosition(positionComboBox->currentText());
    } else {
        qDebug() << "PositionInput not found!";
    }

    QComboBox* NomEquipeComboBox = joueurWidget->findChild<QComboBox*>("EquipeInput");
    if (NomEquipeComboBox) {
        j.setNom_Equipe(NomEquipeComboBox->currentText());
    } else {
        qDebug() << "PositionInput not found!";
    }

    QDateEdit* dsInput = joueurWidget->findChild<QDateEdit*>("dsInput");
    if (dsInput){
        j.setDateDeNaissance(dsInput->date());
    }else{
        qDebug() << "ds input not found!";
    }

    qDebug() << "Adding Joueur: " << j.getNom() << j.getPrenom()
             << j.getDateDeNaissance().toString() << j.getPaysOrigine();

    qDebug() << "Position Selected: " << j.getPosition();

    j.addJoueur();
    QTableWidget* tableWidgetPlayers = joueurWidget->findChild<QTableWidget*>("tableWidgetPlayers");
    if (tableWidgetPlayers){
        //refresh the table
        mainWin->setupTableWithDeleteButtons(ui->tableWidgetPlayers);
        mainWin->freeInputs();
    }else{
        qDebug() << "table widgetPlayers not foud !";
    }

}

void deleteJoueurFromUI(MainWindow *mainWin, const QString &nom) {
    if (!mainWin) return;

    Ui::MainWindow *ui = mainWin->getUi();

    Joueur j;
    j.deleteJoueur(nom);

    // Refresh the table after deletion
    mainWin->setupTableWithDeleteButtons(ui->tableWidgetPlayers);
}

void updateJoueurFromUI(MainWindow *mainWin, int row){
    if (!mainWin) return;

    Ui::MainWindow *ui = mainWin->getUi();

    QString nom = ui->tableWidgetPlayers->item(row, 0)->text();
    QString prenom = ui->tableWidgetPlayers->item(row, 1)->text();
    QString position = ui->tableWidgetPlayers->item(row, 3)->text();
    QString pays_origine = ui->tableWidgetPlayers->item(row, 4)->text();
    QDate date = QDate::fromString(ui->tableWidgetPlayers->item(row, 2)->text(), "yyyy-MM-dd");

    ui->NomInput->setText(nom);
    ui->PrenomInput->setText(prenom);
    ui->dsInput->setDate(date);
    //ui->PositionInput->setText(position);
    ui->PositionInput2->setCurrentText(position);
    ui->NationaliteInput->setText(pays_origine);
}

void confirmUpdate(MainWindow *mainWin, int row){
    if (!mainWin || row == -1) return; // Ensure valid inputs

    Ui::MainWindow *ui = mainWin->getUi();
    if (!ui->modifierButton->isEnabled()) {
        QMessageBox::warning(mainWin, "Input Error", "Please correct all invalid inputs before proceeding.");
        return;
    }

    // Get updated values from input fields
    QString nom = ui->NomInput->text();
    QString prenom = ui->PrenomInput->text();
    QDate date = ui->dsInput->date();
    //QString position = ui->PositionInput->text();
    QString position = ui->PositionInput2->currentText();
    QString paysOrigine = ui->NationaliteInput->text();

    // Update the table
    ui->tableWidgetPlayers->item(row, 0)->setText(nom);
    ui->tableWidgetPlayers->item(row, 1)->setText(prenom);
    ui->tableWidgetPlayers->item(row, 2)->setText(date.toString("yyyy-MM-dd"));
    ui->tableWidgetPlayers->item(row, 3)->setText(position);
    ui->tableWidgetPlayers->item(row, 4)->setText(paysOrigine);

    // Call the update function to update the database if needed
    Joueur j;
    j.updateJoueur(nom, prenom, date, position, paysOrigine);

    // Refresh the table to reflect changes
    mainWin->setupTableWithDeleteButtons(ui->tableWidgetPlayers);
    mainWin->freeInputs();
}

void rechercheJoueurFromUI(MainWindow *mainWin){
    if (!mainWin){
        return;
    }
    Ui::MainWindow *ui = mainWin->getUi();

    QString nom_recherche = ui->rechercherInput->text();
    QTableWidget* tableWidget = ui->tableWidgetPlayers;

    Joueur j;
    j.rechercheJoueur(tableWidget, nom_recherche);

    // Refresh the table to reflect changes
    mainWin->setupTableWithDeleteButtons2(tableWidget, nom_recherche);
}

QChartView* createNationalityChart() {
    QMap<QString, int> nationalityCount;
    Joueur j;
    QVector<Joueur> playerList = j.getJoueurs(); // Correctly storing the result

    if (playerList.isEmpty()) {
        qDebug() << "No players found!";
        return nullptr; // Return null if no data
    }

    // Count players by nationality
    for (const Joueur& player : playerList) {
        nationalityCount[player.getPaysOrigine()]++;
    }

    // Create the pie chart series
    QPieSeries *series = new QPieSeries();
    for (auto it = nationalityCount.begin(); it != nationalityCount.end(); ++it) {
        series->append(it.key(), it.value());
    }

    // Set up the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Players by Nationality");
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create and return the chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

QChartView* createPositionChart() {
    QMap<QString, int> positionCount;
    Joueur j;
    const QVector<Joueur>& playerList = j.getJoueurs();

    // Count players by position
    for (const Joueur& j : playerList) {
        positionCount[j.getPosition()]++;
    }

    // Create the pie chart series
    QPieSeries* series = new QPieSeries();
    for (auto it = positionCount.begin(); it != positionCount.end(); ++it) {
        series->append(it.key(), it.value());
    }

    // Set up the chart
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Players by Position");
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create and return the chart view
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

