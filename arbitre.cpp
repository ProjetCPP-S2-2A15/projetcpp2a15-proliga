
#include <QTextTable>

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
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>

// Default constructor




// Constructor without ID since it's auto-incremented
Arbitre::Arbitre(QString nom, QString prenom, int age, int experience,
                 QString sexe, QString email, QString telephone, QObject *parent) :
    QObject(parent),  // Initialize QObject parent first
    mainWindow(nullptr),
    currentArbitreId(0),
    id_arbitre(0),
    nom(nom),
    prenom(prenom),
    sexe(sexe),
    email(email),
    telephone(telephone),
    age(age),
    experience(experience),
    networkManager(new QNetworkAccessManager(this)),  // 'this' is now valid
    smtpServer("smtp.gmail.com"),
    smtpPort(465),
    smtpUser("heditrabelsi412@gmail.com"),
    smtpPassword("kton jsoh jtmh blrf")
{
    // Constructor body can remain empty since all members are initialized above
}


Arbitre::Arbitre(MainWindow *mainWindow, QObject *parent) :
    QObject(parent),
    mainWindow(mainWindow),
    currentArbitreId(0),
    id_arbitre(0),
    age(0),
    experience(0),
    networkManager(new QNetworkAccessManager(this)),
    smtpServer("smtp.gmail.com"),
    smtpPort(465),
    smtpUser("heditrabelsi412@gmail.com"),
    smtpPassword("kton jsoh jtmh blrf")
{
    if (!mainWindow) {
        qDebug() << "Warning: MainWindow pointer is NULL";
    }
}



void Arbitre::setSmtpCredentials(const QString &server, int port,
                                 const QString &user, const QString &password)
{
    smtpServer = server;
    smtpPort = port;
    smtpUser = user;
    smtpPassword = password;
}

// Add a referee (ID is auto-generated)
bool Arbitre::ajouter() {
    QString erreur;
    if (!validerDonnees(erreur)) {
        QMessageBox::critical(nullptr, "Erreur de validation", erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO \"C##HEDI\".\"ARBITRE\" (NOM, PRENOM, AGE, EXPERIENCE, SEXE, EMAIL, TELEPHONE) "
                  "VALUES (:nom, :prenom, :age, :experience, :sexe, :email, :telephone)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":experience", experience);
    query.bindValue(":sexe", sexe);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", "Échec de l'ajout de l'arbitre :\n" + query.lastError().text());
        return false;
    }

    return true;
}

// Delete a referee
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

// Modify a referee
bool Arbitre::modifier(int id, QString nom, QString prenom, int age, int experience, QString sexe, QString email, QString telephone) {
    QSqlQuery query;
    query.prepare("UPDATE \"C##HEDI\".\"ARBITRE\" SET NOM = :nom, PRENOM = :prenom, AGE = :age, "
                  "EXPERIENCE = :experience, SEXE = :sexe, EMAIL = :email, TELEPHONE = :telephone "
                  "WHERE ID_ARBITRE = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":experience", experience);
    query.bindValue(":sexe", sexe);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    if (!query.exec()) {
        qDebug() << "❌ SQL Update Error:" << query.lastError().text();
        return false;
    }

    return true;
}

void Arbitre::generateCV(QPainter& painter, const QString& id, const QString& nom,
                         const QString& prenom, const QString& age,
                         const QString& experience, const QString& sexe,
                         const QString& email, const QString& telephone) {
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    // Page setup with very large margins and spacing
    const int pageWidth = painter.window().width();
    const int pageHeight = 1200;  // Increased total height
    const int margin = 80;  // Very large margin
    const int contentWidth = pageWidth - 2 * margin;
    int yPos = margin;

    // ===== 1. HEADER =====
    // Extra large header section
    QLinearGradient headerGrad(0, 0, pageWidth, 220);  // Very tall header
    headerGrad.setColorAt(0, QColor("#1a2980"));
    headerGrad.setColorAt(1, QColor("#26d0ce"));
    painter.fillRect(QRect(0, 0, pageWidth, 220), headerGrad);

    // Title with massive spacing
    QFont titleFont("Arial", 32, QFont::Bold);  // Larger title
    painter.setFont(titleFont);

    // Shadow
    painter.setPen(QColor(0, 0, 0, 100));
    painter.drawText(QRect(3, 83, pageWidth, 100), Qt::AlignCenter, "CV OFFICIEL");

    // Main text
    painter.setPen(Qt::white);
    painter.drawText(QRect(0, 80, pageWidth, 100), Qt::AlignCenter, "CV OFFICIEL");

    // ===== 2. NAME SECTION =====
    yPos = 250;  // Massive space after header
    QFont nameFont("Arial", 26, QFont::Bold);  // Larger name
    painter.setFont(nameFont);
    painter.setPen(QColor("#333333"));
    painter.drawText(QRect(margin, yPos + 90, contentWidth, 90), Qt::AlignLeft, nom.toUpper() + " " + prenom.toUpper());
    yPos += 80;  // Huge spacing

    QFont positionFont("Arial", 18, QFont::Normal);  // Larger position
    painter.setFont(positionFont);
    painter.setPen(QColor("#555555"));
    painter.drawText(QRect(margin, yPos + 110, contentWidth, 80), Qt::AlignLeft, "ARBITRE PROFESSIONNEL");
    yPos += 90;  // Very large space

    // ===== 3. CONTACT INFO =====
    QFont sectionFont("Arial", 20, QFont::Bold);  // Larger section titles
    QFont contentFont("Arial", 14);  // Larger content text

    // Extra large contact box
    painter.setPen(QPen(QColor("#1a2980"), 1.5));
    painter.setBrush(QColor("#f8f9fa"));
    painter.drawRoundedRect(margin, yPos + 200, contentWidth, 290, 20, 20);  // Very tall box

    // Section title with big spacing
    painter.setFont(sectionFont);
    painter.setPen(QColor("#1a2980"));
    painter.drawText(QRect(margin + 25, yPos + 200, contentWidth, 90), "INFORMATIONS PERSONNELLES");

    // Content with massive line spacing
    painter.setFont(contentFont);
    painter.setPen(Qt::black);

    int column1 = margin + 50;
    int column2 = margin + contentWidth/2 + 30;
    int lineHeight = 60;  // Huge line spacing

    QStringList leftDetails = {
        "ID: " + id,
        "Email: " + email,
        "Âge: " + age + " ans"
    };

    QStringList rightDetails = {
        "Téléphone: " + telephone,
        "Expérience: " + experience + " ans",
        "Sexe: " + sexe
    };

    // Draw left column with massive spacing
    for (int i = 0; i < leftDetails.size(); i++) {
        painter.drawText(column1, yPos + 350 + (i * lineHeight), leftDetails[i]);
    }

    // Draw right column with massive spacing
    for (int i = 0; i < rightDetails.size(); i++) {
        painter.drawText(column2, yPos + 350 + (i * lineHeight), rightDetails[i]);
    }

    yPos += 300;  // Huge space after contact section

    // ===== 4. EXPERIENCE =====
    painter.setFont(sectionFont);
    painter.setPen(QColor("#1a2980"));
    painter.drawText(QRect(margin, yPos + 350, contentWidth, 90), "EXPÉRIENCE PROFESSIONNELLE");

    // Extra thick divider with big spacing
    int lineY = yPos + 320;  // Better vertical alignment matching other sections
    int lineLength = contentWidth * 0.6;  // 60% of content width like others
    int lineStart = margin + (contentWidth - lineLength)/2;  // Centered like other dividers
    painter.drawLine(lineStart -440, lineY, lineStart + lineLength, lineY);  // Draw centered line
    yPos += 200;  // Massive space before bullets

    QFont bulletFont("Arial", 14);  // Larger bullet points
    painter.setFont(bulletFont);
    painter.setPen(Qt::black);

    QStringList experiencePoints = {
        "• Arbitre professionnel depuis " + experience + " ans",
        "• Expertise en arbitrage de matches internationaux",
        "• Certifié par la Fédération Internationale de Football",
        "• Expérience dans plus de 10 compétitions majeures",
        "• Spécialisé dans les matches de haut niveau",
        "• Formateur pour jeunes arbitres"
    };

    int bulletSpacing = 60;  // Huge space between bullet points
    for (const QString &point : experiencePoints) {
        if (yPos > pageHeight - 50) {  // Large bottom margin
            painter.drawText(QRect(margin, pageHeight + 450, contentWidth, 80),
                             "Spécialisé dans l'arbitrage vidéo");
            break;
        }
        painter.drawText(QRect(margin + 40, yPos + 300, contentWidth - 80, 60), point);
        yPos += bulletSpacing;
    }

    // ===== 5. FOOTER =====
    painter.setFont(QFont("Arial", 12));  // Larger footer
    painter.setPen(QColor("#7f8c8d"));
    painter.drawText(QRect(margin, pageHeight + 650, contentWidth, 80),  // Large bottom space
                     Qt::AlignRight,
                     "Document généré le " + QDate::currentDate().toString("dd/MM/yyyy"));
}

void Arbitre::afficher(QTableWidget *tableWidget) {
    QSqlQuery query;
    query.prepare("SELECT * FROM \"C##HEDI\".\"ARBITRE\"");

    if (!query.exec()) {
        qDebug() << "❌ SQL Select Error:" << query.lastError().text();
        return;
    }

    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(11);
    tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Age", "Expérience", "Sexe", "Email", "Téléphone", "CV", "Modifier", "Supprimer"});

    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);

        for (int col = 0; col < 8; col++) {
            tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        // CV button
        QPushButton *cvBtn = new QPushButton("CV");
        tableWidget->setCellWidget(row, 8, cvBtn);

        // Edit button
        QPushButton *editBtn = new QPushButton("Modifier");
        tableWidget->setCellWidget(row, 9, editBtn);

        // Delete button
        QPushButton *deleteBtn = new QPushButton("Supprimer");
        tableWidget->setCellWidget(row, 10, deleteBtn);

        // Capture row values
        QString id = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString age = query.value(3).toString();
        QString experience = query.value(4).toString();
        QString sexe = query.value(5).toString();
        QString email = query.value(6).toString();
        QString telephone = query.value(7).toString();

        // CV button action
        QObject::connect(cvBtn, &QPushButton::clicked, [=]() {
            QString filePath = QFileDialog::getSaveFileName(nullptr,
                                                            "Enregistrer le CV",
                                                            nom + "_" + prenom + "_CV.pdf",
                                                            "PDF Files (*.pdf)");

            if (filePath.isEmpty()) return;

            QPdfWriter writer(filePath);
            writer.setPageSize(QPageSize(QPageSize::A4));
            writer.setResolution(300);
            writer.setTitle("CV Arbitre - " + nom + " " + prenom);

            QPainter painter(&writer);
            generateCV(painter, id, nom, prenom, age, experience, sexe, email, telephone);
            painter.end();

            QMessageBox::information(nullptr, "Succès", "CV généré avec succès");
        });

        // Edit button action
        QObject::connect(editBtn, &QPushButton::clicked, tableWidget, [=]() {
            if (mainWindow) {
                mainWindow->remplirFormulaireArbitre(
                    nom,
                    prenom,
                    age.toInt(),
                    experience.toInt(),
                    sexe,
                    email,
                    telephone
                    );
                mainWindow->setCurrentArbitreId(id.toInt());
                mainWindow->ui->lineEdit_id->setText(id);
            }
        });

        // Delete button action
        QObject::connect(deleteBtn, &QPushButton::clicked, tableWidget, [=]() {
            bool deleted = supprimer(id.toInt());
            if (deleted) {
                tableWidget->removeRow(row);
                QMessageBox::information(nullptr, "Succès", "L'arbitre a été supprimé avec succès !");
            } else {
                QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression de l'arbitre.");
            }
        });

        row++;
    }

    qDebug() << "✅ Data loaded into tableWidget successfully!";
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

void Arbitre::exporterEnPDF(QTableWidget *table) {
    QString filePath = QFileDialog::getSaveFileName(nullptr,
                                                    "Exporter les arbitres",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/liste_arbitres.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (filePath.isEmpty()) return;

    QTextDocument doc;
    QTextCursor cursor(&doc);

    // Title
    QTextCharFormat titleFormat;
    titleFormat.setFont(QFont("Arial", 16, QFont::Bold));
    cursor.insertText("Liste des Arbitres\n", titleFormat);

    // Date
    QTextCharFormat dateFormat;
    dateFormat.setFont(QFont("Arial", 10));
    cursor.insertText("Généré le: " + QDate::currentDate().toString("dd/MM/yyyy") + "\n\n", dateFormat);

    // Create table
    const int columnCount = table->columnCount() - 3;
    const int rowCount = table->rowCount();

    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setCellPadding(5);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTable *pdfTable = cursor.insertTable(rowCount + 1, columnCount, tableFormat);

    // Headers
    QTextCharFormat headerFormat;
    headerFormat.setFont(QFont("Arial", 10, QFont::Bold));
    headerFormat.setBackground(QColor(240, 240, 240));

    for (int col = 0; col < columnCount; ++col) {
        QTableWidgetItem *header = table->horizontalHeaderItem(col);
        if (header) {
            pdfTable->cellAt(0, col).firstCursorPosition().insertText(header->text(), headerFormat);
        }
    }

    // Content
    QTextCharFormat cellFormat;
    cellFormat.setFont(QFont("Arial", 9));

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = table->item(row, col);
            QString text = item ? item->text() : "";
            pdfTable->cellAt(row + 1, col).firstCursorPosition().insertText(text, cellFormat);
        }
    }

    // Export
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    doc.print(&printer);

    if (QFileInfo(filePath).size() > 0) {
        QMessageBox::information(nullptr, "Succès", "Liste exportée avec succès!\n" + filePath);
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la création du PDF");
    }
}

QString Arbitre::getExperienceStats() {
    QMap<QString, int> stats;
    stats["0-5 ans"] = 0;
    stats["6-10 ans"] = 0;
    stats["11-15 ans"] = 0;
    stats["16+ ans"] = 0;

    QSqlQuery query;
    query.prepare("SELECT EXPERIENCE FROM \"C##HEDI\".\"ARBITRE\"");

    if (query.exec()) {
        while (query.next()) {
            int exp = query.value(0).toInt();
            if (exp <= 5) stats["0-5 ans"]++;
            else if (exp <= 10) stats["6-10 ans"]++;
            else if (exp <= 15) stats["11-15 ans"]++;
            else stats["16+ ans"]++;
        }
    }

    QString html = "<html><body style='font-family: Segoe UI;'>";
    html += "<h2 style='color:#4CAF50; text-align:center;'>Statistiques des Arbitres</h2>";
    html += "<table border='1' cellpadding='5' style='border-collapse:collapse; width:100%;'>";
    html += "<tr style='background-color:#4CAF50; color:white;'>"
            "<th>Experience Range</th><th>Number of Referees</th></tr>";

    for (auto it = stats.begin(); it != stats.end(); ++it) {
        html += QString("<tr><td>%1</td><td style='text-align:center;'>%2</td></tr>")
        .arg(it.key()).arg(it.value());
    }

    int total = stats["0-5 ans"] + stats["6-10 ans"] + stats["11-15 ans"] + stats["16+ ans"];
    html += QString("<tr style='font-weight:bold; background-color:#f2f2f2;'>"
                    "<td>Total Arbitres</td><td style='text-align:center;'>%1</td></tr>")
                .arg(total);

    html += "</table></body></html>";

    return html;
}

void Arbitre::sendEmail(const QString &email, const QString &subject, const QString &body)
{
    qDebug() << "Attempting to send email to:" << email;
    qDebug() << "Using SMTP server:" << smtpServer << ":" << smtpPort;

    QSslSocket socket;
    socket.connectToHostEncrypted(smtpServer, smtpPort);

    if (!socket.waitForConnected(3000)) {
        QString error = "Connection failed: " + socket.errorString();
        qDebug() << error;
        QMessageBox::critical(nullptr, "Erreur SMTP", error);
        return;
    }
    qDebug() << "Connected to SMTP server";

    if (!socket.waitForEncrypted(3000)) {
        QString error = "SSL handshake failed: " + socket.errorString();
        qDebug() << error;
        QMessageBox::critical(nullptr, "Erreur SMTP", error);
        return;
    }
    qDebug() << "SSL encryption established";

    // SMTP Protocol
    socket.waitForReadyRead(1000);
    socket.write("HELO localhost\r\n");
    socket.waitForReadyRead(1000);
    socket.write("AUTH LOGIN\r\n");
    socket.waitForReadyRead(1000);
    socket.write(QByteArray().append(smtpUser.toUtf8()).toBase64().append("\r\n"));
    socket.waitForReadyRead(1000);
    socket.write(QByteArray().append(smtpPassword.toUtf8()).toBase64().append("\r\n"));
    socket.waitForReadyRead(1000);
    socket.write(QString("MAIL FROM: <%1>\r\n").arg(smtpUser).toUtf8());
    socket.waitForReadyRead(1000);
    socket.write(QString("RCPT TO: <%1>\r\n").arg(email).toUtf8());
    socket.waitForReadyRead(1000);
    socket.write("DATA\r\n");
    socket.waitForReadyRead(1000);
    socket.write(QString("Subject: %1\r\n").arg(subject).toUtf8());
    socket.write("To: " + email.toUtf8() + "\r\n");
    socket.write("MIME-Version: 1.0\r\n");
    socket.write("Content-Type: text/plain; charset=utf-8\r\n");
    socket.write("\r\n"); // End headers
    socket.write(body.toUtf8() + "\r\n");
    socket.write(".\r\n"); // End data
    socket.waitForReadyRead(1000);
    socket.write("QUIT\r\n");

    socket.waitForBytesWritten(3000);
    socket.close();
}


void Arbitre::envoyerConfirmationsArbitres()
{
    QSqlQuery query;
    query.prepare("SELECT EMAIL, NOM, PRENOM FROM \"C##HEDI\".\"ARBITRE\" ORDER BY DBMS_RANDOM.VALUE");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Database query failed: " + query.lastError().text());
        return;
    }

    int count = 0;
    QStringList emailsSent;

    while (query.next() && count < 4) {
        QString email = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();

        QString subject = "Confirmation de participation comme arbitre";
        QString body = QString("Cher %1 %2,\n\n"
                               "Vous avez été sélectionné pour arbitrer le prochain match.\n"
                               "Cette confirmation est automatique - pas besoin de répondre.\n\n"
                               "Cordialement,\n"
                               "L'équipe d'organisation").arg(prenom).arg(nom);

        sendEmail(email, subject, body);
        emailsSent << email;
        count++;
    }

    if (count > 0) {
        QMessageBox::information(nullptr, "Succès",
                                 QString("Emails envoyés à %1 arbitres:\n%2")
                                     .arg(count)
                                     .arg(emailsSent.join("\n")));
    } else {
        QMessageBox::warning(nullptr, "Avertissement",
                             "Aucun email n'a pu être envoyé.");
    }
}


void Arbitre::searchTable(const QString &text) {
    QTableWidget *tableWidget = mainWindow->ui->tableWidget_arbitres;

    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        bool matchFound = false;

        // Check each column for a match (excluding action buttons columns)
        for (int j = 0; j < tableWidget->columnCount() - 3; ++j) {
            QTableWidgetItem *item = tableWidget->item(i, j);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                matchFound = true;
                break;
            }
        }

        // Show/hide row based on search result
        tableWidget->setRowHidden(i, !matchFound);
    }
}
