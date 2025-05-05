#include "joueur.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>
#include <QFile>
#include <QByteArray>
#include <QSqlDatabase>
#include <QLabel>

Joueur::Joueur() {}

void Joueur::addJoueur() {
    /*
    Connection conn;

    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();

    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);*/
    QSqlQuery query;
    // Prepare the SQL query to insert data including the image BLOB
    query.prepare("INSERT INTO joueur1 (Nom, Prenom, Date_de_naissance, Pays_origine, Position, Img) "
                  "VALUES (:Nom, :Prenom, :Date_de_naissance, :Pays_origine, :Position, :Img)");

    // Bind values for other fields
    query.bindValue(":Nom", this->Nom);
    query.bindValue(":Prenom", this->Prenom);
    query.bindValue(":Date_de_naissance", this->Date_de_naissance);
    query.bindValue(":Pays_origine", this->Pays_origine);
    query.bindValue(":Position", this->Position);

    // Read the image from the file path and bind it as a BLOB
    QString imagePath = this->ImgPath;  // Assuming Img_pathInput contains the path to the image
    QFile imageFile(imagePath);

    if (imageFile.open(QIODevice::ReadOnly)) {
        QByteArray imageData = imageFile.readAll();  // Read the image data as binary
        query.bindValue(":Img", imageData);  // Bind the image data as a BLOB
    } else {
        qDebug() << "Failed to open image file: " << imagePath;
    }

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Failed to add Joueur: " << query.lastError().text();
    } else {
        qDebug() << "Joueur added successfully!";
    }
}



void Joueur::getListe(QComboBox* liste){
    /*Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }*/

    QString queryString;
    queryString = "SELECT NOM FROM joueur1";

    QSqlQuery query;

    liste->clear();

    while (query.next()){
        QString playerName = query.value(0).toString();  // Retrieve the player name
        liste->addItem(playerName);
    }

}


void Joueur::readJoueur(QTableWidget *tableWidget) {
    if (!tableWidget) return;


    QString queryString;
    if (filter == 1) {
        queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE, IMG, NB_YELLOW, RED_CARD FROM joueur1 ORDER BY NOM";
    } else if (filter == 2) {
        queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE, IMG, NB_YELLOW, RED_CARD FROM joueur1 ORDER BY PAYS_ORIGINE";
    } else {
        queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE, IMG, NB_YELLOW, RED_CARD FROM joueur1 ORDER BY POSITION";
    }

    QSqlQuery query(queryString);

    tableWidget->setColumnCount(9);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Nom" << "Prenom" << "Date de Naissance" << "Position" << "Pays d'origine" << "Nb_Yellow" << "Red_Card" << "Photo");
    tableWidget->setRowCount(0);

    int row = 0;

    while (query.next()) {
        tableWidget->insertRow(row);

        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("Nom").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Prenom").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Date_de_naissance").toDate().toString("yyyy-MM-dd")));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("Position").toString()));
        tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("PAYS_ORIGINE").toString()));
        tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("Nb_Yellow").toString()));
        tableWidget->setItem(row, 6, new QTableWidgetItem(query.value("Red_Card").toString()));

        // 🖼️ Load image
        QByteArray imageData = query.value("IMG").toByteArray();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);

        // Create QLabel to display the image
        QLabel *imageLabel = new QLabel();
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLabel->setAlignment(Qt::AlignCenter);
            imageLabel->setScaledContents(true);  // Ensure the image scales to fit
        } else {
            imageLabel->setText("No image");
            imageLabel->setAlignment(Qt::AlignCenter);
        }

        // Set the image label in the table
        tableWidget->setCellWidget(row, 7, imageLabel);

        // Optional: Adjust row height and column width
        tableWidget->setRowHeight(row, 100);
        tableWidget->setColumnWidth(7, 100);

        row++;

    }

    if (query.lastError().isValid()) {
        qDebug() << "Error reading players:" << query.lastError().text();
    } else {
        qDebug() << "Players loaded successfully!";
    }
}



QVector<Joueur> Joueur::getJoueurs() {
    QVector<Joueur> joueurs;

    /*Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return joueurs;
    }

    QSqlDatabase db = conn.getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return joueurs;
    }*/

    QString queryString;
    queryString = "SELECT NOM, PRENOM, PAYS_ORIGINE, POSITION, DATE_DE_NAISSANCE FROM joueur1";

    QSqlQuery query;

    while (query.next()) {
        Joueur j;
        j.setNom(query.value("NOM").toString());
        j.setPrenom(query.value("PRENOM").toString());
        j.setPaysOrigine(query.value("PAYS_ORIGINE").toString());
        j.setPosition(query.value("POSITION").toString());
        j.setDateDeNaissance(query.value("DATE_DE_NAISSANCE").toDate());

        joueurs.append(j);
    }

    if (query.lastError().isValid()) {
        qDebug() << "Error reading players:" << query.lastError().text();
    } else {
        qDebug() << "Players loaded successfully!";
    }

    return joueurs;
}



void Joueur::deleteJoueur(const QString &nom) {
    if (nom.isEmpty()) {
        qDebug() << "Player name is empty!";
        return;
    }

    /*Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }*/

    QSqlQuery query;
    query.prepare("DELETE FROM joueur1 WHERE Nom = :Nom");
    query.bindValue(":Nom", nom);

    if (!query.exec()) {
        qDebug() << "Failed to delete Joueur:" << query.lastError().text();
    } else {
        qDebug() << "Joueur deleted successfully!";
    }
}

bool Joueur::updateJoueur(const QString &nom, const QString &prenom, const QDate &date, const QString &position, const QString &paysOrigine) {
    /*Connection conn;

    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return false;
    }

    QSqlDatabase db = conn.getDatabase();

    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return false;
    }
*/
    QSqlQuery query;
    query.prepare("UPDATE joueur1 SET Prenom = :prenom, Date_de_naissance = TO_DATE(:date, 'YYYY-MM-DD'), Position = :position, Pays_origine = :paysOrigine WHERE Nom = :nom");

    // Bind values
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":position", position);
    query.bindValue(":paysOrigine", paysOrigine);

    if (!query.exec()) {
        qDebug() << "Error updating player: " << query.lastError().text();
        return false;
    } else {
        qDebug() << "Player updated successfully!";
        return true;
    }
}

void Joueur::rechercheJoueur(QTableWidget *tableWidget, const QString &nom) {
    if (!tableWidget) return;

    /*Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }*/

    QString trimmedNom = nom.trimmed();
    QString lowerCaseNom = trimmedNom.toLower();

    QSqlQuery query;
    QString queryString = QString("SELECT Nom, Prenom, Pays_origine, Position, Date_de_naissance "
                                  "FROM joueur1 WHERE LOWER(Nom) = LOWER('%1')").arg(lowerCaseNom);
    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("Nom").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Prenom").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("Position").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Date_de_naissance").toDate().toString("yyyy-MM-dd")));
        tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("Pays_origine").toString()));

        row++;
    }

    if (row == 0) {
        qDebug() << "No players found for the given name:" << nom;
    } else {
        qDebug() << "Players loaded successfully!";
    }
}

void Joueur::rechercheJoueurFilter(QTableWidget *tableWidget, const QString &arg1) {
    if (!tableWidget) return;

    /*Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to database!";
        return;
    }

    QSqlDatabase db = conn.getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }
*/
    QSqlQuery query;
    QString queryString = "SELECT Nom, Prenom, Pays_origine, Position, Date_de_naissance FROM joueur1";

    // Modify query based on selected filter option
    if (arg1 == "Nom") {
        queryString += " ORDER BY Nom";
    } else if (arg1 == "Nationalite") {
        queryString += " ORDER BY Pays_origine";
    } else if (arg1 == "Position") {
        queryString += " ORDER BY Position";
    }

    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    tableWidget->clearContents();  // Clear previous rows but keep the table structure (headers)

    int row = 0;
    bool playersFound = false;

    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("Nom").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Prenom").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("Position").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Date_de_naissance").toDate().toString("yyyy-MM-dd")));
        tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("Pays_origine").toString()));

        row++;
        playersFound = true;
    }

    if (!playersFound) {
        qDebug() << "No players found for the selected filter: " << arg1;
    } else {
        qDebug() << "Players loaded successfully!";
    }
}
