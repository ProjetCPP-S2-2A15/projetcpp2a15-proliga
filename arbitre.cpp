#include "arbitre.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <QRegularExpression>
#include <QMessageBox>
#include <QPushButton>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "qstandardpaths.h"
#include "ui_mainwindow.h"
#include <QPrinter>

// Default constructor
Arbitre::Arbitre() : id_arbitre(0), age(0), experience(0) {}

Arbitre::Arbitre(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
    if (!mainWindow) {
        qDebug() << "Error: MainWindow pointer is NULL";
    }
}

// Constructor without ID since it's auto-incremented
Arbitre::Arbitre(QString nom, QString prenom, int age, int experience, QString sexe, QString email)
{
    this->nom = nom;
    this->prenom = prenom;
    this->age = age;
    this->experience = experience;
    this->sexe = sexe;
    this->email = email;
}

// ✅ Add a referee (ID is auto-generated)
bool Arbitre::ajouter() {
    QString erreur;
    if (!validerDonnees(erreur)) {
        qDebug() << erreur;  // still useful for debugging
        QMessageBox::critical(nullptr, "Erreur de validation", erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO \"C##HEDI\".\"ARBITRE\" (NOM, PRENOM, AGE, EXPERIENCE, SEXE, EMAIL) "
                  "VALUES (:nom, :prenom, :age, :experience, :sexe, :email)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":experience", experience);
    query.bindValue(":sexe", sexe);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "❌ SQL Insert Error:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur SQL", "Échec de l'ajout de l'arbitre :\n" + query.lastError().text());
        return false;
    }

    qDebug() << "✅ Arbitre added successfully!";
    return true;
}


// ✅ Delete a referee
bool Arbitre::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM \"C##HEDI\".\"ARBITRE\" WHERE ID_ARBITRE = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ SQL Delete Error:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Arbitre deleted successfully!";
    return true;
}

// ✅ Modify a referee
bool Arbitre::modifier(int id, QString nom, QString prenom, int age, int experience, QString sexe, QString email) {
    QSqlQuery query;
    query.prepare("UPDATE \"C##HEDI\".\"ARBITRE\" SET NOM = :nom, PRENOM = :prenom, AGE = :age, "
                  "EXPERIENCE = :experience, SEXE = :sexe, EMAIL = :email WHERE ID_ARBITRE = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":experience", experience);
    query.bindValue(":sexe", sexe);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "❌ SQL Update Error:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Arbitre modified successfully!";
    return true;
}




// ✅ Display referees in QTableWidget
// In Arbitre.cpp:




void Arbitre::afficher(QTableWidget *tableWidget) {
    QSqlQuery query;
    query.prepare("SELECT * FROM \"C##HEDI\".\"ARBITRE\"");

    if (!query.exec()) {
        qDebug() << "❌ SQL Select Error:" << query.lastError().text();
        return;
    }

    QPushButton *exportBtn = new QPushButton("Exporter PDF");
    exportBtn->setStyleSheet("padding: 5px; font-weight: bold;");
    tableWidget->setCellWidget(0, 10, exportBtn);

    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(10);  // 3 extra columns for CV, Edit, and Delete buttons
    tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Age", "Expérience", "Sexe", "Email", "CV", "Modifier", "Supprimer"});

    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);

        for (int col = 0; col < 7; col++) {
            tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        // Add 'CV' button
        QPushButton *cvBtn = new QPushButton("CV");
        tableWidget->setCellWidget(row, 7, cvBtn);

        // Add 'Edit' button
        QPushButton *editBtn = new QPushButton("Modifier");
        tableWidget->setCellWidget(row, 8, editBtn);

        // Add 'Delete' button
        QPushButton *deleteBtn = new QPushButton("Supprimer");
        tableWidget->setCellWidget(row, 9, deleteBtn);

        QPushButton *btn = new QPushButton("CV");
        tableWidget->setCellWidget(row, 7, btn);

        // Capture row values for PDF generation
        QString id = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString age = query.value(3).toString();
        QString experience = query.value(4).toString();
        QString sexe = query.value(5).toString();
        QString email = query.value(6).toString();

        QObject::connect(btn, &QPushButton::clicked, tableWidget, [=]() {

            QString filePath = QFileDialog::getSaveFileName(nullptr, "Enregistrer le CV", nom + "_CV.pdf", "*.pdf");
            if (filePath.isEmpty()) return;

            QPdfWriter writer(filePath);
            writer.setPageSize(QPageSize(QPageSize::A4));
            QPainter painter(&writer);

            QImage backgroundImage("C:/Users/The Night TerRoR/Documents/arbitre2/interface/images/aa.png"); // Replace with your path or Qt resource

            // Scale the image to fit the full A4 size (in pixels: 8.27 x 11.69 inches at 300 DPI ~ 2480 x 3508)
            if (!backgroundImage.isNull()) {
                painter.drawImage(QRect(0, 0, writer.width(), writer.height()), backgroundImage);
            } else {
                qDebug() << "Failed to load background image";
            }


            int x = 800;          // Centered X
            int y = 1200;          // Start below the title
            int lineHeight = 500;  // Space between lines (adjust as needed)


            painter.setPen(QColor("#F4EFD9"));

            // Adjust these coordinates to place text at the top-right (e.g., around face area)
            painter.setFont(QFont("Candles in Easter", 45, QFont::Bold ));
            painter.drawText(6000, 1200, nom);
            painter.drawText(7200, 1200 + 150,prenom);
            painter.setPen(QColor("#F4D9A6"));
            painter.drawText(5300, 2000 + 150,"Professional Referee");
            painter.setPen(Qt::white);
                // Title text




            // Text content in the CV
            painter.setPen(Qt::black); // Use black for the text
            painter.setFont(QFont("Candles in Easter", 16));

            // Add referee information
            painter.drawText(x, y, "ID: " + id);                         y += lineHeight;

            painter.drawText(x, y, "Email: " + email);                   y += lineHeight;
            painter.drawText(x, y, "Âge: " + age);                       y += lineHeight;
            painter.drawText(x, y, "Expérience: " + experience + " ans"); y += lineHeight;
            painter.drawText(x, y, "Sexe: " + sexe);

            // End painter
            painter.end();

            QMessageBox::information(nullptr, "Succès", "📄 CV généré avec succès !");
        });

        // Modifier button action
        QObject::connect(editBtn, &QPushButton::clicked, tableWidget, [=]() {
            if (mainWindow) {
                mainWindow->remplirFormulaireArbitre(nom, prenom, age.toInt(), experience.toInt(), sexe, email);
                mainWindow->setCurrentArbitreId(id.toInt());  // Set the ID
                mainWindow->ui->lineEdit_id->setText(id);     // Update UI
            }
        });

        // Delete button action
        QObject::connect(deleteBtn, &QPushButton::clicked, tableWidget, [=]() {
            // Perform the delete operation directly
            bool deleted = supprimer(id.toInt()); // Call your 'supprimer' method passing the ID
            if (deleted) {
                tableWidget->removeRow(row);  // Remove the row from the table
                QMessageBox::information(nullptr, "Succès", "L'arbitre a été supprimé avec succès !");
            } else {
                QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression de l'arbitre.");
            }
        });



        row++;
    }

    qDebug() << "✅ Data loaded into tableWidget successfully with CV, Edit, and Delete buttons!";
}





bool Arbitre::validerDonnees(QString& erreur) {
    static const QRegularExpression lettersOnly("^[A-Za-zÀ-ÿ\\s'-]+$");
    static const QRegularExpression emailRegex("^[\\w.-]+@gmail\\.com$");

    if (!lettersOnly.match(nom).hasMatch()) {
        erreur = "❌ Le nom ne doit pas contenir de chiffres ou de caractères spéciaux.";
        return false;
    }

    if (!lettersOnly.match(prenom).hasMatch()) {
        erreur = "❌ Le prénom ne doit pas contenir de chiffres ou de caractères spéciaux.";
        return false;
    }

    if (!emailRegex.match(email).hasMatch()) {
        erreur = "❌ L'adresse email doit être une adresse Gmail valide.";
        return false;
    }

    if (age <= 0 || age > 100) {
        erreur = "❌ L'âge doit être entre 1 et 100.";
        return false;
    }

    if (experience < 0 || experience > 80) {
        erreur = "❌ L'expérience doit être un nombre positif et raisonnable.";
        return false;
    }

    return true;
}


#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>

void Arbitre::exporterEnPDF(QTableWidget *table) {
    // 1. Get save location
    QString filePath = QFileDialog::getSaveFileName(nullptr,
                                                    "Exporter les arbitres",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/liste_arbitres.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (filePath.isEmpty()) return;

    // 2. Create a text document
    QTextDocument doc;
    QTextCursor cursor(&doc);

    // 3. Add title
    QTextCharFormat titleFormat;
    titleFormat.setFont(QFont("Arial", 16, QFont::Bold));
    cursor.insertText("Liste des Arbitres\n", titleFormat);

    // 4. Add date
    QTextCharFormat dateFormat;
    dateFormat.setFont(QFont("Arial", 10));
    cursor.insertText("Généré le: " + QDate::currentDate().toString("dd/MM/yyyy") + "\n\n", dateFormat);

    // 5. Create a table in the document
    const int columnCount = table->columnCount() - 3; // Skip action columns
    const int rowCount = table->rowCount();

    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setCellPadding(5);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTable *pdfTable = cursor.insertTable(rowCount + 1, columnCount, tableFormat);

    // 6. Add headers
    QTextCharFormat headerFormat;
    headerFormat.setFont(QFont("Arial", 10, QFont::Bold));
    headerFormat.setBackground(QColor(240, 240, 240));

    for (int col = 0; col < columnCount; ++col) {
        QTableWidgetItem *header = table->horizontalHeaderItem(col);
        if (header) {
            pdfTable->cellAt(0, col).firstCursorPosition().insertText(header->text(), headerFormat);
        }
    }

    // 7. Add table content
    QTextCharFormat cellFormat;
    cellFormat.setFont(QFont("Arial", 9));

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = table->item(row, col);
            QString text = item ? item->text() : "";
            pdfTable->cellAt(row + 1, col).firstCursorPosition().insertText(text, cellFormat);
        }
    }

    // 8. Export to PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    doc.print(&printer);

    // 9. Verify result
    if (QFileInfo(filePath).size() > 0) {
        QMessageBox::information(nullptr, "Succès",
                                 "Liste exportée avec succès!\n" + filePath);
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la création du PDF");
    }
}
